/// \file SteppingAction.hh
/// \brief Definition of the SteppingAction class

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

#include "globals.hh"

class DetectorConstruction;
class EventAction;
class G4Event;
class G4Track;
class G4Step;
class G4EmSaturation;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in the active material and
/// updated in RunData object.

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(const DetectorConstruction* detectorConstruction, EventAction* eventAction);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step);

private:
    G4double BirksAttenuationG4(const G4Step* step) const;

    const DetectorConstruction* fDetConstruction;
    EventAction* fEvtAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
