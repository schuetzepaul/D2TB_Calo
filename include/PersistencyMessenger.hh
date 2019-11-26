#ifndef PersistencyMessenger_hh
#define PersistencyMessenger_hh 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;

class PersistencyManager;

class PersistencyMessenger: public G4UImessenger {
public:
    PersistencyMessenger(PersistencyManager* persistencyMgr);
    virtual ~PersistencyMessenger();

    void SetNewValue(G4UIcommand* command,G4String newValues);
    G4String GetCurrentValue(G4UIcommand* command);

private:
    PersistencyManager* fPersistencyManager;

    G4UIdirectory*             fPersistencyDIR;
    G4UIcmdWithAString*        fOpenCMD;
    G4UIcmdWithoutParameter*   fCloseCMD;

};
#endif
