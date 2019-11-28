#include "Trajectory.hh"

#include "G4Trajectory.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ThreeVector.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4Polymarker.hh"

G4ThreadLocal G4Allocator<Trajectory>* TrajectoryAllocator = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Trajectory::Trajectory()
: G4Trajectory(),
fDrawit(false)
{
    fParticleDefinition = nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Trajectory::Trajectory(const G4Track* aTrack)
: G4Trajectory(aTrack),
fDrawit(false)
{
    fParticleDefinition = aTrack->GetDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Trajectory::Trajectory(Trajectory & right)
: G4Trajectory(right),
fDrawit(right.fDrawit)
{
    fParticleDefinition=right.fParticleDefinition;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Trajectory::~Trajectory()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Trajectory::DrawTrajectory() const
{
    const G4int i_mode = 50;

    if(!fDrawit) return;

    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (!pVVisManager) return;

    const G4double markerSize = std::abs(i_mode)/1000;
    G4bool lineRequired (i_mode >= 0);
    G4bool markersRequired (markerSize > 0.);

    G4Polyline trajectoryLine;
    G4Polymarker stepPoints;
    G4Polymarker auxiliaryPoints;

    for (G4int i = 0; i < GetPointEntries() ; i++)
    {
        G4VTrajectoryPoint* aTrajectoryPoint = GetPoint(i);
        const std::vector<G4ThreeVector>* auxiliaries
        = aTrajectoryPoint->GetAuxiliaryPoints();
        if (auxiliaries)
        {
            for (size_t iAux = 0; iAux < auxiliaries->size(); ++iAux)
            {
                const G4ThreeVector pos((*auxiliaries)[iAux]);
                if (lineRequired) {
                    trajectoryLine.push_back(pos);
                }
                if (markersRequired) {
                    auxiliaryPoints.push_back(pos);
                }
            }
        }

        const G4ThreeVector pos(aTrajectoryPoint->GetPosition());
        if (lineRequired) {
            trajectoryLine.push_back(pos);
        }
        if (markersRequired) {
            stepPoints.push_back(pos);
        }
    }

    if (lineRequired)
    {
        G4Colour colour;
        if(fParticleDefinition==G4OpticalPhoton::OpticalPhotonDefinition()) {
            //Scintillation and Cerenkov photons are green
            colour = G4Colour(0.,1.,0.);
        }
        else {
            //All other particles are blue
            colour = G4Colour(0.,0.,1.);
        }

        G4VisAttributes trajectoryLineAttribs(colour);
        trajectoryLine.SetVisAttributes(&trajectoryLineAttribs);
        pVVisManager->Draw(trajectoryLine);
    }
    if (markersRequired)
    {
        auxiliaryPoints.SetMarkerType(G4Polymarker::squares);
        auxiliaryPoints.SetScreenSize(markerSize);
        auxiliaryPoints.SetFillStyle(G4VMarker::filled);
        G4VisAttributes auxiliaryPointsAttribs(G4Colour(0.,1.,1.));  // Magenta
        auxiliaryPoints.SetVisAttributes(&auxiliaryPointsAttribs);
        pVVisManager->Draw(auxiliaryPoints);

        stepPoints.SetMarkerType(G4Polymarker::circles);
        stepPoints.SetScreenSize(markerSize);
        stepPoints.SetFillStyle(G4VMarker::filled);
        G4VisAttributes stepPointsAttribs(G4Colour(1.,1.,0.));  // Yellow
        stepPoints.SetVisAttributes(&stepPointsAttribs);
        pVVisManager->Draw(stepPoints);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
