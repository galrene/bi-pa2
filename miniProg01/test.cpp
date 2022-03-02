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

bool isEntry ( string & line ) {
  stringstream ss(line);
  string word;
  int cnt = 0;
  while ( ss >> word )
    ++cnt;
  if ( cnt != 3 ) {
    cout << "40" << endl;
    return false;
  }
  if ( ! isPhoneNum ( word ) ) 
    return false;
  return true;
}

bool loadEntries ( ifstream & ifs, vector<string> & entries ) {
  string line;
  while ( getline( ifs, line ) && line != "" ) {
    if ( ! isEntry(line) ) {
      return false;
    }
    entries.push_back(line);
  }
  return true;
}

void printLine ( string & line, ostream & out ) {
  stringstream ss(line);
  string word;
  int cnt = 0;
  while ( ss >> word ) {
    ++cnt;
    out << word;
    if ( cnt != 3 )
      out << " ";
  }
  out << endl;
}

void searchEntries ( ifstream & ifs , vector<string> entries, ostream & out ) {
  string request;
  while ( getline( ifs, request ) ) {
    int cnt = 0;
    /*iterate through lines*/
    for ( string & entry : entries ) {
      stringstream ss(entry);
      string word;
      /*iterate through words in a line*/
      while ( ss >> word ) {
        if ( word == request ) {
          ++cnt;
          printLine(entry, out );
          break;
        }
      }
    }
    /*the number of matched lines*/
    out << "-> " << cnt << endl;
  }
}

bool report ( const string & fileName, ostream & out ) {

  ifstream ifs;
  vector<string> entries;
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
  cout << report("tests/test03_in.txt", cout ) << endl;
  return 0;
}
#endif /* __PROGTEST__ */
