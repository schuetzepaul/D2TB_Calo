/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4PVReplica;
class G4Material;
class G4GlobalMagFieldMessenger;
class DetectorMessenger;
class G4UnitDefinition;

/// Detector construction class to define materials and geometry.

///
/// In ConstructSDandField() a transverse uniform magnetic field is defined
/// via G4GlobalMagFieldMessenger class.

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
    void PrintCaloParameters();
    G4VPhysicalVolume* DefineVolumes();

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

    DetectorMessenger* fDetectorMessenger;    //to change some geometry parameters
    G4int   fVerboseLevel;                    //verbose

    G4Material* fDefaultMaterial;
    G4Material* fDefaultCrystalMaterial;

    G4LogicalVolume*   fWorldLogical;
    G4LogicalVolume*   fCaloLogical;
    G4LogicalVolume*   fCrystalLogical;

    G4VPhysicalVolume* fWorldPhysical;
    G4VPhysicalVolume*   fCaloPhysical;
    G4VPhysicalVolume*   fCrystalPhysical;

    G4String fROOTFilename;
    G4double fStepSize;

    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; // magnetic field messenger
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
