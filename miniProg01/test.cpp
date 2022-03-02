#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

bool isPhoneNum ( string & phoneNum ) {
  if ( phoneNum.size() != 9 ) {
    cout << "17" << endl;
    return false;
  }
  if ( phoneNum[0] == '0' ) {
    cout << "21" << endl;
    return false;
  }
  for ( char c : phoneNum ) {
    if ( c < '0' || c > '9' ) {
      cout << "26" << endl;
      return false;
    }
  }
  return true;
}

struct TEntry {
  string m_Name;
  string m_Surname;
  string m_phoneNum;
};

bool isEntry ( string & line, TEntry & entry ) {
  stringstream ss(line);
  string word;

  int cnt = 0;
  while ( ss >> word && cnt <= 4) {

    switch (++cnt) {
      case (1):
        entry.m_Name = word;
        break;
      case (2):
        entry.m_Surname = word;
        break;
      case (3):
        entry.m_phoneNum = word;
        break;
      default:
        break;
    }
  }
  if ( cnt != 3 ) {
    cout << "40" << endl;
    return false;
  }
  if ( ! isPhoneNum ( entry.m_phoneNum ) ) 
    return false;
  
  return true;
}

bool loadEntries ( ifstream & ifs, vector<TEntry> & entries ) {
  string line;
  while ( getline( ifs, line ) && line != "" ) {
    TEntry entry = {};
    if ( ! isEntry( line, entry ) ) {
      return false;
    }
    entries.push_back(entry);
  }
  return true;
}


void searchEntries ( ifstream & ifs , vector<TEntry> entries, ostream & out ) {
  string request;
  while ( getline( ifs, request ) ) {
    int cnt = 0;
    /*iterate through lines*/
    for ( TEntry & entry : entries ) {
      /*iterate through words in a line*/
        if ( entry.m_Name == request || entry.m_Surname == request ) {
          ++cnt;
          out << entry.m_Name << " " << entry.m_Surname << " " << entry.m_phoneNum << endl;
        }
      }
    /*the number of matched lines*/
    out << "-> " << cnt << endl;
    }
}

bool report ( const string & fileName, ostream & out ) {

  ifstream ifs;
  vector<TEntry> entries;
  
  ifs.open(fileName);
  if ( ! ifs.is_open() || ! ifs.good() ) {
    cout << "100" << endl;
    return false;
  }

  if ( ! loadEntries( ifs, entries ) )
    return false;
  searchEntries( ifs, entries, out );

  ifs.close();  
  return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  report( "tests/test0_in.txt", cout );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  return 0;
}
#endif /* __PROGTEST__ */
