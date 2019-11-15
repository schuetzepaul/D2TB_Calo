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
fCaloDepth(0),
fNCrystal(0),
fCrystalLengthXY(0),
fCrystalDepth(0),
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

    fCaloSizeXY = fCrystalLengthXY*fNCrystal;
    fCaloDepth = fCrystalDepth;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    ComputeParameters();

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
    G4Material *LYSO = new G4Material(name="LYSO", density, nel=4);
    LYSO->AddElement(elLu, fractionmass = 0.714467891);
    LYSO->AddElement(elY, fractionmass = 0.04033805);
    LYSO->AddElement(elSi, fractionmass = 0.063714272);
    LYSO->AddElement(elO, fractionmass = 0.181479788);

    const G4int nLYSO = 2;
    G4double eLYSO[nLYSO] = { 0.1 * eV, 10 * eV };
    G4double rLYSO[nLYSO] = { 1.81, 1.81 };
    G4double aLYSO[nLYSO] = { 27.85*cm, 27.85*cm };

    G4MaterialPropertiesTable* propLYSO = new G4MaterialPropertiesTable();
    propLYSO->AddConstProperty("SCINTILLATIONYIELD", 32000./MeV);
    propLYSO->AddConstProperty("RESOLUTIONSCALE", 1.0);
    propLYSO->AddConstProperty("FASTTIMECONSTANT", 1.3*ns);
    propLYSO->AddConstProperty("YIELDRATIO", 1.0);
    propLYSO->AddConstProperty("ROUGHNESS", 5.0*deg);

    propLYSO->AddProperty("ABSLENGTH", eLYSO, aLYSO, nLYSO);
    propLYSO->AddProperty("RINDEX", eLYSO, rLYSO, nLYSO);
    //Wavelength to relative light output
    // propLYSO->AddProperty("FASTCOMPONENT", eLYSO, lLYSO, nLYSO);

    LYSO->SetMaterialPropertiesTable(propLYSO);

    //TODO: Wrapping material? Surface definition for reflections?

    fDefaultMaterial = G4Material::GetMaterial("G4_AIR");
    fDefaultCrystalMaterial = G4Material::GetMaterial("LYSO");

    if ( not fDefaultMaterial || not fDefaultCrystalMaterial ) {
        G4ExceptionDescription msg;
        msg << "Cannot retrieve materials.";
        G4Exception("DetectorConstruction::DefineMaterials()",
        "ErrorCode1", FatalException, msg);
    }

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
    // Geometry parameters
    ComputeParameters();

    // World Box
    auto worldBox = new G4Box("WorldBox", fWorldSizeXY/2, fWorldSizeXY/2, fWorldSizeZ/2);
    // World Logical Volume (associate it with the world box)
    fWorldLogical = new G4LogicalVolume(worldBox, fDefaultMaterial, "WorldBoxLV");
    // Placement of the World Logical Volume
    fWorldPhysical = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, "WorldBoxPhys", 0, false, 0, fCheckOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // Calorimeter Box
    auto caloBox = new G4Box("CalorimeterBox", fCaloSizeXY/2, fCaloSizeXY/2, fCaloDepth/2);
    // Calorimeter logical volume
    fCaloLogical = new G4LogicalVolume(caloBox, fDefaultMaterial, "CalorimeterBoxLV");
    // Calorimeter physical volume
    fCaloPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, -fCaloDepth/2), fCaloLogical, "CalorimeterBoxPhys", fWorldLogical, false, 0, fCheckOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // Crystal Box
    auto crystalBox = new G4Box("CrystalBox", fCrystalLengthXY/2, fCrystalLengthXY/2, fCrystalDepth/2);
    // Crystal logical volume
    fCrystalLogical = new G4LogicalVolume(crystalBox, fDefaultCrystalMaterial, "CrystalBoxLV");
    // Crystal physical volume
    fCrystalPhysical = new G4PVReplica("Crystal", fCrystalLogical, fCaloLogical, kXAxis, fNCrystal, fCrystalLengthXY);

    // Attach limits to the layer logical volume (G4UserLimits.hh)
    G4UserLimits *limits = new G4UserLimits();
    limits->SetMaxAllowedStep(fStepSize);
    fCrystalLogical->SetUserLimits(limits);

    // Visualization attributes
    // Visualization attributes
    fWorldLogical->SetVisAttributes (G4VisAttributes::GetInvisible());

    auto CaloVisAtt = new G4VisAttributes(G4Colour(0.5,0.,0.));
    CaloVisAtt->SetVisibility(true);
    fCaloLogical->SetVisAttributes(CaloVisAtt);

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
    << " CaloBox size " << G4BestUnit(fCaloSizeXY, "Length") << " depth " << G4BestUnit(fCaloDepth, "Length") << G4endl
    << " NCrystal: " << fNCrystal << " The crystal size in XY is " << G4BestUnit(fCrystalLengthXY, "Length") << " and the depth is " << G4BestUnit(fCrystalDepth, "Length") << G4endl
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
