#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

public:
    // constructor, destructor
    CTime () {}
    CTime ( int hour, int min )
    {
        m_Hour = hour;
        m_Minute = min;
        m_Second = 0;
        if ( hour > 23 || hour < 0 || min > 59 || min < 0 )
            throw invalid_argument("Invalid parameters");
    }
    CTime ( int hour, int min, int sec )
    : m_Hour(hour), m_Minute(min), m_Second(sec) {
        if ( hour > 23 || hour < 0 
           || min > 59 || min < 0
           || sec > 59 || sec < 0 )
            throw invalid_argument("Invalid parameters");
    }
    CTime & operator + ( int sec ) {

        m_Second = m_Second + (sec % 60);
        //if ( sec - (sec % 60) > );
        return *this;
    }
    
    CTime & operator += ( int sec ) {
        m_Second = m_Second + sec;
        return *this;
    }
    CTime & operator - ( int sec ) {
        m_Second = m_Second - sec;
        return *this;
    }
    
    CTime & operator -= ( int sec ) {
        m_Second = m_Second - sec;
        return *this;
    }
    CTime & operator ++ ( void ) {
        m_Second += 1;
        return *this;
    }
    CTime & operator -- ( void ) {
        m_Second -= 1;
        return *this;
    }
    friend int operator - ( CTime & time1, CTime & time2 ) {
        int time1Secs = time1.m_Hour*3600 + time1.m_Minute*60 + time1.m_Second;
        int time2Secs = time2.m_Hour*3600 + time2.m_Minute*60 + time2.m_Second;
        return abs(time1Secs - time2Secs);
    }
    // comparison operators
    friend bool operator == ( CTime & t1, CTime & t2 ) {
        return t1.m_Hour == t2.m_Hour && t1.m_Minute == t2.m_Minute && t1.m_Second == t2.m_Second;
    }
    friend bool operator != ( CTime & t1, CTime & t2 ) {
        return ! (t1 == t2);
    }
    friend bool operator < ( CTime & t1, CTime & t2 ) {
        if ( t1.m_Hour > t2.m_Hour )
            return false;
        if ( t1.m_Hour == t2.m_Hour && t1.m_Minute > t2.m_Minute )
            return false;
        if ( t1.m_Hour == t2.m_Hour && t1.m_Minute == t2.m_Minute && t1.m_Second > t2.m_Second )
            return false;
        if ( t1.m_Hour == t2.m_Hour && t1.m_Minute == t2.m_Minute && t1.m_Second == t2.m_Second )
            return false;
        return true;
    }
    friend bool operator > ( CTime & t1, CTime & t2 ) {
        if ( (t1 != t2) && ( ! (t1 < t2) ) )
            return true;
        return false;
    }
    friend bool operator <= ( CTime & t1, CTime & t2 ) {
        if ( t1.m_Hour > t2.m_Hour )
            return false;
        if ( t1.m_Hour == t2.m_Hour && t1.m_Minute > t2.m_Minute )
            return false;
        if ( t1.m_Hour == t2.m_Hour && t1.m_Minute == t2.m_Minute && t1.m_Second > t2.m_Second )
            return false;
        return true;
    }
    friend bool operator >= ( CTime & t1, CTime & t2 ) {
        if ( (t1 == t2) || ( ! (t1 < t2) ) )
            return true;
        return false;
    }
    
    // output operator
    friend std::ostream & operator << ( std::ostream & os, const CTime & time ) {
        os << time.m_Hour << ":" << time.m_Minute << ":" << time.m_Second;
        return os;
    }
    friend class ::CTimeTester;
};

#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        return time.m_Hour == hour 
            && time.m_Minute == minute
            && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b { 13, 30 };
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );
    /*
    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );
    */
    return 0;
}
#endif /* __PROGTEST__ */
