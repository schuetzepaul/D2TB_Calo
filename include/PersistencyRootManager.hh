#ifndef PersistencyRootManager_hh
#define PersistencyRootManager_hh 1

#include <string>
#include <vector>
#include <map>

class TFile;
class TTree;
class TGeoManager;

#include "PersistencyManager.hh"

class PersistencyRootManager : public PersistencyManager
{
public:
    PersistencyRootManager();
    virtual ~PersistencyRootManager();

    bool IsOpen();

    TFile* GetTFile() const {return fOutput;}

    virtual G4bool Store(const G4Event* anEvent);
    virtual G4bool Store(const G4Run* aRun);
    virtual G4bool Store(const G4VPhysicalVolume* aWorld);

    virtual G4bool Retrieve(G4Event *&e) {e=NULL; return false;}
    virtual G4bool Retrieve(G4Run* &r) {r=NULL; return false;}
    virtual G4bool Retrieve(G4VPhysicalVolume* &w) {w=NULL; return false;}

    virtual G4bool Open(G4String dbname);
    virtual G4bool Close(void);

private:

private:

    TFile *fOutput;
    TTree *fEventTree;
    int fEventsNotSaved;

};

#endif
