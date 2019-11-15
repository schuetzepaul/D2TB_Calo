#include "globals.hh"

#include "PhysicsListMessenger.hh"
#include "PhysicsList.hh"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include "G4UIcmdWithAnInteger.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
: fPhysicsList(pPhys)
{
    fDirectory = new G4UIdirectory("/d2tb/phys/");
    fDirectory->SetGuidance("Control the physics lists");

    fVerboseCmd = new G4UIcmdWithAnInteger("/d2tb/phys/verbose",this);
    fVerboseCmd->SetGuidance("set verbose for physics processes");
    fVerboseCmd->SetParameterName("verbose",true);
    fVerboseCmd->SetDefaultValue(1);
    fVerboseCmd->SetRange("verbose>=0");
    fVerboseCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fGammaCutCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/phys/gammaCut", this);
    fGammaCutCmd->SetGuidance("Set gamma cut");
    fGammaCutCmd->SetParameterName("Gcut",false);
    fGammaCutCmd->SetUnitCategory("Length");
    fGammaCutCmd->SetRange("Gcut>0.0");
    fGammaCutCmd->SetDefaultUnit("mm");
    fGammaCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fElectCutCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/phys/electronCut", this);
    fElectCutCmd->SetGuidance("Set electron cut");
    fElectCutCmd->SetParameterName("Ecut",false);
    fElectCutCmd->SetUnitCategory("Length");
    fElectCutCmd->SetRange("Ecut>0.0");
    fElectCutCmd->SetDefaultUnit("mm");
    fElectCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fPosCutCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/phys/positronCut", this);
    fPosCutCmd->SetGuidance("Set positron cut");
    fPosCutCmd->SetParameterName("Pcut",false);
    fPosCutCmd->SetUnitCategory("Length");
    fPosCutCmd->SetRange("Pcut>0.0");
    fPosCutCmd->SetDefaultUnit("mm");
    fPosCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fAllCutCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/phys/allCuts", this);
    fAllCutCmd->SetGuidance("Set cut for all");
    fAllCutCmd->SetParameterName("cut",false);
    fAllCutCmd->SetUnitCategory("Length");
    fAllCutCmd->SetRange("cut>0.0");
    fAllCutCmd->SetDefaultUnit("mm");
    fAllCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PhysicsListMessenger::~PhysicsListMessenger()
{
    delete fVerboseCmd;
    delete fGammaCutCmd;
    delete fElectCutCmd;
    delete fPosCutCmd;
    delete fAllCutCmd;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == fVerboseCmd )
    {
        fPhysicsList->SetVerbose(fVerboseCmd->GetNewIntValue(newValue));
    }
    else if (command == fGammaCutCmd) {
        fPhysicsList->SetCutForGamma(fGammaCutCmd->GetNewDoubleValue(newValue));
    }
    else if (command == fElectCutCmd) {
        fPhysicsList->SetCutForElectron(fElectCutCmd->GetNewDoubleValue(newValue));
    }
    else if (command == fPosCutCmd) {
        fPhysicsList->SetCutForPositron(fPosCutCmd->GetNewDoubleValue(newValue));
    }
    else if (command == fAllCutCmd) {
        G4double cut = fAllCutCmd->GetNewDoubleValue(newValue);
        fPhysicsList->SetCutForGamma(cut);
        fPhysicsList->SetCutForElectron(cut);
        fPhysicsList->SetCutForPositron(cut);
    }
}
