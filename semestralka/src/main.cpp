#include "CMenu.h"

int main ( int argc, char const *argv[] ) {
    CMenu menu;
    while ( menu.handleMainMenu () ) {}
    return 0;
}