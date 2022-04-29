#include <string>

class CCard {
  protected:
    string m_Name;
    string m_Type;
    int m_Cost;
  public:
    virtual ~CCard ( void ) noexcept = default;
    virtual void useCard ( void );
    /* for saving */
    virtual void dumpInfo ( void );
};

class CAttack : public CCard {
  virtual void useCard ( void ) override ;
  virtual void dumpInfo ( void ) override;
};

class CDefense : public CCard {
  virtual void useCard ( void ) override ;
  virtual void dumpInfo ( void ) override;
};

class CSpecial : public CCard {
  virtual void useCard ( void ) override ;
  virtual void dumpInfo ( void ) override;
};

class CPassive : public CCard {
  virtual void useCard ( void ) override ;
  virtual void dumpInfo ( void ) override;
};