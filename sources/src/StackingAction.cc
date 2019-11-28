/// \file StackingAction.cc
/// \brief Implementation of the StackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "StackingAction.hh"
#include "EventAction.hh"

#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::StackingAction(EventAction* ea)
: fEventAction(ea),
fScintillationCounter(0)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
    if (aTrack->GetParentID() == 0) return fUrgent;

    // particle is optical photon
    if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
        if(aTrack->GetParentID() > 0)     // particle is secondary
        {
            if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation"){
                fScintillationCounter++;
                fEventAction->IncPhotonCount_Scint();
            }
        }
    }

    return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StackingAction::NewStage()
{
    G4cout << "StackingAction::NewStage() : Number of Scintillation photons produced in this event : " << fScintillationCounter << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StackingAction::PrepareNewEvent()
{
    fScintillationCounter = 0;
}
