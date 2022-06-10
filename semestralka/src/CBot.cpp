#include "CBot.h"

CBot::CBot ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck )
: CPlayer ( name, defaultCharacter, currentCharacter, deck )  {}

CBot::CBot ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck, const CDeck & hand )
: CPlayer ( name, defaultCharacter, currentCharacter, deck, hand ) {}

vector<pair<shared_ptr<CCard>,size_t>> CBot::canPlay ( void ) {
    vector<pair<shared_ptr<CCard>,size_t>> playableCards; 
    for ( size_t i = 0; i < m_Hand.size(); i++ )
        if ( hasEnoughMana ( i ) )
            playableCards.emplace_back ( make_pair ( m_Hand.getCardAt ( i ), i ) );
    return playableCards;
}

int CBot::getMostDmgCard ( vector<pair<shared_ptr<CCard>,size_t>> & canPlay ) {
    vector<int> damages;
    for ( const auto & [ card, i ] : canPlay )
        damages.push_back ( card->getEffects().first.m_HPDiff );
    int minValue = *min_element ( damages.begin(), damages.end() );
    size_t cardIndex = find ( damages.begin(), damages.end(), minValue ) - damages.begin();
    if ( minValue >= 0 )
        return -1;
    else
        return cardIndex;
}
int CBot::getMostHealCard ( vector<pair<shared_ptr<CCard>,size_t>> & canPlay ) {
    vector<int> heals;
    for ( const auto & [ card, i ] : canPlay )
        heals.push_back ( card->getEffects().first.m_HPDiff );
    int maxValue = *max_element ( heals.begin(), heals.end() );
    size_t cardIndex = find ( heals.begin(), heals.end(), maxValue ) - heals.begin();
    if ( maxValue < 0 )
        return -1;
    else
        return cardIndex;
}

int CBot::readAction ( void ) {
    vector<pair<shared_ptr<CCard>,size_t>> playableCards = canPlay(); // enough mana to play them
    if ( playableCards.empty() ) // out of mana, end turn
        return defaultEndTurnButton;
    int res;
    if ( m_PlayedCharacter.getHP() > m_LoadedCharacter.getHP() / 2 ) { // we are above half hp, let's attack!
        res = getMostDmgCard ( playableCards );
        if ( res != -1 ) { // damage card found
            m_UseOnEnemy = true;
            return playableCards[res].second + '0';
        }
        // no damage cards found
        res = getMostHealCard ( playableCards );
        if ( res != -1 ) { // heal card found
            m_UseOnEnemy = false;
            return playableCards[res].second + '0';
        }
        // no healing cards found
        // so play a random one idk
        m_UseOnEnemy = true;
        return (random() % playableCards.size()) + '0';
    }
    res = getMostHealCard ( playableCards );
    if ( res != -1 ) { // heal card found
        m_UseOnEnemy = false;
        return playableCards[res].second + '0';
    }
    m_UseOnEnemy = true;
    return (random() % playableCards.size()) + '0';
}

int CBot::readReceiver ( void ) {
    if ( m_UseOnEnemy )
        return 259; // KEY_UP == USE ON ENEMY
    return 258; // KEY_DOWN == USE ON SELF
}