#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4RunManager.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4UnitsTable.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4UserLimits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
fCheckOverlaps(true),
fWorldSizeXY(0),
fWorldSizeZ(0),
fCaloSizeXY(0),
fCaloThickness(0),
fCrystalThickness(0),
fDetectorMessenger(nullptr),
fVerboseLevel(1),
fDefaultMaterial(nullptr),
fWorldLogical(nullptr),
fCaloLogical(nullptr),
fCrystalLogical(nullptr),
fWorldPhysical(nullptr),
fCaloPhysical(nullptr),
fCrystalPhysical(nullptr),
fROOTFilename(""),
fStepSize(1*mm)
{
    //Compute Params
    ComputeParameters();
    fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fDetectorMessenger;
}

void DetectorConstruction::ComputeParameters()
{
    //World size
    fWorldSizeXY = 1*m;
    fWorldSizeZ  = 1*m;

    fCaloSizeXY = 10*cm;
    fCaloThickness = 30*cm;
    fCrystalThickness = fCaloThickness;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Define materials
    DefineMaterials();

    // Define volumes
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
    // Lead material defined using NIST Manager
    auto nistManager = G4NistManager::Instance();
    nistManager->FindOrBuildMaterial("G4_AIR");

    G4double a, z, density, fractionmass;
    G4String name, symbol;
    G4int nel;

    a = 174.97*g/mole;
    G4Element *elLu = new G4Element(name="Lutetium", symbol="Lu", z=71., a);

    a = 88.91*g/mole;
    G4Element *elY = new G4Element(name="Yttrium", symbol="Y", z=39., a);

    a = 28.09*g/mole;
    G4Element *elSi = new G4Element(name="Silicon", symbol="Si", z=14., a);

    a = 16.00*g/mole;
    G4Element *elO = new G4Element(name="Oxygen", symbol="O", z=8., a);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    // LYSO
    density = 7.36*g/cm3;
    G4Material *lyso = new G4Material(name="LYSO", density, nel=4);
    lyso->AddElement(elLu, fractionmass = 0.714467891);
    lyso->AddElement(elY, fractionmass = 0.04033805);
    lyso->AddElement(elSi, fractionmass = 0.063714272);
    lyso->AddElement(elO, fractionmass = 0.181479788);

    fDefaultMaterial = G4Material::GetMaterial("G4_AIR");
    fDefaultCrystalMaterial = G4Material::GetMaterial("LYSO");

    if ( not fDefaultMaterial || not fDefaultCrystalMaterial ) {
        G4ExceptionDescription msg;
        msg << "Cannot retrieve materials.";
        G4Exception("DetectorConstruction::DefineMaterials()",
        "ErrorCode1", FatalException, msg);
    }

    // Print materials
    // G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
    // Geometry parameters
    ComputeParameters();

    // World Box
    auto worldBox = new G4Box("World", fWorldSizeXY/2, fWorldSizeXY/2, fWorldSizeZ/2);
    // World Logical Volume (associate it with the world box)
    fWorldLogical = new G4LogicalVolume(worldBox, fDefaultMaterial, "World");
    // Placement of the World Logical Volume
    fWorldPhysical = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, "World", 0, false, 0, fCheckOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // Calorimeter Box
    auto caloBox = new G4Box("Calorimeter", fCaloSizeXY/2, fCaloSizeXY/2, fCaloThickness/2);
    // Calorimeter logical volume
    fCaloLogical = new G4LogicalVolume(caloBox, fDefaultMaterial, "Calorimeter");
    // Calorimeter physical volume
    fCaloPhysical = new G4PVPlacement(0, G4ThreeVector(), fCaloLogical, "Calorimeter", fWorldLogical, false, 0, fCheckOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // Crystal Box
    auto crystalBox = new G4Box("Crystal", fCaloSizeXY/2, fCaloSizeXY/2, fCrystalThickness/2);
    // Crystal logical volume
    fCrystalLogical = new G4LogicalVolume(crystalBox, fDefaultCrystalMaterial, "Crystal");
    // Crystal physical volume
    fCrystalPhysical = new G4PVPlacement(0, G4ThreeVector(), fCrystalLogical, "Crystal", fWorldLogical, false, 0, fCheckOverlaps);

    // Attach limits to the layer logical volume (G4UserLimits.hh)
    G4UserLimits *limits = new G4UserLimits();
    limits->SetMaxAllowedStep(fStepSize);
    fCrystalLogical->SetUserLimits(limits);

    // Visualization attributes
    // Visualization attributes
    fWorldLogical->SetVisAttributes (G4VisAttributes::GetInvisible());

    auto CrystalVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    CrystalVisAtt->SetVisibility(true);
    fCrystalLogical->SetVisAttributes(CrystalVisAtt);

    PrintCaloParameters();

    // Always return the physical World
    return fWorldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintCaloParameters()
{
    // print parameters
    G4cout
    << G4endl
    << "------------------------------------------------------------" << G4endl
    << " Output ROOT file set to " << fROOTFilename << G4endl
    << " The max step size is set to " << G4BestUnit(fStepSize, "Length") << G4endl
    << "------------------------------------------------------------" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
    //Add Mag Field here if wanted

    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    // G4ThreeVector fieldValue;
    // fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
    // fMagFieldMessenger->SetVerboseLevel(1);

    // Register the field messenger for deleting
    // G4AutoDelete::Register(fMagFieldMessenger);
}
