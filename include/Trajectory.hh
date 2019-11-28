#ifndef Trajectory_hh
#define Trajectory_hh 1

#include <vector>
#include <stdlib.h>

#include "globals.hh"

#include "G4ios.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4Trajectory.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"

class G4Polyline;

class Trajectory : public G4Trajectory
{
public:

    // Constructor/Destructor
    Trajectory();
    Trajectory(const G4Track* );
    Trajectory(Trajectory &);
    virtual ~Trajectory();

    virtual void DrawTrajectory() const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);
    inline int operator == (const Trajectory& right) const { return (this == &right); }

    // Get/Set functions
    void SetDrawTrajectory(G4bool b){fDrawit=b;}

private:
    // Member data
    G4bool fDrawit;

    G4ParticleDefinition* fParticleDefinition;

};

extern G4ThreadLocal G4Allocator<Trajectory>* TrajectoryAllocator;

inline void* Trajectory::operator new(size_t)
{
    if(!TrajectoryAllocator)
    TrajectoryAllocator = new G4Allocator<Trajectory>;
    return (void*)TrajectoryAllocator->MallocSingle();
}

inline void Trajectory::operator delete(void* aTrajectory)
{
    TrajectoryAllocator->FreeSingle((Trajectory*)aTrajectory);
}

#endif
