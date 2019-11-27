#include "D2TBRun.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D2TBRun::D2TBRun() : G4Run()
{
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

    fPhotonCount_Scint        += localRun->fPhotonCount_Scint;
    fAbsorptionCount          += localRun->fAbsorptionCount;
    fBoundaryAbsorptionCount  += localRun->fBoundaryAbsorptionCount;

    G4Run::Merge(run);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D2TBRun::EndOfRun()
{

}
