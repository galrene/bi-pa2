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
  unsigned int overallSize = 0;
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
  unsigned char readByte ( ifstream& ifs ) {
    int read = 0;
    binString = "";
    while ( read != 8  ) {
      if ( m_Pos == -1 ) {
        ifs.get(m_C);
        m_Pos = 7;
      }
      binString += to_string( (m_C >> m_Pos--) & 1 );
      read++;
    }
    cout << " " << binString << endl;
    return stoi(binString, 0, 2);
  }
  string readByteString ( ifstream& ifs ) {
    int read = 0;
    binString = "";
    while ( read != 8  ) {
      if ( m_Pos == -1 ) {
        ifs.get(m_C);
        m_Pos = 7;
      }
      binString += to_string( (m_C >> m_Pos--) & 1 );
      read++;
    }
    cout << " " << binString << endl;
    return binString;
  }
  
  /*read a 12 bit sequence and return its decimal value*/
  int getCnt ( ifstream & ifs ) {
    string cnt = "";
    for ( int i = 0; i < 12; i ++ ) 
      cnt += to_string( readBit(ifs) );
    return stoi(cnt, 0, 2);
  }

  int getUTFSize ( ifstream & ifs, unsigned int firstByte ) {
      if ( firstByte < 224 )
        return 2;
      else if ( firstByte < 240 )
        return 3;
      else if ( firstByte <= 247 )
        return 4;
      return -1;
  }

  int subtractionValue ( int byteCnt ) {
    switch ( byteCnt ) {
      case(2):
        return 192;
      case(3):
        return 224;
      case(4):
        return 240;
    }
    return 6999999;
  }
  
  long readUTF ( ifstream & ifs, unsigned char firstByte, vector<unsigned char> & UTF ) {
    int byteCnt = getUTFSize ( ifs, firstByte );
    if ( byteCnt == -1 ) {
      ifs.setstate(ios::iostate::_S_failbit);
      return 0;
    }
    string UTFString = "";
    UTFString = binString;
    UTF.push_back(firstByte);
    for ( int i = 0; i < byteCnt-1; i++ ) {
      string readString = readByteString(ifs); //mohol som aj spravit tak ako to bolo predtym iba by som appendoval binString po kazdom volani readByte
      unsigned char readB = stoi(readString,0,2);
      if ( readB < 128 || readB > 191 ) //byte doesnt start with 10
        return 0 ;
      UTFString += readString;
      UTF.push_back(readB);
    }
    long res = stol(UTFString,0,2);
    return res;
  }
  int m_Pos;
  bool m_Start;
  char m_C;
  string binString;
};

struct TNode { 
  unsigned char m_Val;
  TNode * m_Left = nullptr;
  TNode * m_Right = nullptr;
  vector<unsigned char> UTF;
  TNode( unsigned char x, TNode * l, TNode * r ) : m_Val(x), m_Left(l), m_Right(r) {}
};

class huffmanUtils {
public:
  void traverseTree ( ifstream & ifs, ofstream & ofs, bitReader & bitReader, TNode * node, int charCnt ) {
    int bit;
    if ( charCnt == 0 ) return;
    if ( ! node->m_Right && ! node->m_Left ) {
      //cout << node->m_Val << endl;
      if ( ! node->UTF.empty() ) {
        for ( unsigned char c : node->UTF )
          ofs << c;
      }
      else 
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
  void createTree ( TNode *& node, int bit, bitReader & b, ifstream& ifs ) {
    node = new TNode (0, nullptr, nullptr );
    if ( !ifs.good() )
      return;
    
    if ( bit == 0 ) {
      createTree( node->m_Left, b.readBit(ifs), b, ifs );
      if ( !ifs.good() )
        return;
      createTree( node->m_Right, b.readBit(ifs), b, ifs );
      if ( !ifs.good() )
        return;
    }
    else if ( bit == 1 ) {
      unsigned char firstByte = b.readByte(ifs);
      if ( firstByte > 127 ) {
        long UTFSize = b.readUTF(ifs, firstByte, node->UTF);
        if ( UTFSize >= 4103110784 || UTFSize <= 0 ) //11110100 10000000 10000011 10111111
          ifs.setstate(ios::iostate::_S_failbit);
      }
      else {
        node->m_Val = firstByte;
        cout << node->m_Val;
      }
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
    if ( ! ifs.good() )
      return false;
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
  while ( a.readBit(ifs) && ifs.good() )
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
  /*
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
  
  cout << endl << "------------------" << endl;
  cout << endl << "|ASCII successful|" << endl;
  cout << endl << "------------------" << endl;

  assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );
  cout << "Success" << endl;
  */
  assert ( ! decompressFile ( "tests/napoveda2.bin", "tempfile" ) );
  cout << "Success" << endl;

  assert ( decompressFile ( "tests/napoveda3.bin", "tempfile" ) );

  cout << "All successful" << endl;
  return 0;
}
#endif /* __PROGTEST__ */
