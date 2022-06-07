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
  friend class CCard;
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

    void dumpInfo ( ostream & os );
    bool containsDeps ( map <string,string> & data );
    bool buildCharacter ( void );
    void save ( string path );
    bool isSet ( void );
    void renderStats ( WINDOW * win );

    int getHP ( void ) { return m_HP; }
    int getMana ( void ) { return m_Mana; }
    string getHeader ( void );
    string getName ( void );
};
#endif