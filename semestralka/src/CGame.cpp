#include "CGame.h"

CGame::CGame ( CGameStateManager gsm )
: m_Gsm ( gsm) {
    if ( ! initCurses() ) {
        cerr << "Your terminal doesn't support colors" << endl;
        return;
    }
    vector<WINDOW*> m_P1Cards;
    vector<WINDOW*> m_P2Cards;
    m_CardHeight = m_yMax/4, m_CardWidth = m_xMax/handSize;
    m_StatsHeight = 1+m_yMax/8, m_StatsWidth = m_xMax-1;
    m_InfoWidth = maxNameLength+15+1; // 15 = reserve for unknown max character name length
    for ( size_t i = 0; i < handSize; i++ ) {
        m_P1Cards.push_back ( newwin ( m_CardHeight, m_CardWidth,  0, 0+(i*m_CardWidth) ) );
        m_P2Cards.push_back ( newwin ( m_CardHeight, m_CardWidth,  m_yMax - m_CardHeight, 0+(i*m_CardWidth) ) );
    }
    m_Gsm.setWindows ( m_P1Cards, newwin ( m_StatsHeight, m_StatsWidth, m_CardHeight, 0 ),
                       m_P2Cards, newwin ( m_StatsHeight, m_StatsWidth, m_yMax - m_CardHeight - m_StatsHeight, 0 ),
                                  newwin ( 5, m_InfoWidth, (m_yMax/2) - 2, m_xMax/2 - (m_InfoWidth/2)-1 ) );
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
    mvprintw ( m_yMax / 2, 1, "DISCARD A CARD (%c)", defaultDiscardButton );
    mvprintw ( m_yMax / 2, m_xMax - 13, "END TURN (%c)", defaultEndTurnButton );
    attroff ( A_STANDOUT );
    refresh();
    m_Gsm.renderPlayerStats ();
    m_Gsm.renderHands ();
}

bool CGame::beginGame ( void ) {
    m_Gsm.dealCards();
    drawLayout();
    m_Gsm.whoIsOnTurn ();
    int a;
    while ( (a = getch()) ) {
        if ( a == defaultEndTurnButton )
            m_Gsm.endTurn();
        else if ( a == defaultDiscardButton )
            m_Gsm.discardCard();
        else if ( a <= (int) handSize - 1 + '0' && a >= '0' )
            m_Gsm.playCard ( a - '0' );
        else if ( a == defaultMenuButton ) {
            int b = m_Gsm.handleMenu();
            if ( b == 0 )
                return true;
            else if ( b == -1 )
                return false;
            else if ( b == 1 )
                m_Gsm.saveGame();
        }
        // ctrl - d
        else if ( a == ( 'd' & 0x1F ) )
            return false;
        drawLayout(); // temporary, quite ineffective to refresh everything
        m_Gsm.whoIsOnTurn ();
        if ( m_Gsm.winnerDecided() )
           break;
    }
    return true;
}