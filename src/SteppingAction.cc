/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Track.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

#include "G4ProcessManager.hh"
#include "G4OpBoundaryProcess.hh"

#include "PhotonDetSD.hh"

#include "UserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* detector)
: G4UserSteppingAction(),
fDetector(detector)
{
    fBounceLimit = 100000;
    fOpProcess = nullptr;
    ResetCounters();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

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

    // Record Photons that missed the photon detector but escaped from readout
    if ( !thePostPV && trackInformation->IsStatus(EscapedFromReadOut) ) {
        ResetCounters();
        return;
    }

    switch (theStatus)
    {
        // Exiting the crystal
    case FresnelRefraction:
    case SameMaterial:
        {
            G4cout << "Case SameMaterial" << G4endl;
            G4cout << "thePrePVname " << thePrePVname << G4endl;
            G4cout << "thePostPVname " << thePostPVname << G4endl;

            G4bool isCrystal;
            isCrystal = thePostPVname == "Crystal";

            if(isCrystal){
                if (trackInformation->IsStatus(OutsideOfCrystal))
                trackInformation->AddStatusFlag(InsideOfCrystal);
            }
            // Set the Exit flag when the photon refracted out of the crystal
            else if (trackInformation->IsStatus(InsideOfCrystal)) {
                G4bool isWorld;
                isWorld = thePostPVname == "WorldBox";
                if(isWorld) {
                    // EscapedFromReadOut if the z position is the same as crystal's end
                    if(theTrack->GetPosition().z() == fDetector->GetCrystalEnd())
                    {
                        trackInformation->AddStatusFlag(EscapedFromReadOut);
                        trackInformation->SetExitPosition(theTrack->GetPosition());
                    }
                    else{
                        //Track exit from side
                        trackInformation->AddStatusFlag(EscapedFromSide);
                        trackInformation->SetExitPosition(theTrack->GetPosition());
                        ResetCounters();
                    }

                    trackInformation->AddStatusFlag(OutsideOfCrystal);
                    trackInformation->SetExitPosition(theTrack->GetPosition());

                    //Kill the track that exits the crystal to the world volume
                    theTrack->SetTrackStatus(fStopAndKill);
                    trackInformation->AddStatusFlag(murderee);
                }
            }
            return;
        }
        //Internal Reflections
    case TotalInternalReflection:
        {
            // G4cout << "Case TotalInternalReflection" << G4endl;
            fCounterBounce++;
            // Kill the track if it's number of bounces exceeded the limit
            if (fBounceLimit > 0 && fCounterBounce >= fBounceLimit)
            {
                theTrack->SetTrackStatus(fStopAndKill);
                trackInformation->AddStatusFlag(murderee);
                ResetCounters();
                G4cout << "\n Bounce Limit Exceeded" << G4endl;
                return;
            }
            break;
        }
    case FresnelReflection:
        {
            // G4cout << "Case FresnelReflection" << G4endl;
            fCounterBounce++;

            if (theTrack->GetPosition().z() == fDetector->GetCrystalEnd())
            {
                if (!trackInformation->IsStatus(ReflectedAtReadOut) && trackInformation->IsStatus(InsideOfCrystal))
                {
                    trackInformation->AddStatusFlag(ReflectedAtReadOut);
                }
            }
            return;
        }
        // Detected by a detector
    case Detection:
        {
            G4cout << "Case Detection" << G4endl;
            G4cout << "thePrePVname " << thePrePVname << G4endl;
            G4cout << "thePostPVname " << thePostPVname << G4endl;
            
            if ( thePostPVname == "PhotonDet" ) {

                G4SDManager* SDman = G4SDManager::GetSDMpointer();
                G4String SDname="d2tb/PhotonDet";
                PhotonDetSD* mppcSD = (PhotonDetSD*)SDman->FindSensitiveDetector(SDname);

                if (mppcSD) mppcSD->ProcessHits_constStep(theStep, NULL);
                trackInformation->AddStatusFlag(hitSiPM);

                // Stop Tracking when it hits the detector's surface
                ResetCounters();
                return;
            }
            break;
        }

    default: break;
    }

    // Check for absorbed photons
    if (theTrack->GetTrackStatus() != fAlive && trackInformation->IsStatus(InsideOfCrystal)){
        ResetCounters();
        return;
    }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
