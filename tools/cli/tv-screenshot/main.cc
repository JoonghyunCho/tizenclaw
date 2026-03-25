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
 */

#include <sys/stat.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
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

}  // namespace

int main(int argc, char* argv[]) {
  if (HasFlag(argc, argv, "-h") ||
      HasFlag(argc, argv, "--help")) {
    std::cout << "{\"usage\": \"tv-screenshot "
              << "[--path DIR] [--name FILE]\"}\n";
    return 0;
  }

  std::string out_path =
      GetArg(argc, argv, "--path", "/tmp");
  std::string out_name =
      GetArg(argc, argv, "--name", "dump_screen.png");
  std::string output_file = out_path + "/" + out_name;

  // Build command
  std::string cmd = "enlightenment_info -dump_screen"
      " -p " + out_path +
      " -n " + out_name + " 2>/dev/null";

  int ret = system(cmd.c_str());
  (void)ret;

  long size = GetFileSize(output_file);
  if (size > 0) {
    std::cout << "{\"success\": true, \"path\": \""
              << output_file << "\", \"size\": "
              << size << "}\n";
    return 0;
  }

  std::cout << "{\"success\": false, \"error\": "
            << "\"Screenshot failed\"}\n";
  return 1;
}
