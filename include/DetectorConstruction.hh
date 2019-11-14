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


    G4int GetVerboseLevel() const { return fVerboseLevel; }
    G4String GetROOTFilename() const { return fROOTFilename; }
    G4double GetMaxStepSize() const { return fStepSize; }

    const G4VPhysicalVolume* GetCrystalPV() const;

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
    DetectorMessenger* fDetectorMessenger;    //to change some geometry parameters
    G4int   fVerboseLevel;                    //verbose
    G4Material* fDefaultMaterial;

    G4LogicalVolume*   fWorldLogical;
    G4LogicalVolume*   fCaloLogical;
    G4LogicalVolume*   fLayerLogical;

    G4VPhysicalVolume* fWorldPhysical;
    G4VPhysicalVolume* fCaloPhysical;
    G4PVReplica*       fLayerPhysical;

    G4VPhysicalVolume* fCrystalPV;

    G4String fROOTFilename;
    G4double fStepSize;

    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; // magnetic field messenger
};

// inline functions

inline const G4VPhysicalVolume* DetectorConstruction::GetCrystalPV() const  {
    return fCrystalPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
