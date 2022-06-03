#pragma once
#include <filesystem>

const int defaultDeckSize = 16;
/**
 * @brief Path where ConfigParser looks for character a card definitions
 * 
 */
const std::filesystem::path defaultPath = std::filesystem::current_path().parent_path() /= "example";
/**
 * @brief Path where CSaver saves cards and characters
 * 
 */
const std::filesystem::path defaultSaveLocation = std::filesystem::current_path().parent_path() /= "example";
const std::string defaultCharacterDirectory = "characters";

/**
 * @brief Two player game or not by default
 * 
 */
const bool defaultTwoPlayerGame = true;
/**
 * @brief Maximum player name length
 * 
 */
const int maxNameLength = 20;
/**
 * @brief Default computer-player name
 * 
 */
const string defaultBotNickname = "TERMINATOR";