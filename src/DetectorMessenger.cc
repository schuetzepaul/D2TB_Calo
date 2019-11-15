/// \file /src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
//

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* det)
: G4UImessenger(),
fDetector(det),
fDirectory(0),
fVerboseCmd(0),
fFilenameCmd(0),
fStepSizeCmd(0),
fNCrystalCmd(0),
fCrystalLengthXYCmd(0),
fCrystalDepthCmd(0)
{
    fDirectory = new G4UIdirectory("/d2tb/det/");
    fDirectory->SetGuidance("UI commands of this example");

    fVerboseCmd = new G4UIcmdWithAnInteger("/d2tb/det/verbose",this);
    fVerboseCmd->SetGuidance("Set verbosity level");
    fVerboseCmd->SetParameterName("verbose", false);
    fVerboseCmd->AvailableForStates(G4State_PreInit);
    fVerboseCmd->SetRange("verbose>=0");

    fFilenameCmd = new G4UIcmdWithAString("/d2tb/det/filename",this);
    fFilenameCmd->SetGuidance("Set filename of the rootfile.");
    fFilenameCmd->SetParameterName("value", false);
    fFilenameCmd->AvailableForStates(G4State_PreInit);

    fStepSizeCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/det/stepMax",this);
    fStepSizeCmd->SetGuidance("Set the maximum step size.");
    fStepSizeCmd->SetParameterName("value", false);
    fStepSizeCmd->SetUnitCategory("Length");
    fStepSizeCmd->AvailableForStates(G4State_PreInit);
    fStepSizeCmd->SetRange("value>0");

    fNCrystalCmd = new G4UIcmdWithAnInteger("/d2tb/det/NumberOfCrystals",this);
    fNCrystalCmd->SetGuidance("Set number of crystals.");
    fNCrystalCmd->SetParameterName("value", false);
    fNCrystalCmd->AvailableForStates(G4State_PreInit);
    fNCrystalCmd->SetRange("value>0");

    fCrystalLengthXYCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/det/CrystalLengthXY",this);
    fCrystalLengthXYCmd->SetGuidance("Set the crystal size in the XY plane.");
    fCrystalLengthXYCmd->SetParameterName("value", false);
    fCrystalLengthXYCmd->SetUnitCategory("Length");
    fCrystalLengthXYCmd->AvailableForStates(G4State_PreInit);
    fCrystalLengthXYCmd->SetRange("value>0");

    fCrystalDepthCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/det/CrystalDepth",this);
    fCrystalDepthCmd->SetGuidance("Set the crystal size along the beam.");
    fCrystalDepthCmd->SetParameterName("value", false);
    fCrystalDepthCmd->SetUnitCategory("Length");
    fCrystalDepthCmd->AvailableForStates(G4State_PreInit);
    fCrystalDepthCmd->SetRange("value>0");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
    delete fDirectory;
    delete fVerboseCmd;
    delete fFilenameCmd;
    delete fStepSizeCmd;
    delete fNCrystalCmd;
    delete fCrystalLengthXYCmd;
    delete fCrystalDepthCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == fVerboseCmd ) {
        fDetector->SetVerboseLevel(fVerboseCmd->GetNewIntValue(newValue));
    }
    else if( command == fFilenameCmd ) {
        fDetector->SetROOTFilename(newValue);
    }
    else if( command == fStepSizeCmd ) {
        fDetector->SetMaxStepSize(fStepSizeCmd->GetNewDoubleValue(newValue));
    }
    else if( command == fNCrystalCmd ) {
        fDetector->SetNCrystal(fNCrystalCmd->GetNewIntValue(newValue));
    }
    else if( command == fCrystalLengthXYCmd ) {
        fDetector->SetCrystalLengthXY(fCrystalLengthXYCmd->GetNewDoubleValue(newValue));
    }
    else if( command == fCrystalDepthCmd ) {
        fDetector->SetCrystalDepth(fCrystalDepthCmd->GetNewDoubleValue(newValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String DetectorMessenger::GetCurrentValue(G4UIcommand * command)
{
    G4String ans;
    if( command == fVerboseCmd ) {
        ans=fVerboseCmd->ConvertToString(fDetector->GetVerboseLevel());
    }
    else if( command == fFilenameCmd ) {
        ans=fFilenameCmd->ConvertToString(fDetector->GetROOTFilename());
    }
    else if( command == fStepSizeCmd ) {
        ans=fStepSizeCmd->ConvertToString(fDetector->GetMaxStepSize());
    }
    else if( command == fNCrystalCmd ) {
        ans=fNCrystalCmd->ConvertToString(fDetector->GetNCrystal());
    }
    else if( command == fCrystalLengthXYCmd ) {
        ans=fCrystalLengthXYCmd->ConvertToString(fDetector->GetCrystalLengthXY());
    }
    else if( command == fCrystalDepthCmd ) {
        ans=fCrystalDepthCmd->ConvertToString(fDetector->GetCrystalDepth());
    }

    return ans;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
