#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4PVParameterised.hh"

#include "G4RunManager.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4UnitsTable.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "PhotonDetSD.hh"

#include "G4UserLimits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CrystalParameterisation::CrystalParameterisation(G4double XYLength, G4double ZLength, G4double firstX, G4double firstY, G4double firstZ)
{
    fXhalfLength = XYLength/2.;
    fYhalfLength = XYLength/2.;
    fZhalfLength = ZLength/2.;

    fStartX = firstX;
    fStartY = firstY;
    fStartZ = firstZ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CrystalParameterisation::~CrystalParameterisation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CrystalParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const
{
    G4double Xpos = fStartX;
    G4double Ypos = fStartY;
    G4double Zpos = fStartZ;

    if(copyNo < 3) {
        Xpos = fStartX + copyNo * fXhalfLength*2;
    }

    if(copyNo > 2 && copyNo < 6) {
        Xpos = fStartX + (copyNo - 3)* fXhalfLength*2;
        Ypos = fStartY + fXhalfLength*2;
    }

    if(copyNo > 5) {
        Xpos = fStartX + (copyNo - 6)* fXhalfLength*2;
        Ypos = fStartY + fXhalfLength*4;
    }

    // G4cout << "Copy " << copyNo << " placed at (x, y, z) : (" << Xpos << ", " << Ypos << ", " << Zpos << ")" << G4endl;

    G4ThreeVector origin(Xpos, Ypos, Zpos);
    physVol->SetTranslation(origin);
    physVol->SetRotation(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CrystalParameterisation::ComputeDimensions(G4Box& crystalBox, const G4int copyNo, const G4VPhysicalVolume* physVol) const
{
    crystalBox.SetXHalfLength(fXhalfLength);
    crystalBox.SetYHalfLength(fYhalfLength);
    crystalBox.SetZHalfLength(fZhalfLength);
}

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
    G4int fNCrystalPerRow = 3;
    fGlassDepth = 3*mm;
    fCaloSizeXY = fCrystalLengthXY*fNCrystalPerRow;
    fCaloDepth = fCrystalDepth + fGlassDepth;
    fFoilThickness = 0.1*mm;
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
    nistManager->FindOrBuildMaterial("G4_Pyrex_Glass");

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

    a = 12.0107*g/mole;
    G4Element *elC = new G4Element(name="Carbon", symbol="C", z=6., a);

    a = 1.00794*g/mole;
    G4Element *elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    density = 1.065*g/cm3;
    G4Material *polystyrole = new G4Material(name="polystyrole", density, nel=2);
    polystyrole->AddElement(elC, fractionmass = 0.5);
    polystyrole->AddElement(elH, fractionmass = 0.5);

    // LYSO https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/lyso-material-data-sheet_1.pdf
    // https://iopscience.iop.org/article/10.1088/1748-0221/9/06/C06008/pdf

    density = 7.1*g/cm3;
    G4Material *LYSO = new G4Material(name="LYSO", density, nel=4);
    LYSO->AddElement(elLu, fractionmass = 0.714467891);
    LYSO->AddElement(elY, fractionmass = 0.04033805);
    LYSO->AddElement(elSi, fractionmass = 0.063714272);
    LYSO->AddElement(elO, fractionmass = 0.181479788);

    const G4int nLYSO = 1;
    G4double eLYSO[nLYSO] = { 2.8 *eV };
    G4double rLYSO[nLYSO] = { 1.81 };
    G4double aLYSO[nLYSO] = { 41.3 *cm };
    G4double lLYSO[nLYSO] = { 1.0 };

    G4MaterialPropertiesTable* propLYSO = new G4MaterialPropertiesTable();
    propLYSO->AddConstProperty("SCINTILLATIONYIELD", 32./keV);
    propLYSO->AddConstProperty("RESOLUTIONSCALE", 1.0);
    propLYSO->AddConstProperty("FASTTIMECONSTANT", 0.09*ns);
    propLYSO->AddConstProperty("SlOWTIMECONSTANT", 41*ns);
    propLYSO->AddConstProperty("YIELDRATIO", 1.0);

    propLYSO->AddProperty("ABSLENGTH", eLYSO, aLYSO, nLYSO);
    propLYSO->AddProperty("RINDEX", eLYSO, rLYSO, nLYSO);
    // Wavelength to relative light output //TODO check this
    propLYSO->AddProperty("FASTCOMPONENT", eLYSO, lLYSO, nLYSO);

    LYSO->SetMaterialPropertiesTable(propLYSO);

    //Set the birks constant for LYSO // TODO check this
    LYSO->GetIonisation()->SetBirksConstant(0.0076*mm/MeV);

    //TODO: Wrapping material? Surface definition for reflections?

    fDefaultMaterial = G4Material::GetMaterial("G4_AIR");
    fCrystalMaterial = G4Material::GetMaterial("LYSO");
    fGlassMaterial = G4Material::GetMaterial("G4_Pyrex_Glass");
    fFoilMaterial = G4Material::GetMaterial("polystyrole");

    if ( not fDefaultMaterial || not fCrystalMaterial ) {
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
    auto worldBox = new G4Box("WorldBox", fWorldSizeXY/2, fWorldSizeXY/2, fWorldSizeZ/2);
    // World Logical Volume (associate it with the world box)
    fWorldLogical = new G4LogicalVolume(worldBox, fDefaultMaterial, "WorldBoxLV");
    // Placement of the World Logical Volume
    fWorldPhysical = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, "WorldBox", 0, false, 0, fCheckOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // Calorimeter Box
    auto caloBox = new G4Box("CalorimeterBox", fCaloSizeXY/2, fCaloSizeXY/2, fCaloDepth/2);
    // Calorimeter logical volume
    fCaloLogical = new G4LogicalVolume(caloBox, fDefaultMaterial, "CalorimeterBoxLV");
    // Calorimeter physical volume
    fCaloPhysical = new G4PVPlacement(0, G4ThreeVector(fCaloSizeXY/2., fCaloSizeXY/2., -fCaloDepth/2.), fCaloLogical, "CalorimeterBox", fWorldLogical, false, 0, fCheckOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // Glass at the back of the crystals
    auto glassBox = new G4Box("GlassBox", fCaloSizeXY/2, fCaloSizeXY/2, fGlassDepth/2);
    // Crystal logical volume
    auto fGlassLogical = new G4LogicalVolume(glassBox, fGlassMaterial, "GlassBoxLV");
    auto fGlassPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, -fCaloDepth/2. + fGlassDepth/2.), fGlassLogical, "GlassBox", fCaloLogical, false, 0, fCheckOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    auto crystalFakeBox = new G4Box("CrystalFakeBox", fCaloSizeXY/2, fCaloSizeXY/2, fCrystalDepth/2);
    auto fcrystalFakeBoxLogical = new G4LogicalVolume(crystalFakeBox, fDefaultMaterial, "CrystalFakeBoxLV");
    auto fcrystalFakeBoxPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, fGlassDepth/2), fcrystalFakeBoxLogical, "CrystalFakeBox", fCaloLogical, false, 0, fCheckOverlaps);

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // Crystal Box
    auto crystalBox = new G4Box("CrystalBox", fCrystalLengthXY/2, fCrystalLengthXY/2, fCrystalDepth/2);
    // Crystal logical volume
    fCrystalLogical = new G4LogicalVolume(crystalBox, fCrystalMaterial, "CrystalBoxLV");
    G4VPVParameterisation* crystalParam = new CrystalParameterisation(fCrystalLengthXY-fFoilThickness, fCrystalDepth, -fCrystalLengthXY, -fCrystalLengthXY, 0);
    // Crystal physical volume
    fCrystalPhysical = new G4PVParameterised("CrystalBox", fCrystalLogical, fcrystalFakeBoxLogical, kUndefined, fNCrystal, crystalParam, true);

    //Optical surface of the crystal assuming Tyvek and polished crystal
    G4OpticalSurface* OpSurfaceCrystal = new G4OpticalSurface("CrystalSurface");
    OpSurfaceCrystal->SetType(dielectric_dielectric);
    OpSurfaceCrystal->SetModel(glisur);
    OpSurfaceCrystal->SetFinish(polished);
    new G4LogicalSkinSurface("CrystalSurface", fCrystalLogical, OpSurfaceCrystal);

    //Surface between the crystal and the glass
    G4OpticalSurface* OpSurfaceGlass = new G4OpticalSurface("GlassSurface");
    OpSurfaceGlass->SetType(dielectric_dielectric);
    OpSurfaceGlass->SetModel(glisur);
    OpSurfaceGlass->SetFinish(polished);
    new G4LogicalSkinSurface("GlassSurface", fGlassPhysical, OpSurfaceGlass);

    //OpticalAirSurface
    const G4int num = 1;
    G4double ephoton[num] = { 2.8 *eV };
    G4double reflectivity[num] = { 0. };
    G4double efficiency[num]   = { 1.0 };

    G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();
    myST2->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
    myST2->AddProperty("EFFICIENCY", ephoton, efficiency, num);
    OpSurfaceGlass->SetMaterialPropertiesTable(myST2);

    // Visualization attributes
    fWorldLogical->SetVisAttributes (G4VisAttributes::GetInvisible());

    auto CaloVisAtt = new G4VisAttributes(G4Colour(0.5,0.,0.));
    CaloVisAtt->SetVisibility(true);
    fCaloLogical->SetVisAttributes(CaloVisAtt);

    auto CrystalVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    CrystalVisAtt->SetVisibility(true);
    fCrystalLogical->SetVisAttributes(CrystalVisAtt);

    auto GlassVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    GlassVisAtt->SetVisibility(true);
    fGlassLogical->SetVisAttributes(GlassVisAtt);

    PrintParameters();

    // Always return the physical World
    return fWorldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
    // print parameters
    G4cout << G4endl
    << "------------------------------------------------------------" << G4endl
    << " CaloBox size " << G4BestUnit(fCaloSizeXY, "Length") << "depth " << G4BestUnit(fCaloDepth, "Length") << G4endl
    << " NCrystal: " << fNCrystal << " The crystal size in XY is " << G4BestUnit(fCrystalLengthXY, "Length") << "and the depth is " << G4BestUnit(fCrystalDepth, "Length") << G4endl
    << " Output ROOT file set to " << fROOTFilename << G4endl
    << " The max step size is set to " << G4BestUnit(fStepSize, "Length") << G4endl
    << "------------------------------------------------------------" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
    if (!fSD.Get()) {
        G4String SDName = "d2tb/PhotonDet";
        PhotonDetSD* SD = new PhotonDetSD(SDName);
        G4SDManager::GetSDMpointer()->AddNewDetector(SD);
        fSD.Put(SD);
    }
    SetSensitiveDetector("GlassBoxLV", fSD.Get(), true);
}
