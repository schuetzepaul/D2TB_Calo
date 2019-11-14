/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#include "G4EmSaturation.hh"
#include "G4Event.hh"
#include "G4NavigationHistory.hh"
#include "G4Step.hh"
#include "G4TouchableHandle.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4Version.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(const DetectorConstruction *detectorConstruction,
                               EventAction *eventAction)
    : G4UserSteppingAction(), fDetConstruction(detectorConstruction),
      fEvtAction(eventAction) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step *step) {
  // Collect energy and track length step by step

  // get volume of the current step
  // auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

  // if (volume == "") {
  //   // energy deposit
  //   auto edep = step->GetTotalEnergyDeposit();
  //
  //   // Skip no deposited energy
  //   if (edep <= 0)
  //     return;
  //
  //   // Birks using method provided by G4
  //   auto edepBirksG4 = BirksAttenuationG4(step);
  //
  //   // step length
  //   G4double stepLength = 0.;
  //   if (step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) {
  //     stepLength = step->GetStepLength();
  //   }
  //
  //   // G4Touchable
  //   G4TouchableHandle theTouchable =
  //       step->GetPreStepPoint()->GetTouchableHandle();
  //
  //   // pos
  //   auto G4Global = 0.5 * (step->GetPreStepPoint()->GetPosition() +
  //                          step->GetPostStepPoint()->GetPosition());
  //   // Transform from global coordinates to local coordinates
  //   auto G4Local = step->GetPreStepPoint()
  //                      ->GetTouchable()
  //                      ->GetHistory()
  //                      ->GetTopTransform()
  //                      .TransformPoint(G4Global);
  //
  //   // Get the layer number
  //   G4double layer = theTouchable->GetReplicaNumber(1);
  //
  //   // time
  //   float time = step->GetPreStepPoint()->GetGlobalTime();
  //
  //   // process name
  //   const G4VProcess *process =
  //       step->GetPostStepPoint()->GetProcessDefinedStep();
  //   auto processName = process->GetProcessName();
  //   auto processSubType = process->GetProcessSubType();
  //
  // }
}

G4double SteppingAction::BirksAttenuationG4(const G4Step *step) const {
#if G4VERSION_NUMBER >= 1001
  static G4EmSaturation fEmSaturation(0);
#else
  static G4EmSaturation fEmSaturation();
  fEmSaturation.SetVerbose(0);
#endif

#if G4VERSION_NUMBER >= 1030
  fEmSaturation.InitialiseG4Saturation();
#else
#endif

  double energyDeposition = step->GetTotalEnergyDeposit();
  double length = step->GetStepLength();
  double niel = step->GetNonIonizingEnergyDeposit();
  const G4Track *trk = step->GetTrack();
  const G4ParticleDefinition *particle = trk->GetDefinition();
  const G4MaterialCutsCouple *couple = trk->GetMaterialCutsCouple();
  double engyVis = fEmSaturation.VisibleEnergyDeposition(
      particle, couple, length, energyDeposition, niel);

  return engyVis;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
