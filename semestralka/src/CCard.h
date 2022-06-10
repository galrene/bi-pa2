#pragma once
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <ncurses.h>
#include "CEffect.h"
using namespace std;


class CCard {
  public:
    /**
     * @brief Construct a new CCard object.
     * 
     * @param name 
     * @param type 
     * @param cost 
     */
    CCard ( const string & name, const string & type, int cost );
    /**
     * @brief Create a blank CCard that could be built using the provided data (key-value pairs).
     * 
     * @param data 
     */
    CCard ( map <string,string> & data );
    virtual ~CCard ( void ) noexcept = default;
    /**
     * @brief Get card effects.
     * 
     * @return pair<CEffect,CEffect> first = receiver effect, second = user effect
     */
    virtual pair<CEffect,CEffect> getEffects ( void ) const = 0;
    /**
     * @brief Check if the given data source contains everything needed to build a card.
     * 
     * @param data 
     * @return true = Contains all necessities, false = An attribute is missing
     */
    virtual bool containsDeps ( map <string,string> & data ) const = 0; 
    /**
     * @brief Construct card from m_Data source.
     * 
     * @return true success
     * @return false data source doesnt contain all necessary values
     */
    virtual bool buildCard ( void ) = 0;
    /**
     * @brief Dump card attributes to stream.
     * 
     * @param os Stream for dumping
     */
    virtual void dumpInfo ( ostream & os ) const = 0;
    /**
     * @brief Render a card in a given window.
     * 
     * @param win Window where to render
     */
    virtual void render ( WINDOW * win ) const = 0;
    /**
     * @brief Compare cards using their names
     */
    bool operator == ( CCard & rhs ) const { return rhs.m_Name == m_Name; } ;
    virtual string getHeader ( void ) const { return m_Name + "_" + m_Type; }
    virtual string getName ( void ) const { return m_Name; };
    int getManaCost ( void ) const { return m_Cost; }
  protected:
    string m_Name;
    string m_Type;
    map<string,string> m_Data;
    int m_Cost;
};