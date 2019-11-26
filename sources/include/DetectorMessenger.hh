/// \file /include/DetectorMessenger.hh
/// \brief Definition of the DetectorMessenger class
//
//

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UnitDefinition;

class DetectorMessenger: public G4UImessenger
{
public:
    DetectorMessenger(DetectorConstruction*);
    virtual ~DetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);
    virtual G4String GetCurrentValue(G4UIcommand * command);

private:
    DetectorConstruction* fDetector;

    G4UIdirectory*                  fDirectory;
    G4UIcmdWithAnInteger*           fVerboseCmd;
    G4UIcmdWithAnInteger*           fNCrystalCmd;
    G4UIcmdWithADoubleAndUnit*      fCrystalSizeXYCmd;
    G4UIcmdWithADoubleAndUnit*      fCrystalDepthCmd;
    G4UIcmdWithADoubleAndUnit*      fSiPMSizeXYCmd;
    G4UIcmdWithADoubleAndUnit*      fSiPMDepthCmd;
};


#endif
