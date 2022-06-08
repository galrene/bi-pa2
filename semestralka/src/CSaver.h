#pragma once
#ifndef CSaver_H
#define CSaver_H
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
    CSaver ( void );
    CSaver ( fs::path p );
    /**
     * @brief save elements inside a directory in .ini format files
     * 
     * @tparam C CCard or Character
     * @param elements Cards or Characters
     * @param dirName directory in which to save
     * @return true saved successfully
     * @return false couldnt create requested directory
     */
    // template < typename C >
    bool save ( deque<shared_ptr<CCard>> & elements, const string & dirName );
    /**
     * @brief should try catch this i think
     *  
     * @param dirName 
     * @return true 
     * @return false 
     */
    bool createDirectory ( const string & dirName );
  private:
    fs::path m_Path;
};
#endif