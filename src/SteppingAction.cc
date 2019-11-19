/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4ProcessManager.hh"
#include "G4OpBoundaryProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
: G4UserSteppingAction()
{
    fBounceLimit = 100000;
    fOpProcess = NULL;
    ResetCounters();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step *theStep) {

    G4Track* theTrack = theStep->GetTrack();

    G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
    G4StepPoint* thePostPoint = theStep->GetPostStepPoint();

    G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
    G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

    G4String thePrePVname  = " ";
    G4String thePostPVname = " ";

    if (thePostPV) {
        thePrePVname  = thePrePV->GetName();
        thePostPVname = thePostPV->GetName();
    }

    // Retrieve the status of the photon
    G4OpBoundaryProcessStatus theStatus = Undefined;
    G4ProcessManager* OpManager =  G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

    if (OpManager) {
        G4int MAXofPostStepLoops =
        OpManager->GetPostStepProcessVector()->entries();
        G4ProcessVector* fPostStepDoItVector =
        OpManager->GetPostStepProcessVector(typeDoIt);

        for ( G4int i = 0; i < MAXofPostStepLoops; i++) {
            G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
            fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
            if (fOpProcess) { theStatus = fOpProcess->GetStatus(); break;}
        }
    }

    switch (theStatus) {

    case FresnelReflection:
        fCounterBounce++;
        return;

    case TotalInternalReflection:
        // Kill the track if it's number of bounces exceeded the limit
        if (fBounceLimit > 0 && fCounterBounce >= fBounceLimit)
        {
            theTrack->SetTrackStatus(fStopAndKill);
            ResetCounters();
            G4cout << "\n Bounce Limit Exceeded" << G4endl;
            return;
        }
        break;

        // Detected by a detector
    case Detection:
        if ( thePostPVname == "GlassBox" ) {
            G4cout << "Arrived at the glass" << G4endl;
            // Stop Tracking when it hits the detector's surface
            ResetCounters();
            theTrack->SetTrackStatus(fStopAndKill);
            return;
        }
        break;

    default: break;

    }

    // Check for absorbed photons
    if (theTrack->GetTrackStatus() != fAlive){
        ResetCounters();
        return;
    }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
