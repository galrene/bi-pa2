#pragma once
#include <filesystem>
#include <string>

const int defaultDeckSize = 30;
/**
 * @brief Parent directory where ConfigParser looks for card, deck and character directories
 * 
 */
const std::filesystem::path defaultPath = std::filesystem::current_path() /= "example";
/**
 * @brief Parent directory where CSaver saves card, deck and character directories
 * 
 */
const std::filesystem::path defaultSaveLocation = std::filesystem::current_path() /= "example";
/**
 * @brief For loading, located in defaultPath directory
 * 
 */
const std::string defaultCharacterDirectory = "characters";
const std::string defaultCardDirectory = "cards";
const std::string defaultDeckDirectory = "decks";

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
const std::string defaultBotNickname = "TERMINATOR";
/**
 * @brief default max length of an inputted number in Deck size input field
 * 
 */
const int defaultInputLengthDeckSize = 3;