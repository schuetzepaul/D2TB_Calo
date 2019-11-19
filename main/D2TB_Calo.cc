#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
    void PrintUsage() {
        G4cerr << " Usage: " << G4endl;
        G4cerr << " D2TB_Calo [-m macro] [-h help]" << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    // Evaluate arguments
    if ( argc > 5 ) {
        PrintUsage();
        return 1;
    }

    G4String macro;
    for ( G4int i = 1; i < argc; i = i+2 ) {
        if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
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
    G4UIExecutive* ui = nullptr;

    if (macro.size() == 0) {
        ui = new G4UIExecutive(argc, argv);
    }

    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Construct the default run manager
    #ifdef G4MULTITHREADED
    G4MTRunManager * runManager = new G4MTRunManager;
    G4int nThreads = std::min(G4Threading::G4GetNumberOfCores(), 4);
    runManager->SetNumberOfThreads(nThreads);
    G4cout << "===== D2TB_Calo is started with "
    <<  runManager->GetNumberOfThreads() << " threads =====" << G4endl;
    #else
    G4RunManager * runManager = new G4RunManager;
    #endif

    // Set mandatory initialization classes
    // Construction of the detector
    auto detConstruction = new DetectorConstruction();
    runManager->SetUserInitialization(detConstruction);

    // Choice of the physics List
    auto phys = new PhysicsList();
    runManager->SetUserInitialization(phys);

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

    return 0;
}
