#include "PersistencyManager.hh"
#include "PersistencyMessenger.hh"
#include "PhotonDetHit.hh"
#include "RunAction.hh"
#include "D2TBRun.hh"

#include <G4ios.hh>
#include <G4RunManager.hh>
#include <G4Event.hh>
#include <G4Run.hh>
#include <G4PrimaryParticle.hh>
#include <G4StepStatus.hh>
#include <G4UnitsTable.hh>
#include <G4ParticleTable.hh>
#include <G4SDManager.hh>
#include <G4HCtable.hh>

#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>

#include <memory>

PersistencyManager::PersistencyManager()
: G4VPersistencyManager(),
fFilename("/dev/null")
{
    fPersistencyMessenger = new PersistencyMessenger(this);
}

PersistencyManager::~PersistencyManager()
{
    delete fPersistencyMessenger;
}

G4bool PersistencyManager::Open(G4String filename)
{
    SetFilename(filename);
    return false;
}

/// Make sure the output file is closed.
G4bool PersistencyManager::Close(void)
{
    return false;
}

G4bool PersistencyManager::Store(const G4Event* anEvent)
{
    UpdateSummaries(anEvent);
    return false;
}

G4bool PersistencyManager::Store(const G4Run* aRun)
{
    if (!aRun) return false;
    return false;
}

G4bool PersistencyManager::Store(const G4VPhysicalVolume* aWorld) {
    if (!aWorld) return false;
    return false;
}

void PersistencyManager::UpdateSummaries(const G4Event* event) {

    D2TBRun* runInfo = static_cast<D2TBRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

    fEventSummary.RunId = runInfo->GetRunID();
    fEventSummary.NScint = runInfo->GetPhotonCount_Scint();
    fEventSummary.EventId = event->GetEventID();
    G4cout << "PersistencyManager::UpdateSummaries() : Event Summary for run " << fEventSummary.RunId << " event " << fEventSummary.EventId << G4endl;

    SummarizeHitDetectors(fEventSummary.Detectors, event);
}

void PersistencyManager::SummarizeHitDetectors( TG4HitDetectors& dest, const G4Event* event)
{
    dest.clear();
    G4HCofThisEvent* HCofEvent = event->GetHCofThisEvent();
    if (!HCofEvent) return;

    G4SDManager *sdM = G4SDManager::GetSDMpointer();
    G4HCtable *hcT = sdM->GetHCtable();

    // Copy each of the hit categories into the output event.
    for (int i = 0; i < hcT->entries(); ++i)
    {
        G4String SDname = hcT->GetSDname(i);
        G4String HCname = hcT->GetHCname(i);

        int HCId = sdM->GetCollectionID(SDname+"/"+HCname);
        G4VHitsCollection* g4Hits = HCofEvent->GetHC(HCId);

        if (g4Hits->GetSize() < 1) continue;

        PhotonDetHit* hitSeg = dynamic_cast<PhotonDetHit*>(g4Hits->GetHit(0));
        if (!hitSeg) continue;

        SummarizeHits(dest[SDname], g4Hits);
    }
}

void PersistencyManager::SummarizeHits(TG4PhotonDetHitContainer& dest, G4VHitsCollection* g4Hits)
{
    dest.clear();
    PhotonDetHit* g4Hit = dynamic_cast<PhotonDetHit*>(g4Hits->GetHit(0));
    if (!g4Hit) return;

    G4cout << "PersistencyManager::SummarizeHits() : Number of photons hitting the SiPMs " << g4Hits->GetSize() << G4endl;

    for (std::size_t h = 0; h < g4Hits->GetSize(); ++h)
    {
        g4Hit = dynamic_cast<PhotonDetHit*>(g4Hits->GetHit(h));
        TG4PhotonDetHit hit;

        hit.fArrivalTime = g4Hit->GetArrivalTime();
        hit.fCrystalNo = g4Hit->GetCrystalNo();
        hit.fSiPMNo = g4Hit->GetSiPMNo();

        hit.fPosExit.SetXYZ(g4Hit->GetExitPos().x(), g4Hit->GetExitPos().y(), g4Hit->GetExitPos().z());
        hit.fPosArrive.SetXYZ(g4Hit->GetArrivalPos().x(), g4Hit->GetArrivalPos().y(), g4Hit->GetArrivalPos().z());
        hit.fPosArriveLocal.SetXYZ(g4Hit->GetArrivalPosLocal().x(), g4Hit->GetArrivalPosLocal().y(), g4Hit->GetArrivalPosLocal().z());

        dest.push_back(hit);
    }
}
