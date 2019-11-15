#ifndef OpticalPhysics_h
#define OpticalPhysics_h 1

#include "globals.hh"
#include "G4Scintillation.hh"
#include "G4OpMieHG.hh"
#include "G4OpRayleigh.hh"
#include "G4OpAbsorption.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4VPhysicsConstructor.hh"

class OpticalPhysics : public G4VPhysicsConstructor
{
public:

    OpticalPhysics(G4bool toggle, G4int verbose);

    virtual ~OpticalPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

    G4Scintillation* GetScintillationProcess() {return fScintillationProcess;}
    G4OpAbsorption* GetAbsorptionProcess() {return fAbsorptionProcess;}
    G4OpRayleigh* GetRayleighScatteringProcess() {return fRayleighScatteringProcess;}
    G4OpMieHG* GetMieHGScatteringProcess() {return fMieHGScatteringProcess;}
    G4OpBoundaryProcess* GetBoundaryProcess() { return fBoundaryProcess;}

private:

    G4Scintillation*     fScintillationProcess;
    G4OpAbsorption*      fAbsorptionProcess;
    G4OpRayleigh*        fRayleighScatteringProcess;
    G4OpMieHG*           fMieHGScatteringProcess;
    G4OpBoundaryProcess* fBoundaryProcess;

    G4bool fAbsorptionOn;
    G4int fVerboseLevel;
};

#endif
