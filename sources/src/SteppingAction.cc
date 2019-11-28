/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "SteppingActionMessenger.hh"

#include "G4Step.hh"
#include "G4Track.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

#include "G4ProcessManager.hh"

#include "PhotonDetSD.hh"

#include "UserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* detector, EventAction *ea)
: fDetector(detector),
fEventAction(ea),
fOpProcess(nullptr),
fBounceLimit(10000)
{
    fSteppingMessenger = new SteppingActionMessenger(this);
    ResetCounters();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {
    delete fSteppingMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::SetBounceLimit(G4int i)
{
    fBounceLimit = i;
    G4cout << "SteppingAction::SetBounceLimit() : Set reflection limit to " << fBounceLimit << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step *theStep) {

    G4Track* theTrack = theStep->GetTrack();
    UserTrackInformation* trackInformation = (UserTrackInformation*)theTrack->GetUserInformation();

    G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
    G4StepPoint* thePostPoint = theStep->GetPostStepPoint();

    G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
    G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

    G4String thePrePVname  = " ";
    G4String thePostPVname = " ";

    //out of world
    if ( !thePostPV  ) {
        ResetCounters();
        return;
    }
    else{
        thePrePVname  = thePrePV->GetName();
        thePostPVname = thePostPV->GetName();
    }

    // Retrieve the status of the photon
    G4OpBoundaryProcessStatus theStatus = Undefined;
    G4ProcessManager* OpManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

    if (OpManager) {
        G4int MAXofPostStepLoops = OpManager->GetPostStepProcessVector()->entries();
        G4ProcessVector* fPostStepDoItVector = OpManager->GetPostStepProcessVector(typeDoIt);

        for ( G4int i=0; i<MAXofPostStepLoops; i++) {
            G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
            fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
            if (fOpProcess) { theStatus = fOpProcess->GetStatus(); break;}
        }
    }

    G4ParticleDefinition* particleType = theTrack->GetDefinition();
    if( particleType == G4OpticalPhoton::OpticalPhotonDefinition() )
    {
        //Was the photon absorbed by the absorption process
        if( thePostPoint->GetProcessDefinedStep()->GetProcessName() == "OpAbsorption" ){
            fEventAction->IncAbsorption();
            trackInformation->AddTrackStatusFlag(absorbed);
        }

        switch (theStatus)
        {
            //Case absorbed at the bondary
        case Absorption:
            trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
            fEventAction->IncBoundaryAbsorption();
            break;
            // Detected by a detector
        case Detection:
            {
                if ( thePostPVname == "PhotonDet" ) {
                    G4SDManager* SDman = G4SDManager::GetSDMpointer();
                    G4String SDname="d2tb/PhotonDet";
                    PhotonDetSD* mppcSD = (PhotonDetSD*)SDman->FindSensitiveDetector(SDname);
                    if (mppcSD) mppcSD->ProcessHits_constStep(theStep, NULL);
                    trackInformation->AddTrackStatusFlag(hitSiPM);
                }
                // Stop Tracking when it hits the detector's surface
                ResetCounters();
                break;
            }
            //Same Material case
        case SameMaterial:
            {
                if( thePrePVname == "Crystal" && thePostPVname == "WorldBox") {
                    //Kill photons entering worldbox from the crystal
                    theTrack->SetTrackStatus(fStopAndKill);
                    trackInformation->AddTrackStatusFlag(murderee);
                    //Sets the exit point
                    trackInformation->SetExitPosition( thePostPoint->GetPosition() );
                }
                break;
            }
            //Reflections
        case TotalInternalReflection:
        case FresnelReflection:
        case LambertianReflection:
        case LobeReflection:
        case SpikeReflection:
        case BackScattering:
            fCounterBounce++;
            break;
        default: break;
        }

        //Check for bounce limit
        if (fBounceLimit > 0 && fCounterBounce >= fBounceLimit)
        {
            theTrack->SetTrackStatus(fStopAndKill);
            trackInformation->AddTrackStatusFlag(murderee);
            ResetCounters();
            G4cout << "SteppingAction::UserSteppingAction() : Bounce Limit Exceeded" << G4endl;
            return;
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
