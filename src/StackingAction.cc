/// \file StackingAction.cc
/// \brief Implementation of the StackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "StackingAction.hh"

#include "G4Track.hh"
#include "G4ParticleDefinition.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::StackingAction()
:G4UserStackingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
    // Get the particle type of the new track.
    const G4ParticleDefinition* particle = aTrack->GetDefinition();

    if (aTrack->GetParentID() <= 0) return fUrgent;

    // This is where we can throw away particles that we don't want to track.
    // Drop photons below the "lowest" nuclear lines.  The lowest I know if is
    // about 6 keV, and atomic shells start messing with the cross section at
    // about 70 keV.
    if (particle->GetParticleName() == "gamma") {
        if (aTrack->GetKineticEnergy() < 10.*CLHEP::keV) return fKill;
    }

    // if (particle->GetParticleName() == "opticalphoton") {
    //     return fKill;
    // }
    //
    // if (particle->GetParticleName() == "thermalelectron") {
    //     return fKill;
    // }

    return G4UserStackingAction::ClassifyNewTrack(aTrack);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
