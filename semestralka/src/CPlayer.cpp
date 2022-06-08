#include "CPlayer.h"

CPlayer::CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck )
: m_PlayedCharacter ( currentCharacter ), m_Name ( name ), m_LoadedCharacter ( defaultCharacter ), m_Deck ( deck ), m_StatsWin ( nullptr ) {}

CPlayer::~CPlayer ( void ) {
  if ( ! m_HandWins.empty() ) {
    for ( size_t i = 0; i < m_HandWins.size(); i++ )
      delwin(m_HandWins[i]);
  }
  delwin ( m_StatsWin );
}

bool CPlayer::operator == ( CPlayer & rhs ) {
  return rhs.m_Name == m_Name;
}
void CPlayer::renderName ( WINDOW * win, int yCoord, int xCoord ) {
  string name = m_Name + "'s " + m_PlayedCharacter.getName();
  mvwprintw ( win, yCoord, xCoord - name.size()/2, "%s", name.c_str() );
}
void CPlayer::renderPlayer ( void ) {
  wclear ( m_StatsWin );
  box ( m_StatsWin, 0 ,0 );
  int yMax, xMax;
  getmaxyx ( m_StatsWin, yMax, xMax );
  renderName ( m_StatsWin, yMax/2, xMax/2 );
  m_PlayedCharacter.renderStats ( m_StatsWin );
  wrefresh ( m_StatsWin );
}
void CPlayer::shuffleDeck ( void ) {
  m_Deck.shuffleCards();
}
void CPlayer::drawCard ( size_t cnt ) {
  for ( size_t i = 0; i < cnt; i++ )
    m_Hand.addCard ( m_Deck.drawCard() );
}
void CPlayer::renderCard ( WINDOW * win, size_t & i ) {
  box ( win, 0, 0 );
  mvwprintw ( win, 1, 1, "(%ld)",i );
  m_Hand.renderCard ( win, i );
}
void CPlayer::renderHand ( void ) {
  for ( size_t i = 0; i < m_HandWins.size(); i++ ) {
      wclear ( m_HandWins[i] );
      renderCard ( m_HandWins[i], i );
      wrefresh ( m_HandWins[i] );
    }
}
void CPlayer::hideHand ( void ) {
  for ( size_t i = 0; i < m_HandWins.size(); i++ ) {
      wclear ( m_HandWins[i] );
      wrefresh ( m_HandWins[i] );
    }
}
void CPlayer::fillHand ( void ) {
  drawCard ( handSize - m_Hand.size() );
}
void CPlayer::discardCard ( size_t i ) {
  m_Deck.addCard ( m_Hand.drawCardAt(i) );
}
void CPlayer::playCard ( size_t i, shared_ptr<CPlayer> & user, shared_ptr<CPlayer> & receiver ) {
  pair<CEffect,CEffect> effs = m_Hand.getCardAt ( i )->getEffects();
  receiver->m_PlayedCharacter.applyEffect ( effs.first );
  user->m_PlayedCharacter.applyEffect ( effs.second );
  m_Deck.addCard ( m_Hand.drawCardAt ( i ) );
  m_Hand.addCard ( m_Deck.drawCard () );
}
void CPlayer::fillMana ( void ) {
  m_PlayedCharacter.applyEffect ( CEffect ( 0, m_LoadedCharacter.getMana() - m_PlayedCharacter.getMana() ) );
}
// ! TEMPORARY
bool CPlayer::tmp_hasEnoughMana ( size_t i ) {
  return m_PlayedCharacter.getMana() >= m_Hand.getCardAt(i)->getManaCost();
}
