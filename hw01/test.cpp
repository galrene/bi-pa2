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

bool g_FoundLeaf = false;
int g_TreeDepth = 0;
int g_ReadChars = 0;

class bitReader {
private:
  char c; //nemoze byt inicializovane na nulu, potom to ignoruje nulovy byte
  int pos = 7;
  bool start = false;
public:
  int readBit ( ifstream& ifs ) {
    if ( ! start ) {
      ifs.get(c);
      start = true;
    }
    if ( pos == -1 ) {
      ifs.get(c);
      pos = 7;
    }
    if ( ifs.eof() ) return -1;
    return (c >> pos--) & 1;
  }
  char readByte ( ifstream& ifs ) {
    int read = 0;
    string binString = "";
    while ( read != 8  ) {
      if ( pos == -1 ) {
        ifs.get(c);
        pos = 7;
      }
      binString += to_string( (c >> pos--) & 1 );
      read++;
    }
    return stoi(binString, 0, 2);
  }
  int getCnt ( ifstream & ifs ) {
    string cnt;
    for ( int i = 0; i < 12; i ++ ) 
      cnt += to_string( readBit(ifs) );
    return stoi(cnt, 0, 2);
  }
};

struct TNode { 
  char m_Val;
  TNode * m_Left = nullptr;
  TNode * m_Right = nullptr;
  TNode( int x, TNode * l, TNode * r) : m_Val(x), m_Left(l), m_Right(r) {}
};

void createTree ( TNode *& node, int bit, bitReader & b, ifstream& ifs ) {
  //cout << bit << endl;
  node = new TNode (0, nullptr, nullptr);
  //??
  if ( bit == 0 ) {
    createTree( node->m_Left, b.readBit(ifs), b, ifs );
    createTree( node->m_Right, b.readBit(ifs), b, ifs );
  }
  else {
    node->m_Val = b.readByte(ifs);
    cout << node->m_Val << endl;
    return;
  }
  return;
}

void traverseTree ( ifstream & ifs, ofstream & ofs, bitReader & b, TNode * node, int charCnt ) {
  int bit;
  if ( ! node->m_Right && ! node->m_Left ) {
    ofs << node->m_Val;
    g_ReadChars++;
    g_FoundLeaf = true;
    return;
  }
  while ( (bit = b.readBit(ifs)) != -1 ) {
    //cout << bit << endl;
    if ( bit == 0 ) {
      g_TreeDepth++;
      traverseTree(ifs, ofs, b, node->m_Left, charCnt);
    }
    else {
      g_TreeDepth++;
      traverseTree ( ifs, ofs, b, node->m_Right, charCnt);
    }
    if ( g_FoundLeaf && (g_TreeDepth--) != 1 ) {
      return;
    }
    if ( g_TreeDepth == 0 )
      g_FoundLeaf = false;
    if ( g_ReadChars == charCnt )
      return;
  }
}

bool decompressFile ( const char * inFileName, const char * outFileName )
{
  ifstream ifs( inFileName, ios::binary | ios::in );
  ofstream ofs(outFileName);
  static bitReader a;
  TNode * root = nullptr;

  createTree( root, a.readBit(ifs), a, ifs );

  int ongoingChunk = a.readBit(ifs); //0 if yes
  if ( ! ongoingChunk ) //found last chunk
    traverseTree(ifs, ofs, a, root, a.getCnt ( ifs ) );
  traverseTree(ifs, ofs, a, root, 4096);

  return true;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
  // keep this dummy implementation (no bonus) or implement the compression (bonus)
  return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
  ifstream ifs1 (fileName1);
  ifstream ifs2 (fileName2);
  string word;
  string word2;
  while ( ifs1 >> word && ifs2 >> word2 ) {
    if ( word != word2)
      return false;
  }
  return true;   
}

int main ( void )
{
  /*
  assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );
  */
  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );
  /*
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
  */
  return 0;
}
#endif /* __PROGTEST__ */
