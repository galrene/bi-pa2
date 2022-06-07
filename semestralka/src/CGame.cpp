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
    mvprintw ( m_yMax/2, 1, "DISCARD A CARD (8)" );
    mvprintw ( m_yMax/2, m_xMax - 13, "END TURN (9)" );
    attroff ( A_STANDOUT );
    m_Gsm.renderPlayerStats ( m_P1Stats, 1 );
    m_Gsm.renderPlayerStats ( m_P2Stats, 2 );
    m_Gsm.renderPlayerHand ( m_P1Cards, 1 );
    m_Gsm.renderPlayerHand ( m_P2Cards, 2 );
}

void CGame::beginGame ( void ) {
    m_Gsm.dealCards();
    m_Gsm.whoIsOnTurn ( m_yMax/2, m_xMax/2 );
    drawLayout();
    int a;
    while ( ( a = getch() ) && ( a != ( 'd' & 0x1F ) ) ) {
        m_Gsm.whoIsOnTurn ( m_yMax/2, m_xMax/2 );
        if ( a == 9 ) 
            m_Gsm.endTurn();
        else if ( a == 8 ) { // discard card
            int cardIndex = getch();
            if ( cardIndex <= 5 && cardIndex >= 0 ) {
                m_Gsm.discardCard ( cardIndex );
                drawLayout(); // temporary, quite ineffective to refresh everything
            }
        }
        else if ( a <= 5 && a >= 0 ) {
            m_Gsm.playCard ( a );
            drawLayout(); // makes more sense to refresh everything here
        }
        // if ( ! bothAlive() )
        //    break;
    }
    // winnerScreen ();
}