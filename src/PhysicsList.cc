#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "OpticalPhysics.hh"

#include "QGSP_BERT_HP.hh"
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
    fVerboseLevel = 0;

    fMessenger = new PhysicsListMessenger(this);

    G4VModularPhysicsList* phys = new QGSP_BERT_HP();
    for (G4int i = 0; ; ++i) {
        G4VPhysicsConstructor* elem =
        const_cast<G4VPhysicsConstructor*> (phys->GetPhysics(i));
        if (elem == nullptr) break;
        G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
        RegisterPhysics(elem);
    }

    fAbsorptionOn = true;
    G4cout << "RegisterPhysics: OpticalPhysics" << G4endl;
    RegisterPhysics(fOpticalPhysics = new OpticalPhysics(fAbsorptionOn, fVerboseLevel));

    //Add the step limiter
    G4cout << "RegisterPhysics: G4StepLimiterPhysics" << G4endl;
    RegisterPhysics(new G4StepLimiterPhysics());
}

PhysicsList::~PhysicsList()
{
    delete fMessenger;
}

void PhysicsList::ConstructParticle()
{
    G4VModularPhysicsList::ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();
}

void PhysicsList::SetCuts()
{
    if (fVerboseLevel > 0)
    {
        G4cout << "PhysicsList::SetCuts:";
        G4cout << "RangeCut Length : " << G4BestUnit(defaultCutValue, "Length") << G4endl;
    }

    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(fCutForGamma, "gamma");
    SetCutValue(fCutForElectron, "e-");
    SetCutValue(fCutForPositron, "e+");

    if (fVerboseLevel > 0) DumpCutValuesTable();
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

void PhysicsList::SetVerbose(G4int verbose)
{
    fVerboseLevel = verbose;

    fOpticalPhysics->GetScintillationProcess()->SetVerboseLevel(verbose);
    fOpticalPhysics->GetAbsorptionProcess()->SetVerboseLevel(verbose);
    fOpticalPhysics->GetRayleighScatteringProcess()->SetVerboseLevel(verbose);
    fOpticalPhysics->GetMieHGScatteringProcess()->SetVerboseLevel(verbose);
    fOpticalPhysics->GetBoundaryProcess()->SetVerboseLevel(verbose);
}
