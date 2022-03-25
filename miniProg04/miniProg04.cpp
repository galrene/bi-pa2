#ifndef __PROGTEST__
#include <cstring>
using namespace std;

class CLinkedSetTester;
#endif /* __PROGTEST__ */


class CLinkedSet
{
private:
    struct CNode
    {
        char * m_Value;
        CNode * m_Next;
        const char * Value () const {
            return m_Value;
        }
        CNode ( char * value, CNode * next )
        : m_Value(value), m_Next(next) {}
    };

    CNode * m_Begin;
public:
    // default constructor
    CLinkedSet () {
        m_Begin = nullptr;
    }
    // copy constructor
    CLinkedSet ( const CLinkedSet & src ) {
        m_Begin = nullptr;
        CNode * currSrc = src.m_Begin;
        CNode * curr;
        while ( currSrc ) {
            if ( ! m_Begin ) {
                m_Begin = new CNode(new char [ strlen (currSrc->m_Value) + 1 ], nullptr);
                curr = m_Begin;
                strcpy(curr->m_Value, currSrc->m_Value);
                currSrc = currSrc->m_Next;
                continue;
            }
            else
                curr->m_Next = new CNode(new char [ strlen (currSrc->m_Value) + 1 ], nullptr);
            strcpy(curr->m_Next->m_Value, currSrc->m_Value);
            curr = curr->m_Next;
            currSrc = currSrc->m_Next;
        }
    }
    // operator=
    CLinkedSet& operator = ( const CLinkedSet & src ) {
        if ( &src == this ) return *this;
        CNode * curr = m_Begin;
        CNode * currSrc = src.m_Begin;
        while ( curr ) {
            CNode * next = curr->m_Next;
            delete [] curr->m_Value;
            delete curr;
            curr = next;
        }
        m_Begin = nullptr;
        currSrc = src.m_Begin;
        while ( currSrc ) {
            if ( ! m_Begin ) {
                m_Begin = new CNode(new char [ strlen (currSrc->m_Value) + 1 ], nullptr);
                curr = m_Begin;
                strcpy(curr->m_Value, currSrc->m_Value);
                currSrc = currSrc->m_Next;
                continue;
            }
            else
                curr->m_Next = new CNode(new char [ strlen (currSrc->m_Value) + 1 ], nullptr);
            strcpy(curr->m_Next->m_Value, currSrc->m_Value);
            curr = curr->m_Next;
            currSrc = currSrc->m_Next;
        }
        return *this;
    }

    // destructor
    ~CLinkedSet ( void ) {
        while ( m_Begin ) {
            CNode * curr = m_Begin->m_Next;
            delete [] m_Begin->m_Value;
            delete m_Begin;
            m_Begin = curr;
        }
    }

    bool Insert ( const char * value ) {
        if ( ! m_Begin ) {
            m_Begin = new CNode ( new char [strlen(value)+1], nullptr );
            strcpy(m_Begin->m_Value,value);
            return true;
        }
        CNode * curr = m_Begin;
        CNode * correctPos = m_Begin;
        while ( curr->m_Next ) {
            if ( ! strcmp ( curr->m_Value, value ) )
                return false;
            else if ( strcmp( curr->m_Value, value ) < 0 )
                correctPos = correctPos->m_Next;
            curr = curr->m_Next;
        }

        if ( strcmp( curr->m_Value, value ) < 0 ) {
            correctPos->m_Next = new CNode ( new char [strlen(value)+1], correctPos->m_Next );
            correctPos = correctPos->m_Next;
        }
            
        else {
            correctPos = new CNode ( new char [strlen(value)+1], correctPos );
            m_Begin = correctPos;
        }
        strcpy(correctPos->m_Value, value);
        return true;
    }
    
    bool Remove ( const char * value ) {
        if ( Empty() ) return false;
        CNode * curr = m_Begin;
        CNode * currNext = m_Begin->m_Next;
        
        while ( currNext ) {
            if ( ! strcmp ( currNext->m_Value, value ) ) {
                if ( m_Begin == curr ) {
                    m_Begin = currNext->m_Next;
                }
                curr = currNext->m_Next;
                delete [] currNext->m_Value;
                delete currNext;
                return true;
            }
            curr = curr->m_Next;
            currNext = currNext->m_Next;
        }
        /*check first node*/
        if ( ! strcmp ( m_Begin->m_Value, value ) )  {
            CNode * next = m_Begin->m_Next;
            delete [] m_Begin->m_Value;
            delete m_Begin;
            m_Begin = next;
            return true;
        }
        return false;
    }

    bool Empty () const {
        return m_Begin == nullptr;
    }

    size_t Size () const {
        size_t cnt = 0;
        CNode * curr = m_Begin;
        while ( curr ) {
            cnt++;
            curr = curr->m_Next;
        }
        return cnt;
    }

    bool Contains ( const char * value ) const {
        CNode * curr = m_Begin;
        while ( curr ) {
            if ( ! strcmp ( curr->m_Value, value ) )
                return true;
            curr = curr->m_Next;
        } 
        return false;
    }

    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__
#include <cassert>

struct CLinkedSetTester
{
    static void test0()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        assert( !x0 . Empty() );
    }

    static void test1()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CLinkedSet x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CLinkedSet x0;
        CLinkedSet x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
        x0 = x1;
        assert( x0 . Contains( "Pavlik Ales" ) );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( ! x0 . Contains( "Vodickova Saskie" ) );
    }

};

int main ()
{
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    return 0;
}
#endif /* __PROGTEST__ */
