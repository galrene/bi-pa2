#include <string>
using namespace std;

class CCharacter {
  protected:
    string m_Name;
    int m_Mana;
    int m_HP;
    int m_Strength;
  public:
    /* from file */
    void load ( string path );
    /* to file */
    void save ( string path );
};