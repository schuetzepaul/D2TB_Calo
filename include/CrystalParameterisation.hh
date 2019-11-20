#include "G4VPVParameterisation.hh"

class G4VPhysicalVolume;
class G4Box;

//Parametrisation for the crystal position
class CrystalParameterisation : public G4VPVParameterisation
{

public:
    CrystalParameterisation(G4double XYLength, G4double ZLength, G4double firstX, G4double firstY, G4double firstZ);

    virtual ~CrystalParameterisation();

    // position, rotation
    virtual void ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const;

    // size
    virtual void ComputeDimensions(G4Box& crystalBox, const G4int copyNo, const G4VPhysicalVolume* physVol) const;

    // shape
    // virtual G4VSolid* ComputeSolid(const G4int copyNo, G4VPhysicalVolume* physVol);

    // material, sensitivity, visAtt
    // virtual G4Material* ComputeMaterial(const G4int copyNo, G4VPhysicalVolume* physVol, const G4VTouchable *parentTouch=0);
    // G4VTouchable should not be used for ordinary parameterization

private:
    G4double fXhalfLength;
    G4double fYhalfLength;
    G4double fZhalfLength;
    G4double fStartX;
    G4double fStartY;
    G4double fStartZ;
};
