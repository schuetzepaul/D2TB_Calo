#ifndef UserTrackInformation_h
#define UserTrackInformation_h 1

#include "G4VUserTrackInformation.hh"

#include "G4ThreeVector.hh"

enum TrackStatus {
    active=1, hitSiPM=2, absorbed=4, boundaryAbsorbed=8,
    murderee=16, inactive=14, insideOfCrystal=18
};

/*TrackStatus:
active: still being tracked
hitSiPM: stopped by being detected in a SiPM
absorbed: stopped by being absorbed with G4OpAbsorbtion
boundaryAbsorbed: stopped by being aborbed with G4OpAbsorbtion
murderee: killed track
inactive: track is stopped for some reason
InsideOfCrystal: track is inside the crystal
-This is the sum of all stopped flags so can be used to remove stopped flags
*/

class UserTrackInformation : public G4VUserTrackInformation
{

public:

    //Constructor/Destructor
    UserTrackInformation();
    virtual ~UserTrackInformation();

    const G4ThreeVector& GetExitPosition() const { return fExitPosition; }
    void SetExitPosition (const G4ThreeVector& pos) { fExitPosition = pos; }

    //Does a smart add of track status flags (disabling old flags that conflict)
    //If s conflicts with itself it will not be detected
    void AddTrackStatusFlag(TrackStatus s);

    //Returns the Track status
    int GetTrackStatus() const { return fStatus; }

private:
    //members
    G4int fStatus;
    G4ThreeVector fExitPosition;

};

#endif
