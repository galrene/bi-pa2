#include "CMenu.h"
#include "CGame.h"
int startMenu ( CGameStateManager & gsm ) {
    CMenu menu;
    return menu.handleMainMenu ( gsm );
}

int main ( int argc, char const *argv[] ) {
    CGameStateManager gsm;
    while ( startMenu ( gsm ) ) { 
        CGame game ( gsm );
        game.beginGame();
    }
    /*
    CGame game;
    game.beginGame();
    */
    return 0;
}