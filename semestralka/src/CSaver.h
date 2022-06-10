#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <deque>
#include "CCard.h"
#include "Constants.h"

using namespace std;
namespace fs = filesystem;

class CSaver {
  public:
    CSaver (void ) = default;
    /**
     * @brief Save cards inside a directory
     * 
     * @param dirName Directory in which to save
     * @return true saved successfully
     * @return false couldn't create requested directory
     */
    bool saveCards ( const deque<shared_ptr<CCard>> & cards, fs::path & dirPath ) const;
    /**
     * @brief Create a directory at defaultSaveLocation, if alredy exists, create "dirname_n"
     *  
     * @param dirName Name of the create directory
     * @return true Sucess
     * @return false Couldn't create directory
     */
    bool createDirectory ( fs::path & dirPath ) const;
};