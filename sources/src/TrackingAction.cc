/// \file electromagnetic/TestEm1/src/TrackingAction.cc
/// \brief Implementation of the TrackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TrackingAction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

#include "Trajectory.hh"
#include "UserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    //Use custom trajectory class
    fpTrackingManager->SetTrajectory(new Trajectory(aTrack));
    UserTrackInformation* trackInformation = new UserTrackInformation();

    G4String PVName = aTrack->GetVolume()->GetName();

    if (PVName == "Crystal")
    trackInformation->AddTrackStatusFlag(insideOfCrystal);

    fpTrackingManager->SetUserTrackInformation(trackInformation);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    Trajectory* trajectory = (Trajectory*)fpTrackingManager->GimmeTrajectory();
    UserTrackInformation* trackInformation = (UserTrackInformation*)aTrack->GetUserInformation();

    //Lets choose to draw only the photons that hit the sipm
    if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        if(trackInformation->GetTrackStatus()&hitSiPM) {
            trajectory->SetDrawTrajectory(true);
        }
    }
    else //draw all other trajectories
    trajectory->SetDrawTrajectory(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
