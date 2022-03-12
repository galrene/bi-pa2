#include <iostream>
using namespace std;
int main ( void ) {
    string companyId = "jozko";
    string address = "vajda";
    string pool = "abcdefghijklmnopqrstvuzx";
    for ( int i = 0; i < 1500; i ++ ) {
        
        companyId += (pool[0] + ( random() % 24 ) );
        address += (pool[0] + ( random() % 24 ) );
        cout << "assert ( b2 . newCompany ( \"ACME\"," << '\"' <<address << '\"' << ',' << '\"' << companyId << '\"' << " ) );" << endl;
    }
  
}