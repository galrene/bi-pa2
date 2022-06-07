#pragma once
#ifndef CCard_H
#define CCard_H
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <ncurses.h>
#include <string.h>
#include "CEffect.h"
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
    /**
     * @brief get card effects
     * 
     * @return pair<CEffect,CEffect> first = receiver effect, second = user effect
     */
    virtual pair<CEffect,CEffect> getEffects ( void ) = 0;
    virtual bool containsDeps ( map <string,string> & data ) = 0; 
    /**
    * @brief construct card from provided map<string,string> data source
    * 
    * @return true success
    * @return false data source doesnt contain all necessary values
    */
    virtual bool buildCard ( void ) = 0;
    virtual void dumpInfo ( ostream & os ) = 0;
    virtual string getHeader ( void );
    virtual string getName ( void );
    virtual void render ( WINDOW * win ) = 0;
    bool operator == ( CCard & rhs );
    // ! TEMP ?
    int getManaCost ( void ) {return m_Cost;}
    // virtual shared_ptr<CCard> create ( void ) = 0; na vytvorenie shared_ptr
};
#endif