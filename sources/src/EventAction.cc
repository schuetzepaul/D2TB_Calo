/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "D2TBRun.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"


#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
: fVerboseLevel(1),
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
    fPhotonCount_Scint = 0;
    fAbsorptionCount = 0;
    fBoundaryAbsorptionCount = 0;

    if(fVerboseLevel>0)
    G4cout << "<<< Event " << evtNb << " started." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
    if (fVerboseLevel>0)
    G4cout << "<<< Event " << evt->GetEventID() << " ended." << G4endl;

    // update the run statistics
    D2TBRun* run = static_cast<D2TBRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

    run->IncPhotonCount_Scint(fPhotonCount_Scint);
    run->IncAbsorption(fAbsorptionCount);
    run->IncBoundaryAbsorption(fBoundaryAbsorptionCount);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
