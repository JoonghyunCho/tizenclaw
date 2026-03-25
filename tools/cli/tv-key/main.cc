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
 * tv-key: Send virtual key events to Samsung TV via vk_send.
 * All output is JSON for LLM consumption.
 */

#include <unistd.h>

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

int GetIntArg(int argc, char** argv,
              const std::string& flag, int def = 0) {
  std::string val = GetArg(argc, argv, flag);
  if (val.empty()) return def;
  try { return std::stoi(val); }
  catch (...) { return def; }
}

void PrintHelp() {
  std::cout <<
      "{\"usage\": \"tv-key <keycode> [--repeat N] "
      "[--delay MS]\", \"common_keys\": {"
      "\"up\": 111, \"down\": 116, "
      "\"left\": 113, \"right\": 114, "
      "\"ok\": 36, \"back\": 9, \"prior\": 112, "
      "\"next\": 117, \"end\": 115, "
      "\"sys_menu\": 133, \"simple_menu\": 135, "
      "\"channel_guide\": 138, \"exit\": 182, "
      "\"info\": 196, \"power_off\": 124, "
      "\"lfd_power_off\": 120, "
      "\"picture_size\": 140, "
      "\"picture_mode\": 141, "
      "\"sound_mode\": 197, "
      "\"num_0\": 19, \"num_1\": 10, "
      "\"num_2\": 11, \"num_3\": 12, "
      "\"num_4\": 13, \"num_5\": 14, "
      "\"num_6\": 15, \"num_7\": 16, "
      "\"num_8\": 17, \"num_9\": 18}}\n";
}

}  // namespace

int main(int argc, char* argv[]) {
  if (argc < 2 ||
      HasFlag(argc, argv, "-h") ||
      HasFlag(argc, argv, "--help")) {
    PrintHelp();
    return (argc < 2) ? 1 : 0;
  }

  // Find keycode (first non-flag argument)
  int keycode = -1;
  for (int i = 1; i < argc; ++i) {
    std::string a = argv[i];
    if (a == "--repeat" || a == "--delay") {
      ++i;  // skip value
      continue;
    }
    if (a[0] == '-') continue;
    try { keycode = std::stoi(a); }
    catch (...) {}
    break;
  }

  if (keycode < 0) {
    std::cout << "{\"error\": \"Invalid keycode\"}\n";
    return 1;
  }

  int repeat = GetIntArg(argc, argv, "--repeat", 1);
  int delay_ms = GetIntArg(argc, argv, "--delay", 100);

  // Send key events
  std::string cmd = "vk_send " + std::to_string(keycode)
      + " 2>/dev/null";
  for (int i = 0; i < repeat; ++i) {
    system(cmd.c_str());
    if (i < repeat - 1 && delay_ms > 0) {
      usleep(delay_ms * 1000);
    }
  }

  std::cout << "{\"success\": true, \"keycode\": "
            << keycode << ", \"repeat\": " << repeat
            << ", \"delay_ms\": " << delay_ms << "}\n";
  return 0;
}
