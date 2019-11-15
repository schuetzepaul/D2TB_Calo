#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class G4VPhysicsConstructor;
class PhysicsListMessenger;
class OpticalPhysics;

/// Use the G4PhysListFactory to select a physics list for this run.  The
/// physics list can be set from the PHYSLIST environment variable, or a macro
/// file. All of the physics lists need to be defined before using the
/// /run/initialize command.

class PhysicsList: public G4VModularPhysicsList
{
public:

    /// Construct the physics list.
    explicit PhysicsList();

    virtual ~PhysicsList();

    /// Contruct the physics list
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    /// Used by GEANT4 to set the cuts defined below.
    virtual void SetCuts();

    /// Set the range cut for photons.
    void SetCutForGamma(G4double cut);

    /// Set the range cut for electrons.
    void SetCutForElectron(G4double cut);

    /// Set the range cut for positrons.
    void SetCutForPositron(G4double cut);

    /// Set the verbose level
    void SetVerbose(G4int verbose);

private:

    /// The gamma-ray range cut.
    G4double fCutForGamma;

    /// The electron range cut.
    G4double fCutForElectron;

    /// The positron range cut.
    G4double fCutForPositron;

    /// The messenger to control this class.
    PhysicsListMessenger* fMessenger;

    /// Optical Physics List
    OpticalPhysics* fOpticalPhysics;

    // Toggle Absorption
    G4bool fAbsorptionOn;

    // Verbose
    G4int fVerboseLevel;

};

#endif
