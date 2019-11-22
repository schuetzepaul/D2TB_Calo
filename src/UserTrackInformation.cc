#include "G4ios.hh"
#include "G4ThreeVector.hh"

#include "UserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserTrackInformation::UserTrackInformation ()
{
    fStatus = undefined;
    fExitPosition = G4ThreeVector(0.,0.,0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserTrackInformation::~UserTrackInformation () { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool UserTrackInformation::AddStatusFlag(TrackStatus s)
// Try adding a status flag and return if it is successful or not
// Cannot Add Undefine or a flag that conflicts with another flag
// Return true if the addition of flag is successful, false otherwise
{
    switch (s) {

    case EscapedFromSide:
    case EscapedFromReadOut:
        // Allow the user to set escaped flag
        // only if the photon hasn't exited the fiber yet
        if ((fStatus == undefined) || (fStatus & OutsideOfCrystal)) return false;
        return fStatus |= s;

    case murderee:
        return fStatus |= s;

    case InsideOfCrystal:
        return ( fStatus = (fStatus & ~(EscapedFromSide + EscapedFromReadOut + OutsideOfCrystal)) | s);

    case OutsideOfCrystal:
        return ( fStatus = (fStatus & ~InsideOfCrystal) | s );

    case hitSiPM:

    default:
        return false;
    }
}
