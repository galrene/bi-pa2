#pragma once
#include <string>

using namespace std;

class CCharacter {
  protected:
    string m_Name;
    string m_Class;
    int m_HP;
    int m_Mana;
    int m_Strength;
  public:
    CCharacter ( string name, string charClass, int hp, int mana, int strength );
    /* to file */
    void save ( string path );
    bool isSet ( void );
};

CCharacter::CCharacter ( string name, string charClass, int hp, int mana, int strength)
: m_Name ( name ), m_Class ( charClass ), m_HP ( hp ), m_Mana ( mana ), m_Strength ( strength ) {}

bool CCharacter::isSet ( void ) {
  if ( m_Name == "" || m_Class == "" || m_HP == 0 || m_Mana == 0 || m_Strength == 0 )
    return false;
  return true;
}

