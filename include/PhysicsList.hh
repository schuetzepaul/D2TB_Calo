#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "globals.hh"
#include "G4VUserPhysicsList.hh"

class PhysicsListMessenger;

class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;

class PhysicsList: public G4VUserPhysicsList
{
public:

    /// Construct the physics list.
    PhysicsList();

    virtual ~PhysicsList();

    virtual void ConstructParticle();

    virtual void ConstructProcess();

    virtual void SetCuts();

    //these methods Construct physics processes and register them
    void ConstructDecay();
    void ConstructEM();
    void ConstructOp();

    //for the Messenger
    void SetVerbose(G4int);

private:

    /// The messenger to control this class.
    PhysicsListMessenger* fMessenger;
    G4double fDefaultCutValue;

    static G4ThreadLocal G4int fVerboseLevel;
    static G4ThreadLocal G4Scintillation* fScintillationProcess;
    static G4ThreadLocal G4OpAbsorption* fAbsorptionProcess;
    static G4ThreadLocal G4OpRayleigh* fRayleighScatteringProcess;
    static G4ThreadLocal G4OpBoundaryProcess* fBoundaryProcess;

};

#endif
