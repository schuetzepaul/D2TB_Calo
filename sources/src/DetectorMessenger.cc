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
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* det)
: G4UImessenger(),
fDetector(det),
fDirectory(0),
fVerboseCmd(0),
fSDVerboseCmd(0),
fValidateCmd(0),
fNCrystalCmd(0),
fCrystalSizeXYCmd(0),
fCrystalDepthCmd(0),
fSiPMSizeXYCmd(0),
fSiPMDepthCmd(0),
fSiPMPDECmd(0)
{
    fDirectory = new G4UIdirectory("/d2tb/det/");
    fDirectory->SetGuidance(" Geometry Setup ");

    fVerboseCmd = new G4UIcmdWithAnInteger("/d2tb/det/verbose",this);
    fVerboseCmd->SetGuidance("Set verbosity level");
    fVerboseCmd->SetParameterName("verbose", false);
    fVerboseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fVerboseCmd->SetRange("verbose>=0");
    fVerboseCmd->SetToBeBroadcasted(false);

    fSDVerboseCmd = new G4UIcmdWithAnInteger("/d2tb/det/sd/verbose",this);
    fSDVerboseCmd->SetGuidance("Set verbosity level");
    fSDVerboseCmd->SetParameterName("verbose", false);
    fSDVerboseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fSDVerboseCmd->SetRange("verbose>=0");
    fSDVerboseCmd->SetToBeBroadcasted(false);

    fValidateCmd = new G4UIcmdWithoutParameter("/d2tb/det/validateGeo",this);
    fValidateCmd->SetGuidance("Check the geometry for overlaps.");
    fValidateCmd->AvailableForStates(G4State_PreInit);

    fNCrystalCmd = new G4UIcmdWithAnInteger("/d2tb/det/NumberOfCrystals",this);
    fNCrystalCmd->SetGuidance("Set number of crystals.");
    fNCrystalCmd->SetParameterName("NumberOfCrystals", false);
    fNCrystalCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fNCrystalCmd->SetRange("NumberOfCrystals>0 && NumberOfCrystals<=9");
    fNCrystalCmd->SetToBeBroadcasted(false);

    fCrystalSizeXYCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/det/CrystalSizeXY",this);
    fCrystalSizeXYCmd->SetGuidance("Set the crystal size in the XY plane.");
    fCrystalSizeXYCmd->SetParameterName("CrystalSizeXY", false);
    fCrystalSizeXYCmd->SetUnitCategory("Length");
    fCrystalSizeXYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fCrystalSizeXYCmd->SetRange("CrystalSizeXY>0");
    fCrystalSizeXYCmd->SetToBeBroadcasted(false);

    fCrystalDepthCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/det/CrystalDepth",this);
    fCrystalDepthCmd->SetGuidance("Set the crystal size along the beam.");
    fCrystalDepthCmd->SetParameterName("CrystalDepth", false);
    fCrystalDepthCmd->SetUnitCategory("Length");
    fCrystalDepthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fCrystalDepthCmd->SetRange("CrystalDepth>0");
    fCrystalDepthCmd->SetToBeBroadcasted(false);

    fSiPMSizeXYCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/det/SiPMSizeXY",this);
    fSiPMSizeXYCmd->SetGuidance("Set the SiPM size in the XY plane.");
    fSiPMSizeXYCmd->SetParameterName("SiPMSizeXY", false);
    fSiPMSizeXYCmd->SetUnitCategory("Length");
    fSiPMSizeXYCmd->SetRange("SiPMSizeXY>0.");
    fSiPMSizeXYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fSiPMSizeXYCmd->SetToBeBroadcasted(false);

    fSiPMDepthCmd = new G4UIcmdWithADoubleAndUnit("/d2tb/det/SiPMDepth",this);
    fSiPMDepthCmd->SetGuidance("Set the SiPM size along the beam.");
    fSiPMDepthCmd->SetParameterName("SiPMDepth", false);
    fSiPMDepthCmd->SetUnitCategory("Length");
    fSiPMDepthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fSiPMDepthCmd->SetRange("SiPMDepth>0. && SiPMDepth<=1");
    fSiPMDepthCmd->SetToBeBroadcasted(false);

    fSiPMPDECmd = new G4UIcmdWithADouble("/d2tb/det/SiPM_PDE",this);
    fSiPMPDECmd->SetGuidance("Set the SiPM PDE efficiency.");
    fSiPMPDECmd->SetParameterName("PDE", false);
    fSiPMPDECmd->SetRange("PDE>=0. && PDE <= 1.0");
    fSiPMPDECmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fSiPMPDECmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
    delete fDirectory;
    delete fVerboseCmd;
    delete fSDVerboseCmd;
    delete fValidateCmd;
    delete fNCrystalCmd;
    delete fCrystalSizeXYCmd;
    delete fCrystalDepthCmd;
    delete fSiPMSizeXYCmd;
    delete fSiPMDepthCmd;
    delete fSiPMPDECmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == fVerboseCmd ) {
        fDetector->SetVerboseLevel(fVerboseCmd->GetNewIntValue(newValue));
    }
    else if( command == fSDVerboseCmd ) {
        fDetector->SetSDVerboseLevel(fSDVerboseCmd->GetNewIntValue(newValue));
    }
    else if ( command == fValidateCmd ) {
        G4cout << "Geometry will be checked for overlaps" << G4endl;
        fDetector->ValidateGeometry();
    }
    else if( command == fNCrystalCmd ) {
        fDetector->SetNCrystal(fNCrystalCmd->GetNewIntValue(newValue));
    }
    else if( command == fCrystalSizeXYCmd ) {
        fDetector->SetCrystalSizeXY(fCrystalSizeXYCmd->GetNewDoubleValue(newValue));
    }
    else if( command == fCrystalDepthCmd ) {
        fDetector->SetCrystalDepth(fCrystalDepthCmd->GetNewDoubleValue(newValue));
    }
    else if( command == fSiPMSizeXYCmd ) {
        fDetector->SetSiPMSizeXY(fSiPMSizeXYCmd->GetNewDoubleValue(newValue));
    }
    else if( command == fSiPMDepthCmd ) {
        fDetector->SetSiPMDepth(fSiPMDepthCmd->GetNewDoubleValue(newValue));
    }
    else if( command == fSiPMPDECmd ) {
        fDetector->SetSiPM_PDE(fSiPMPDECmd->GetNewDoubleValue(newValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String DetectorMessenger::GetCurrentValue(G4UIcommand * command)
{
    G4String ans;
    if( command == fVerboseCmd ) {
        ans=fVerboseCmd->ConvertToString(fDetector->GetVerboseLevel());
    }
    else if( command == fSDVerboseCmd ) {
        ans=fSDVerboseCmd->ConvertToString(fDetector->GetSDVerboseLevel());
    }
    else if( command == fNCrystalCmd ) {
        ans=fNCrystalCmd->ConvertToString(fDetector->GetNCrystal());
    }
    else if( command == fCrystalSizeXYCmd ) {
        ans=fCrystalSizeXYCmd->ConvertToString(fDetector->GetCrystalSizeXY());
    }
    else if( command == fCrystalDepthCmd ) {
        ans=fCrystalDepthCmd->ConvertToString(fDetector->GetCrystalDepth());
    }
    else if( command == fSiPMSizeXYCmd ) {
        ans=fSiPMSizeXYCmd->ConvertToString(fDetector->GetSiPMSizeXY());
    }
    else if( command == fSiPMDepthCmd ) {
        ans=fSiPMDepthCmd->ConvertToString(fDetector->GetSiPMDepth());
    }
    else if( command == fSiPMPDECmd ) {
        ans=fSiPMPDECmd->ConvertToString(fDetector->GetSiPM_PDE());
    }

    return ans;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
