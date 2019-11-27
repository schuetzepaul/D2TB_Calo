#ifndef D2TBRun_hh
#define D2TBRun_hh 1

#include "G4Run.hh"
#include "globals.hh"

class D2TBRun : public G4Run
{
public:
    D2TBRun();
    ~D2TBRun();

    void IncPhotonCount_Scint(G4int count) {
        fPhotonCount_Scint  += count;
    }

    void IncAbsorption(G4int count) {
        fAbsorptionCount  += count;
    }

    void IncBoundaryAbsorption(G4int count) {
        fBoundaryAbsorptionCount  += count;
    }

    virtual void Merge(const G4Run* run);

    void EndOfRun();

    G4int GetPhotonCount_Scint() { return fPhotonCount_Scint; }

private:
    G4int fPhotonCount_Scint;
    G4int fAbsorptionCount;
    G4int fBoundaryAbsorptionCount;
};

#endif // D2TBRun_hh
