#include "globals.hh"

#include "PhysicsListMessenger.hh"
#include "PhysicsList.hh"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithABool.hh>

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
: fPhysicsList(pPhys)
{
    fDirectory = new G4UIdirectory("/d2tb/phys/");
    fDirectory->SetGuidance("Control the physics lists");

    fGammaCutCMD = new G4UIcmdWithADoubleAndUnit("/d2tb/phys/gammaCut", this);
    fGammaCutCMD->SetGuidance("Set gamma cut");
    fGammaCutCMD->SetParameterName("Gcut",false);
    fGammaCutCMD->SetUnitCategory("Length");
    fGammaCutCMD->SetRange("Gcut>0.0");
    fGammaCutCMD->SetDefaultUnit("mm");
    fGammaCutCMD->AvailableForStates(G4State_PreInit, G4State_Idle);

    fElectCutCMD = new G4UIcmdWithADoubleAndUnit("/d2tb/phys/electronCut", this);
    fElectCutCMD->SetGuidance("Set electron cut");
    fElectCutCMD->SetParameterName("Ecut",false);
    fElectCutCMD->SetUnitCategory("Length");
    fElectCutCMD->SetRange("Ecut>0.0");
    fElectCutCMD->SetDefaultUnit("mm");
    fElectCutCMD->AvailableForStates(G4State_PreInit, G4State_Idle);

    fPosCutCMD = new G4UIcmdWithADoubleAndUnit("/d2tb/phys/positronCut", this);
    fPosCutCMD->SetGuidance("Set positron cut");
    fPosCutCMD->SetParameterName("Pcut",false);
    fPosCutCMD->SetUnitCategory("Length");
    fPosCutCMD->SetRange("Pcut>0.0");
    fPosCutCMD->SetDefaultUnit("mm");
    fPosCutCMD->AvailableForStates(G4State_PreInit, G4State_Idle);

    fAllCutCMD = new G4UIcmdWithADoubleAndUnit("/d2tb/phys/allCuts", this);
    fAllCutCMD->SetGuidance("Set cut for all");
    fAllCutCMD->SetParameterName("cut",false);
    fAllCutCMD->SetUnitCategory("Length");
    fAllCutCMD->SetRange("cut>0.0");
    fAllCutCMD->SetDefaultUnit("mm");
    fAllCutCMD->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PhysicsListMessenger::~PhysicsListMessenger()
{
    delete fGammaCutCMD;
    delete fElectCutCMD;
    delete fPosCutCMD;
    delete fAllCutCMD;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fGammaCutCMD) {
        fPhysicsList->SetCutForGamma(fGammaCutCMD->GetNewDoubleValue(newValue));
    }
    else if (command == fElectCutCMD) {
        fPhysicsList->SetCutForElectron(fElectCutCMD->GetNewDoubleValue(newValue));
    }
    else if (command == fPosCutCMD) {
        fPhysicsList->SetCutForPositron(fPosCutCMD->GetNewDoubleValue(newValue));
    }
    else if (command == fAllCutCMD) {
        G4double cut = fAllCutCMD->GetNewDoubleValue(newValue);
        fPhysicsList->SetCutForGamma(cut);
        fPhysicsList->SetCutForElectron(cut);
        fPhysicsList->SetCutForPositron(cut);
    }
}
