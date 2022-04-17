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
      return tie ( lhs.m_Year, lhs.m_Month, lhs.m_Day ) < tie ( rhs.m_Year, rhs.m_Month, rhs.m_Day );    
    }
};
class CSupermarket
{
  public:
    typedef list<pair<string,int> >::iterator sLIt;
    typedef unordered_map<string, map<CDate,int>>::iterator mapIt;
    CSupermarket() = default;
    CSupermarket & store ( string name, CDate expDate, int count ) {
      m_AllItems[name][expDate] += count;
      return *this;
    }
    void sell ( list<pair<string,int> > & shopList ) {
      vector<pair<string, sLIt> > foundItems;
      //cout << *this << endl;
      for ( auto sLIt = shopList.begin(); sLIt != shopList.end(); ++sLIt )
        findItems ( foundItems, sLIt );
      for ( auto foundIt = foundItems.begin(); foundIt != foundItems.end(); ++foundIt )
        removeItems ( shopList, foundIt );
    }
    list<pair<string,int>> expired ( CDate expDate ) const {
      list<pair<string,int>> returnList;
      for ( auto it = m_AllItems.begin(); it != m_AllItems.end(); ++it ) {
        size_t cnt = 0;
        for ( auto itInner = it->second.begin(); itInner != it->second.end(); ++itInner ) {
          if ( itInner->first < expDate )
            cnt += itInner->second;
          else
            break;
        }
        if ( cnt != 0 ) returnList.emplace_back ( it->first, cnt );
      }
      returnList.sort ( [] ( const pair<string, int> a, const pair<string, int> b ) {
        return a.second > b.second;
      });
      return returnList;
    }
  private:
    void removeItems ( list<pair<string,int> > & shopList, vector<pair<string,sLIt> >::iterator & foundItemIt ) {
      auto mapIt = m_AllItems.find ( foundItemIt->first );
      if ( mapIt == m_AllItems.end() )
        return;
      
      vector<map<CDate,int>::iterator> toRemove;
      for ( auto innerMapIt = mapIt->second.begin(); innerMapIt != mapIt->second.end() && ! mapIt->second.empty(); ++innerMapIt ) {
        int shopListCnt = foundItemIt->second->second;
        int mapCnt = innerMapIt->second;
        if ( mapCnt > shopListCnt ) {
          shopList.erase(foundItemIt->second);
          innerMapIt->second -= shopListCnt;
          break;
        }
        else if ( shopListCnt > mapCnt ) {
          toRemove.emplace_back(innerMapIt);
          //mapIt->second.erase(innerMapIt++);
          foundItemIt->second->second -= mapCnt;
        }
        else if ( shopListCnt == mapCnt ) {
          toRemove.emplace_back(innerMapIt);
          //mapIt->second.erase(innerMapIt);
          shopList.erase(foundItemIt->second);
          break;
        }
      }
      for ( auto & it : toRemove )
        mapIt->second.erase(it);
      // deleted all instances of an element
      if ( mapIt != m_AllItems.end() && mapIt->second.empty() )
        m_AllItems.erase(foundItemIt->first);
    }
    bool isSimilar ( mapIt & item, sLIt & shopListItem ) {
      auto mapStr = item->first.begin();
      auto slStr = shopListItem->first.begin();
      size_t diffLetterCnt = 0;
      for ( ; mapStr != item->first.end() && slStr != shopListItem->first.end(); ++mapStr, ++slStr ) {
        if ( *mapStr != *slStr )
          diffLetterCnt++;
        if ( diffLetterCnt >= 2 )
          return false;
      }
      return true;
    }
    auto similarMatch ( sLIt & sLItem ) {
      auto foundSimilar = m_AllItems.end();
      for ( auto item = m_AllItems.begin(); item != m_AllItems.end(); ++item ) {
        if ( sLItem->first.length() != item->first.length() )
          continue;
        if ( isSimilar ( item, sLItem ) ) {
          if ( foundSimilar == m_AllItems.end() )
            foundSimilar = item;
          else  
            return m_AllItems.end();
        }
      }
      return foundSimilar;
    }
    void findItems ( vector<pair<string,sLIt> > & foundItems, sLIt & sLItem ) {
      auto foundExact = m_AllItems.find( sLItem->first );
      if ( foundExact != m_AllItems.end() ) {
        foundItems.emplace_back ( sLItem->first, sLItem );
        return;
      } 
      auto foundSimilar = similarMatch( sLItem );
      if ( foundSimilar != m_AllItems.end() )
        foundItems.emplace_back ( foundSimilar->first, sLItem );
    }
    unordered_map<string, map<CDate, int>> m_AllItems;
    friend ostream & operator << ( ostream & os, const CSupermarket & src ) {
      for ( auto it = src.m_AllItems.begin(); it != src.m_AllItems.end(); ++it ) {
        os << it->first << endl;
        for ( auto itInner = it->second.begin(); itInner != it->second.end(); ++itInner ) {
          os << "  " << itInner->first << " (" << itInner->second << ")\n";
        }
      }
      os << "-----------------------------------------";
      return os;
    }


};
#ifndef __PROGTEST__
int main ( void )
{
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
  cout << l1 . size () << endl;
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
  x . store ( "azz", CDate ( 999, 5, 9 ), 20 )
    . store ( "aaa", CDate ( 999, 2, 9 ), 10 )
    . store ( "aaa", CDate ( 997, 2, 9 ), 10 )
    . store ( "aaa", CDate ( 996, 2, 9 ), 10 )
    . store ( "aaa", CDate ( 995, 2, 9 ), 10 );
  
  list<pair<string,int> > l16 { { "zzz", 10 }, {"aab" , 50} };
  x . sell ( l16 );
  assert ( l16 . size () == 1 );
  assert ( ( l16 == list<pair<string,int> > { { "aab", 10 } } ) );

  CSupermarket y;
  y . store ( "bread", CDate ( 2000, 1, 1 ), 10 )
    . store ( "bread", CDate ( 2000, 2, 1 ), 10 );

  list<pair<string,int> > l17 { { "bread", 10 }, { "brrad", 10 } };
  y . sell ( l17 );
  assert ( l17 . size () == 0 );
  assert ( ( l17 == list<pair<string,int> > { } ) );

  CSupermarket z;
  z . store ( "Bread", CDate ( 2000, 1, 1 ), 10 )
    . store ( "bread", CDate ( 2000, 2, 1 ), 10 );

  list<pair<string,int> > l18 { { "bread", 10 }, { "bread", 10 } };
  z . sell ( l18 );
  assert ( l18 . size () == 1 );
  assert ( ( l18 == list<pair<string,int> > { { "bread", 10 } } ) );
 
  CSupermarket zz;
  zz . store ( "Bread", CDate ( 2000, 1, 1 ), 10 )
    . store ( "bread", CDate ( 2000, 2, 1 ), 10 );

  list<pair<string,int> > l19 { { "bread", 10 }, { "brrad", 10 } };
  zz . sell ( l19 );
  assert ( l19 . size () == 1 );
  assert ( ( l19 == list<pair<string,int> > { { "brrad", 10 } } ) );

  CSupermarket z2;
  z2 . store ( "cake", CDate ( 2000, 2, 1 ), 10 )
    . store ( "coke", CDate ( 2000, 2, 2 ), 10 )
    . store ( "Coke", CDate ( 2000, 2, 3 ), 10 );

  list<pair<string,int> > l20 { { "Coke", 3 }, { "cake", 2 }, { "Cake", 3 } };
  z2 . sell ( l20 );
  assert ( l20 . size () == 1 );
  assert ( ( l20 == list<pair<string,int> > { { "Cake", 3 } } ) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
