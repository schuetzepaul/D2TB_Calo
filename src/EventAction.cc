/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"

#include "PhotonDetHit.hh"

#include "G4SDManager.hh"

#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
: fVerboseLevel(1),
fMPPCCollID(0)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
    G4int evtNb = evt->GetEventID();

    if(fVerboseLevel>0)
    G4cout << "<<< Event " << evtNb << " started." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
    if (fVerboseLevel>0)
    G4cout << "<<< Event " << evt->GetEventID() << " ended." << G4endl;

    // Get Hits from the detector if any
    G4SDManager * SDman = G4SDManager::GetSDMpointer();
    G4String colName = "PhotonDetHitCollection";
    fMPPCCollID = SDman->GetCollectionID(colName);

    G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
    PhotonDetHitsCollection* mppcHC = 0;

    // Get the hit collections
    if (HCE)
    {
        if (fMPPCCollID>=0) mppcHC = (PhotonDetHitsCollection*)(HCE->GetHC(fMPPCCollID));
    }

    // Get hit information about photons that reached the detector in this event
    if (mppcHC)
    {
        G4int n_hit = mppcHC->entries();
        G4cout << "<<< Number of photon detected " << n_hit << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
