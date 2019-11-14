#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4PhysListFactory.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4ProcessTable.hh"
#include "G4SystemOfUnits.hh"

#include "unistd.h"

PhysicsList::PhysicsList(G4String physName)
: G4VModularPhysicsList()
{
    G4LossTableManager::Instance();
    defaultCutValue  = 0.7*mm;
    fCutForGamma     = defaultCutValue;
    fCutForElectron  = defaultCutValue;
    fCutForPositron  = defaultCutValue;

    fMessenger = new PhysicsListMessenger(this);

    SetVerboseLevel(1);

    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = NULL;

    // Check to see if the physics list has been over ridden from the
    // environment variable PHYSLIST
    char* list = getenv("PHYSLIST");
    if (list) {
        phys = factory.ReferencePhysList();
    }

    // Check if a list name was provided on the command line.  It usually is
    // not provided.
    if (!phys && physName.size() > 1 && factory.IsReferencePhysList(physName))
    {
        G4cout << "!!!------- Using the Geant4 physics list " << physName << " -------!!!" << G4endl;
        phys = factory.GetReferencePhysList(physName);
    }

    // Use the default physics list.
    if (!phys) {
        G4cout << "!!!------- Using default Geant4 physics list QGSP_BERT -------!!!" << G4endl;
        phys = factory.GetReferencePhysList("QGSP_BERT");
    }

    if (!phys) {
        G4cout << "!!!------- No physics list was created -------!!!" << G4endl;
        return;
    }

    //Add the step limiter
    phys->RegisterPhysics(new G4StepLimiterPhysics());

    // Transfer the physics list from the factory to this one.
    for (G4int i = 0; ; ++i)
    {
        G4VPhysicsConstructor* elem = const_cast<G4VPhysicsConstructor*> (phys->GetPhysics(i));
        if (elem == NULL) break;
        G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
        this->RegisterPhysics(elem);
    }
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
