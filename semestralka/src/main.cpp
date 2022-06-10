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
        if ( res == 1 ) { // created a game
            if ( ! game.beginGame() )
                return 0;
        }
        else if ( res == 2 ) { // loaded game from savefile
            if ( ! game.continueGame() )
                return 0;
        }
    }
    return 0;
}