/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4VPVParameterisation.hh"
#include "G4Cache.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;
class G4UnitDefinition;
class G4Box;
class PhotonDetSD;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetVerboseLevel(G4int val) { fVerboseLevel = val; }
    void SetROOTFilename(G4String val) { fROOTFilename = val; }
    void SetMaxStepSize(G4double val) { fStepSize = val; }
    void SetNCrystal(G4int val) { fNCrystal = val; }
    void SetCrystalLengthXY(G4double val) { fCrystalLengthXY = val; }
    void SetCrystalDepth(G4double val) { fCrystalDepth = val; }


    G4int GetVerboseLevel() const { return fVerboseLevel; }
    G4String GetROOTFilename() const { return fROOTFilename; }
    G4double GetMaxStepSize() const { return fStepSize; }
    G4int GetNCrystal() const { return fNCrystal; }
    G4double GetCrystalLengthXY() const { return fCrystalLengthXY; }
    G4double GetCrystalDepth() const { return fCrystalDepth; }

private:
    // methods
    void DefineMaterials();
    void ComputeParameters();
    void PrintParameters();

    G4VPhysicalVolume* DefineVolumes();

    void BuildCrystalandSiPM();

    //data members
    G4bool  fCheckOverlaps;                   // option to activate checking of volumes overlaps
    G4double fWorldSizeXY;
    G4double fWorldSizeZ;

    //CaloBox
    G4double fCaloSizeXY;
    G4double fCaloDepth;

    //Crystal
    G4int fNCrystal;
    G4double fCrystalLengthXY;
    G4double fCrystalDepth;

    //SiPM
    G4double fSiPMSize;
    G4double fSiPMDepth;

    DetectorMessenger* fDetectorMessenger;    //to change some geometry parameters
    G4int   fVerboseLevel;                    //verbose

    G4Material* fDefaultMaterial;
    G4Material* fCrystalMaterial;
    G4Material* fSiPMMaterial;

    G4LogicalVolume*   fWorldLogical;
    G4LogicalVolume*   fCaloLogical;
    G4LogicalVolume*   fCrystalLogical;

    G4VPhysicalVolume* fWorldPhysical;
    G4VPhysicalVolume* fCrystalPhysical;

    G4String fROOTFilename;
    G4double fStepSize;

    G4Cache<PhotonDetSD*> fSD;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
