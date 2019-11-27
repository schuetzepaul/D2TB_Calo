#include "G4UIdirectory.hh"
#include "SteppingAction.hh"
#include "SteppingActionMessenger.hh"

#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingActionMessenger::SteppingActionMessenger(SteppingAction* steppingaction)
: fSteppingAction (steppingaction)
{
    fSteppingDir = new G4UIdirectory("/d2tb/step/");
    fSteppingDir->SetGuidance("stepping control");

    fSetBounceLimitCmd = new G4UIcmdWithAnInteger("/d2tb/step/setBounceLimit", this);
    fSetBounceLimitCmd->SetGuidance("Select the maximum number of allowed bounce");
    fSetBounceLimitCmd->SetGuidance("Set this number to zero if you don't want to limit");
    fSetBounceLimitCmd->SetParameterName("limit",false);
    fSetBounceLimitCmd->SetRange("limit>=0");
    fSetBounceLimitCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingActionMessenger::~SteppingActionMessenger()
{
    delete fSteppingDir;
    delete fSetBounceLimitCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if ( command == fSetBounceLimitCmd ) {
        fSteppingAction->SetBounceLimit(G4UIcmdWithAnInteger::GetNewIntValue(newValue));
    }
}
