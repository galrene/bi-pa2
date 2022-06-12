#pragma once
#include <string>
#include <map>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include "CEffect.h"

using namespace std;

class CCharacter {
  public:
    CCharacter ( const string & name, const string & charClass, int hp, int mana, int strength, int defense );
    CCharacter ( map <string,string> & data );
    /**
     * @brief Applies an effect of a card.
     * 
     * @param eff the effect to apply
     */
    void applyEffect ( const CEffect & eff );
    /**
     * @brief Dump all atributes to stream.
     * 
     * @param os output stream where to dump
     */
    void dumpInfo ( ostream & os ) const;
    /**
     * @brief Check if data source contains all required data to create the character.
     * 
     * @param data data source
     * @return true = contains all that's needed, false = doesn't contain all that's needed
     */
    bool containsDeps ( map <string,string> & data ) const;
    /**
     * @brief Set attributes from m_DataSource.
     * 
     * @return true =  success, false = data source doesn't donctain necessary data
     */
    bool buildCharacter ( void );
    /**
     * @brief Print attributes to window, centered.
     * 
     * @param win window for printing
     */
    void renderStats ( WINDOW * win ) const ;

    int getHP ( void ) const  { return m_HP; }
    int getMana ( void ) const  { return m_Mana; }
    string getHeader ( void ) const { return m_Name + "_" + m_Class; }
    string getName ( void ) const { return m_Name; }
  protected:
    string m_Name;
    string m_Class;
    int m_HP;
    int m_Mana;
    int m_Strength;
    int m_Defense;
    /**
     * @brief Key value pairs the character was built from.
     */
    map<string,string> m_DataSource; 
};