/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
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

    void SetVerboseLevel(G4int);
    void SetROOTFilename(G4String);
    void SetNCrystal(G4int);
    void SetCrystalSizeXY(G4double);
    void SetCrystalDepth(G4double);
    void SetSiPMSizeXY(G4double);
    void SetSiPMDepth(G4double);

    G4int GetVerboseLevel() const { return fVerboseLevel; }
    G4String GetROOTFilename() const { return fROOTFilename; }
    G4int GetNCrystal() const { return fNCrystal; }
    G4double GetCrystalSizeXY() const { return fCrystalSizeXY; }
    G4double GetCrystalDepth() const { return fCrystalDepth; }
    G4double GetSiPMSizeXY() const { return fSiPMSizeXY; }
    G4double GetSiPMDepth() const { return fSiPMDepth; }
    G4double GetCrystalEnd();

private:
    // methods
    void DefineMaterials();
    void UpdateGeometryParameters();
    void PrintParameters();
    G4VPhysicalVolume* ConstructDetector();
    void BuildCrystalandSiPM();

    //data members
    G4bool  fCheckOverlaps;                 //Option to activate checking of volumes overlaps
    G4double fWorldSizeXY;                  //Size of the world in XZ
    G4double fWorldSizeZ;                   //Size of the world in XZ
    //CaloBox
    G4double fCaloSizeXY;                   //Size of the full calorimeter in XZ (fCrystalSizeXY*fNCrystalPerRow)
    G4double fCaloDepth;                    //Size of the full calorimeter in Z (fCrystalDepth+fSiPMDepth)
    //Crystal
    G4int fNCrystal;                        //Number of crystals
    G4double fCrystalSizeXY;              //Size of the crystal in XZ
    G4double fCrystalDepth;                 //Size of the crystal in Z
    G4int fNCrystalPerRow;                  //Number of crystals per row
    //SiPM
    G4double fSiPMSizeXY;                    //Size of the SiPM in XZ
    G4double fSiPMDepth;                   //Size of the SiPM in Z

    DetectorMessenger* fDetectorMessenger; //To change some geometry parameters
    G4int   fVerboseLevel;                 //verbose level
    //Materials
    G4Material* fDefaultMaterial;          //Default material (G4_AIR)
    G4Material* fCrystalMaterial;          //Crystal material (LYSO)
    G4Material* fSiPMMaterial;             //SiPM photocathode material (G4_Si)

    G4LogicalVolume*   fWorldLogical;      //World logical volume
    G4LogicalVolume*   fCrystalLogical;    //Crystal logical volume

    G4VPhysicalVolume* fWorldPhysical;     //World physical volume (returns from ConstructDetector())
    G4VPhysicalVolume* fCrystalPhysical;   //Crystal physical volume

    G4String fROOTFilename;                //Filename of the output file

    G4Cache<PhotonDetSD*> fSD;             //Sensitive G4 detector handle
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
