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
    CDate ( unsigned int y, unsigned int m, unsigned int d )
    : m_Year(y), m_Month(m), m_Day(d) {}
    unsigned int m_Year;
    unsigned int m_Month;
    unsigned int m_Day;
    friend ostream & operator << ( ostream & os, const CDate & src ) {
      os << "{" << src.m_Year << "/" << src.m_Month << "/" << src.m_Day << "}";
      return os;
    }
    friend bool operator <= ( const CDate & lhs, const CDate & rhs ) {
      return lhs < rhs || ( tie ( lhs.m_Day, lhs.m_Month, lhs.m_Year ) == tie ( rhs.m_Day, rhs.m_Month, rhs.m_Year ) );
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
    /* contains name and expiration date of an item  */
    struct TItem {
      string m_Name;
      CDate m_ExpDate;
      TItem ( string name, CDate expdate )
      : m_Name(name), m_ExpDate(expdate) {}
    };
    struct ByNameAndDate {
      static string firstLetInsens ( string name ) {
        char a = tolower(name[0]);
        name.erase(name.begin());
        return a + name;
      }
      bool operator () ( const TItem & a, const TItem & b ) const {
        if ( a.m_Name == b.m_Name )
          return a.m_ExpDate < b.m_ExpDate;
        size_t aSize = a.m_Name.size();
        size_t bSize = b.m_Name.size();
        return tie ( aSize, a.m_Name) < tie ( bSize, b.m_Name);
      }
    };
    struct ByDate {
      bool operator () ( const TItem & a, const TItem & b ) const {
        return a.m_ExpDate < b.m_ExpDate;
      }
    };
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
    bool isSimilar ( string a, string name ) {
      size_t diffLetterCnt = 0;
      auto itName = name.begin();
      auto itMapItem = a.begin();
      for ( ; itName != name.end() && itMapItem != a.end() ; ++itName, ++itMapItem ) {
        if ( *itName != *itMapItem )
          diffLetterCnt++;
        if ( diffLetterCnt > 1 )
          return false;
      }
      return true;
    }
    
    /**
     * removes the corresponding items from the shopping list and the maps of all items
     */
    void removeItem ( const TIteratorPair & foundItem, list<pair<string,int>> & shoppingList ) {
      auto mapCnt = foundItem.m_MapIt->second;
      auto listCnt = foundItem.m_ListIt->second;
      if ( mapCnt < listCnt ) {
        foundItem.m_ListIt->second -= foundItem.m_MapIt->second;
        m_AllItemsByDate.erase(foundItem.m_MapIt->first);
        m_AllItems.erase(foundItem.m_MapIt);
      }
      else if ( mapCnt > listCnt ) {
        foundItem.m_MapIt->second -= foundItem.m_ListIt->second;
        m_AllItemsByDate[foundItem.m_MapIt->first] -= foundItem.m_ListIt->second;
        shoppingList.erase(foundItem.m_ListIt);
      }
      else {
        m_AllItemsByDate.erase(foundItem.m_MapIt->first);
        m_AllItems.erase(foundItem.m_MapIt);
        shoppingList.erase(foundItem.m_ListIt);
      }
    }
    void exactMatch ( list<TIteratorPair> & finds, map<TItem, int,ByNameAndDate>::iterator & it,
                      list<pair<string,int>>::iterator & shopListIt, string name, size_t sellCnt ) {
      size_t foundAmount = 0;
      while ( it != m_AllItems.end() && it->first.m_Name == name && foundAmount < sellCnt ) {
        foundAmount += it->second;
        finds.emplace_back ( TIteratorPair ( it, shopListIt ) );
        ++it;
      }
      if ( foundAmount >= sellCnt ) {
        return;
      }
    }
    auto similarMatch ( string name, list<pair<string,int>>::iterator & shopListIt ) {
      // fake name of the same length as the searched name, so that lower bound returns the first possible mistyped item
      // or .end() when there are only shorter items
      string fakeName;
      for ( size_t i = 0; i < name.length(); i++ )
        fakeName += "A";
      auto it = m_AllItems.lower_bound ( TItem ( fakeName, CDate ( 0, 0, 0 ) ) );
      size_t matchCnt = 0;
      auto foundSimilar = m_AllItems.end();
      while ( it != m_AllItems.end() && it->first.m_Name.length() == name.length() ) {
        if ( isSimilar ( it->first.m_Name, name ) ) {
          matchCnt++;
          // didnt find exact or decisive similar match
          if ( matchCnt >= 2 )
            return m_AllItems.end();
          foundSimilar = it;
        }
        ++it;
      }
      return foundSimilar;
    }

    void findItems ( list<TIteratorPair> & finds, list<pair<string,int>>::iterator & shopListIt, string name, size_t sellCnt ) { 
      // returns .end() when there are only items shorter or same length but lexicographically smaller
      auto it = m_AllItems.lower_bound ( TItem ( name, CDate ( 0, 0, 0 ) ) );
      if ( it != m_AllItems.end() && it->first.m_Name == name ) {
        exactMatch( finds, it, shopListIt, name, sellCnt );
        ++shopListIt;
        return;
      }
      auto foundSimilar = similarMatch ( name, shopListIt );
      if ( foundSimilar != m_AllItems.end() )
        finds.emplace_back ( TIteratorPair ( foundSimilar, shopListIt ) );
      ++shopListIt;
      return;
    }
    map<TItem, int, ByNameAndDate> m_AllItems;
    map<TItem, int, ByDate> m_AllItemsByDate;
  public:
    CSupermarket () = default;
    
    CSupermarket & store ( string name, CDate expDate, size_t itemCnt ) {
      m_AllItems.emplace ( TItem ( name, expDate ), itemCnt );
      m_AllItemsByDate.emplace ( TItem ( name, expDate ), itemCnt );
      return *this;
    }
    void sell ( list<pair<string,int> > & shopList ) {
      list<TIteratorPair> foundItems;
      auto shopListIt = shopList.begin();
      for ( const auto & [ str, cnt ] : shopList )
        findItems ( foundItems, shopListIt, str, cnt );
      for ( const auto & foundItem : foundItems )
        removeItem ( foundItem, shopList );
    }
    list<pair<string,int> > expired ( const CDate & expDate ) const {
      map<string,int> foundItems;
      for ( const auto & x : m_AllItemsByDate ) {
        if ( x.first.m_ExpDate < expDate )
          foundItems[x.first.m_Name] += x.second;
        else
          break;
      }
      list<pair<string,int> > returnList ( foundItems.begin(), foundItems.end() );
      returnList.sort( [] ( pair<string,int> & a, pair<string,int> & b) {
        return b.second < a.second;
      });
      return returnList;
    }
    friend ostream & operator << ( ostream & os, const CSupermarket & src ) {
      for ( const auto & [ key, value ] : src.m_AllItems ) {
        os << key.m_Name << key.m_ExpDate << " (" << value << ")\n";
      }
      return os;
    }
    void printByDate ( ostream & os ) {
      for ( const auto & [ key, value ] : m_AllItemsByDate )
        os << key.m_Name << " (" << value << ")\n";
    }
};
#ifndef __PROGTEST__
/**
 * POSSIBLE_BUGS:
 * 
 */
