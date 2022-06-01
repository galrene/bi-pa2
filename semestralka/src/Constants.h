#pragma once
#include <filesystem>

const int defaultDeckSize = 16;
const std::filesystem::path defaultPath = std::filesystem::current_path().parent_path(); // main program directory