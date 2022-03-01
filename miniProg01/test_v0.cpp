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

void printLine ( const string & line, ostream & out ) {
  string output;
  string word;
  stringstream ss(line);

  while ( ss >> word ) {
    if ( ! output.empty() )
      output += ' ';
    output += word;
  }
  out << output << endl;
  cout << output << endl;
}

/**
 * ked je rovnake slovo dva krat v riadku, vypise ho dva krat
 * 
*/
void findWord ( const string & request, vector<string> & lines,
                ostream & out ) {
  string word;

  int cnt = 0;
  /*iterates through lines*/
  for ( string & line : lines ) {
    stringstream ssLine(line);
    /*iterates through words*/
    while ( ssLine >> word ) {
      if ( word == request ) {
        printLine( line, ssLine );
        ++cnt;
      }
    }
  }
  out << "-> " << cnt << endl;
  cout << "-> " << cnt << endl;
}

bool report ( const string & fileName, ostream & out )
{
  ifstream is;
  is.open( fileName );
  
  vector<string> lines;
  string buff;

  while ( getline( is, buff ) && ( buff != "" ) )
    lines.push_back( buff );
  
  while ( getline( is, buff ) )
    findWord( buff, lines, out );
  is.close();  
  return true;
}

#ifndef __PROGTEST__
int main ()
{
  
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  cout << "oss.str():\n";
  cout << oss.str() << endl;
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
