/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "D2TBRun.hh"
#include "Trajectory.hh"
#include "PhotonDetHit.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
: fVerboseLevel(1),
fPhotonDetCollID(-1),
fHitCount(0),
fPhotonCount_Scint(0),
fAbsorptionCount(0),
fBoundaryAbsorptionCount(0)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
    G4int evtNb = evt->GetEventID();
    fHitCount = 0;
    fPhotonCount_Scint = 0;
    fAbsorptionCount = 0;
    fBoundaryAbsorptionCount = 0;

    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    if(fPhotonDetCollID < 0)
    fPhotonDetCollID = SDman->GetCollectionID("PhotonDetHitCollection");

    if(fVerboseLevel>0)
    G4cout << "<<< Event " << evtNb << " started." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
    if (fVerboseLevel>0)
    G4cout << "<<< Event " << evt->GetEventID() << " ended." << G4endl;

    G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

    // extract the trajectories and draw them
    if (G4VVisManager::GetConcreteInstance()) {
        for (G4int i=0; i<n_trajectories; i++) {
            Trajectory* trj = (Trajectory*)((*(evt->GetTrajectoryContainer()))[i]);
            trj->DrawTrajectory();
        }
    }

    G4HCofThisEvent* hitsCE = evt->GetHCofThisEvent();
    PhotonDetHitsCollection* SiPMHC = nullptr;
    if(hitsCE){
        if(fPhotonDetCollID>=0) {
            SiPMHC = (PhotonDetHitsCollection*)(hitsCE->GetHC(fPhotonDetCollID));
        }
    }

    if(SiPMHC){
        G4int nhits = SiPMHC->entries();
        fHitCount += nhits;
    }

    // update the run statistics
    D2TBRun* run = static_cast<D2TBRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

    run->IncHitCount(fHitCount);
    run->IncPhotonCount_Scint(fPhotonCount_Scint);
    run->IncAbsorption(fAbsorptionCount);
    run->IncBoundaryAbsorption(fBoundaryAbsorptionCount);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
