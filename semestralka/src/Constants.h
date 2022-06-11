#pragma once
#include <filesystem>
#include <string>

const int defaultDeckSize = 30;
/**
 * @brief Parent directory where ConfigParser looks for card, deck and character directories
 * 
 */
const std::filesystem::path defaultPath = std::filesystem::current_path() /= "examples";
/**
 * @brief Parent directory where CSaver saves card, deck and character directories
 * 
 */
const std::filesystem::path defaultSaveLocation = std::filesystem::current_path() /= "examples";
/**
 * @brief CConfigParser log path
 */
const std::filesystem::path defaultLogFilePath = std::filesystem::current_path() /= "examples";
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
 * @brief Maximum player name length inputtable
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
/**
 * @brief amount of cards each player plays with ( won't be able to play more than 10 because of current controls )
 */
const size_t handSize = 6;
/**
 * @brief Can players see each other's hands by default?
 * 
 */
const bool defaultHandVisibility = false;
/**
 * @brief First on turn by default TRUE = Player1, FALSE = Player2
 * 
 */
const bool defaultFirstOnTurn = true;

// keybindings
const int defaultDiscardButton = 'd';
const int defaultEndTurnButton = 'e';
const int defaultMenuButton = 'q'; // ingame menu