#include "CrystalParameterisation.hh"

#include "G4Box.hh"

#include "G4VPhysicalVolume.hh"

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
