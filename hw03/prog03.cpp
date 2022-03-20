#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
  public:
    InvalidDateException ( )
      : invalid_argument ( "invalid date or format" )
    {
    }
};
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
  return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================
class CDate
{
  private:                                  
    int m_Year;
    int m_Month;
    int m_Day;

    static bool isValid ( int month, int day, bool isLeap ) {
      if ( month > 12 || month < 1 )
        return false;
      if ( isLeap && month == 2 && day > 29 )
        return false;
      return day <= daysInMonth(month,isLeap) && day >= 1;
    }
    static bool isLeap ( int year ) {
      if ( year % 400 == 0 )
        return true;
      if ( year % 100 == 0 )
        return false;
      if ( year % 4 == 0 )
        return true;
      return false;
    }
    static int getLeapCnt ( int year ) {
      int cnt = 0;
      for ( int i = 2000; i < year; i++ ) {
        if ( isLeap(i) )
          cnt++;
      }
      return cnt;
    }
    static int daysInMonth ( int month, bool isLeap ) {
      int daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
      if ( isLeap && month == 2 )
        return 29;
      return daysInMonth[month-1];
    }

    void addYears ( int &days ) {
      while ( days >= 366 ) {
        days -= isLeap(m_Year) ? 366 : 365;
        m_Year++;
      }
    }
  public:
    CDate ( int y, int m, int d ) {
      if ( ! isValid ( m, d, isLeap(y) ) )
        throw InvalidDateException();
      else {
        m_Year = y;
        m_Month = m;
        m_Day = d;
      }
    }
    /*calculates days between 1.1.2000 and CDate, last day excluded*/
    static int reneDay ( CDate a ) {
      int day = 0;
      day += (a.m_Year-2000) * 365 + getLeapCnt(a.m_Year);
      bool isCurrYearLeap = isLeap(a.m_Year);
      for ( int i = 1; i < a.m_Month; i++ )
        day += daysInMonth ( i, isCurrYearLeap);
      day += a.m_Day;
      return day-1;
    }
    
