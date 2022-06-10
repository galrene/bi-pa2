#pragma once
#include "CPlayer.h"

class CBot : public CPlayer {
  public:
    CBot ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck );
    CBot ( const string & name, const CCharacter & defaultCharacter, const CCharacter & currentCharacter, const CDeck & deck, const CDeck & hand );
    
    virtual int readAction ( void ) override;
    virtual int readReceiver ( void ) const override;
    /**
     * @brief Get the index of the card that deals the most dmg inside playable cards.
     * 
     * @param canPlay playable cards
     * @return int index of the card, -1 if no damage card is available
     */
    int getMostDmgCard ( vector<pair<shared_ptr<CCard>,size_t>> & canPlay ) const;
    /**
     * @brief Get the index of the card that heals the most inside playable cards.
     * 
     * @param canPlay playable cards
     * @return int index of the card, -1 if no healing card is available
     */
    int getMostHealCard ( vector<pair<shared_ptr<CCard>,size_t>> & canPlay ) const;
    /**
     * @brief Get cards that are currently playable (enough mana for them).
     * 
     * @return vector<pair<shared_ptr<CCard>,size_t>> first = card, second = it's index inside hand
     */
    vector<pair<shared_ptr<CCard>,size_t>> canPlay ( void ) const;
  protected:
    /**
     * @brief Whether to use the previously selected card on the enemy or self.
     */
    bool m_UseOnEnemy;
};
