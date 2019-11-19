#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4ProcessManager.hh"

#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "G4Decay.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4hIonisation.hh"

#include "G4Threading.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4int PhysicsList::fVerboseLevel = 1;
G4ThreadLocal G4Scintillation* PhysicsList::fScintillationProcess = 0;
G4ThreadLocal G4OpAbsorption* PhysicsList::fAbsorptionProcess = 0;
G4ThreadLocal G4OpRayleigh* PhysicsList::fRayleighScatteringProcess = 0;
G4ThreadLocal G4OpBoundaryProcess* PhysicsList::fBoundaryProcess = 0;

PhysicsList::PhysicsList()
: G4VUserPhysicsList()
{
    fDefaultCutValue = 1.0*mm;
    fMessenger = new PhysicsListMessenger(this);
}

PhysicsList::~PhysicsList()
{
    delete fMessenger;
}

void PhysicsList::ConstructParticle()
{
    // In this method, static member functions should be called
    // for all particles which you want to use.
    // This ensures that objects of these particle types will be
    // created in the program.

    G4BosonConstructor bConstructor;
    bConstructor.ConstructParticle();

    G4LeptonConstructor lConstructor;
    lConstructor.ConstructParticle();

    G4MesonConstructor mConstructor;
    mConstructor.ConstructParticle();

    G4BaryonConstructor rConstructor;
    rConstructor.ConstructParticle();

    G4IonConstructor iConstructor;
    iConstructor.ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
    AddTransportation();
    ConstructDecay();
    ConstructEM();
    ConstructOp();
}

void PhysicsList::ConstructDecay()
{
    // Add Decay Process
    G4Decay* theDecayProcess = new G4Decay();
    auto particleIterator=GetParticleIterator();
    particleIterator->reset();
    while( (*particleIterator)() ){
        G4ParticleDefinition* particle = particleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        if (theDecayProcess->IsApplicable(*particle)) {
            pmanager->AddProcess(theDecayProcess);
            // set ordering for PostStepDoIt and AtRestDoIt
            pmanager->SetProcessOrdering(theDecayProcess, idxPostStep);
            pmanager->SetProcessOrdering(theDecayProcess, idxAtRest);
        }
    }
}

void PhysicsList::ConstructEM()
{
    auto particleIterator=GetParticleIterator();
    particleIterator->reset();
    while( (*particleIterator)() ){
        G4ParticleDefinition* particle = particleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();

        if (particleName == "gamma") {
            // gamma
            // Construct processes for gamma
            pmanager->AddDiscreteProcess(new G4GammaConversion());
            pmanager->AddDiscreteProcess(new G4ComptonScattering());
            pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

        } else if (particleName == "e-") {
            //electron
            // Construct processes for electron
            pmanager->AddProcess(new G4eMultipleScattering(),-1, 1, 1);
            pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);

        } else if (particleName == "e+") {
            //positron
            // Construct processes for positron
            pmanager->AddProcess(new G4eMultipleScattering(),-1, 1, 1);
            pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);
            pmanager->AddProcess(new G4eplusAnnihilation(),  0,-1, 4);

        } else if( particleName == "mu+" ||
        particleName == "mu-"    ) {
            //muon
            // Construct processes for muon
            pmanager->AddProcess(new G4MuMultipleScattering(),-1, 1, 1);
            pmanager->AddProcess(new G4MuIonisation(),      -1, 2, 2);
            pmanager->AddProcess(new G4MuBremsstrahlung(),  -1, 3, 3);
            pmanager->AddProcess(new G4MuPairProduction(),  -1, 4, 4);

        } else {
            if ((particle->GetPDGCharge() != 0.0) &&
            (particle->GetParticleName() != "chargedgeantino") &&
            !particle->IsShortLived()) {
                // all others charged particles except geantino
                pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
                pmanager->AddProcess(new G4hIonisation(),       -1,2,2);
            }
        }
    }
}

void PhysicsList::ConstructOp()
{
    fScintillationProcess = new G4Scintillation("Scintillation");
    fScintillationProcess->SetScintillationYieldFactor(1.);
    fScintillationProcess->SetScintillationExcitationRatio(0.0);
    fScintillationProcess->SetTrackSecondariesFirst(true);
    fAbsorptionProcess = new G4OpAbsorption();
    fRayleighScatteringProcess = new G4OpRayleigh();
    fBoundaryProcess = new G4OpBoundaryProcess();

    fScintillationProcess->SetVerboseLevel(fVerboseLevel);
    fAbsorptionProcess->SetVerboseLevel(fVerboseLevel);
    fRayleighScatteringProcess->SetVerboseLevel(fVerboseLevel);
    fBoundaryProcess->SetVerboseLevel(fVerboseLevel);

    // Use Birks Correction in the Scintillation process
    if(G4Threading::IsMasterThread())
    {
        G4cout << " Use Birks Correction for Scintillation " << G4endl;
        G4EmSaturation* emSaturation =
        G4LossTableManager::Instance()->EmSaturation();
        fScintillationProcess->AddSaturation(emSaturation);
    }

    auto particleIterator=GetParticleIterator();
    particleIterator->reset();
    while( (*particleIterator)() ){
        G4ParticleDefinition* particle = particleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();

        if (fScintillationProcess->IsApplicable(*particle)) {
            pmanager->AddProcess(fScintillationProcess);
            pmanager->SetProcessOrderingToLast(fScintillationProcess, idxAtRest);
            pmanager->SetProcessOrderingToLast(fScintillationProcess, idxPostStep);
        }

        if (particleName == "opticalphoton") {
            G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
            pmanager->AddDiscreteProcess(fAbsorptionProcess);
            pmanager->AddDiscreteProcess(fRayleighScatteringProcess);
            pmanager->AddDiscreteProcess(fBoundaryProcess);
        }
    }
}

void PhysicsList::SetVerbose(G4int verbose)
{
    fVerboseLevel = verbose;

    fScintillationProcess->SetVerboseLevel(fVerboseLevel);
    fAbsorptionProcess->SetVerboseLevel(fVerboseLevel);
    fRayleighScatteringProcess->SetVerboseLevel(fVerboseLevel);
    fBoundaryProcess->SetVerboseLevel(fVerboseLevel);
}

void PhysicsList::SetCuts()
{
    if(verboseLevel > 0){
        G4cout << "PhysicsList::SetCuts:" << G4endl;
        G4cout << "Default CutLength : " << G4BestUnit(fDefaultCutValue,"Length") << G4endl;
    }

    G4double lowlimit = 250*eV;
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowlimit, 100.*GeV);

    SetCutValue(fDefaultCutValue, "gamma");
    SetCutValue(fDefaultCutValue, "e-");
    SetCutValue(fDefaultCutValue, "proton");
    SetCutValue(fDefaultCutValue, "e+");

    if (verboseLevel > 0) DumpCutValuesTable();
}
