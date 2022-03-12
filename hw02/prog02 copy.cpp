#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CVATRegister
{
  public:
    struct TCompany {
      string m_Name;
      string m_Address;
      string m_Id;
      int    m_InvoiceSum;
      TCompany ( string name, string address, string id = "", int sum = 0 )
      : m_Name(name), m_Address(address), m_Id(id), m_InvoiceSum(sum) {}
    };
    struct TInvoice {
      unsigned int m_Amount;
      TCompany m_Company;
      TInvoice * m_Next;
      TInvoice ( unsigned int amount, TCompany company, TInvoice * next )
      : m_Amount(amount), m_Company(company), m_Next(next) {}
    };
    /*
                  CVATRegister   ( void ) 
                  ~CVATRegister  ( void );
    */
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID ) {
      TCompany company(name, addr, taxID);
      if ( findCompany(company) == -1 ) {
        companies.push_back(company);
        sortCompanies();
        return true;
      }
      return false;
    }
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr ) {
      int found = findCompany( TCompany(name,addr) );
      if ( found == -1 )
        return false;
      companies.erase(companies.begin()+found);
      return true;
    }
    bool          cancelCompany  ( const string    & taxID ) {
      int found = findCompany( TCompany("","",taxID) );
      if ( found == -1 )
        return false;
      companies.erase(companies.begin()+found);
      return true;
    }
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount ) {
      int found = findCompany( TCompany("","",taxID) );
      if ( found == -1 )
        return false;
      createInvoice ( amount, companies[found] );
      m_InvoiceCnt++;
      return true;
    }
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount ) {
      int found = findCompany( TCompany(name,addr,"") );
      if ( found == -1 )
        return false;
      createInvoice ( amount, companies[found] );
      m_InvoiceCnt++;
      return true;
    }
    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const {
      int found = findCompany(TCompany(name,addr,"") );
      if ( found == -1 )
        return false;
      sumIncome = companies[found].m_InvoiceSum;
      return true;
    }
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const {
      int found = findCompany(TCompany("","",taxID) );
      if ( found == -1 )
        return false;
      sumIncome = companies[found].m_InvoiceSum;
      return true;
    }
    static bool cmp ( const TCompany & c1, const TCompany & c2 ) {
      if ( c1.m_Name == c2.m_Name )
        return c1.m_Id < c2.m_Id;
      return c1.m_Name < c2.m_Name;
    }
    void sortCompanies ( void ) {
      sort(companies.begin(), companies.begin(), []( const TCompany & c1, const TCompany & c2 ) { return c1.m_Name < c2.m_Name; } );
    }

    bool          firstCompany   ( string          & name,
                                   string          & addr ) const {
      if ( companies.empty() )
        return false;
      name = companies[0].m_Name;
      addr = companies[0].m_Address;
      return true;
    }
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const;
    unsigned int  medianInvoice  ( void ) const {
      TInvoice * curr = invoices;
      int mid = (m_InvoiceCnt % 2 ) == 1 ? (m_InvoiceCnt / 2) + 1 : m_InvoiceCnt / 2;
      for ( int i = 1; i < mid ; i++ )
        curr = curr->m_Next;
      if ( ( m_InvoiceCnt % 2 ) == 0 ) {
        if ( curr->m_Next && (curr->m_Next->m_Amount > curr->m_Amount) )
          return curr->m_Next->m_Amount;
      }
      return curr->m_Amount;
    }
  private:
    vector<TCompany> companies;
    TInvoice * invoices;
    size_t m_InvoiceCnt;
    string lowerCase ( const string & str ) const {
      string newStr = "";
      for ( char c : str )
        newStr += tolower(c);
      return newStr;
    }
    
    int findCompany ( const TCompany & company ) const {
      for ( size_t i = 0; i < companies.size(); i ++ ) {
        if ( company.m_Id == companies[i].m_Id ||
              ( ( lowerCase(company.m_Name) == lowerCase(companies[i].m_Name) ) && company.m_Address == companies[i].m_Address ) )
          return i;
      }
      return -1;
    }
    /*singly linked LL, sorted ascending*/
    void createInvoice ( unsigned int amount, TCompany& company ) {
      TInvoice * next = nullptr;
      if ( ! invoices ) {
        invoices = new TInvoice ( amount, company, next );
        return;
      }

      TInvoice * curr = invoices;
      TInvoice * currNext = invoices->m_Next;
      while ( currNext && (currNext->m_Amount < amount ) ) {
        currNext = currNext->m_Next;
        curr = curr->m_Next; 
      }
      TInvoice * newInvoice;
      if ( curr->m_Amount > amount ) {
        newInvoice = new TInvoice( amount, company, curr );
      }
      else {
        newInvoice = new TInvoice( amount, company, currNext );
        curr->m_Next = newInvoice;
      }
      company.m_InvoiceSum += amount;
    }
};

#ifndef __PROGTEST__
int               main           ( void )
{
  string name, addr;
  unsigned int sumIncome;

  CVATRegister b1;
  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . invoice ( "666/666", 2000 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  /*
  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . cancelCompany ( "666/666" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . invoice ( "123456", 100 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 300 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 230 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 830 ) );
  assert ( b1 . medianInvoice () == 830 );
  assert ( b1 . invoice ( "123456", 1830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 3200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "123456" ) );
  assert ( ! b1 . firstCompany ( name, addr ) );


  CVATRegister b2;
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . medianInvoice () == 0 );
  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . medianInvoice () == 1000 );
  assert ( b2 . invoice ( "abcdef", 2000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . invoice ( "1234567", 100 ) );
  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . audit ( "1234567", sumIncome ) );
  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . cancelCompany ( "1234567" ) );
  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );
  */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
