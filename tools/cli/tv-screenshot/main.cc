/*
 * Copyright (c) 2026 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * tv-screenshot: Capture Samsung TV screen via Enlightenment compositor.
 * Wraps `enlightenment_info -dump_screen` with JSON output.
 *
 * Features:
 *  - Deletes old screenshot before capture (overwrite fix)
 *  - Optionally converts to JPEG for size reduction (~10x smaller)
 */

#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

namespace {

std::string GetArg(int argc, char** argv,
                   const std::string& flag,
                   const std::string& def = "") {
  for (int i = 1; i < argc - 1; ++i) {
    if (argv[i] == flag)
      return argv[i + 1];
  }
  return def;
}

bool HasFlag(int argc, char** argv,
             const std::string& flag) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i] == flag) return true;
  }
  return false;
}

long GetFileSize(const std::string& path) {
  struct stat st;
  if (stat(path.c_str(), &st) == 0)
    return st.st_size;
  return 0;
}

bool FileExists(const std::string& path) {
  struct stat st;
  return stat(path.c_str(), &st) == 0;
}

}  // namespace

int main(int argc, char* argv[]) {
  if (HasFlag(argc, argv, "-h") ||
      HasFlag(argc, argv, "--help")) {
    std::cout << "{\"usage\": \"tv-screenshot "
              << "[--path DIR] [--name FILE] "
              << "[--quality Q]\", "
              << "\"notes\": \"Deletes old file "
              << "before capture. Uses JPEG "
              << "compression if available.\"}\n";
    return 0;
  }

  std::string out_path =
      GetArg(argc, argv, "--path", "/tmp");
  std::string base_name =
      GetArg(argc, argv, "--name",
             "tizenclaw_screen");
  std::string quality =
      GetArg(argc, argv, "--quality", "50");

  // Use a unique name to avoid overwrite issues
  std::string png_name = base_name + ".png";
  std::string png_file = out_path + "/" + png_name;

  // Delete old screenshot before capture
  unlink(png_file.c_str());

  // Capture screenshot
  std::string cmd = "enlightenment_info -dump_screen"
      " -p " + out_path +
      " -n " + png_name + " 2>/dev/null";
  system(cmd.c_str());

  if (!FileExists(png_file) ||
      GetFileSize(png_file) == 0) {
    std::cout << "{\"success\": false, \"error\": "
              << "\"Screenshot capture failed\"}\n";
    return 1;
  }

  // Try to convert to JPEG for size reduction
  // (JPEG is ~10x smaller than raw PNG for photos)
  std::string jpg_file = out_path + "/"
      + base_name + ".jpg";
  std::string convert_cmd =
      "convert " + png_file + " -resize 960x540 "
      "-quality " + quality + " " + jpg_file
      + " 2>/dev/null";

  std::string final_file = png_file;
  if (system(convert_cmd.c_str()) == 0 &&
      FileExists(jpg_file) &&
      GetFileSize(jpg_file) > 0) {
    // JPEG conversion succeeded, use it
    final_file = jpg_file;
    unlink(png_file.c_str());
  }

  long size = GetFileSize(final_file);
  std::cout << "{\"success\": true, \"path\": \""
            << final_file << "\", \"size\": "
            << size << "}\n";
  return 0;
}
