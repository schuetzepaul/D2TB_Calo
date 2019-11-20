/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "G4Timer.hh"

#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include <ctime>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: fTimer(0)
{
    fTimer = new G4Timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
    delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    //to be sure to generate different events!
    G4cout << "*******************" << G4endl;
    G4cout << "*** AUTOSEED ON ***" << G4endl;
    G4cout << "*******************" << G4endl;

    long seeds[2];
    long systime = time(NULL);
    seeds[0] = systime;
    seeds[1] = systime * G4UniformRand();
    G4Random::setTheSeeds(seeds);
    G4Random::showEngineStatus();

    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    fTimer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    fTimer->Stop();
    G4cout << "Number of event = " << aRun->GetNumberOfEvent() << " " << *fTimer << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
