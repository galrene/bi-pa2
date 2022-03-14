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
    
                  CVATRegister   (  ) {}
                  ~CVATRegister  ( void ) {
                  }
    /*O(n)*/
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID ) {
      TCompany company = TCompany(name, addr, taxID);
      if ( name.empty() || addr.empty() || taxID.empty() )
        return false;
      if ( findCompany( company ) != -1 || findCompanyById( company ) != -1 )
        return false;

      companies.insert(upper_bound(companies.begin(), companies.end(), company), company);
      companiesById.insert(upper_bound(companiesById.begin(), companiesById.end(), company, [] ( const TCompany & c1, const TCompany & c2 ) { return c1.m_Id < c2.m_Id; } ), company);
      return true;
    }
    /*O (n) */
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr ) {
      int found = findCompany( TCompany(name,addr) );
      if ( found == -1 )
        return false;
      int found2 = findCompanyById(TCompany("","",companies[found].m_Id) );
      companiesById.erase(companiesById.begin()+found2);
      companies.erase(companies.begin()+found);
      return true;
    }
    bool          cancelCompany  ( const string    & taxID ) {
      int found = findCompany( TCompany("","",taxID) );
      if ( found == -1 )
        return false;
      int found2 = findCompany(TCompany(companiesById[found].m_Name,companiesById[found].m_Address,"") );
      companies.erase(companies.begin()+found2);
      companiesById.erase(companiesById.begin()+found);
      return true;
    }
    /*O(log(n)) */
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount ) {
      int found = findCompany( TCompany("","",taxID) );
      if ( found == -1 )
        return false;
      int found2 = findCompany( TCompany(companiesById[found].m_Name,companiesById[found].m_Address,"") );
      m_Invoices.push_back(amount);
      companies[found2].m_InvoiceSum += amount;
      companiesById[found].m_InvoiceSum += amount;
      return true;
    }
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount ) {
      int found = findCompany( TCompany(name,addr,"") );
      if ( found == -1 )
        return false;
      int found2 = findCompany( TCompany("","",companies[found].m_Id) );
      m_Invoices.push_back(amount);
      companies[found].m_InvoiceSum += amount;
      companiesById[found2].m_InvoiceSum += amount;
      return true;
    }
    /*O ( log(n)*/
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
      sumIncome = companiesById[found].m_InvoiceSum;
      return true;
    }
    /*O(1) */
    bool          firstCompany   ( string          & name,
                                   string          & addr ) const {
      if ( companies.empty() )
        return false;
      name = companies[0].m_Name;
      addr = companies[0].m_Address;
      return true;
    }
    /*O(log(n)) */
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const {
      int found =  findCompany ( TCompany (name, addr, "" ) );
      if ( found + 1 >= static_cast<int>(companies.size()) || found == -1 )
        return false;
      name = companies[found+1].m_Name;
      addr = companies[found+1].m_Address;
      return true;
    }
    /*O(n*log(n)) */
    unsigned int  medianInvoice  ( void ) const {
      if ( m_Invoices.empty() )
        return 0;
      size_t mid = (m_Invoices.size() - 1) / 2;
      vector<unsigned int> tempInvoices;
      for ( size_t i = 0; i < m_Invoices.size(); i++ )
        tempInvoices.push_back(m_Invoices[i]);
      sort(tempInvoices.begin(),tempInvoices.end());
      if ( ( m_Invoices.size() % 2 ) == 0 ) {
        if ( mid+1 < m_Invoices.size() && tempInvoices[mid] < tempInvoices[mid+1] )
          return tempInvoices[mid+1];
      }
      return tempInvoices[mid];
    }


  private:
    vector<TCompany> companies;
    vector<TCompany> companiesById;
    vector<unsigned int> m_Invoices;
    static string lowerCase ( const string & str ) {
      string newStr = "";
      for ( char c : str )
        newStr += tolower(c);
      return newStr;
    }
    friend bool operator == ( const TCompany& lhs, const TCompany& rhs ) { 
      return (lhs.m_Id == rhs.m_Id) || (lowerCase(lhs.m_Address) == lowerCase(rhs.m_Address) && lowerCase(lhs.m_Name) == lowerCase(rhs.m_Name)); }
    friend bool operator < ( const TCompany& lhs, const TCompany& rhs ) { 
        if (lowerCase(lhs.m_Name) == lowerCase(rhs.m_Name)) return lowerCase(lhs.m_Address) < lowerCase(rhs.m_Address);
        return lowerCase(lhs.m_Name) < lowerCase(rhs.m_Name);
      }
    /*O(log(n))*/
    int findCompany ( const TCompany & company ) const {
      if ( company.m_Id.empty() && (company.m_Name.empty() || company.m_Address.empty() ) )
        return -1;
      if ( company.m_Address.empty() )
        return findCompanyById(company);
      int lo = 0; 
      int hi = companies.size();
      while ( lo < hi ) {
        int mid = (lo + hi) / 2;
        if ( company == companies[mid] ) 
          return mid;
        if ( company < companies[mid] )
          hi = mid;
        else
          lo = mid + 1;
      }
      return -1;
    }

    /*O(log(n))*/
    int findCompanyById ( const TCompany & company ) const {
      int lo = 0; 
      int hi = companiesById.size();
      while ( lo < hi ) {
        int mid = (lo + hi) / 2;
        if ( company.m_Id == companiesById[mid].m_Id ) 
          return mid;
        if ( company.m_Id < companiesById[mid].m_Id )
          hi = mid;
        else
          lo = mid + 1;
      }
      return -1;
    }
};

#ifndef __PROGTEST__
/**
 * prerobit invoice na efektivnejsi
 * newCompany spravit cez sort a pushback asi
 *
 */
int               main           ( void )
{
  string name, addr;
  unsigned int sumIncome;
  string jozo = "";
  string vajda = "\n";
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

  assert (b1.newCompany ( "Tjictaqcbcscca", "Ojvkvmgmziiad", "Kfpolgznzkka" ));
  assert (! b1.newCompany ( "Tjictaqcbcscca", "Ojvkvmgmziiad", "Kfpolgznzkka" ));

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

  CVATRegister b3;
  assert ( b3.newCompany( "penis", "abcde", "182" ) );
  assert ( b3.newCompany( "penis", "abcd", "123" ) );
  assert ( b3.newCompany( "microsoft", "bratislavska", "2" ) );
  assert ( b3.newCompany( "comix", "bratislavska", "3" ) );
  assert ( b3.newCompany( "vajda", "jozo", "4" ) );
  assert ( ! b3.newCompany( "penis", "abcde", "1823" ) );
  assert ( ! b3.newCompany( "", "jozo", "4" ) );
  assert ( b3.invoice( "182", 43) );
  assert ( ! b3.invoice( "1", 43) );
  assert ( ! b3.invoice( "vajda", "jozef", 43) );
  assert ( ! b3.invoice( "vajda", "jozef", 43) );  
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
