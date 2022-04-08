#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
  public:
    // CDate ( y, m, d )
    CDate ( uint y, uint m, uint d )
    : m_Year(y), m_Month(m), m_Day(d) {}
    uint m_Year;
    uint m_Month;
    uint m_Day;
    friend ostream & operator << ( ostream & os, const CDate & src ) {
      os << "{" << src.m_Year << "/" << src.m_Month << "/" << src.m_Day << "}";
      return os;
    }
    friend bool operator < ( const CDate & lhs, const CDate & rhs ) {
      if ( lhs.m_Year == rhs.m_Year ) {
        if ( lhs.m_Month == rhs.m_Month )
          return lhs.m_Day < rhs.m_Day;
        return lhs.m_Month < rhs.m_Month;
      }
      return lhs.m_Year < rhs.m_Year;      
    }
};
class CSupermarket
{
  private:
   
    struct TItem {
      string m_Name;
      CDate m_ExpDate;
      TItem ( string name, CDate expdate )
      : m_Name(name), m_ExpDate(expdate) {}
    };

    struct ByNameAndDate {
      bool operator () ( const TItem & a, const TItem & b ) const{
        return tie ( a.m_Name, a.m_ExpDate ) < tie ( b.m_Name, b.m_ExpDate );
      }
    };
    // if doesnt find item, returns .end()
    map<TItem, int, ByNameAndDate>::iterator findRealName ( string name ) {
      auto it =  m_AllItems.lower_bound ( TItem ( name, CDate ( 0, 0, 0 ) ) );
      if ( it->first.m_Name != name ) return m_AllItems.end();
      return it;
    }
    
    /**
     * Iterator of the map of all items
     * Iterator of the shopping list
     */
    struct TIteratorPair {
      map<TItem, int, ByNameAndDate>::iterator m_MapIt;
      list<pair<string,int>>::iterator m_ListIt;
      TIteratorPair ( map<TItem, int, ByNameAndDate>::iterator it, list<pair<string,int>>::iterator it2 )
      : m_MapIt(it), m_ListIt(it2) {}
    };
    /**
     * removes the corresponding items from the shopping list and the map of all items
     */
    void removeItem ( TIteratorPair & foundItem, list<pair<string,int>> & shoppingList ) {
      auto mapCnt = foundItem.m_MapIt->second;
      auto listCnt = foundItem.m_ListIt->second;
      if ( mapCnt < listCnt ) {
        foundItem.m_ListIt->second -= foundItem.m_MapIt->second;
        m_AllItems.erase(foundItem.m_MapIt);
      }
      else if ( mapCnt > listCnt ) {
        foundItem.m_MapIt->second -= foundItem.m_ListIt->second;
        shoppingList.erase(foundItem.m_ListIt);
      }
      else {
        m_AllItems.erase(foundItem.m_MapIt);
        shoppingList.erase(foundItem.m_ListIt);
      }
    }

    map<TItem, int, ByNameAndDate> m_AllItems;
  public:
    CSupermarket () {}
    
    CSupermarket & store ( string name, CDate expDate, size_t itemCnt ) {
      m_AllItems.emplace ( TItem ( name, expDate ), itemCnt );
      return *this;
    }
    void sell ( list<pair<string,int> > & shopList ) {
      list<TIteratorPair> foundItems;
      auto shopListIt = shopList.begin();
      for ( const auto & [ str, cnt ] : shopList ) {
        auto mapIt = findRealName ( str );
        if ( mapIt != m_AllItems.end() ) {
          foundItems.push_back ( TIteratorPair ( mapIt, shopListIt) );
        }
        ++shopListIt;
      }
      for ( auto & foundItem : foundItems )
          removeItem ( foundItem, shopList );
    }
    list<pair<string,int>> expired ( CDate & expDate ) const;

    friend ostream & operator << ( ostream & os, const CSupermarket & src ) {
      for ( const auto & [ key, value ] : src.m_AllItems ) {
        os << key.m_Name << key.m_ExpDate << " (" << value << ")\n";
      }
      return os;
    }
};
#ifndef __PROGTEST__

/**
 * NOTES:
 * Veci chcem vydavat podla expDate, najskorsie first: 
 * mapa by mala byt asi zoradena podla CDate ascending - key by mal byt CDate a name
 * 
 * Pri vydavani najskor chceme najst vsetky itemy zo shoppingListu, vydavat od prveho po posledny
 * v ramci listu
 *  
 */
int main ( void )
{
  CSupermarket s;
  s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
  . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
  . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
  . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
  . store ( "okey", CDate ( 2016, 7, 18 ), 5 );
  cout << s << endl;
  /*
  list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
  assert ( l0 . size () == 4 );
  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );
  */

  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
  s . sell ( l1 );
  assert ( l1 . size () == 2 );
  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );
  /*
  list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
  assert ( l2 . size () == 1 );
  assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

  list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
  assert ( l3 . size () == 1 );
  assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

  list<pair<string,int> > l4 { { "bread", 105 } };
  s . sell ( l4 );
  assert ( l4 . size () == 0 );
  assert ( ( l4 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l5 . size () == 3 );
  assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

  s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

  list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
  s . sell ( l6 );
  assert ( l6 . size () == 3 );
  assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

  list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l7 . size () == 4 );
  assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

  s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

  list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
  s . sell ( l8 );
  assert ( l8 . size () == 2 );
  assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

  list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l9 . size () == 5 );
  assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

  list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
  s . sell ( l10 );
  assert ( l10 . size () == 2 );
  assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

  list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l11 . size () == 4 );
  assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

  list<pair<string,int> > l12 { { "Cake", 4 } };
  s . sell ( l12 );
  assert ( l12 . size () == 0 );
  assert ( ( l12 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l13 . size () == 4 );
  assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

  list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
  s . sell ( l14 );
  assert ( l14 . size () == 1 );
  assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

  s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
    . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
    . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

  list<pair<string,int> > l15 { { "ccccc", 10 } };
  s . sell ( l15 );
  assert ( l15 . size () == 1 );
  assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );
  */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
