#ifndef Trajectory_hh
#define Trajectory_hh 1

#include <vector>
#include <stdlib.h>

#include "globals.hh"

#include "G4ios.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VTrajectory.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"

typedef std::vector<G4VTrajectoryPoint*> TrajectoryPointContainer;

class Trajectory : public G4VTrajectory
{
public:

    // Constructor/Destructor
    Trajectory();
    Trajectory(const G4Track* );
    Trajectory(Trajectory &);
    virtual ~Trajectory();

    // Operators
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    inline int operator == (const Trajectory& right) const { return (this == &right); }

    // Get/Set functions
    inline virtual G4int GetTrackID() const { return fTrackID; }
    inline virtual G4int GetParentID() const { return fParentID; }
    inline virtual G4String GetParticleName() const { return fParticleName; }
    inline virtual G4double GetCharge() const { return fPDGCharge; }
    inline virtual G4int GetPDGEncoding() const { return fPDGEncoding; }
    inline virtual G4ThreeVector GetInitialMomentum() const
    {return fInitialMomentum;}

    // Other member functions
    virtual void ShowTrajectory(std::ostream& os=G4cout) const;
    virtual void AppendStep(const G4Step* aStep);
    virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

    G4ParticleDefinition* GetParticleDefinition();

    virtual int GetPointEntries() const { return fpPointsContainer->size(); }
    virtual G4VTrajectoryPoint* GetPoint(G4int i) const { return (*fpPointsContainer)[i]; }

    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;

private:
    // Member data
    TrajectoryPointContainer* fpPointsContainer;

    G4int fTrackID;
    G4int fParentID;
    G4double fPDGCharge;
    G4int    fPDGEncoding;
    G4String fParticleName;
    G4ThreeVector fInitialMomentum;

    G4ParticleDefinition* fParticleDefinition;

};

extern G4ThreadLocal G4Allocator<Trajectory>* TrajectoryAllocator;

inline void* Trajectory::operator new(size_t) {
    if(!TrajectoryAllocator)
    TrajectoryAllocator = new G4Allocator<Trajectory>;
    return (void*) TrajectoryAllocator->MallocSingle();
}

inline void Trajectory::operator delete(void* aTrajectory) {
    TrajectoryAllocator->FreeSingle((Trajectory*)aTrajectory);
}

#endif