int main ( void )
{
  using namespace std;
  /*
  CSupermarket cc;
  cc . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
  . store ( "zread", CDate ( 2016, 4, 30 ), 100 )
  . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
  . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
  . store ( "Breed", CDate ( 2016, 4, 25 ), 100 )
  . store ( "Bread", CDate ( 2016, 4, 25 ), 100 )
  . store ( "okey", CDate ( 2016, 7, 18 ), 5 );
  cout << cc << endl;
  
  list<pair<string,int> > l69 { { "breed", 2 }, { "Coke", 5 }, { "butter", 20 } };
  cc . sell ( l69 );
  assert ( l69 . size () == 2 );
  assert ( ( l69 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );
  */

  CSupermarket s;
  s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
  . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
  . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
  . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
  . store ( "okey", CDate ( 2016, 7, 18 ), 5 );
  list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
  assert ( l0 . size () == 4 );
  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
  s . sell ( l1 );
  assert ( l1 . size () == 2 );
  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );
  
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

  CSupermarket x;
  x . store ( "abb", CDate ( 999, 5, 9 ), 20 )
    . store ( "aaa", CDate ( 999, 2, 9 ), 10 );
  
  list<pair<string,int> > l16 { { "zzz", 10 } };
  x . sell ( l16 );
  assert ( l16 . size () == 1 );
  assert ( ( l16 == list<pair<string,int> > { { "zzz", 10 } } ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
