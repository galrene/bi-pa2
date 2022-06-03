#pragma once
#include <string>
#include <map>
#include <iostream>

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

    void dumpInfo ( ostream & os );
    bool containsDeps ( map <string,string> & data );
    bool buildCharacter ( void );
    void save ( string path );
    bool isSet ( void );
    string getHeader ( void );
    string getName ( void );
};