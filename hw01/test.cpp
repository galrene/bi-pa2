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

class bitReader {
public:
  bitReader ( int pos, bool start, char c ) {
    m_Pos = pos;
    m_Start = start;
    m_C = c;
  }
  ~bitReader ( ) { 
      m_Pos = 7;
      m_Start = false;
      m_C = 0;
    }
  /*return -1 on eof*/
  int readBit ( ifstream& ifs ) {
    if ( ! m_Start ) {
      ifs.get(m_C);
      m_Start = true;
    }
    if ( m_Pos == -1 ) {
      ifs.get(m_C);
      m_Pos = 7;
    }
    if ( ifs.eof() ) return -1;
    return (m_C >> m_Pos--) & 1;
  }
  char readByte ( ifstream& ifs ) {
    int read = 0;
    string binString = "";
    while ( read != 8  ) {
      if ( m_Pos == -1 ) {
        ifs.get(m_C);
        m_Pos = 7;
      }
      binString += to_string( (m_C >> m_Pos--) & 1 );
      read++;
    }
    return stoi(binString, 0, 2);
  }
  /*read a 12 bit sequence and return its decimal value*/
  int getCnt ( ifstream & ifs ) {
    string cnt = "";
    for ( int i = 0; i < 12; i ++ ) 
      cnt += to_string( readBit(ifs) );
    return stoi(cnt, 0, 2);
  }
  string readUTF ( ifstream & ifs, char firstByte ) {
    /*todo*/
  }
  int m_Pos;
  bool m_Start;
  char m_C;
};

struct TNode { 
  char m_Val;
  string m_Val;
  TNode * m_Left = nullptr;
  TNode * m_Right = nullptr;
  TNode( int x, TNode * l, TNode * r) : m_Val(x), m_Left(l), m_Right(r) {}
};

class huffmanUtils {
public:
  void traverseTree ( ifstream & ifs, ofstream & ofs, bitReader & bitReader, TNode * node, int charCnt ) {
    int bit;
    if ( charCnt == 0 ) return;
    if ( ! node->m_Right && ! node->m_Left ) {
      //cout << node->m_Val << endl;
      ofs << node->m_Val;
      ReadChars++;
      FoundLeaf = true;
      return;
    }
    while ( (bit = bitReader.readBit(ifs)) != -1 ) {
      if ( ! ifs.good() ) {
        FoundLeaf = true;
        return;
      }
      //cout << bit << endl;
      if ( bit == 0 ) {
        TreeDepth++;
        traverseTree(ifs, ofs, bitReader, node->m_Left, charCnt );
      }
      else {
        TreeDepth++;
        traverseTree ( ifs, ofs, bitReader, node->m_Right, charCnt );
      }
      if ( FoundLeaf && (TreeDepth--) != 1 ) {
        return;
      }
      if ( TreeDepth == 0 )
        FoundLeaf = false;
      if ( ReadChars == charCnt ) {
        ReadChars = 0;
        return;
      }
    }
  }
  /*este tu ked int bit je napicu tak sa chces vratit*/
  void createTree ( TNode *& node, int bit, bitReader & b, ifstream& ifs ) {
    //cout << bit << endl;
    node = new TNode (0, nullptr, nullptr);
    //??
    if ( bit == 0 ) {
      createTree( node->m_Left, b.readBit(ifs), b, ifs );
      createTree( node->m_Right, b.readBit(ifs), b, ifs );
    }
    else if ( bit == 1 ) {
      int firstByte = b.readByte(ifs);
      if ( firstByte >= 0x007F )
        node->m_Val = b.readUTF ( ifs, firstByte );
      else
        node->m_Val = firstByte;
      //cout << node->m_Val << endl;
      return;
    }
    return;
  }

  bool createTreeWrapper ( TNode *& node, bitReader & bitReader, ifstream& ifs ) {
    int firstBit = bitReader.readBit(ifs);
    //empty file
    if ( firstBit == -1 || ! ifs.good() )
      return false;
    createTree( node, firstBit, bitReader, ifs );
    return true;
  }
private:
  bool FoundLeaf = false;
  int TreeDepth = 0;
  int ReadChars = 0;
};

void freeTree ( TNode * root ) {
  if ( root == nullptr )
    return;
  freeTree(root->m_Left);
  freeTree(root->m_Right);
  //cout << "Deleting node: " << root->m_Val << endl;
  delete root;
}

bool decompressFile ( const char * inFileName, const char * outFileName )
{
  ifstream ifs( inFileName, ios::binary | ios::in );
  ofstream ofs(outFileName);
  if ( ! ifs.is_open() )
    return false;

  static bitReader a(7, false, 0);
  huffmanUtils b;
  a.m_Start = false;
  a.m_Pos = 7;
  a.m_C = 0;
  TNode * root = nullptr;

  if ( ! b.createTreeWrapper(root, a, ifs ) )
    return false;
  while ( a.readBit(ifs) != 0 && ifs.good() )
    b.traverseTree(ifs, ofs, a, root, 4096 );
  b.traverseTree(ifs, ofs, a, root, a.getCnt ( ifs ) );

  if ( ! ifs.good() || ! ofs.good() )
    return false;
  freeTree(root);
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
  assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  cout << "Success" << endl;
  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );
  cout << "Success" << endl;
  assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

  cout << "Success" << endl;
  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  cout << "Success" << endl;
  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );
  cout << "Success" << endl;

  assert ( ! decompressFile( "vojtaConka", "tempfile") );
  assert ( ! decompressFile( "tests/noPermissions", "tempfile") );
  assert ( ! decompressFile( "tests/emptyFile", "tempfile") );
  
  assert( decompressFile("tests/input.bin", "allAsciiCharsIguess.txt") );
  
  assert ( decompressFile("tests/in_napoveda.bin", "tempfile") );
  
  cout << "ASCII successful" << endl;

  /*
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
