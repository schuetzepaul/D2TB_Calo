#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
fG4ParticleGun(nullptr),
fGPSParticleGun(nullptr)
{
    fG4ParticleGun  = new G4ParticleGun(1);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    fG4ParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
    //Default energy,position,momentum
    fG4ParticleGun->SetParticleEnergy(1.*keV);
    fG4ParticleGun->SetParticlePosition(G4ThreeVector(0*cm , 0*cm, -0.5*cm));
    fG4ParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

    // fGPSParticleGun  = new G4GeneralParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fG4ParticleGun;
    // delete fGPSParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // This function is called at the begining of event
    fG4ParticleGun->GeneratePrimaryVertex(anEvent);
    // fGPSParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
