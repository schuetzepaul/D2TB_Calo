#include "PersistencyMessenger.hh"
#include "PersistencyManager.hh"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcommand.hh>
#include <G4ios.hh>

PersistencyMessenger::PersistencyMessenger( PersistencyManager* persistencyMgr )
: fPersistencyManager(persistencyMgr)
{
    fPersistencyDIR = new G4UIdirectory("/d2tb/root/");
    fPersistencyDIR->SetGuidance("Output rootfile commands.");

    fOpenCMD = new G4UIcmdWithAString("/d2tb/root/open", this);
    fOpenCMD->SetGuidance("Set the name of the output file and open it.");
    fOpenCMD->SetParameterName("filename", true);
    fOpenCMD->SetDefaultValue("simulation-output.root");
    fOpenCMD->AvailableForStates(G4State_PreInit, G4State_Idle);

    fCloseCMD = new G4UIcmdWithoutParameter("/d2tb/root/close", this);
    fCloseCMD->SetGuidance("Close the output file.");
}

PersistencyMessenger::~PersistencyMessenger()
{
    delete fOpenCMD;
    delete fCloseCMD;
    delete fPersistencyDIR;
}

void PersistencyMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fOpenCMD) {
        fPersistencyManager->Open(newValue);
    }
    else if (command == fCloseCMD) {
        fPersistencyManager->Close();
    }
}

G4String PersistencyMessenger::GetCurrentValue(G4UIcommand * command)
{
    G4String currentValue;

    if (command == fOpenCMD) {
        currentValue = fPersistencyManager->GetFilename();
    }

    return currentValue;
}
