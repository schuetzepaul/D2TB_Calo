#include "PhotonDetSD.hh"
#include "UserTrackInformation.hh"

#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonDetSD::PhotonDetSD(G4String name, G4int verbose)
: G4VSensitiveDetector(name),
fPhotonDetHitCollection(0),
fVerbose(verbose)
{
    collectionName.insert("PhotonDetHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonDetSD::~PhotonDetSD() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhotonDetSD::Initialize(G4HCofThisEvent* HCE)
{
    fPhotonDetHitCollection = new PhotonDetHitsCollection(SensitiveDetectorName, collectionName[0]);
    //Store collection with event and keep ID
    static G4int HCID = -1;
    if (HCID<0) HCID = GetCollectionID(0);
    HCE->AddHitsCollection( HCID, fPhotonDetHitCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PhotonDetSD::ProcessHits(G4Step* , G4TouchableHistory* )
{
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Generates a hit and uses the postStepPoint; PostStepPoint because the hit
//is generated manually when the photon hits the detector
G4bool PhotonDetSD::ProcessHits_constStep(const G4Step* aStep, G4TouchableHistory* )
{
    if (aStep == nullptr) return false;
    G4Track* theTrack = aStep->GetTrack();

    // Need to know if this is an optical photon
    if(theTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

    // Find out information regarding the hit
    G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
    UserTrackInformation* trackInformation = (UserTrackInformation*)theTrack->GetUserInformation();
    G4TouchableHistory* theTouchable = (G4TouchableHistory*)(thePostPoint->GetTouchable());

    //Physical volume of the hit
    auto physical = theTouchable->GetVolume();
    G4ThreeVector photonExit   = trackInformation->GetExitPosition();
    G4ThreeVector photonArrive = thePostPoint->GetPosition();
    G4double arrivalTime  = theTrack->GetGlobalTime();

    // Convert the global coordinate for arriving photons into
    // the local coordinate of the detector
    G4ThreeVector photonArriveLocal = theTouchable->GetHistory()->GetTopTransform().TransformPoint(photonArrive);

    //Get the copy number of the volume (starts at 0) (depth = 3 / 1 = SiPM / 2 = Hole / 3 = Crystal)
    G4int crystalNo = theTouchable->GetCopyNumber(3)+1;
    //Get the copy number of the volume (starts at 0) (depth = 3 / 1 = SiPM / 2 = Hole / 3 = Crystal)
    G4int SiPMNo = theTouchable->GetCopyNumber(2)+1;

    // Creating the hit and add it to the collection
    PhotonDetHit* hit = new PhotonDetHit(photonExit, photonArrive, photonArriveLocal, arrivalTime, physical->GetLogicalVolume(), crystalNo, SiPMNo);
    fPhotonDetHitCollection->insert(hit);

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhotonDetSD::EndOfEvent(G4HCofThisEvent*)
{
    if(fVerbose > 1)
    {
        G4int nDetected = fPhotonDetHitCollection->entries();
        G4cout << "<<< Number of photon detected " << nDetected << G4endl;
        for (G4int i = 0; i < nDetected; i++) (*fPhotonDetHitCollection)[i]->Print();
    }
}
