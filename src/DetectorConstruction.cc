#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "CrystalParameterisation.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4RunManager.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "PhotonDetSD.hh"

#include "G4UserLimits.hh"

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

    fSiPMSize = 1*cm;
    fSiPMDepth = 0.05*mm;

    G4int fNCrystalPerRow = 3;
    fCaloSizeXY = fCrystalLengthXY*fNCrystalPerRow;
    fCaloDepth = fCrystalDepth+2*fSiPMDepth;
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
    nistManager->FindOrBuildMaterial("G4_Si");

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

    // LYSO https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/lyso-material-data-sheet_1.pdf
    // https://iopscience.iop.org/article/10.1088/1748-0221/9/06/C06008/pdf

    density = 7.1*g/cm3;
    G4Material *LYSO = new G4Material(name="LYSO", density, nel=4);
    LYSO->AddElement(elLu, fractionmass = 0.714467891);
    LYSO->AddElement(elY, fractionmass = 0.04033805);
    LYSO->AddElement(elSi, fractionmass = 0.063714272);
    LYSO->AddElement(elO, fractionmass = 0.181479788);


    fDefaultMaterial = G4Material::GetMaterial("G4_AIR");
    fCrystalMaterial = G4Material::GetMaterial("LYSO");
    fSiPMMaterial = G4Material::GetMaterial("G4_Si");

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

    fCrystalMaterial->SetMaterialPropertiesTable(propLYSO);
    //Set the birks constant for LYSO // TODO check this
    fCrystalMaterial->GetIonisation()->SetBirksConstant(0.0076*mm/MeV);

    G4double rAir[nLYSO] = { 1.0 };
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX", eLYSO, rAir, nLYSO);

    fDefaultMaterial->SetMaterialPropertiesTable(mpt);

    if ( not fDefaultMaterial || not fCrystalMaterial || not fSiPMMaterial ) {
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

    // BuildCrystalandSiPM();

    //Crystal
    auto Crystal = new G4Box("Crystal", fCrystalLengthXY/2, fCrystalLengthXY/2, fCrystalDepth/2);
    fCrystalLogical = new G4LogicalVolume(Crystal, fCrystalMaterial, "CrystalLV");
    fCrystalPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, -fCrystalDepth/2), fCrystalLogical, "Crystal", fWorldLogical, false, 0, fCheckOverlaps);

    G4OpticalSurface* CrystalSurface = new G4OpticalSurface("CrystalSurface", glisur, polished, dielectric_dielectric, 1.0);

    G4MaterialPropertiesTable* crystalSurfaceProperty = new G4MaterialPropertiesTable();
    G4int nbins = 1;
    G4double p_crystal[] = { 2.8 *eV };
    G4double refl_crystal[] = { 1. };
    G4double effi_crystal[] = { 0. };
    crystalSurfaceProperty->AddProperty("REFLECTIVITY", p_crystal, refl_crystal, nbins);
    crystalSurfaceProperty->AddProperty("EFFICIENCY", p_crystal, effi_crystal, nbins);
    CrystalSurface->SetMaterialPropertiesTable(crystalSurfaceProperty);

    new G4LogicalSkinSurface("CrystalSurface", fCrystalLogical, CrystalSurface);

    auto SiPM = new G4Box("SiPM", fSiPMSize/2, fSiPMSize/2, fSiPMDepth);
    auto logicSiPM = new G4LogicalVolume(SiPM, fDefaultMaterial, "SiPMLV");
    new G4PVPlacement(0, G4ThreeVector(0., 0., -fCrystalDepth-fSiPMDepth), logicSiPM, "SiPM", fWorldLogical, false, 0, fCheckOverlaps);

    auto PhotonDet = new G4Box("PhotonDet", fSiPMSize/2, fSiPMSize/2, fSiPMDepth/2);
    auto logicPhotonDet = new G4LogicalVolume(PhotonDet, fSiPMMaterial, "PhotonDetLV");
    new G4PVPlacement(0, G4ThreeVector(0., 0., -fSiPMDepth/2.), logicPhotonDet, "PhotonDet", logicSiPM, false, 0, fCheckOverlaps);

    // PhotonDet Surface Properties
    G4OpticalSurface* photonDetSurface = new G4OpticalSurface("PhotonDetSurface", glisur, ground, dielectric_metal, 1.0);

    G4MaterialPropertiesTable* photonDetSurfaceProperty = new G4MaterialPropertiesTable();
    // G4int nbins = 1;
    G4double p_mppc[] = { 2.8 *eV };
    G4double refl_mppc[] = { 0 };
    G4double effi_mppc[] = { 1 };
    photonDetSurfaceProperty->AddProperty("REFLECTIVITY", p_mppc, refl_mppc, nbins);
    photonDetSurfaceProperty->AddProperty("EFFICIENCY", p_mppc, effi_mppc, nbins);
    photonDetSurface->SetMaterialPropertiesTable(photonDetSurfaceProperty);

    new G4LogicalSkinSurface("PhotonDetSurface",logicPhotonDet, photonDetSurface);

    // Visualization attributes
    fWorldLogical->SetVisAttributes (G4VisAttributes::GetInvisible());

    auto CrystalVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
    CrystalVisAtt->SetVisibility(true);
    fCrystalLogical->SetVisAttributes(CrystalVisAtt);

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
    SetSensitiveDetector("PhotonDetLV", fSD.Get(), true);
}

