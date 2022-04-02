#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

class CFile
{
  private:
    uint8_t * m_File;
    size_t m_Size;
    size_t m_Cap;

    size_t m_Pos;
    CFile * m_Versions;
    size_t m_VerCnt;
    size_t m_VerCap;
  public:
    CFile ( void ) {
      m_File = nullptr;
      m_Size = 0;
      m_Cap = 0;
      m_Pos = 0;
      m_Versions = nullptr;
      m_VerCnt = 0;
      m_VerCap = 0;
    }

    CFile ( const CFile & file ) {
      m_File = new uint8_t [file.m_Cap];
      for ( size_t i = 0; i < file.m_Size; i ++ )
        m_File[i] = file.m_File[i];
      m_Cap = file.m_Cap;
      m_Pos = file.m_Pos;
      m_Size = file.m_Size;
      
      m_Versions = new CFile [file.m_VerCap];
      for ( size_t i = 0; i < file.m_VerCnt; i++ )
        m_Versions[i] = file.m_Versions[i];
      m_VerCnt = file.m_VerCnt;
      m_VerCap = file.m_VerCap;
    }
    ~CFile ( void ) {
      delete [] m_File;
      m_File = nullptr;
      m_Size = 0;
      m_Cap = 0;
      m_Pos = 0;
      delete [] m_Versions;
      m_Versions = nullptr;
      m_VerCnt = 0;
      m_VerCap = 0;;
    }
    
    CFile & operator = ( const CFile & rhs ) {
      if ( &rhs == this ) return *this;
      copyBytes(*this, rhs);
      m_Cap = rhs.m_Cap;
      m_Pos = rhs.m_Pos;
      m_Size = rhs.m_Size;

      copyVersions(*this, rhs);
      m_VerCap = rhs.m_VerCap;
      m_VerCnt = rhs.m_VerCnt;
      return *this;
    }
    bool                     seek                          ( uint32_t          offset ) {
      if ( offset > m_Size || offset < 0 )
        return false;
      m_Pos = offset;
      return true;
    }
    uint32_t                 read                          ( uint8_t         * dst,
                                                             uint32_t          bytes ) {
      size_t i = 0;
      uint32_t toRead = bytes;
      while ( m_Pos < m_Size && (bytes--) )
        dst[i++] = m_File[m_Pos++];
      if ( bytes )
        return toRead - bytes;
      return toRead;
    }
    uint32_t                 write                         ( const uint8_t   * src,
                                                             uint32_t          bytes ) {
      for ( uint32_t i = 0; i < bytes; i++ ) {
        if ( m_Pos >= m_Cap ) {
          m_Cap = m_Cap * 2 + 5;
          uint8_t * tmp = new uint8_t [m_Cap];
          for ( size_t i = 0; i < m_Pos; i++ )
            tmp[i] = m_File[i];
          delete [] m_File;
          m_File = tmp;
        }
        m_File[m_Pos++] = src[i];
        if ( m_Pos > m_Size )
          m_Size++;
      }
      return bytes;
    }
    void                     truncate                      ( void ) {
      if ( m_Pos == m_Size )
        return;
      uint8_t * tmp = new uint8_t [m_Pos];
      for ( size_t i = 0; i < m_Pos; i++ )
        tmp[i] = m_File[i];
      m_Size = m_Pos;
      m_Cap = m_Pos;
      delete [] m_File;
      m_File = tmp;
    }
    uint32_t                 fileSize                      ( void ) const { return m_Size; }
    void                     addVersion                    ( void ) {
      if ( m_VerCnt >= m_VerCap ) {
        m_VerCap = m_VerCap * 2 + 5;
        CFile * tmp = new CFile [m_VerCap];
        for ( size_t i = 0; i < m_VerCnt; i++ )
          tmp[i] = m_Versions[i];
        delete [] m_Versions;
        m_Versions = tmp;
      }
      m_Versions[m_VerCnt++] = *this;
    }
    bool                     undoVersion                   ( void ) {
      if ( ! m_VerCnt )
        return false;
      copyBytes (*this, m_Versions[m_VerCnt-1]);
      m_Cap = m_Versions[m_VerCnt-1].m_Cap;
      m_Pos = m_Versions[m_VerCnt-1].m_Pos;
      m_Size = m_Versions[m_VerCnt-1].m_Size;
      
      m_VerCnt--;
      return true;
    }
    void printFile ( void ) {
      for ( size_t i = 0; i < m_Size; i++ )
        cout << (int) m_File[i] << " ";
      cout << endl;
     }
    void copyBytes ( CFile & dst, const CFile & src ) {
      if ( dst.m_Size == src.m_Size ) {
        for ( size_t i = 0; i < src.m_Size; i++ ) {
          dst.m_File[i] = src.m_File[i];
          return;
        }
      }
      delete [] dst.m_File;
      dst.m_File = new uint8_t [src.m_Cap];
      for ( size_t i = 0; i < src.m_Size; i++ )
        dst.m_File[i] = src.m_File[i];
    }
    /* copies a dyn arr of versions */
    void copyVersions ( CFile & dst, const CFile & src ) {
      delete [] dst.m_Versions;
      dst.m_Versions = new CFile [src.m_VerCap];
      for ( size_t i = 0; i < src.m_VerCnt; i++ )
        dst.m_Versions[i] = src.m_Versions[i];
    }
};

