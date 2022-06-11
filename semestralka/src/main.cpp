#include "CMenu.h"
#include "CGame.h"
#include <cassert>

#ifdef TEST
int main ( void ) {
    CConfigParser parser ( fs::current_path() / "tests", fs::current_path() / "tests" );
    map<string,shared_ptr<CCard>> brokenCards = parser.loadCards ( "cards_broken" );
    assert ( brokenCards.empty() == true );
    
    CGameStateManager gsm;
    parser.setPath ( fs::current_path() / "tests" / "broken_saves" );
    for ( const auto & entry : fs::directory_iterator ( fs::current_path() / "tests" / "broken_saves" ) )
        assert ( parser.loadSave ( gsm, entry.path() ) == false );
    
    parser.setPath ( fs::current_path() / "tests" / "correct_saves" );
    for ( const auto & entry : fs::directory_iterator ( fs::current_path() / "tests" / "correct_saves" ) )
        assert ( parser.loadSave ( gsm, entry.path() ) == true );
    
    
    cout << "========================" << endl;
    cout << "All tests were sucessful" << endl;
    cout << "========================" << endl;
    return 0;
}
#endif

#ifndef TEST
int startMenu ( CGameStateManager & gsm ) {
    CMenu menu;
    return menu.handleMainMenu ( gsm );
}
int main ( void ) {
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
#endif