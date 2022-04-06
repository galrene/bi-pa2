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
  struct TFileWrap {
    uint8_t * m_File;
    size_t m_Size;
    size_t m_Cap;
    size_t m_RefCnt;
    TFileWrap ( void ) {
      m_File = nullptr;
      m_Size = 0;
      m_Cap = 0;
      m_RefCnt = 1;
    }
   ~TFileWrap ( void ) {
      delete [] m_File;
      m_File = nullptr;
      m_Size = 0;
      m_Cap = 0;
      m_RefCnt = 0;
   }
  };
  TFileWrap * m_File;
  size_t m_Pos;

  struct TVersWrap {
    CFile * m_Versions;
    size_t m_VerCap;
    size_t m_RefCnt;
    TVersWrap ( void ) {
      m_Versions = nullptr;
      m_VerCap = 0;
      m_RefCnt = 1;
    }
    ~TVersWrap ( void ) {
      delete [] m_Versions;
      m_Versions = nullptr;
      m_VerCap = 0;
      m_RefCnt = 0;
    }
  };
  
  size_t m_VerCnt;
  TVersWrap * m_Versions;
  void attach ( TFileWrap * file ) {
    m_File = file;
    m_File->m_RefCnt++;
  }
  void detach ( void ) {
    if ( --m_File->m_RefCnt == 0 )
      delete m_File;
  }
  void detachVers ( void ) {
    if ( --m_Versions->m_RefCnt == 0 )
      delete m_Versions;
  }
  void attachVers ( TVersWrap * vers ) {
    m_Versions = vers;
    m_Versions->m_RefCnt++;
  }
  // m_Pos should be replacable by m_Size and work the same
  void growFile ( void ) {
    m_File->m_Cap = m_File->m_Cap * 2 + 5;
    uint8_t * tmp = new uint8_t [m_File->m_Cap];
    for ( size_t j = 0; j < m_Pos; j++ )
      tmp[j] = m_File->m_File[j];
    delete [] m_File->m_File;
    m_File->m_File = tmp;
  }
  void growVersions ( void ) {
    m_Versions->m_VerCap = m_Versions->m_VerCap * 2 + 5;
    CFile * tmp = new CFile [m_Versions->m_VerCap];
    for ( size_t i = 0; i < m_VerCnt; i++ )
      tmp[i] = m_Versions->m_Versions[i];
    delete [] m_Versions->m_Versions;
    m_Versions->m_Versions = tmp;
  }
  /* detaches and makes a deep copy of TFileWrap */
  void deepCopyTFile ( void ) {
    TFileWrap * newFile = new TFileWrap;
    newFile->m_File = new uint8_t [m_File->m_Cap];
    for ( size_t i = 0; i < m_File->m_Size; i++ )
      newFile->m_File[i] = m_File->m_File[i];
    newFile->m_Cap = m_File->m_Cap;
    newFile->m_Size = m_File->m_Size;
    detach();
    m_File = newFile;
  }
    /* detaches and makes shallow copies of existing versions of CFile */
  void copyTVers ( void ) {
    TVersWrap * newVers = new TVersWrap;
    newVers->m_Versions = new CFile [m_Versions->m_VerCap];
    for ( size_t i = 0; i < m_VerCnt; i++ )
      newVers->m_Versions[i] = m_Versions->m_Versions[i];
    newVers->m_VerCap = m_Versions->m_VerCap;
    detachVers();
    m_Versions = newVers;
  }
  public:
    CFile ( void ) {
      m_File = new TFileWrap;
      m_Pos = 0;
      m_Versions = new TVersWrap;
      m_VerCnt = 0;
    }
    CFile ( const CFile & file ) {
      attach ( file.m_File );
      m_Pos = file.m_Pos;
      attachVers ( file.m_Versions );
      m_VerCnt = file.m_VerCnt;
    }
    ~CFile ( void ) {
      detach();
      detachVers();
    }
    CFile & operator = ( const CFile & rhs ) {
      if ( &rhs == this ) return *this;
      detach();
      attach(rhs.m_File);
      m_Pos = rhs.m_Pos;
      detachVers();
      attachVers(rhs.m_Versions);
      m_VerCnt = rhs.m_VerCnt;
      return *this;
    }
    bool                     seek                          ( uint32_t          offset ) {
      if ( offset > m_File->m_Size || offset < 0 )
        return false;
      m_Pos = offset;
      return true;
    }
    uint32_t                 read                          ( uint8_t         * dst,
                                                             uint32_t          bytes ) {
      size_t i = 0;
      uint32_t r = 0;
      while ( (m_Pos < m_File -> m_Size) && (r++ < bytes) )
        dst[i++] = m_File->m_File[m_Pos++];
      if ( r < bytes )
        return r;
      return bytes;
    }
    /* writes bytes into m_File, if rewriting a shared cpy, makes deep copy */
    uint32_t                 write                         ( const uint8_t   * src,
                                                             uint32_t          bytes ) {
      if ( m_File->m_RefCnt > 1 ) {
        deepCopyTFile();
        copyTVers();
      }
      for ( uint32_t i = 0; i < bytes; i++ ) {
        if ( m_Pos >= m_File->m_Cap )
          growFile();
        m_File->m_File[m_Pos++] = src[i];
        if ( m_Pos > m_File->m_Size )
          m_File->m_Size++;
      }
      return bytes;
    }
    /* makes a deep copy of bytes on truncate */
    void                     truncate                      ( void ) {
      if ( m_Pos == m_File->m_Size )
        return;
      /* spravit hlboku kopiu - done */
      TFileWrap * temp = new TFileWrap;
      uint8_t * tmp = new uint8_t [m_Pos];
      for ( size_t i = 0; i < m_Pos; i++ )
        tmp[i] = m_File->m_File[i];
      temp->m_Cap = m_Pos;
      temp->m_Size = m_Pos;
      temp->m_File = tmp;
      detach();
      m_File = temp;
    }
    uint32_t                 fileSize                      ( void ) const { return m_File->m_Size; }
    void                     addVersion                    ( void ) {
      if ( m_VerCnt >= m_Versions->m_VerCap )
        growVersions();
      CFile a;
      a.attach(m_File);
      a.m_Pos = m_Pos;
      m_Versions->m_Versions[m_VerCnt++] = a;
    }
    bool                     undoVersion                   ( void ) {
      if ( ! m_VerCnt )
        return false;
      detach ();
      attach ( m_Versions->m_Versions[m_VerCnt-1].m_File );
      m_Pos = m_Versions->m_Versions[m_VerCnt-1].m_Pos;
      m_VerCnt--;
      return true;
    }
    friend ostream & operator << ( ostream & os, const CFile & a ) {
      cout << "[ ";
      for ( size_t i = 0; i < a.m_File->m_Size; i++ )
        cout <<(int) a.m_File->m_File[i] << " ";
      cout << "]" << endl;
      return os;
    }
    CFile & getVersions ( size_t cnt ) { return m_Versions->m_Versions[cnt];}
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
 * spravit pocitanie referencii cez zaobalovaciu strukturu a attach/detach funckie v const/dest
 * and i guess vo write spravit potom nech to pri volani spravi deep copy
 * 
 * Pri deep kopiroviani, verzie nakopiruj shallow, nie ale pointer ale novy TVersWrap s polom CFileov
 * a to napln cez operator = ( shallow copy s counted ref ), m_VerCnt je useless mat v CFile
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
  cout << f0.getVersions(0);
  cout << f0.getVersions(1);
  CFile f1 ( f0 );
  f0 . truncate ();
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70 }, 20 ));
  assert ( f0 . undoVersion () );
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80, 100, 101, 102, 103 }, 20 ));
  assert ( f0 . undoVersion () );
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80 }, 20 ));
  assert ( !f0 . seek ( 100 ));
  assert ( writeTest ( f1, { 200, 210, 220 }, 3 ) );
  assert ( f1 . seek ( 0 ));
  assert ( readTest ( f1, { 10, 20, 5, 4, 70, 200, 210, 220, 102, 103 }, 20 ));
  cout << f1.getVersions(0);
  cout << f1.getVersions(1);
  assert ( f1 . undoVersion () );
  assert ( f1 . undoVersion () );
  assert ( readTest ( f1, { 4, 70, 80 }, 20 ));
  assert ( !f1 . undoVersion () );

  CFile * f2 = new CFile;
  CFile f3 ( *f2 );
  assert ( writeTest ( *f2, { 10, 20, 30 }, 3 ) );
  assert ( (*f2) . fileSize () == 3 );
  assert ( f3 . seek ( 0 ));
  assert ( writeTest ( f3, { 60, 70, 80, 90 }, 4 ) );
  assert ( (*f2) . fileSize () == 3 );
  assert ( f3 . fileSize () == 4 );
  delete f2;
  assert ( f3 . fileSize () == 4 );
  assert ( f3 . seek ( 0 ));
  assert ( readTest ( f3, { 60, 70, 80, 90 }, 4 ) );
  CFile f4 ( f3 );
  f4.addVersion();
  assert ( writeTest ( f4, { 10, 20, 30 }, 3 ) );
  assert ( writeTest ( f4, { 10, 50, 60, 20, 30 }, 5 ) );
  f4.addVersion();
  assert ( writeTest ( f4, { 10, 50, 60, 20, 30 }, 5 ) );
  f4.addVersion();
  assert ( f4.undoVersion() );
  assert ( f4.undoVersion() );
  assert ( f4.undoVersion() );
  cout << f4;
  assert ( f4 . seek ( 0 ));
  assert ( readTest ( f4, { 60, 70, 80, 90 }, 4 ) );
  /*
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
  */

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
