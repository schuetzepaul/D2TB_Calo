#ifndef PHOTONDETHIT_HH
#define PHOTONDETHIT_HH 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

class G4VTouchable;

class PhotonDetHit : public G4VHit
{
public:

    PhotonDetHit();
    PhotonDetHit(G4ThreeVector pArrive, G4double pTime);
    virtual ~PhotonDetHit();

    PhotonDetHit(const PhotonDetHit &right);
    const PhotonDetHit& operator=(const PhotonDetHit& right);

    G4bool operator==(const PhotonDetHit& right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    inline void SetArrivalPos(G4ThreeVector xyz) { fPosArrive = xyz; }
    inline G4ThreeVector GetArrivalPos() { return fPosArrive; }

    inline void SetArrivalTime(G4double t) { fArrivalTime = t; }
    inline G4double GetArrivalTime() { return fArrivalTime; }

private:

    // the arrival time of the photon
    G4double      fArrivalTime;
    // where the photon hit the detector (detector's coordinate)
    G4ThreeVector fPosArrive;
};

//--------------------------------------------------
// Type Definitions
//--------------------------------------------------

typedef G4THitsCollection<PhotonDetHit> PhotonDetHitsCollection;

extern G4ThreadLocal G4Allocator<PhotonDetHit>* PhotonDetHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* PhotonDetHit::operator new(size_t)
{
    if(!PhotonDetHitAllocator)
    PhotonDetHitAllocator = new G4Allocator<PhotonDetHit>;
    return (void *) PhotonDetHitAllocator->MallocSingle();
}

inline void PhotonDetHit::operator delete(void *aHit)
{
    PhotonDetHitAllocator->FreeSingle((PhotonDetHit*) aHit);
}

#endif
