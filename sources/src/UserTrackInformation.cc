#include "G4ios.hh"
#include "G4ThreeVector.hh"

#include "UserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserTrackInformation::UserTrackInformation ()
{
    fStatus = active;
    fExitPosition = G4ThreeVector(0.,0.,0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UserTrackInformation::~UserTrackInformation () { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UserTrackInformation::AddTrackStatusFlag(TrackStatus s)
{
    if(s & active) {
        //track is now active
        fStatus &= ~inactive; //remove any flags indicating it is inactive
    }
    else if(s & inactive) {
        //track is now inactive
        fStatus &= ~active; //remove any flags indicating it is active
        fStatus |= s; //add new flags
    }
}
