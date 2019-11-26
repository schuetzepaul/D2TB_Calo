#ifndef PersistencyManager_hh
#define PersistencyManager_hh 1

#include "TG4Event.hh"

#include <G4VPersistencyManager.hh>
#include <G4StepStatus.hh>

#include <vector>
#include <map>

class G4Event;
class G4Run;
class G4VPhysicalVolume;
class G4VHitsCollection;

class PersistencyMessenger;

class PersistencyManager : public G4VPersistencyManager {
public:

    PersistencyManager();
    virtual ~PersistencyManager();

    virtual G4bool Store(const G4Event* anEvent);
    virtual G4bool Store(const G4Run* aRun);
    virtual G4bool Store(const G4VPhysicalVolume* aWorld);

    virtual G4bool Retrieve(G4Event *&e) {e=NULL; return false;}
    virtual G4bool Retrieve(G4Run* &r) {r=NULL; return false;}
    virtual G4bool Retrieve(G4VPhysicalVolume* &w) {w=NULL; return false;}

    const TG4Event& GetEventSummary();

    /// A public accessor to the summarized hit detectors.
    const TG4HitDetectors& GetHitDetectors() const;

    /// Open the output (ie database) file.  This is used by the persistency
    /// messenger to open files using the G4 macro language.  It can be an
    /// empty method.
    virtual G4bool Open(G4String filename);

    /// Make sure the output file is closed.  This is used to make sure that
    /// any information being summarized has been saved.
    virtual G4bool Close(void);

    /// Return the output file name.
    virtual G4String GetFilename(void) const {return fFilename;}

protected:
    /// Set the output filename.  This can be used by the derived classes to
    /// inform the base class of the output file name.
    void SetFilename(G4String file) {
        fFilename = file;
    }

    /// Update the event summary fields.
    void UpdateSummaries(const G4Event* event);

    /// A summary of the primary vertices in the event.
    TG4Event fEventSummary;

private:

    /// sensitive detector.
    void SummarizeHitDetectors(TG4HitDetectors& dets,
    const G4Event* event);

    /// Fill a container of hit segments.
    void SummarizeHits(TG4PhotonDetHitContainer& pHits,
    G4VHitsCollection* hits);

    /// The filename of the output file.
    G4String fFilename;

    // A pointer to the messenger.
    PersistencyMessenger* fPersistencyMessenger;
};
#endif
