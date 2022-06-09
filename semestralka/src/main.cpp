#include "CMenu.h"
#include "CGame.h"
int startMenu ( CGameStateManager & gsm ) {
    CMenu menu;
    return menu.handleMainMenu ( gsm );
}

int main ( int argc, char const *argv[] ) {
    CGameStateManager gsm;
    int res;
    while ( ( res = startMenu ( gsm ) ) != -1  ) { 
        CGame game ( gsm );
        if ( res == 1 ) {
            if ( ! game.beginGame() )
                return 0;
        }
        else if ( res == 2 ) {
            if ( ! game.continueGame() )
                return 0;
        }
    }
    return 0;
}