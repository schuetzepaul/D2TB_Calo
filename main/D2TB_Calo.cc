#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
    void PrintUsage() {
        G4cerr << " Usage: " << G4endl;
        G4cerr << " D2TB_Calo [-m macro] [-u UIsession] [-h help]" << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    // Evaluate arguments
    if ( argc > 7 ) {
        PrintUsage();
        return 1;
    }

    G4String macro;
    G4String session;

    for ( G4int i=1; i<argc; i=i+2 ) {
        if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
        else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
        else if ( G4String(argv[i]) == "-h" ) {
            PrintUsage();
            return 1;
        }
        else {
            PrintUsage();
            return 1;
        }
    }

    // Detect interactive mode (if no macro provided) and define UI session
    G4UIExecutive* ui = 0;

    if ( ! macro.size() ) {
        ui = new G4UIExecutive(argc, argv, session);
    }

    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Construct the default run manager
    G4RunManager * runManager = new G4RunManager;

    // Set mandatory initialization classes
    // Construction of the detector
    auto detConstruction = new DetectorConstruction();
    runManager->SetUserInitialization(detConstruction);

    // Choice of the physics List
    runManager->SetUserInitialization(new PhysicsList());

    // ActionInitialisation
    auto actionInitialization = new ActionInitialization(detConstruction);
    runManager->SetUserInitialization(actionInitialization);

    // Initialize visualization
    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    if ( macro.size() ) {
        // batch mode
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);
    }
    else  {
        // interactive mode : define UI session
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI()) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    delete visManager;
    delete runManager;
}
