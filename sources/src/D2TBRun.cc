#include "D2TBRun.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D2TBRun::D2TBRun() : G4Run()
{
    fHitCount                = 0;
    fPhotonCount_Scint       = 0;
    fAbsorptionCount         = 0;
    fBoundaryAbsorptionCount = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D2TBRun::~D2TBRun()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D2TBRun::Merge(const G4Run* run)
{
    const D2TBRun* localRun = static_cast<const D2TBRun*>(run);

    fHitCount                 += localRun->fHitCount;
    fPhotonCount_Scint        += localRun->fPhotonCount_Scint;
    fAbsorptionCount          += localRun->fAbsorptionCount;
    fBoundaryAbsorptionCount  += localRun->fBoundaryAbsorptionCount;

    G4Run::Merge(run);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D2TBRun::EndOfRun()
{
    G4cout << "\n ======================== Run Summary ======================\n";
    G4int prec = G4cout.precision();

    G4int n_evt = numberOfEvent;
    G4cout << "The run was " << n_evt << " events." << G4endl;

    G4cout.precision(4);
    G4double hits = G4double(fHitCount)/n_evt;
    G4cout << "Number of hits per event:\t " << hits << G4endl;

    G4double scint = G4double(fPhotonCount_Scint)/n_evt;
    G4cout << "Number of scintillation photons per event :\t " << scint << G4endl;

    G4double absorb = G4double(fAbsorptionCount)/n_evt;
    G4cout << "Number of absorbed photons per event :\t " << absorb << G4endl;

    G4double bdry = G4double(fBoundaryAbsorptionCount)/n_evt;
    G4cout << "Number of photons absorbed at boundary per event:\t " << bdry << G4endl;
    
    G4cout << G4endl;
    G4cout.precision(prec);
}
