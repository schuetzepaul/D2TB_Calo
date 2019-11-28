/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy
/// deposit and track lengths of charged particles in Absorber and Scint layers
/// stored in the hits collections.

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event*);
    virtual void    EndOfEventAction(const G4Event*);

    void IncHitCount(G4int i = 1){ fHitCount+=i; }
    void IncPhotonCount_Scint() { fPhotonCount_Scint++; }
    void IncAbsorption() { fAbsorptionCount++; }
    void IncBoundaryAbsorption() { fBoundaryAbsorptionCount++; }

    G4int GetHitCount() const { return fHitCount; }
    G4int GetPhotonCount_Scint() const { return fPhotonCount_Scint; }
    G4int GetAbsorptionCount() const { return fAbsorptionCount; }
    G4int GetBoundaryAbsorptionCount() const { return fBoundaryAbsorptionCount; }

private:
    //members
    G4int fVerboseLevel;
    G4int fPhotonDetCollID;
    G4int fHitCount;
    G4int fPhotonCount_Scint;
    G4int fAbsorptionCount;
    G4int fBoundaryAbsorptionCount;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
