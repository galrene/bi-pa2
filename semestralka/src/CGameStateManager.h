#include "CGameSettings.h"
#include "CPlayer.h"
using namespace std;

class CGameStateManager {
  protected:
    size_t m_TurnNumber;
    CPlayer m_Player1;
    CPlayer m_Player2;
  public:
    void beginGame ( CGameSettings & settings );
    void endGame   ( void );
    void saveGame  ( /* TODO */  );
    void loadGame  ( /* path */  );
    void loadCards ( void );
    void endTurn   ( void );
    void loadPlayer ( CPlayer & player );
};