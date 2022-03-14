#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;


void genInvoices ( int cnt, string companyId ) {
    ofstream ofs("newCompanies.txt",ios::app);
    for ( int i = 0; i < cnt; i ++ ) {
        ofs << "assert ( b2 . invoice (" << '\"' << companyId << '\"' << ',' << random()%10000000 << " ) );" << endl;
    }
}

void genCompaniesAndInvoices ( int cnt ) {
    ofstream ofs("newCompanies.txt");
    string companyId = "jozko";
    string address = "vajda";
    string pool = "abcdefghijklmnopqrstvuzx";
    for ( int i = 0; i < cnt; i ++ ) {
        companyId += (pool[0] + ( random() % 24 ) );
        address += (pool[0] + ( random() % 24 ) );
        ofs << "assert ( b2 . newCompany ( \"ACME\"," << '\"' <<address << '\"' << ',' << '\"' << companyId << '\"' << " ) );" << endl;
        genInvoices(cnt, companyId);
    }
}

int main ( void ) {
    genCompaniesAndInvoices(150);
    return 0;
}