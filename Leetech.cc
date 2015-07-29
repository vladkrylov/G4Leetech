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
#include "SteppingVerbose.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc,char** argv)
{
	// Choose the Random engine
	//
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

	// User Verbose output class
	//
	G4VSteppingVerbose::SetInstance(new SteppingVerbose);

	// Construct the default run manager
	//
#ifdef G4MULTITHREADED
	G4MTRunManager* runManager = new G4MTRunManager;
//	runManager->SetNumberOfThreads(1);
#else
	G4RunManager* runManager = new G4RunManager;
#endif

	// Set mandatory initialization classes
	//
	runManager->SetUserInitialization(new DetectorConstruction);
	runManager->SetUserInitialization(new PhysicsList);
	runManager->SetUserInitialization(new ActionInitialization);

	// Initialize G4 kernel
	//
//	runManager->Initialize();
  
	// Initialize visualization
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if (argc!=1) {   // batch mode
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    } else {  // interactive mode : define UI session
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
     // UImanager->ApplyCommand("/control/execute vis.mac");
      if (ui->IsGUI())
	  //UImanager->ApplyCommand("/control/execute gui.mac");
      ui->SessionStart();
      delete ui;
    }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
	delete visManager;
	delete runManager;

	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
