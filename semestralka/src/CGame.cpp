#include "CGame.h"

CGame::CGame ( CGameStateManager gsm )
: m_Gsm ( gsm) {
    if ( ! initCurses() ) {
        cerr << "Your terminal doesn't support colors" << endl;
        return;
    }
    m_CardHeight = m_yMax/4, m_CardWidth = m_xMax/handSize;
    m_StatsHeight = 1+m_yMax/8, m_StatsWidth = m_xMax-1;
    m_P1Stats = newwin ( m_StatsHeight, m_StatsWidth, m_CardHeight, 0 );
    m_P2Stats = newwin ( m_StatsHeight, m_StatsWidth, m_yMax - m_CardHeight - m_StatsHeight, 0 );
    for ( size_t i = 0; i < handSize; i++ ) {
        m_P1Cards.push_back ( newwin ( m_CardHeight, m_CardWidth,  0, 0+(i*m_CardWidth) ) );
        m_P2Cards.push_back ( newwin ( m_CardHeight, m_CardWidth,  m_yMax - m_CardHeight, 0+(i*m_CardWidth) ) );
    }
    int rows = maxNameLength+15+1;
    m_Info = newwin ( 5, rows, (m_yMax/2) - 2, m_xMax/2 - (rows/2)-1 );
}

CGame::~CGame ( void ) {
    endwin();
}

bool CGame::initCurses ( void ) {
    initscr();
    cbreak();
    noecho();
    if ( ! has_colors () )
        return false;
    start_color();
    getmaxyx ( stdscr, m_yMax, m_xMax );
    clear();
    refresh();
    return true;
}

void CGame::drawLayout ( void ) {
    attron ( A_STANDOUT );
    mvprintw ( m_yMax / 2, 1, "DISCARD A CARD (8)" );
    mvprintw ( m_yMax / 2, m_xMax - 13, "END TURN (9)" );
    attroff ( A_STANDOUT );
    refresh();
    m_Gsm.renderPlayerStats ( m_P1Stats, 1 );
    m_Gsm.renderPlayerStats ( m_P2Stats, 2 );
    m_Gsm.renderPlayerHand ( m_P1Cards, 1 );
    m_Gsm.renderPlayerHand ( m_P2Cards, 2 );
    box ( m_Info, 0,0);
    wrefresh( m_Info);
}

bool CGame::beginGame ( void ) {
    m_Gsm.dealCards();
    drawLayout();
    m_Gsm.whoIsOnTurn ( m_Info );
    int a;
    while ( ( a = getch() ) ) {
        if ( a == '9' )
            m_Gsm.endTurn();
        else if ( a == '8' ) { // discard card
            if ( int cardIndex = getch(); cardIndex <= (int) handSize - 1 + '0' && cardIndex >= '0' )
                m_Gsm.discardCard ( cardIndex - '0' );
        }
        else if ( a <= (int) handSize - 1 + '0' && a >= '0' ) {
            m_Gsm.playCard ( a - '0', m_Info );
        }
        // ctrl - d
        else if ( a == ( 'd' & 0x1F ) )
            return false;
        // if ( ! bothAlive() )
        //    break;
        drawLayout(); // temporary, quite ineffective to refresh everything
        m_Gsm.whoIsOnTurn ( m_Info );
    }
    // winnerScreen ();
    return true;
}