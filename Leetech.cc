#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
	// Choose the Random engine
	//
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

#ifdef G4UI_USE
	// Detect interactive mode (if no arguments) and define UI session
	//
	G4UIExecutive* ui = 0;
	if ( argc == 1 ) {
		ui = new G4UIExecutive(argc, argv);
	}
#endif

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
	G4MTRunManager* runManager = new G4MTRunManager;
#else
	G4RunManager* runManager = new G4RunManager;
#endif

	// Mandatory user initialization classes
	runManager->SetUserInitialization(new DetectorConstruction);
	runManager->SetUserInitialization(new PhysicsList);

	// User action initialization
	runManager->SetUserInitialization(new ActionInitialization());

#ifdef G4VIS_USE
	// Visualization manager construction
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();
#endif

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if ( argc != 1 ) {
		// execute an argument macro file if exist
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	else {
#ifdef G4UI_USE
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
    UImanager->ApplyCommand("/control/execute init.mac");
#endif
    if (ui->IsGUI()) {
         UImanager->ApplyCommand("/control/execute gui.mac");
    }
    // start interactive session
    ui->SessionStart();
    delete ui;
#endif
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted
	// in the main() program !
#ifdef G4VIS_USE
	delete visManager;
#endif
	delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
