#pragma once
#include <filesystem>

const int defaultDeckSize = 16;
// default path where CConfigParser looks for directory names used in arguemnts
// example directory
const std::filesystem::path defaultPath = std::filesystem::current_path().parent_path() /= "example";