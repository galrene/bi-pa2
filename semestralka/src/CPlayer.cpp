#include "CPlayer.h"

CPlayer::CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck )
: m_PlayedCharacter ( currentCharacter ), m_Name ( name ), m_LoadedCharacter ( defaultCharacter ), m_Deck ( deck ), m_StatsWin ( nullptr ) {}

CPlayer::CPlayer ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck, const CDeck & hand )
: m_PlayedCharacter ( currentCharacter ), m_Name ( name ), m_LoadedCharacter ( defaultCharacter ), m_Deck ( deck ), m_Hand ( hand ), m_StatsWin ( nullptr ) {}

CPlayer::~CPlayer ( void ) {
  for ( size_t i = 0; i < m_HandWins.size(); i++ )
    delwin( m_HandWins[i] );
  delwin ( m_StatsWin );
}
void CPlayer::renderName ( WINDOW * win, int yCoord, int xCoord ) const {
  string name = m_Name + "'s " + m_PlayedCharacter.getName() + " the " + m_PlayedCharacter.getClass();
  mvwprintw ( win, yCoord, xCoord - name.size()/2, "%s", name.c_str() );
}
void CPlayer::renderPlayer ( void ) const {
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
void CPlayer::renderCard ( WINDOW * win, size_t & i ) const {
  box ( win, 0, 0 );
  mvwprintw ( win, 1, 1, "(%ld)",i );
  m_Hand.renderCard ( win, i );
}
void CPlayer::renderHand ( void ) const {
  for ( size_t i = 0; i < m_HandWins.size(); i++ ) {
      wclear ( m_HandWins[i] );
      renderCard ( m_HandWins[i], i );
      wrefresh ( m_HandWins[i] );
    }
}
void CPlayer::hideHand ( void ) const {
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
void CPlayer::playCard ( size_t i, shared_ptr<CPlayer> & receiver ) {
  pair<CEffect,CEffect> effs = m_Hand.getCardAt ( i )->getEffects();
  receiver->m_PlayedCharacter.applyEffect ( effs.first );
  m_PlayedCharacter.applyEffect ( effs.second );
  m_Deck.addCard ( m_Hand.drawCardAt ( i ) );
  m_Hand.addCard ( m_Deck.drawCard () );
}
void CPlayer::fillMana ( void ) {
  m_PlayedCharacter.applyEffect ( CEffect ( 0, m_LoadedCharacter.getMana() - m_PlayedCharacter.getMana() ) );
}
bool CPlayer::hasEnoughMana ( size_t i ) const {
  return m_PlayedCharacter.getMana() >= m_Hand.getCardAt(i)->getManaCost();
}
bool CPlayer::save ( fs::path & playerDir ) {
  ofstream ofs_played ( playerDir / "character_played.ini" );
  ofstream ofs_loaded ( playerDir / "character_loaded.ini" );
  ofstream ofs_deckInfo ( playerDir / "deck.ini" );
  ofstream ofs_handInfo ( playerDir / "hand.ini" );
  if ( ! ofs_loaded.good() || ! ofs_played.good() || ! ofs_handInfo.good() || ! ofs_deckInfo.good() )
    return false;
  m_PlayedCharacter.dumpInfo ( ofs_played );
  m_LoadedCharacter.dumpInfo ( ofs_loaded );
  fs::path deckSaveDir = playerDir /= "card_definitions";
  m_Deck.saveDefinitions ( deckSaveDir ); // savujem karty ktore su v decku ale pokial vsetky karty urciteho typu su v ruke tak o nich nie je informacia v decku
  m_Deck.printData ( ofs_deckInfo );
  m_Hand.printHand ( ofs_handInfo );
  return true;
}