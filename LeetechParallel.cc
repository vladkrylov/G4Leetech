#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "SteppingVerbose.hh"

#include "TrackingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4MPImanager.hh"
#include "G4MPIsession.hh"

#include "Randomize.hh"
#include "time.h"

int main(int argc,char** argv)
{
  // At first, G4MPImanager/G4MPIsession should be created.
  G4MPImanager* g4MPI= new G4MPImanager(argc,argv);

  // MPI session (G4MPIsession) instead of G4UIterminal
  G4MPIsession* session= g4MPI-> GetMPIsession();

  
  // LAM/MPI users can use G4tcsh.
  //G4String prompt= "[40;01;33m";
  //prompt+= "G4MPI";
  //prompt+= "[40;31m(%s)[40;36m[%/][00;30m:";
  //session-> SetPrompt(prompt);
  
  // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // User Verbose output class
  //
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);
     
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  runManager->SetUserInitialization(new DetectorConstruction);
  //
  runManager->SetUserInitialization(new PhysicsList);
    
  // Set user action classes
  //
  runManager->SetUserAction(new PrimaryGeneratorAction);
  //
  runManager->SetUserAction(new RunAction);
  //
  runManager->SetUserAction(new EventAction);
  //
  runManager->SetUserAction(new SteppingAction);
  
  runManager->SetUserAction(new TrackingAction);
  
  // Initialize G4 kernel
  //
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
//      UImanager->ApplyCommand(command+fileName);
    }
  else
    {  // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
     // UImanager->ApplyCommand("/control/execute vis.mac");
#endif
      if (ui->IsGUI())
	//UImanager->ApplyCommand("/control/execute gui.mac");
      ui->SessionStart();
      delete ui;
#endif
    }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  
   	 session-> SessionStart();

  // Finally, terminate the program
  delete g4MPI;
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
