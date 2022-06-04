#pragma once
#include <string>
#include <iostream>
#include <map>
// #include "CPlayer.h" // ! cyklycky include s CDeck
using namespace std;

class CCard {
  protected:
    string m_Name;
    string m_Type;
    map<string,string> m_Data;
    int m_Cost;
  public:
    virtual ~CCard ( void ) noexcept = default;

    CCard ( string name, string type, int cost );
    CCard ( map <string,string> & data );
    // virtual void useCard ( CPlayer & user, CPlayer & opponent ) = 0;
    virtual bool containsDeps ( map <string,string> & data ) = 0; 
    virtual bool buildCard ( void ) = 0;
    virtual void dumpInfo ( ostream & os ) = 0;
    virtual string getHeader ( void );
    virtual string getName ( void );
    bool operator == ( CCard & rhs );
    // virtual void renderCard ( win, y, x ) = 0; na vykreslenie karty
    // virtual shared_ptr<CCard> create ( void ) = 0; na vytvorenie shared_ptr
    /* for saving */
};
