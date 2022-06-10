#pragma once
#ifndef CCharacter_H
#define CCharacter_H
#include <string>
#include <map>
#include <iostream>
#include <memory>
#include <ncurses.h>

#include "CEffect.h"

using namespace std;

class CCharacter {
  protected:
    string m_Name;
    string m_Class;
    int m_HP;
    int m_Mana;
    int m_Strength;
    int m_Defense;

    map<string,string> m_DataSource;
  public:
    CCharacter ( string name, string charClass, int hp, int mana, int strength, int defense );
    CCharacter ( map <string,string> data );
    
    
    // maybe could return if char is alive after apllying effect or sum
    int applyEffect ( CEffect eff );
    /**
     * @brief Dump all atributes to stream
     * 
     * @param os output stream where to dump
     */
    void dumpInfo ( ostream & os );
    /**
     * @brief Check if data source contains all required data to crate the character
     * 
     * @param data data source
     * @return true contains all that's needed
     * @return false doesn't contain all that's needed
     */
    bool containsDeps ( map <string,string> & data );
    /**
     * @brief Set attributes from m_DataSource
     * 
     * @return true success
     * @return false data source doesn't donctain necessary data
     */
    bool buildCharacter ( void );
    bool isSet ( void );
    /**
     * @brief Print attributes to window, centered
     * 
     * @param win window for printing
     */
    void renderStats ( WINDOW * win );

    int getHP ( void ) { return m_HP; }
    int getMana ( void ) { return m_Mana; }
    string getHeader ( void );
    string getName ( void );
};
#endif