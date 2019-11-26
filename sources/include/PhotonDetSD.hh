#ifndef PhotonDetSD_h
#define PhotonDetSD_h 1

#include "PhotonDetHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class PhotonDetSD : public G4VSensitiveDetector
{
  public:

    PhotonDetSD(G4String );
    virtual ~PhotonDetSD();

    virtual void Initialize(G4HCofThisEvent* );

    virtual G4bool ProcessHits(G4Step* , G4TouchableHistory* );

    //A version of processHits that keeps aStep constant
    G4bool ProcessHits_constStep(const G4Step* ,  G4TouchableHistory* );

    //For the end of the event
    virtual void EndOfEvent(G4HCofThisEvent*);

  private:

    PhotonDetHitsCollection* fPhotonDetHitCollection;
};

#endif
