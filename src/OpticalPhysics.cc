#include "OpticalPhysics.hh"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "G4OpticalPhoton.hh"

#include "G4ProcessManager.hh"

OpticalPhysics::OpticalPhysics(G4bool toggle, G4int verbose)
: G4VPhysicsConstructor("Optical"),
fScintillationProcess(nullptr),
fAbsorptionProcess(nullptr),
fRayleighScatteringProcess(nullptr),
fMieHGScatteringProcess(nullptr),
fBoundaryProcess(nullptr),
fAbsorptionOn(toggle),
fVerboseLevel(verbose)
{

}

OpticalPhysics::~OpticalPhysics() { }

void OpticalPhysics::ConstructParticle()
{
    G4OpticalPhoton::OpticalPhotonDefinition();
}

void OpticalPhysics::ConstructProcess()
{
    G4cout << "OpticalPhysics:: Add Optical Physics Processes" << G4endl;

    fScintillationProcess = new G4Scintillation("Scintillation");
    fScintillationProcess->SetScintillationYieldFactor(1.);
    fScintillationProcess->SetTrackSecondariesFirst(true);

    fAbsorptionProcess      = new G4OpAbsorption();
    fRayleighScatteringProcess     = new G4OpRayleigh();
    fMieHGScatteringProcess = new G4OpMieHG();
    fBoundaryProcess        = new G4OpBoundaryProcess();

    fScintillationProcess->SetVerboseLevel(fVerboseLevel);
    fAbsorptionProcess->SetVerboseLevel(fVerboseLevel);
    fRayleighScatteringProcess->SetVerboseLevel(fVerboseLevel);
    fMieHGScatteringProcess->SetVerboseLevel(fVerboseLevel);
    fBoundaryProcess->SetVerboseLevel(fVerboseLevel);

    // Use Birks Correction in the Scintillation process
    G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
    fScintillationProcess->AddSaturation(emSaturation);

    auto particleIterator = GetParticleIterator();
    particleIterator->reset();
    while ( (*particleIterator)() )
    {
        G4ParticleDefinition* particle = particleIterator->value();
        G4String particleName = particle->GetParticleName();
        G4ProcessManager* pManager = particle->GetProcessManager();

        if (!pManager) {
            std::ostringstream o;
            o << "Particle " << particleName << "without a Process Manager";
            G4Exception("OpticalPhysics::ConstructProcess()","", FatalException,o.str().c_str());
        }

        if(fScintillationProcess->IsApplicable(*particle)) {
            pManager->AddProcess(fScintillationProcess);
            pManager->SetProcessOrderingToLast(fScintillationProcess,idxAtRest);
            pManager->SetProcessOrderingToLast(fScintillationProcess,idxPostStep);
        }

        if (particleName == "opticalphoton") {
            G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
            if (fAbsorptionOn) pManager->AddDiscreteProcess(fAbsorptionProcess);
            pManager->AddDiscreteProcess(fRayleighScatteringProcess);
            pManager->AddDiscreteProcess(fMieHGScatteringProcess);
            pManager->AddDiscreteProcess(fBoundaryProcess);
        }
    }
}
