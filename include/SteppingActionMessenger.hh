#ifndef SteppingActionMessenger_hh
#define SteppingActionMessenger_hh 1

#include "G4UImessenger.hh"

class WLSSteppingAction;

class G4UIdirectory;
class G4UIcmdWithAnInteger;

class SteppingActionMessenger : public G4UImessenger
{
  public:

    SteppingActionMessenger(SteppingAction* );
    virtual ~SteppingActionMessenger();

    virtual void SetNewValue(G4UIcommand* ,G4String );

  private:

    SteppingAction* fSteppingAction;
    G4UIdirectory*     fSteppingDir;
    G4UIcmdWithAnInteger* fSetBounceLimitCmd;

};

#endif
