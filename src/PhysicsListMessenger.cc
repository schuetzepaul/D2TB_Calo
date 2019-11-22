#include "PhysicsListMessenger.hh"
#include "PhysicsList.hh"

#include <G4UIdirectory.hh>
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
: G4UImessenger(),
fPhysicsList(pPhys),
fDirectory(0),
fVerboseCmd(0),
fStepMaxSizeCmd(0)
{
    fDirectory = new G4UIdirectory("/d2tb/phys/");
    fDirectory->SetGuidance("Control the physics lists");

    fVerboseCmd = new G4UIcmdWithAnInteger("/d2tb/phys/verbose",this);
    fVerboseCmd->SetGuidance("set verbose for physics processes");
    fVerboseCmd->SetParameterName("verbose",true);
    fVerboseCmd->SetDefaultValue(1);
    fVerboseCmd->SetRange("verbose>=0");
    fVerboseCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fStepMaxSizeCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/phys/StepMaxSize",this);
    fStepMaxSizeCmd->SetGuidance("set max step size for physics processes");
    fStepMaxSizeCmd->SetParameterName("StepMaxSize",true);
    fStepMaxSizeCmd->SetDefaultValue(1);
    fStepMaxSizeCmd->SetRange("StepMaxSize>=0");
    fStepMaxSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

}

PhysicsListMessenger::~PhysicsListMessenger()
{
    delete fVerboseCmd;
    delete fStepMaxSizeCmd;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == fVerboseCmd )
    {
        fPhysicsList->SetVerbose(fVerboseCmd->GetNewIntValue(newValue));
    }
    else if( command == fStepMaxSizeCmd )
    {
        fPhysicsList->SetStepMax(fStepMaxSizeCmd->GetNewDoubleValue(newValue));
    }
}
