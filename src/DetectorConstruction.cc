#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

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
fWorldSizeXY(10*m),
fWorldSizeZ(10*m),
fCaloSizeXY(1*m),
fCaloDepth(1*m),
fNCrystal(1),
fCrystalSizeXY(10*cm),
fCrystalDepth(30*cm),
fNCrystalPerRow(3),
fSiPMSizeXY(1*mm),
fSiPMDepth(0.1*mm),
fDetectorMessenger(nullptr),
fVerboseLevel(1),
fDefaultMaterial(nullptr),
fWorldLogical(nullptr),
fCrystalLogical(nullptr),
fWorldPhysical(nullptr),
fCrystalPhysical(nullptr),
fROOTFilename("")
{
    //Compute Params
    fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    //Reset the geometry
    if (fWorldPhysical) {
        G4GeometryManager::GetInstance()->OpenGeometry();
        G4PhysicalVolumeStore::GetInstance()->Clean();
        G4LogicalVolumeStore::GetInstance()->Clean();
        G4SolidStore::GetInstance()->Clean();
        G4LogicalSkinSurface::CleanSurfaceTable();
        G4LogicalBorderSurface::CleanSurfaceTable();
    }

    // Define materials
    DefineMaterials();

    //Update Geometry parameters
    UpdateGeometryParameters();

    // Define volumes
    return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::UpdateGeometryParameters()
{
    fWorldSizeXY = 1*m;
    fWorldSizeZ  = 1*m;
    fNCrystalPerRow = 3;
    fCaloSizeXY = fCrystalSizeXY*fNCrystalPerRow;
    fCaloDepth = fCrystalDepth+fSiPMDepth;
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

    // a = 12.0107*g/mole;
    // G4Element *elC = new G4Element(name="Carbon", symbol="C", z=6., a);
    //
    // a = 1.00794*g/mole;
    // G4Element *elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);

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

    //Properties of LYSO
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
    fCrystalMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

    //Refraction properties of Air
    G4double rAir[nLYSO] = { 1.0 };
    G4MaterialPropertiesTable* propAir = new G4MaterialPropertiesTable();
    propAir->AddProperty("RINDEX", eLYSO, rAir, nLYSO);

    fDefaultMaterial->SetMaterialPropertiesTable(propAir);

    //Refraction properties of Si
    G4double rSi[nLYSO] = { 1.46 };
    G4MaterialPropertiesTable* propSi = new G4MaterialPropertiesTable();
    propSi->AddProperty("RINDEX", eLYSO, rSi, nLYSO);

    fSiPMMaterial->SetMaterialPropertiesTable(propSi);

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

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
    // World Box
    auto worldBox = new G4Box("WorldBox", fWorldSizeXY/2, fWorldSizeXY/2, fWorldSizeZ/2);
    // World Logical Volume (associate it with the world box)
    fWorldLogical = new G4LogicalVolume(worldBox, fDefaultMaterial, "WorldBoxLV");
    // Placement of the World Logical Volume
    fWorldPhysical = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, "WorldBox", 0, false, 0, fCheckOverlaps);

    BuildCrystalandSiPM();

    // Visualization attributes
    fWorldLogical->SetVisAttributes (G4VisAttributes::GetInvisible());

    //Print Parameters
    PrintParameters();

    // Always return the physical World
    return fWorldPhysical;
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
    G4int nbins = 1;
    G4double p_crystal[] = { 2.8 *eV };
    G4double refl_crystal[] = { 1. };
    G4double effi_crystal[] = { 0. };

    //Crystal
    auto Crystal = new G4Box("Crystal", fCrystalSizeXY/2, fCrystalSizeXY/2, fCrystalDepth/2);
    fCrystalLogical = new G4LogicalVolume(Crystal, fCrystalMaterial, "CrystalLV");

    auto CrystalVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
    CrystalVisAtt->SetVisibility(true);
    fCrystalLogical->SetVisAttributes(CrystalVisAtt);

    //Crystals are just polished
    G4OpticalSurface* CrystalSurface = new G4OpticalSurface("CrystalSurface", glisur, polished, dielectric_dielectric, 1.0);

    G4MaterialPropertiesTable* crystalSurfaceProperty = new G4MaterialPropertiesTable();
    crystalSurfaceProperty->AddProperty("REFLECTIVITY", p_crystal, refl_crystal, nbins);
    crystalSurfaceProperty->AddProperty("EFFICIENCY", p_crystal, effi_crystal, nbins);
    CrystalSurface->SetMaterialPropertiesTable(crystalSurfaceProperty);

    //Is that correct????
    new G4LogicalSkinSurface("CrystalSurface", fCrystalLogical, CrystalSurface);

    //Hole inside the crystal to house the SiPM
    G4double fHoleDepth = 2*fSiPMDepth;
    auto Hole = new G4Box("Hole", fSiPMSizeXY/2, fSiPMSizeXY/2, fHoleDepth);
    auto logicHole = new G4LogicalVolume(Hole, fDefaultMaterial, "HoleLV");
    //SiPM
    auto SiPM = new G4Box("SiPM", fSiPMSizeXY/2, fSiPMSizeXY/2, fSiPMDepth);
    auto logicSiPM = new G4LogicalVolume(SiPM, fDefaultMaterial, "SiPMLV");
    //Photocathode inside the SiPM
    auto PhotonDet = new G4Box("PhotonDet", fSiPMSizeXY/2, fSiPMSizeXY/2, fSiPMDepth/2);
    auto logicPhotonDet = new G4LogicalVolume(PhotonDet, fSiPMMaterial, "PhotonDetLV");
    new G4PVPlacement(0, G4ThreeVector(0., 0., -fSiPMDepth/2.), logicPhotonDet, "PhotonDet", logicSiPM, false, 0, fCheckOverlaps);

    //----------------------------------------------------------------------
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

    new G4LogicalSkinSurface("PhotonDetSurface", logicPhotonDet, photonDetSurface);

    auto SiPMVisAtt = new G4VisAttributes(G4Colour(1.0,0.647,0.0));
    SiPMVisAtt->SetVisibility(true);
    SiPMVisAtt->SetForceSolid(true);
    logicPhotonDet->SetVisAttributes(SiPMVisAtt);

    //---------------------- Placement of the SiPMs ------------------------------------------------

    G4double spacing = 0.5*cm;

    G4int fNSiPMPerRow = 5;
    G4int fNSiPMRow = 5;
    for(int irow = 0; irow < fNSiPMRow; irow++)
    {
        G4double fOffsetX = 0.;
        G4double fOffsetY = 0.;

        fOffsetY = -fCrystalSizeXY/2+spacing + irow*spacing;

        for(int iSiPM = 0; iSiPM < fNSiPMPerRow; iSiPM++)
        {
            G4String SiPMname = "SiPM";
            // SiPMname += G4UIcommand::ConvertToString(iSiPM+1);
            // SiPMname += "_Row";
            // SiPMname += G4UIcommand::ConvertToString(irow+1);

            G4String Holename = "Hole";
            // Holename += G4UIcommand::ConvertToString(iSiPM+1);
            // Holename += "_Row";
            // Holename += G4UIcommand::ConvertToString(irow+1);

            fOffsetX = -fCrystalSizeXY/2+spacing + iSiPM*spacing;

            //Hole placement inside the crystal
            new G4PVPlacement(0, G4ThreeVector(fOffsetX, fOffsetY, -fCrystalDepth/2+fHoleDepth), logicHole, Holename, fCrystalLogical, false, irow+10*irow, fCheckOverlaps);

            //SiPM placement inside the hole
            new G4PVPlacement(0, G4ThreeVector(0., 0., -fSiPMDepth), logicSiPM, SiPMname, logicHole, false, irow+10*irow, fCheckOverlaps);
        }
    }

    //---------------------- Placement of the Crystals --------------------------------------------
    for(int i = 0; i < fNCrystal; i++){
        G4String name = "Crystal";
        // name += G4UIcommand::ConvertToString(i+1);

        G4double fOffsetX = 0.;
        G4double fOffsetY = 0.;

        if(i < fNCrystalPerRow){
            fOffsetX = (-fCaloSizeXY+fCrystalSizeXY)/2 + i*fCrystalSizeXY;
            fOffsetY = (-fCaloSizeXY+fCrystalSizeXY)/2;
        }
        if(i >= fNCrystalPerRow && i < 2*fNCrystalPerRow){
            fOffsetX = (-fCaloSizeXY+fCrystalSizeXY)/2 + (i-fNCrystalPerRow)*fCrystalSizeXY;
            fOffsetY = (-fCaloSizeXY+fCrystalSizeXY)/2 + fCrystalSizeXY;
        }
        if(i >= 2*fNCrystalPerRow){
            fOffsetX = (-fCaloSizeXY+fCrystalSizeXY)/2 + (i-2*fNCrystalPerRow)*fCrystalSizeXY;
            fOffsetY = (-fCaloSizeXY+fCrystalSizeXY)/2 + 2*fCrystalSizeXY;
        }

        //Place Crystal
        new G4PVPlacement(0, G4ThreeVector(fOffsetX, fOffsetY, -fCrystalDepth/2), fCrystalLogical, name, fWorldLogical, false, i, fCheckOverlaps);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
    // print parameters
    G4cout << G4endl
    << "------------------------------------------------------------" << G4endl
    << " World XY size " << G4BestUnit(fWorldSizeXY, "Length") << "depth " << G4BestUnit(fWorldSizeZ, "Length") << G4endl
    << " CaloBox XY size " << G4BestUnit(fCaloSizeXY, "Length") << "depth " << G4BestUnit(fCaloDepth, "Length") << G4endl
    << " SiPM XY size " << G4BestUnit(fSiPMSizeXY, "Length") << "depth " << G4BestUnit(fSiPMDepth, "Length") << G4endl
    << " NCrystal: " << fNCrystal << " (" << fNCrystalPerRow << " per row) - Crystal XY size " << G4BestUnit(fCrystalSizeXY, "Length") << " depth " << G4BestUnit(fCrystalDepth, "Length") << G4endl
    << " Output ROOT file set to " << fROOTFilename << G4endl
    << "------------------------------------------------------------" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetVerboseLevel(G4int val) {
    fVerboseLevel = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetROOTFilename(G4String val) {
    fROOTFilename = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetNCrystal(G4int val) {
    fNCrystal = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetCrystalSizeXY(G4double val) {
    fCrystalSizeXY = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetCrystalDepth(G4double val) {
    fCrystalDepth = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetSiPMSizeXY(G4double val) {
    fSiPMSizeXY = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetSiPMDepth(G4double val) {
    fSiPMDepth = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

G4double DetectorConstruction::GetCrystalEnd()
{
    return fCrystalDepth;
}
