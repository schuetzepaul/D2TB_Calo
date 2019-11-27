#include "PersistencyRootManager.hh"

#include <globals.hh>

#include <G4Event.hh>
#include <G4Run.hh>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>


PersistencyRootManager::PersistencyRootManager()
: PersistencyManager(),
fOutput(NULL),
fEventTree(NULL)
{}

PersistencyRootManager::~PersistencyRootManager()
{
    if (fOutput) delete fOutput;
    fOutput = nullptr;
}

bool PersistencyRootManager::IsOpen()
{
    if (fOutput && fOutput->IsOpen()) {
        fOutput->cd();
        return true;
    }
    return false;
}

bool PersistencyRootManager::Open(G4String filename)
{
    if (fOutput) {
        G4cout <<  "PersistencyRootManager::Open -- Delete current file pointer" << G4endl;
    }

    SetFilename(filename);

    G4cout << "PersistencyRootManager::Open " << GetFilename() << G4endl;
    
    fOutput = TFile::Open(GetFilename(), "RECREATE", "Root Output");
    fOutput->cd();

    fEventTree = new TTree("SimEvents", "Simulated Events");

    static TG4Event *pEvent = &fEventSummary;
    fEventTree->Branch("Event","TG4Event",&pEvent);

    fEventsNotSaved = 0;

    return true;
}

bool PersistencyRootManager::Close()
{
    if (!fOutput) {
        G4cout << "PersistencyRootManager::Close -- No Output File" << G4endl;
        return false;
    }

    fOutput->cd();

    fOutput->Write();
    fOutput->Close();

    fEventTree = nullptr;

    return true;
}

bool PersistencyRootManager::Store(const G4Event* anEvent)
{
    if (!fOutput) {
        G4cout << "PersistencyRootManager::Store -- No Output File" << G4endl;
        return false;
    }

    UpdateSummaries(anEvent);

    fOutput->cd();

    fEventTree->Fill();

    return true;
}

bool PersistencyRootManager::Store(const G4Run*)
{
    /* nop */
    return false;
}

bool PersistencyRootManager::Store(const G4VPhysicalVolume*)
{
    if (!fOutput) {
        G4cout << "PersistencyRootManager::Store -- No Output File" << G4endl;
        return false;
    }

    /* nop */
    return true;
}
