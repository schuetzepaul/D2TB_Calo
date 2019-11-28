#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"
#include "PersistencyManager.hh"
#include "PersistencyRootManager.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UIterminal.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrintUsage() {
    std::cout << "Usage: d2tb_calo [options]" << std::endl;
    std::cout << "    -m      -- Use the macro specified after" << std::endl;
    std::cout << "    -o      -- Set the output file" << std::endl;
    std::cout << "    -U      -- Start an interactive run" << std::endl;
    std::cout << "    -v      -- Validate the geometry" << std::endl;
    std::cout << "    -e <n>  -- Number of events to run" << std::endl;
    std::cout << "    -h      -- This help message." << std::endl;

    exit(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
    // Evaluate arguments
    if ( argc < 2 || argc > 11 ) {
        PrintUsage();
    }

    G4String macro;
    G4String outputFilename;
    G4String nEvts;
    bool useUI = false;
    bool validateGeo = false;

    for ( G4int i = 1; i < argc; i++ ) {
        if      ( G4String(argv[i]) == "-m" ) { macro = argv[i+1]; i++; }
        else if ( G4String(argv[i]) == "-o" ) { outputFilename = argv[i+1]; i++; }
        else if ( G4String(argv[i]) == "-U" ) useUI = true;
        else if ( G4String(argv[i]) == "-v" ) validateGeo = true;
        else if ( G4String(argv[i]) == "-e" ) { nEvts = argv[i+1]; i++; }
        else if ( G4String(argv[i]) == "-h" ) {
            PrintUsage();
        }
        else {
            PrintUsage();
        }
    }

    G4UIsession *session = nullptr;
    G4UIExecutive* ui = nullptr;

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
    auto detConstruction = new DetectorConstruction(validateGeo);
    runManager->SetUserInitialization(detConstruction);

    // Choice of the physics List
    auto phys = new PhysicsList();
    runManager->SetUserInitialization(phys);

    // ActionInitialisation
    auto actionInitialization = new ActionInitialization(detConstruction);
    runManager->SetUserInitialization(actionInitialization);

    PersistencyManager* persistencyManager = nullptr;

    persistencyManager = new PersistencyRootManager();

    if(!persistencyManager){
        persistencyManager = new PersistencyManager();
    }

    // Get the pointer to the User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();

    // Open the file if one was declared on the command line.
    if (persistencyManager && ! outputFilename.empty()) {
        UImanager->ApplyCommand("/d2tb/root/open "+outputFilename);
    }

    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    if (useUI) {
        ui = new G4UIExecutive(argc, argv);

        // interactive mode : define UI session
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI()) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }
    else{
        if ( macro.size() ) {
            UImanager->ApplyCommand("/control/execute " + macro);
            if (nEvts.size()) {
                UImanager->ApplyCommand("/run/beamOn " + nEvts);
            } else {
                //keep G4 idle
                session = new G4UIterminal();
                session->SessionStart();
            }
        }
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    if (persistencyManager) {
        persistencyManager->Close();
        delete persistencyManager;
    }

    if(session) delete session;

    delete visManager;
    delete runManager;

    return 0;
}
