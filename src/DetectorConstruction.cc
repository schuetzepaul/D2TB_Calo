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
fDetectorMessenger(nullptr),
fVerboseLevel(1),
fDefaultMaterial(nullptr),
fWorldLogical(nullptr),
fCaloLogical(nullptr),
fLayerLogical(nullptr),
fWorldPhysical(nullptr),
fCaloPhysical(nullptr),
fLayerPhysical(nullptr),
fCrystalPV(nullptr),
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
    nistManager->FindOrBuildMaterial("G4_POLYSTYRENE");

    G4double a, z, density, fractionmass;
    G4String name, symbol;
    G4int nel, natoms;

    a = 55.845*g/mole;
    G4Element *elFe = new G4Element(name="Iron", symbol="Fe", z=26., a);

    a = 12.011*g/mole;
    G4Element *elC = new G4Element(name="Carbon", symbol="C", z=6., a);

    a = 54.938*g/mole;
    G4Element *elMn = new G4Element(name="Manganese", symbol="Mn", z=25., a);

    a = 28.09*g/mole;
    G4Element *elSi = new G4Element(name="Silicon", symbol="Si", z=14., a);

    a = 16.00*g/mole;
    G4Element *elO = new G4Element(name="Oxygen", symbol="O", z=8., a);

    a = 1.01*g/mole;
    G4Element *elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    // Steel 235
    density = 7.87*g/cm3;
    G4Material *Steel235 = new G4Material(name="Steel235", density, nel=3);
    Steel235->AddElement(elFe, fractionmass = 0.9843);
    Steel235->AddElement(elC, fractionmass = 0.0017);
    Steel235->AddElement(elMn, fractionmass = 0.014);

    // Scintillator
    density = 1.032*g/cm3;
    G4Material* Polystyrene = new G4Material("Polystyrene", density, nel=2);
    Polystyrene->AddElement(elC, natoms=8);
    Polystyrene->AddElement(elH, natoms=8);

    density= 2.200*g/cm3;
    G4Material *SiO2 = new G4Material("quartz", density, nel=2);
    SiO2->AddElement(elSi, natoms = 1);
    SiO2->AddElement(elO , natoms = 2);

    //from http://www.physi.uni-heidelberg.de/~adler/TRD/TRDunterlagen/RadiatonLength/tgc2.htm
    //Epoxy (for FR4 )
    density = 1.2*g/cm3;
    G4Material *Epoxy = new G4Material("Epoxy", density, nel=2);
    Epoxy->AddElement(elH, natoms = 2);
    Epoxy->AddElement(elC, natoms = 2);

    //FR4 (Glass + Epoxy)
    density = 1.86*g/cm3;
    G4Material *FR4 = new G4Material("FR4", density, nel=2);
    FR4->AddMaterial(SiO2, fractionmass = 0.528);
    FR4->AddMaterial(Epoxy, fractionmass = 0.472);

    //Vacuum
    // G4Material *Vacuum = new G4Material("Galactic", z=1., a=1.01*g/mole, density=universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);

    fDefaultMaterial = G4Material::GetMaterial("G4_AIR");

    if ( ! fDefaultMaterial ) {
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

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    // Attach limits to the layer logical volume (G4UserLimits.hh)
    G4UserLimits *limits = new G4UserLimits();
    limits->SetMaxAllowedStep(fStepSize);
    // fLayerLogical->SetUserLimits(limits);

    // Visualization attributes
    fWorldLogical->SetVisAttributes (G4VisAttributes::GetInvisible());

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
