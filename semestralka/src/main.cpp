#include "CMenu.h"
#include "CGame.h"
#include <cassert>

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

#ifdef TEST
/**
 * @brief Main used for testing purpouses
 * 
 */
int main ( void ) {
    CConfigParser parser ( fs::current_path() / "tests", fs::current_path() / "tests" );
    map<string,shared_ptr<CCard>> brokenCards = parser.loadCards ( "broken_cards" );
    assert ( brokenCards.empty() == true );
    
    // number of directories/files in the correct_cards directory
    size_t entryCnt = std::distance(fs::directory_iterator(fs::current_path() / "tests" / "correct_cards"), fs::directory_iterator{});
    parser.setPath ( fs::current_path() / "tests" );
    map<string,shared_ptr<CCard>> correctCards = parser.loadCards ( "correct_cards" );
    assert ( correctCards.size() == entryCnt );
    
    CGameStateManager gsm;
    parser.setPath ( fs::current_path() / "tests" / "broken_saves" );
    for ( const auto & entry : fs::directory_iterator ( fs::current_path() / "tests" / "broken_saves" ) )
        assert ( parser.loadSave ( gsm, entry.path() ) == false );
    
    parser.setPath ( fs::current_path() / "tests" / "correct_saves" );
    for ( const auto & entry : fs::directory_iterator ( fs::current_path() / "tests" / "correct_saves" ) )
        assert ( parser.loadSave ( gsm, entry.path() ) == true );
    
    parser.setPath ( fs::current_path() / "tests" );
    map<string,shared_ptr<CCharacter>> brokenCharacters = parser.loadCharacters ( "broken_characters" );
    assert ( brokenCharacters.empty() == true );

    // number of directories/files in the correct_characters directory
    entryCnt = std::distance(fs::directory_iterator(fs::current_path() / "tests" / "correct_characters"), fs::directory_iterator{});
    parser.setPath ( fs::current_path() / "tests" );
    map<string,shared_ptr<CCharacter>> correctCharacters = parser.loadCharacters ( "correct_characters" );
    assert ( correctCharacters.size() == entryCnt );

    cout << "========================" << endl;
    cout << "All tests were sucessful" << endl;
    cout << "========================" << endl;
    return 0;
}
#endif