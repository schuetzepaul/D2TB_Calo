/// \file SteppingAction.hh
/// \brief Definition of the SteppingAction class

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

#include "globals.hh"

class G4OpBoundaryProcess;
class G4Step;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in the active material and
/// updated in RunData object.

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* theStep);

    inline void ResetCounters()
    {
      fCounterBounce = 0;
    }

private:

    // Artificially kill the photon after it has bounced more than this number
    G4int fBounceLimit;

    // total number of bounces that a photon been through
    G4int fCounterBounce;

     G4OpBoundaryProcess* fOpProcess;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