void DetectorConstruction::BuildCrystalandSiPM()
{
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // Volume coated with TiO2 containing the crystal
    auto CrystalVolume = new G4Box("CrystalVolume", fCrystalLengthXY/2, fCrystalLengthXY/2, fCaloDepth/2);
    auto LogicCrystalVolume = new G4LogicalVolume(CrystalVolume, fDefaultMaterial, "CrystalVolumeLV");

    //Crystal
    auto Crystal = new G4Box("Crystal", fCrystalLengthXY/2, fCrystalLengthXY/2, fCrystalDepth/2);
    fCrystalLogical = new G4LogicalVolume(Crystal, fCrystalMaterial, "CrystalLV");
    fCrystalPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, fSiPMDepth), fCrystalLogical, "Crystal", LogicCrystalVolume, false, 0, fCheckOverlaps);

    auto SiPM = new G4Box("SiPM", fSiPMSize/2, fSiPMSize/2, fSiPMDepth);
    auto logicSiPM = new G4LogicalVolume(SiPM, fDefaultMaterial, "SiPMLV");
    new G4PVPlacement(0, G4ThreeVector(0., 0., -fCrystalDepth/2), logicSiPM, "SiPM", LogicCrystalVolume, false, 0, fCheckOverlaps);

    auto PhotonDet = new G4Box("PhotonDet", fSiPMSize/2, fSiPMSize/2, fSiPMDepth/2);
    auto logicPhotonDet = new G4LogicalVolume(PhotonDet, fSiPMMaterial, "PhotonDetLV");
    new G4PVPlacement(0, G4ThreeVector(0., 0., -fSiPMDepth/2.), logicPhotonDet, "PhotonDet", logicSiPM, false, 0, fCheckOverlaps);

    // PhotonDet Surface Properties
    G4OpticalSurface* photonDetSurface = new G4OpticalSurface("PhotonDetSurface", glisur, ground, dielectric_metal, 1.0);

    G4MaterialPropertiesTable* photonDetSurfaceProperty = new G4MaterialPropertiesTable();
    G4int nbins = 1;
    G4double p_mppc[] = { 2.8 *eV };
    G4double refl_mppc[] = { 0 };
    G4double effi_mppc[] = { 1 };
    photonDetSurfaceProperty->AddProperty("REFLECTIVITY", p_mppc, refl_mppc, nbins);
    photonDetSurfaceProperty->AddProperty("EFFICIENCY", p_mppc, effi_mppc, nbins);
    photonDetSurface->SetMaterialPropertiesTable(photonDetSurfaceProperty);

    new G4LogicalSkinSurface("PhotonDetSurface",logicPhotonDet, photonDetSurface);

    //Replicate the full volume crystal + SiPM
    G4VPVParameterisation* crystalParam = new CrystalParameterisation(fCrystalLengthXY, fCaloDepth, -fCrystalLengthXY, -fCrystalLengthXY, 0);
    // Parametrised volume
    fCrystalPhysical = new G4PVParameterised("CrystalVolumeParam", LogicCrystalVolume, fCaloLogical, kUndefined, fNCrystal, crystalParam, true);
}
