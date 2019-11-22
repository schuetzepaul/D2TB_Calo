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
class G4AttDef;
class G4AttValue;

class PhotonDetHit : public G4VHit
{
public:

    PhotonDetHit();
    PhotonDetHit(G4ThreeVector pExit, G4ThreeVector pArrive, G4ThreeVector pArriveLocal, G4double pTime, G4LogicalVolume* pLogV);
    virtual ~PhotonDetHit();

    PhotonDetHit(const PhotonDetHit &right);
    const PhotonDetHit& operator=(const PhotonDetHit& right);

    G4bool operator==(const PhotonDetHit& right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;

    inline void SetArrivalPos(G4ThreeVector xyz) { fPosArrive = xyz; }
    inline G4ThreeVector GetArrivalPos() { return fPosArrive; }

    inline void SetArrivalPosLocal(G4ThreeVector xyz) { fPosArriveLocal = xyz; }
    inline G4ThreeVector GetArrivalPosLocal() { return fPosArriveLocal; }

    inline void SetExitPos(G4ThreeVector xyz) { fPosExit = xyz; }
    inline G4ThreeVector GetExitPos() { return fPosExit; }

    inline void SetArrivalTime(G4double t) { fArrivalTime = t; }
    inline G4double GetArrivalTime() { return fArrivalTime; }

    inline void SetLogicalVolume(G4LogicalVolume* val) { fLogicalVolume = val; }
    inline const G4LogicalVolume* GetLogicalVolume() const { return fLogicalVolume; }

private:

    // the arrival time of the photon
    G4double      fArrivalTime;
    // where the photon hit the detector (world's coordinate)
    G4ThreeVector fPosArrive;
    // where the photon hit the detector (local volume coordinate)
    G4ThreeVector fPosArriveLocal;
    // where the photon exited the fiber (world's coordinate)
    G4ThreeVector fPosExit;
    // Logical Volume of the Hit
    const G4LogicalVolume* fLogicalVolume;
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