    CDate reneToDate ( int days ) {
      CDate a(2000,1,1);
      while ( days >= 366 ) {
        days -= isLeap(a.m_Year) ? 366 : 365;
        a.m_Year++;
      }
      /*days is more than days left in month*/
      while ( days > ( daysInMonth ( a.m_Month, isLeap(a.m_Year) ) - a.m_Day) ) {
        days -= ( daysInMonth ( a.m_Month, isLeap(a.m_Year) ) - a.m_Day + 1);
        if ( a.m_Month == 12 ) {
          a.m_Month = 1;
          a.m_Year++;
        }
        a.m_Day = 1;
        a.m_Month++;
      }
      a.m_Day += days;
      return a;
    }
    //------------------------------------------------------------------------------------
    CDate operator + ( const int addDays ) {
      return reneToDate(reneDay(*this)+addDays);
    }
    //------------------------------------------------------------------------------------
    CDate operator - ( const int addDays ) {
      return reneToDate(reneDay(*this)-addDays);
    }
    //------------------------------------------------------------------------------------
    int operator - ( CDate rhs ) {
      return reneDay(*this) - reneDay(rhs);
    }
    //------------------------------------------------------------------------------------
    friend bool operator == ( const CDate & lhs, const CDate & rhs ) {
      return lhs.m_Day == rhs.m_Day && lhs.m_Month == rhs.m_Month && lhs.m_Year == rhs.m_Year;
    }
    //------------------------------------------------------------------------------------
    friend bool operator < ( const CDate & lhs, const CDate & rhs ) {
      return reneDay(lhs) < reneDay(rhs);
    }
    //------------------------------------------------------------------------------------
    friend bool operator != ( const CDate & lhs, const CDate & rhs ) {
      return ! (lhs == rhs);
    }
    //------------------------------------------------------------------------------------
    friend bool operator > ( const CDate & lhs, const CDate & rhs ) {
      return reneDay(rhs) < reneDay(lhs); 
    }
    //------------------------------------------------------------------------------------
    friend bool operator <= ( const CDate & lhs, const CDate & rhs ) {
      return ! ( lhs > rhs );
    }
    //------------------------------------------------------------------------------------
    friend bool operator >= ( const CDate & lhs, const CDate & rhs ) {
      return ! ( lhs < rhs );
    }
    //------------------------------------------------------------------------------------
    CDate& operator ++ ( void ) {
      *this = reneToDate(reneDay(*this+1));
      return *this;
    }
    //------------------------------------------------------------------------------------
    CDate operator ++ ( int ) {
      CDate tmp = *this;
      *this =  reneToDate(reneDay(*this+1));
      return tmp;
    }
    //------------------------------------------------------------------------------------
    CDate& operator -- ( void ) {
      *this = reneToDate(reneDay(*this-1));
      return *this;
    }
    //------------------------------------------------------------------------------------
    CDate operator -- ( int ) {
      CDate tmp = *this;
      *this = reneToDate(reneDay(*this-1));
      return tmp;
    }
    //------------------------------------------------------------------------------------
    friend ostream & operator << ( ostream & os, const CDate & rhs ) {
      os << rhs.m_Year << "-";
      if ( rhs.m_Month <= 9 )
        os << "0";
      os << rhs.m_Month << "-";
      if ( rhs.m_Day <= 9 )
        os << "0";
      os << rhs.m_Day;
      return os;
    }
    //------------------------------------------------------------------------------------
    friend istream & operator >> ( istream & is, CDate & lhs) {
      int year;
      char dummy;
      int month;
      int day;
      is >> year >> dummy;
      if ( dummy != '-' ) {
        is.setstate(ios::failbit);
        return is;
      }
      is >> month >> dummy;
      if ( dummy != '-') {
        is.setstate(ios::failbit);
        return is;
      }
      is >> day;
      if ( ! isValid (month, day, isLeap(year) ) ) {
        is.setstate(ios::failbit);
        return is;
      }
      lhs.m_Year = year;
      lhs.m_Month = month;
      lhs.m_Day = day;
      return is;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
  ostringstream oss;
  istringstream iss;

  CDate a ( 2000, 1, 2 );
  CDate b ( 2010, 2, 3 );
  CDate c ( 2004, 2, 10 );
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2000-01-02" );
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2010-02-03" );
  oss . str ("");
  oss << c;
  assert ( oss . str () == "2004-02-10" );
  a = a + 1500;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2004-02-10" );
  b = b - 2000;
  oss . str ("");
  oss << b;
  assert ( oss . str () == "2004-08-13" );
  assert ( b - a == 185 );
  assert ( ( b == a ) == false );
  assert ( ( b != a ) == true );
  assert ( ( b <= a ) == false );
  assert ( ( b < a ) == false );
  assert ( ( b >= a ) == true );
  assert ( ( b > a ) == true );
  assert ( ( c == a ) == true );
  assert ( ( c != a ) == false );
  assert ( ( c <= a ) == true );
  assert ( ( c < a ) == false );
  assert ( ( c >= a ) == true );
  assert ( ( c > a ) == false );
  a = ++c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-11" );
  a = --c;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-10" );
  a = c++;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-10 2004-02-11" );
  a = c--;
  oss . str ( "" );
  oss << a << " " << c;
  assert ( oss . str () == "2004-02-11 2004-02-10" );
  iss . clear ();
  iss . str ( "2015-09-03" );
  assert ( ( iss >> a ) );
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-09-03" );
  a = a + 70;
  oss . str ("");
  oss << a;
  assert ( oss . str () == "2015-11-12" );

  CDate d ( 2000, 1, 1 );
  try
  {
    CDate e ( 2000, 32, 1 );
    assert ( "No exception thrown!" == nullptr );
  }
  catch ( ... )
  {
  }
  iss . clear ();
  iss . str ( "2000-12-33" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-11-31" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-01-01" );
  iss . clear ();
  iss . str ( "2000-02-29" );
  assert ( ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );
  iss . clear ();
  iss . str ( "2001-02-29" );
  assert ( ! ( iss >> d ) );
  oss . str ("");
  oss << d;
  assert ( oss . str () == "2000-02-29" );
  
  CDate f ( 2010, 2, 3 );
  f = f + 534789;
  oss . str ("");
  oss << f;
  assert ( oss . str () == "3474-04-18" );

  CDate g ( 2028, 12, 31 );
  cout << g.reneToDate(g.reneDay(g)+52349) << endl;

  CDate j ( 2028, 1, 31 );
  CDate p ( 2028, 1, 31 );
  cout << "reneDay j " << j.reneDay(j) << endl;
  cout << g - j << endl;
  cout << "minusko " << g + (-335) << endl;
  cout << "minusko " << g - 335 << endl;
  assert ( j <= p );
  assert ( j <= g );
  
  /*
  //-----------------------------------------------------------------------------
  // bonus test examples
  //-----------------------------------------------------------------------------
  CDate f ( 2000, 5, 12 );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  oss . str ("");
  oss << date_format ( "%Y/%m/%d" ) << f;
  assert ( oss . str () == "2000/05/12" );
  oss . str ("");
  oss << date_format ( "%d.%m.%Y" ) << f;
  assert ( oss . str () == "12.05.2000" );
  oss . str ("");
  oss << date_format ( "%m/%d/%Y" ) << f;
  assert ( oss . str () == "05/12/2000" );
  oss . str ("");
  oss << date_format ( "%Y%m%d" ) << f;
  assert ( oss . str () == "20000512" );
  oss . str ("");
  oss << date_format ( "hello kitty" ) << f;
  assert ( oss . str () == "hello kitty" );
  oss . str ("");
  oss << date_format ( "%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%" ) << f;
  assert ( oss . str () == "121212121212050505200020002000%%%%%" );
  oss . str ("");
  oss << date_format ( "%Y-%m-%d" ) << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-01-1" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-1-01" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-001-01" );
  assert ( ! ( iss >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2000-05-12" );
  iss . clear ();
  iss . str ( "2001-01-02" );
  assert ( ( iss >> date_format ( "%Y-%m-%d" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2001-01-02" );
  iss . clear ();
  iss . str ( "05.06.2003" );
  assert ( ( iss >> date_format ( "%d.%m.%Y" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2003-06-05" );
  iss . clear ();
  iss . str ( "07/08/2004" );
  assert ( ( iss >> date_format ( "%m/%d/%Y" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2004-07-08" );
  iss . clear ();
  iss . str ( "2002*03*04" );
  assert ( ( iss >> date_format ( "%Y*%m*%d" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2002-03-04" );
  iss . clear ();
  iss . str ( "C++09format10PA22006rulez" );
  assert ( ( iss >> date_format ( "C++%mformat%dPA2%Yrulez" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2006-09-10" );
  iss . clear ();
  iss . str ( "%12%13%2010%" );
  assert ( ( iss >> date_format ( "%%%m%%%d%%%Y%%" ) >> f ) );
  oss . str ("");
  oss << f;
  assert ( oss . str () == "2010-12-13" );

  CDate g ( 2000, 6, 8 );
  iss . clear ();
  iss . str ( "2001-11-33" );
  assert ( ! ( iss >> date_format ( "%Y-%m-%d" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "29.02.2003" );
  assert ( ! ( iss >> date_format ( "%d.%m.%Y" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "14/02/2004" );
  assert ( ! ( iss >> date_format ( "%m/%d/%Y" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "2002-03" );
  assert ( ! ( iss >> date_format ( "%Y-%m" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "hello kitty" );
  assert ( ! ( iss >> date_format ( "hello kitty" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "2005-07-12-07" );
  assert ( ! ( iss >> date_format ( "%Y-%m-%d-%m" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-06-08" );
  iss . clear ();
  iss . str ( "20000101" );
  assert ( ( iss >> date_format ( "%Y%m%d" ) >> g ) );
  oss . str ("");
  oss << g;
  assert ( oss . str () == "2000-01-01" );
  */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
