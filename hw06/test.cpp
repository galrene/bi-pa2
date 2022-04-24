#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
class CDataType {
  public:
    string m_Type;
    string m_Name;
    CDataType * m_Ptr;
    virtual ~CDataType ( void ) noexcept = default;

    bool operator == ( CDataType & rhs ) {
      return isEqual ( rhs );
    }
    bool operator != ( CDataType & rhs ) {
      return ! isEqual ( rhs );
    }
    friend ostream & operator << ( ostream & os, CDataType & x ) {
      x.print ( os );
      return os;
    }
    virtual size_t getSize ( void ) = 0;
    virtual bool isEqual ( CDataType & rhs ) = 0;
    virtual void print ( ostream & os ) = 0;
    virtual void createPtr ( string name ) = 0;
};
class CDataTypeInt : public CDataType
{
  public:
    CDataTypeInt ( void ) {
      m_Type = "CDataTypeInt";
    }
    virtual void createPtr ( string name ) override {
      m_Ptr = new CDataTypeInt();
      m_Name = name;
    }
    virtual size_t getSize ( void ) override { return 4; }
    virtual bool isEqual ( CDataType & rhs ) override {
      return m_Type == rhs.m_Type;
      // try this return typeid ( rhs ) == typeid ( *this)
    }
    virtual void print ( ostream & os ) override {
      os << m_Type;
      return;
    }
};
class CDataTypeDouble : public CDataType
{
  public:
    CDataTypeDouble ( void ) {
      m_Type = "CDataTypeDouble";
    }
    virtual void createPtr ( string name ) override {
      m_Ptr = new CDataTypeDouble();
      m_Ptr->m_Name = name;
    }
    virtual size_t getSize ( void ) override { return 8; }
    virtual bool isEqual ( CDataType & rhs ) override {
      return m_Type == rhs.m_Type;
    }
    virtual void print ( ostream & os ) override {
      os << m_Type;
      return;
    }
};
class CDataTypeEnum : public CDataType
{
  protected:
    set<string> m_Content;
  public:
    CDataTypeEnum ( void ) {
      m_Type = "CDataTypeEnum";
    }
    virtual size_t getSize ( void ) override { return 4; }
    CDataTypeEnum & add ( string name ) {
      if ( ! m_Content.insert(name).second ) {
        string excMessage = "Duplicate enum value: " + name;
        throw invalid_argument ( excMessage );
      }
      return *this;
    }
    virtual void createPtr ( string name ) override {
      m_Ptr = new CDataTypeEnum();
      m_Ptr->m_Name = name;
    }
    virtual bool isEqual ( CDataType & rhs ) override {
      return m_Type == rhs.m_Type;
    }
    virtual void print ( ostream & os ) override {
      os << m_Type;
      return;
    }
};
class CDataTypeStruct : public CDataType
{
  private:
    void decideType ( CDataType & type ) {
      if ( type.m_Type == "CDataTypeInt" )
        type.m_Ptr = dynamic_cast<CDataTypeInt*> (type.m_Ptr);
      else if ( type.m_Type == "CDataTypeDouble" )
        type.m_Ptr = dynamic_cast<CDataTypeDouble*> (type.m_Ptr);
      else if ( type.m_Type == "CDataTypeEnum" )
        type.m_Ptr = dynamic_cast<CDataTypeEnum*> (type.m_Ptr);
    }
  public:
    CDataTypeStruct ( void ) {
      m_Type = "CDataTypeStruct";
    }
    // asi by mal byt vector, pretoze potrebujem zachovavat poradie
    vector<CDataType*> m_Content;
    virtual size_t getSize ( void ) override {
      size_t wholeSize = 0;
      for ( const auto & x : m_Content )
        wholeSize += x->getSize();
      return wholeSize;
    }
    CDataTypeStruct & addField ( string name, CDataType type ) {
      type.createPtr ( name );
      decideType(type);
      m_Content.push_back ( type.m_Ptr );
      return *this;
    }
    CDataType * field ( string name ) {
      auto found = find_if ( m_Content.begin(), m_Content.end(), [name]  ( CDataType *& a ) {
        return a->m_Type == name;
      });
      if ( found == m_Content.end() ) {
        string excMess = "Unknown field: " + name;
        throw invalid_argument ( excMess );
      }
      return *found;
    }
    virtual void createPtr ( string name ) override;
    bool contentsAreEqual ( CDataType & rhs );
    virtual bool isEqual ( CDataType & rhs ) override {
      return m_Type == rhs.m_Type;
    }
    virtual void print ( ostream & os ) override {
      os << m_Type;
      return;
    }
};
#ifndef __PROGTEST__
static bool        whitespaceMatch                         ( const string    & a,
                                                             const string    & b )
{
  // todo
  return true;
}
template <typename T_>
static bool        whitespaceMatch                         ( const T_        & x,
                                                             const string    & ref )
{
  ostringstream oss;
  oss << x;
  return whitespaceMatch ( oss . str (), ref );
}
int main ( void )
{
  CDataTypeStruct a = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  /*
  CDataTypeStruct b = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "READY" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct c =  CDataTypeStruct () .
                        addField ( "m_First", CDataTypeInt () ) .
                        addField ( "m_Second", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Third", CDataTypeDouble () );

  CDataTypeStruct  d = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeInt () );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "}") );

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "}") );

  assert ( a != b );
  assert ( a == c );
  assert ( a != d );
  assert ( a . field ( "m_Status" ) == CDataTypeEnum () . add ( "NEW" ) . add ( "FIXED" ) . add ( "BROKEN" ) . add ( "DEAD" ) );
  assert ( a . field ( "m_Status" ) != CDataTypeEnum () . add ( "NEW" ) . add ( "BROKEN" ) . add ( "FIXED" ) . add ( "DEAD" ) );
  assert ( a != CDataTypeInt() );
  assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}") );

  CDataTypeStruct aOld = a;
  b . addField ( "m_Other", CDataTypeDouble ());

  a . addField ( "m_Sum", CDataTypeInt ());

  assert ( a != aOld );
  assert ( a != c );
  assert ( aOld == c );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  double m_Other;\n"
    "}") );

  c . addField ( "m_Another", a . field ( "m_Status" ));

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Another;\n"
    "}") );

  d . addField ( "m_Another", a . field ( "m_Ratio" ));

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "  double m_Another;\n"
    "}") );

  assert ( a . getSize () == 20 );
  assert ( b . getSize () == 24 );
  try
  {
    a . addField ( "m_Status", CDataTypeInt () );
    assert ( "addField: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate field: m_Status"sv );
  }

  try
  {
    cout << a . field ( "m_Fail" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Unknown field: m_Fail"sv );
  }

  try
  {
    CDataTypeEnum en;
    en . add ( "FIRST" ) .
         add ( "SECOND" ) .
         add ( "FIRST" );
    assert ( "add: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate enum value: FIRST"sv );
  }
  */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
