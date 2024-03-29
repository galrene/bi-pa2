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
    shared_ptr<CDataType> m_Ptr;
    virtual ~CDataType ( void ) noexcept = default;
    bool operator == ( const CDataType & rhs ) const {
      return isEqual ( rhs );
    }
    bool operator != ( const CDataType & rhs ) const {
      return ! isEqual ( rhs );
    }
    friend ostream & operator << ( ostream & os, const CDataType & x ) {
      x.print ( os );
      return os;
    }
    virtual size_t getSize ( void ) const = 0;
    virtual void create ( void ) = 0;
    virtual bool isEqual ( const CDataType & rhs ) const = 0;
    virtual void print ( ostream & os ) const = 0;
};
class CDataTypeInt : public CDataType
{
  public:
    CDataTypeInt ( void ) {
      m_Type = "int";
    }
    virtual void create ( void ) override {
      m_Ptr = make_shared<CDataTypeInt> (*this);
    }
    virtual size_t getSize ( void ) const override { return 4; }
    virtual bool isEqual ( const CDataType & rhs ) const override {
      return m_Type == rhs.m_Type;
    }
    virtual void print ( ostream & os ) const override {
      os << m_Type;
      return;
    }
};
class CDataTypeDouble : public CDataType
{
  public:
    CDataTypeDouble ( void ) {
      m_Type = "double";
    }
    virtual void create ( void ) override {
      m_Ptr = make_shared<CDataTypeDouble> (*this);
    }
    virtual size_t getSize ( void ) const override { return 8; }
    virtual bool isEqual ( const CDataType & rhs ) const override {
      return m_Type == rhs.m_Type;
    }
    virtual void print ( ostream & os ) const override {
      os << m_Type;
      return;
    }
};
class CDataTypeEnum : public CDataType
{
  public:
    vector<string> m_Content;
    CDataTypeEnum ( void ) {
      m_Type = "enum";
    }
    virtual void create ( void ) override {
      m_Ptr = make_shared<CDataTypeEnum> (*this);
    }
    virtual size_t getSize ( void ) const override { return 4; }
    auto linearSearch ( string & name ) {
      for ( auto it = m_Content.begin(); it != m_Content.end(); ++it ) {
        if ( *it == name )
          return it;
      }
      return m_Content.end();
    }

    CDataTypeEnum & add ( string name ) {
      if ( linearSearch ( name ) != m_Content.end() ) {
        string excMessage = "Duplicate enum value: " + name;
        throw invalid_argument ( excMessage );
      }
      m_Content.emplace_back ( name );
      return *this;
    }
    bool contentsAreEqual ( const CDataTypeEnum * rhs ) const {
      if ( m_Content.size() != rhs->m_Content.size() )
        return false;
      for ( auto itA = m_Content.begin(), itB = rhs->m_Content.begin(); itA != m_Content.end() && itB != rhs->m_Content.end() ; ++itA, ++itB ) {
          if ( *itA != *itB )
            return false;
      }
      return true;
    }
    virtual bool isEqual ( const CDataType & rhs ) const override {
      if ( m_Type != rhs.m_Type )
        return false;
      CDataType * x = const_cast<CDataType*> (&rhs);
      CDataTypeEnum * a = dynamic_cast<CDataTypeEnum*> (x);
      return contentsAreEqual(a);
    }
    virtual void print ( ostream & os ) const override {
      os << m_Type << "\n  {";
      os << "\n";
      for ( const auto & x : m_Content ) {
        os << "    " << x;
        if ( x != *(--(m_Content.end())) )
          os << ",\n";
      }
      os << "\n  }";
      return;
    }
};
class CDataTypeStruct : public CDataType
{
  private:
    vector<shared_ptr<CDataType>> m_Content;
    auto findItem ( string name ) const {
      for ( auto it = m_Content.begin(); it != m_Content.end(); ++it ) {
        if ( (*(*it) ).m_Name == name )
          return it;
      }
      return m_Content.end();
    }
  public:
    CDataTypeStruct ( void ) {
      m_Type = "struct";
    }
    CDataTypeStruct ( const CDataTypeStruct & a ) {
      m_Content = a.m_Content;
      m_Name = a.m_Name;
    }
    CDataTypeStruct & operator = ( const CDataTypeStruct & rhs ) {
      m_Content = rhs.m_Content;
      m_Name = rhs.m_Name;
      return *this;
    }
    virtual void create ( void ) override {
      m_Ptr = make_shared<CDataTypeStruct> (*this);
    }
    virtual size_t getSize ( void ) const override {
      size_t wholeSize = 0;
      for ( const auto & x : m_Content )
        wholeSize += x->getSize();
      return wholeSize;
    }

    CDataTypeStruct & addField ( string name, const CDataType & type ) {
      auto found = findItem ( name );
      if ( found != m_Content.end() ) {
        string excMess = "Duplicate field: " + name;
        throw invalid_argument ( excMess );
      }
      CDataType * x = const_cast<CDataType*> (&type);
      x->create();
      x->m_Ptr->m_Name = name;
      m_Content.push_back ( x->m_Ptr );
      return *this;
    }

    CDataType & field ( string name ) const {
      auto found = findItem(name);
      if ( found == m_Content.end() ) {
        string excMess = "Unknown field: " + name;
        throw invalid_argument ( excMess );
      }
      return *(*found);
    }
    bool contentsAreEqual ( CDataTypeStruct *& rhs ) const {
      auto itA = m_Content.begin();
      auto itB = rhs->m_Content.begin();
      if ( m_Content.size() != rhs->m_Content.size() )
        return false;
      for ( ; itA != m_Content.end() && itB != rhs->m_Content.end(); ++itA, ++itB ) {
          if ( *(*itA) != *(*itB) )
            return false;
      }
      return true;
    }
    virtual bool isEqual ( const CDataType & rhs ) const override {
      if ( m_Type != rhs.m_Type ) return false;
      CDataType * x = const_cast<CDataType*> (&rhs);
      CDataTypeStruct * a = dynamic_cast<CDataTypeStruct*> (x);
      return contentsAreEqual(a);
    }
    virtual void print ( ostream & os ) const override {
      os << "struct\n{\n";
      for ( const auto & x : m_Content ) {
        os << "  " << *x;
        os << " " << x->m_Name << ";";
        if ( x != *(--(m_Content.end())) )
          os << "\n";
      }
      os << "\n}";
      return;
    }
};
#ifndef __PROGTEST__
static bool        whitespaceMatch                         ( const string    & a,
                                                             const string    & b )
{
  stringstream mine ( a );
  stringstream ref ( b );
  string strA;
  string strB;
  while ( 1 ) {
    mine >> strA;
    ref >> strB;
    if ( strB != strA )
      return false;
    if ( ! mine.good() && !ref.good() )
      break;
  }
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

  CDataTypeStruct  a = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
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
  cout << a << endl;
  cout << b << endl;
  cout << c << endl;
  cout << d << endl;
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

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
