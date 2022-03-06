#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

char g_C = 0;
int g_Pos = 7;

class bitReader {
public:
  int readBit ( ifstream& ifs ) {
    if ( ! g_C || g_Pos == -1 ) {
      ifs.get(g_C);
      g_Pos = 7;
    }
    return (g_C >> g_Pos--) & 1;
  }
  char readByte ( ifstream& ifs ) {
    int read = 0;
    string binString = "";
    while ( read != 8  ) {
      if ( g_Pos == -1 ) {
        ifs.get(g_C);
        g_Pos = 7;
      }
      binString += to_string( (g_C >> g_Pos--) & 1 );
      read++;
    }
    return stoi(binString, 0, 2);
  }
};

struct TNode { 
  char m_Val;
  TNode * m_Left = nullptr;
  TNode * m_Right = nullptr;
  TNode( int x, TNode * l, TNode * r) : m_Val(x), m_Left(l), m_Right(r) {}
};
/*!globalne premenne!*/
void createTree ( TNode *& node, int bit, bitReader b, ifstream& ifs ) {

  node = new TNode (0, nullptr, nullptr);
  
  if ( bit == 0 ) {
    createTree( node->m_Left, b.readBit(ifs), b, ifs );
    createTree( node->m_Right, b.readBit(ifs), b, ifs );
  }
  else {
    node->m_Val = b.readByte(ifs);
    return;
  }
  return;
}

void readBytes ( ifstream & ifs, int cnt ) {
  char c;
  for ( int i = 0; i < cnt; i++ ) {
    ifs.get(c);
    for ( int i = 7; i >= 0; i-- ) {
      cout << ( (c >> i) & 1 ) << endl;
    }
    cout << "------" << endl;
  } 
}

bool decompressFile ( const char * inFileName, const char * outFileName )
{
  ifstream ifs( inFileName, ios::binary | ios::in );
  bitReader a;
  TNode * root = nullptr;
  createTree( root, a.readBit(ifs), a, ifs );
  return false;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
  // keep this dummy implementation (no bonus) or implement the compression (bonus)
  return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
  // todo
  return false;
}

int main ( void )
{
  assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );


  assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );

  return 0;
}
#endif /* __PROGTEST__ */
