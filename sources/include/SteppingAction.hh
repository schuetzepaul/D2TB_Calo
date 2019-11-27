/// \file SteppingAction.hh
/// \brief Definition of the SteppingAction class

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4OpBoundaryProcess.hh"

#include "globals.hh"

class DetectorConstruction;
class EventAction;
class G4Step;
class SteppingActionMessenger;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in the active material and
/// updated in RunData object.

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(DetectorConstruction*, EventAction*);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* theStep);

    void SetBounceLimit(G4int);

private:

    inline void ResetCounters()
    {
        fCounterBounce = 0;
    }

    DetectorConstruction*       fDetector;
    EventAction*                fEventAction;
    G4OpBoundaryProcess*        fOpProcess;
    SteppingActionMessenger*    fSteppingMessenger;

    G4int fBounceLimit;
    G4int fCounterBounce;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
