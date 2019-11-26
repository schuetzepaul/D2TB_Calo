#ifndef UserTrackInformation_h
#define UserTrackInformation_h 1

#include "G4VUserTrackInformation.hh"

#include "G4ThreeVector.hh"

enum TrackStatus {
undefined=0, defined=1,
EscapedFromSide=2, EscapedFromReadOut=3, ReflectedAtReadOut=4,
murderee=5, InsideOfCrystal=6, OutsideOfCrystal=7, hitSiPM=8
};

/*TrackStatus:
undefined:
EscapedFromSide:        photon escaped through the side of the crystal
EscapedFromReadOut:     photon escaped through the read-out end of the crystal
ReflectedAtReadOut:     photon has been reflected at the read-out end
murderee                photon is artificially killed
InsideOfCrystal         Flag is on if the photon is inside of crystal
OutsideOfCrystal        Flag is on if the photon is outside of crystal
hitSiPM                 Flag is on if the photon hit the SiPM
*/

class UserTrackInformation : public G4VUserTrackInformation
{

public:

    //Constructor/Destructor
    UserTrackInformation();
    virtual ~UserTrackInformation();

    const G4ThreeVector& GetExitPosition() const { return fExitPosition; }
    void SetExitPosition (const G4ThreeVector& pos) { fExitPosition = pos; }

    // Try adding a status flag and return if it is successful or not
    // Cannot Add Undefine or a flag that conflicts with another flag
    // Return true if the addition of flag is successful, false otherwise
    G4bool AddStatusFlag(TrackStatus s);

    // Check if a certain flag is on
    G4bool IsStatus(TrackStatus s) { return s == undefined ? !(fStatus &= defined) : fStatus & s; }

private:
    //members
    G4int fStatus;
    G4ThreeVector fExitPosition;

};

#endif