#ifndef __PROGTEST__



bool               writeTest                               ( CFile           & x,
                                                             const initializer_list<uint8_t> & data,
                                                             uint32_t          wrLen )
{
  return x . write ( data . begin (), data . size () ) == wrLen;
}
bool               readTest                                ( CFile           & x,
                                                             const initializer_list<uint8_t> & data,
                                                             uint32_t          rdLen )
{
  uint8_t  tmp[100];
  uint32_t idx = 0;

  if ( x . read ( tmp, rdLen ) != data . size ())
    return false;
  for ( auto v : data )
    if ( tmp[idx++] != v )
      return false;
  return true;
}

/**
 * TODO:
 * 
 */
int main ( void ) {

  CFile f0;

  assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
  assert ( f0 . fileSize () == 3 );
  assert ( writeTest ( f0, { 60, 70, 80 }, 3 ) );
  assert ( f0 . fileSize () == 6 );
  assert ( f0 . seek ( 2 ));
  assert ( writeTest ( f0, { 5, 4 }, 2 ) );
  assert ( f0 . fileSize () == 6 );
  assert ( f0 . seek ( 1 ));
  assert ( readTest ( f0, { 20, 5, 4, 70, 80 }, 7 ));
  assert ( f0 . seek ( 3 ));
  f0 . addVersion();
  assert ( f0 . seek ( 6 ));
  assert ( writeTest ( f0, { 100, 101, 102, 103 }, 4 ) );
  f0 . addVersion();
  assert ( f0 . seek ( 5 ));
  CFile f1 ( f0 );
  f0 . truncate ();
  f0.printFile();
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70 }, 20 ));
  assert ( f0 . undoVersion () );
  f0.printFile();
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80, 100, 101, 102, 103 }, 20 ));
  assert ( f0 . undoVersion () );
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80 }, 20 ));
  assert ( !f0 . seek ( 100 ));
  assert ( writeTest ( f1, { 200, 210, 220 }, 3 ) );
  assert ( f1 . seek ( 0 ));
  assert ( readTest ( f1, { 10, 20, 5, 4, 70, 200, 210, 220, 102, 103 }, 20 ));
  assert ( f1 . undoVersion () );
  assert ( f1 . undoVersion () );
  assert ( readTest ( f1, { 4, 70, 80 }, 20 ));
  assert ( !f1 . undoVersion () );

  CFile f2;

  assert ( writeTest ( f2, { 10, 20, 30 }, 3 ) );
  assert ( f2 . fileSize () == 3 );
  assert ( writeTest ( f2, { 60, 70, 80 }, 3 ) );
  assert ( f2 . fileSize () == 6 );
  f2.truncate();
  assert ( f2 . seek ( 0 ));
  assert ( readTest ( f2, { 10, 20, 30, 60, 70, 80 }, 8 ));
  assert ( readTest ( f2, { }, 0 ));
  assert ( f2 . seek ( 5 ));
  assert ( writeTest ( f2, { 60, 70, 80 }, 3 ) );
  assert ( f2 . fileSize () == 8 );
  assert ( f2 . seek ( 7 ));
  assert ( ! readTest ( f2, { 70, 80 }, 2 ));


  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
