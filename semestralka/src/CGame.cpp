#include "CGame.h"

CGame::CGame ( CGameStateManager gsm )
: m_Gsm ( gsm) {
    if ( ! initCurses() ) {
        cerr << "Your terminal doesn't support colors" << endl;
        return;
    }
    m_CardHeight = m_yMax/4, m_CardWidth = m_xMax/6;
    m_StatsHeight = 1+m_yMax/8, m_StatsWidth = m_xMax-1;
    m_P1Stats = newwin ( m_StatsHeight, m_StatsWidth, m_CardHeight, 0 );
    m_P2Stats = newwin ( m_StatsHeight, m_StatsWidth, m_yMax - m_CardHeight - m_StatsHeight, 0 );
    for ( size_t i = 0; i < 6; i++ ) {
        m_P1Cards.push_back( newwin ( m_CardHeight, m_CardWidth,  0, 0+(i*m_CardWidth) ) );
        m_P2Cards.push_back( newwin ( m_CardHeight, m_CardWidth,  m_yMax - m_CardHeight, 0+(i*m_CardWidth) ) );
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

bool CGame::drawLayout ( void ) {
    /* "discard card" border */
    int h = ACS_BOARD;
    int v = ACS_BOARD;
    /*
    mvhline ( m_yMax/2 - 2, 0, h, 21 );
    mvhline ( m_yMax/2 + 2, 0, h, 21 );
        mvvline ( m_yMax/2 - 2, 20, v, 4 );
    */
    attron ( A_STANDOUT );
    mvprintw ( m_yMax/2, 1, "DISCARD A CARD (8)" );
    /* "end turn" border */
    /*
    mvhline ( m_yMax/2 - 2, m_xMax - 21, h, 21 );
    mvhline ( m_yMax/2 + 2, m_xMax - 21, h, 21 );
        mvvline ( m_yMax/2 - 2, m_xMax - 21, v, 4 );
    */
    mvprintw ( m_yMax/2, m_xMax - 13, "END TURN (9)" );
    attroff ( A_STANDOUT );
    return true;
}

bool CGame::beginGame ( void ) {
    if ( ! drawLayout() )
        return false;
    getch();
    m_Gsm.renderPlayerStats ( m_P1Stats, 1 );
    m_Gsm.renderPlayerStats ( m_P2Stats, 2 );
    getch();
    m_Gsm.beginGame(); // deals hands
    m_Gsm.renderPlayerHand ( m_P1Cards, 1 );
    m_Gsm.renderPlayerHand ( m_P2Cards, 2 );
    getch();
    return true;
}