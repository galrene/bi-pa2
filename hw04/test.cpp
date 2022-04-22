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
  /**
   * @brief Wrapper structure containing data array
   * and relevant variables for dynamic allocation and copy-on-write reference counting
   */
  struct TFileWrap {
    uint8_t * m_Data;
    size_t m_Size;
    size_t m_Cap;
    size_t m_RefCnt;
    TFileWrap ( void ) {
      m_Data = nullptr;
      m_Size = 0;
      m_Cap = 0;
      m_RefCnt = 1;
    }
   ~TFileWrap ( void ) {
      delete [] m_Data;
      m_Data = nullptr;
      m_Size = 0;
      m_Cap = 0;
      m_RefCnt = 0;
   }
  };
  TFileWrap * m_FilePtr;
  /* Cursor position in the file */
  size_t m_Pos;
  /**
   * @brief Wrapper structure containing an array of CFile versions,
   * variables relevant to dynamic allocation and pointer reference count
   * for copy-on-write 
   */
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
  TVersWrap * m_VersPtr;
  /* Number of existing versions */
  size_t m_VerCnt;
  /**
   * @brief Attach TFileWrap pointer from arg
   * 
   * @param[in] Pointer to a struct containing data and reference count   
   */
  void attachFile ( TFileWrap * file ) {
    m_FilePtr = file;
    m_FilePtr->m_RefCnt++;
  }
  /**
   * @brief Attach TVersWrap pointer from arg
   * 
   * @param Pointer to a struct containing backed-up versions and reference count   
   */
  void attachVers ( TVersWrap * vers ) {
    m_VersPtr = vers;
    m_VersPtr->m_RefCnt++;
  }
  /**
   * @brief Detach pointer containing data
   */
  void detachFile ( void ) {
    if ( --m_FilePtr->m_RefCnt == 0 )
      delete m_FilePtr;
  }
  /**
   * @brief Detach pointer containing versions
   */
  void detachVers ( void ) {
    if ( --m_VersPtr->m_RefCnt == 0 )
      delete m_VersPtr;
  }
  /**
   * @brief Grow a dynamically allocated array of type uint8_t
   * by size * 2 + 5
   */
  void growFile ( void ) {
    m_FilePtr->m_Cap = m_FilePtr->m_Cap * 2 + 5;
    uint8_t * newData = new uint8_t [m_FilePtr->m_Cap];
    for ( size_t j = 0; j < m_Pos; j++ )
      newData[j] = m_FilePtr->m_Data[j];
    delete [] m_FilePtr->m_Data;
    m_FilePtr->m_Data = newData;
  }
  /**
   * @brief Grow a dynamically allocated array of type CFile
   * by size * 2 + 5
   */
  void growVersions ( void ) {
    m_VersPtr->m_VerCap = m_VersPtr->m_VerCap * 2 + 5;
    CFile * newVers = new CFile [m_VersPtr->m_VerCap];
    for ( size_t i = 0; i < m_VerCnt; i++ )
      newVers[i] = m_VersPtr->m_Versions[i];
    delete [] m_VersPtr->m_Versions;
    m_VersPtr->m_Versions = newVers;
  }
  /**
   * @brief create an independant deep copy of the file
   * pointed to by m_FilePtr contianed in TFileWrap
   */
  void deepCopyTFile ( void ) {
    TFileWrap * newFile = new TFileWrap;
    newFile->m_Data = new uint8_t [m_FilePtr->m_Cap];
    for ( size_t i = 0; i < m_FilePtr->m_Size; i++ )
      newFile->m_Data[i] = m_FilePtr->m_Data[i];
    newFile->m_Cap = m_FilePtr->m_Cap;
    newFile->m_Size = m_FilePtr->m_Size;
    detachFile();
    m_FilePtr = newFile;
  }
  /**
   * @brief create a shallow copy of CFile versions
   * pointed to by m_VersPtr contained in TVersWrap
   */
  void copyTVers ( void ) {
    TVersWrap * newVers = new TVersWrap;
    newVers->m_Versions = new CFile [m_VersPtr->m_VerCap];
    for ( size_t i = 0; i < m_VerCnt; i++ )
      newVers->m_Versions[i] = m_VersPtr->m_Versions[i];
    newVers->m_VerCap = m_VersPtr->m_VerCap;
    detachVers();
    m_VersPtr = newVers;
  }
  public:
    CFile ( void ) {
      m_FilePtr = new TFileWrap;
      m_Pos = 0;
      m_VersPtr = new TVersWrap;
      m_VerCnt = 0;
    }
    /** @brief shallow copy */
    CFile ( const CFile & file ) {
      attachFile ( file.m_FilePtr );
      m_Pos = file.m_Pos;
      attachVers ( file.m_VersPtr );
      m_VerCnt = file.m_VerCnt;
    }
    ~CFile ( void ) {
      detachFile();
      detachVers();
    }
    /** @brief shallow copy */
    CFile & operator = ( const CFile & rhs ) {
      if ( &rhs == this ) return *this;
      detachFile();
      attachFile (rhs.m_FilePtr);
      m_Pos = rhs.m_Pos;
      detachVers();
      attachVers (rhs.m_VersPtr);
      m_VerCnt = rhs.m_VerCnt;
      return *this;
    }
    /**
     * @brief move the cursor - where operations take effect
     * 
     * @param offset where to position the cursor in file
     * @return true if offset is valid, false otherwise
     */
    bool                     seek                          ( uint32_t          offset ) {
      if ( offset > m_FilePtr->m_Size || offset < 0 )
        return false;
      m_Pos = offset;
      return true;
    }
    /**
     * @brief read "bytes" number of bytes from file to "dst", starting at cursor position
     * 
     * @param dst destination C-type array
     * @param bytes number of bytes to read
     * @return number of successfully read bytes
     */
    uint32_t                 read                          ( uint8_t         * dst,
                                                             uint32_t          bytes ) {
      size_t i = 0;
      uint32_t r = 0;
      while ( (m_Pos < m_FilePtr -> m_Size) && (r++ < bytes) )
        dst[i++] = m_FilePtr->m_Data[m_Pos++];
      if ( r < bytes )
        return r;
      return bytes;
    }
    /**
     * @brief write bytes into file at cursor position, create a deep copy if rewriting shared data
     * 
     * @param src C-type array of bytes to write
     * @param bytes number of bytes to write
     * @return number of successfully written bytes 
     */
    uint32_t                 write                         ( const uint8_t   * src,
                                                             uint32_t          bytes ) {
      if ( m_FilePtr->m_RefCnt > 1 ) {
        deepCopyTFile();
        copyTVers();
      }
      for ( uint32_t i = 0; i < bytes; i++ ) {
        if ( m_Pos >= m_FilePtr->m_Cap )
          growFile();
        m_FilePtr->m_Data[m_Pos++] = src[i];
        if ( m_Pos > m_FilePtr->m_Size )
          m_FilePtr->m_Size++;
      }
      return bytes;
    }
    /**
     * @brief truncate the file (cuts off the rest) at cursor position
     */
    void                     truncate                      ( void ) {
      if ( m_Pos == m_FilePtr->m_Size )
        return;
      TFileWrap * newFile = new TFileWrap;
      uint8_t * newData = new uint8_t [m_Pos];
      for ( size_t i = 0; i < m_Pos; i++ )
        newData[i] = m_FilePtr->m_Data[i];
      newFile->m_Cap = m_Pos;
      newFile->m_Size = m_Pos;
      newFile->m_Data = newData;
      detachFile();
      m_FilePtr = newFile;
    }
    /**
     * @brief return the file size
     * @return file size 
     */
    uint32_t                 fileSize                      ( void ) const { return m_FilePtr->m_Size; }
    /**
     * @brief save versions of the current state of the file
     */
    void                     addVersion                    ( void ) {
      if ( m_VerCnt >= m_VersPtr->m_VerCap )
        growVersions();
      CFile a;
      a.detachFile();
      a.attachFile(m_FilePtr);
      a.m_Pos = m_Pos;
      m_VersPtr->m_Versions[m_VerCnt++] = a;
    }
    /**
     * @brief revert to the previous version of file
     * 
     * @return false if there are no existing versions, true otherwise
     */
    bool                     undoVersion                   ( void ) {
      if ( ! m_VerCnt )
        return false;
      detachFile ();
      attachFile ( m_VersPtr->m_Versions[m_VerCnt-1].m_FilePtr );
      m_Pos = m_VersPtr->m_Versions[m_VerCnt-1].m_Pos;
      m_VerCnt--;
      return true;
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
  assert ( f4 . seek ( 0 ));
  assert ( readTest ( f4, { 60, 70, 80, 90 }, 4 ) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
