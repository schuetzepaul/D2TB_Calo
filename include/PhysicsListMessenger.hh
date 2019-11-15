#ifndef PhysicsListMessenger_h
#define PhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PhysicsList;

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

/// Provide control of the physics list and cut parameters

class PhysicsListMessenger : public G4UImessenger
{
public:

    PhysicsListMessenger(PhysicsList* );
    virtual ~PhysicsListMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:

    PhysicsList* fPhysicsList;

    G4UIdirectory* fDirectory;

    G4UIcmdWithAnInteger* fVerboseCmd;
    G4UIcmdWithADoubleAndUnit* fGammaCutCmd;
    G4UIcmdWithADoubleAndUnit* fElectCutCmd;
    G4UIcmdWithADoubleAndUnit* fPosCutCmd;
    G4UIcmdWithADoubleAndUnit* fAllCutCmd;

};

#endif
