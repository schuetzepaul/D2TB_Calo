#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4ProcessTable.hh"
#include "G4SystemOfUnits.hh"

#include "unistd.h"

PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{
    G4LossTableManager::Instance();
    defaultCutValue  = 0.7*mm;
    fCutForGamma     = defaultCutValue;
    fCutForElectron  = defaultCutValue;
    fCutForPositron  = defaultCutValue;

    fMessenger = new PhysicsListMessenger(this);

    SetVerboseLevel(2);

    // * EM Physics
    RegisterPhysics( new G4EmStandardPhysics() );

    // * Decay processes
    RegisterPhysics( new G4DecayPhysics() );

    //Add the Optical Photon physics list
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    opticalPhysics->SetScintillationYieldFactor(1.0);
    opticalPhysics->SetScintillationExcitationRatio(0.0);
    opticalPhysics->SetMaxNumPhotonsPerStep(300);
    opticalPhysics->SetMaxBetaChangePerStep(10.0);
    opticalPhysics->SetTrackSecondariesFirst(kScintillation, true);
    RegisterPhysics(opticalPhysics);

    //Add the step limiter
    RegisterPhysics(new G4StepLimiterPhysics());
}

PhysicsList::~PhysicsList()
{
    delete fMessenger;
}

void PhysicsList::SetCuts()
{
    if (verboseLevel >0)
    {
        G4cout << "PhysicsList::SetCuts:";
        G4cout << "RangeCut Length : " << G4BestUnit(defaultCutValue, "Length") << G4endl;
    }

    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(fCutForGamma, "gamma");
    SetCutValue(fCutForElectron, "e-");
    SetCutValue(fCutForPositron, "e+");

    if (verboseLevel>0) DumpCutValuesTable();
}

void PhysicsList::SetCutForGamma(G4double cut) {
    fCutForGamma = cut;
    SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

void PhysicsList::SetCutForElectron(G4double cut) {
    fCutForElectron = cut;
    SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

void PhysicsList::SetCutForPositron(G4double cut) {
    fCutForPositron = cut;
    SetParticleCuts(fCutForPositron, G4Positron::Positron());
}
