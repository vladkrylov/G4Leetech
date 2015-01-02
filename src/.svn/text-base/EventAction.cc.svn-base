#include "EventAction.hh"

#include "RunAction.hh"
#include "EventActionMessenger.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

//my
#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "G4VisManager.hh"
#include "G4Trajectory.hh"
#include "G4UImanager.hh"
#include "G4TrackingManager.hh"


#include "Randomize.hh"
#include <iomanip>

EventAction::EventAction()
{
  runAct = (RunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
  eventMessenger = new EventActionMessenger(this);
  printModulo = 1000;
}

EventAction::~EventAction()
{
  delete eventMessenger;
}


void EventAction::BeginOfEventAction(const G4Event* evt){  
    /// IF YOU WANT TO CHOOSE SOME THINGS SPACIAL
    /// TO SELECT WHAW EXACLY YOU WANT GO TP TrackingAction::PostUserTrackingAction
    //Flag_to_kill=true;
    /// IF YOU WANT SAVE ALL
    Flag_to_kill=false;
    
    _evtNb = evt->GetEventID();
  if (_evtNb%printModulo == 0) 
  { 
    G4cout << "\n---> Begin of event: " << _evtNb << G4endl;
    //CLHEP::HepRandom::showEngineStatus();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
	_evtNb = evt->GetEventID();
    // extract the trajectories and draw them

	SteppingAction *step_action = (SteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction();
	//TrackingAction *track_action = (TrackingAction*)G4RunManager::GetRunManager()->GetUserTrackingAction();        
        //G4UImanager* UI = G4UImanager::GetUIpointer();          

        //G4EventManager::GetEventManager()->KeepTheCurrentEvent();

    //if (G4VVisManager::GetConcreteInstance())
    if (Flag_to_kill)
    {
    	//UI->ApplyCommand("/tracking/storeTrajectory 1");
        
        G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
        /*
        if(0)
        {
        //if(step_action->GetRunAction()->HitInfo.IsInDetector==1)
                //if(step_action->GetRunAction()->HitInfo.TypeID==22)
                   // if(step_action->GetRunAction()->HitInfo.P>1.1 || step_action->GetRunAction()->HitInfo.P<0.9)
        
           // G4cout<<"Type is "<<step_action->GetRunAction()->HitInfo.TypeID<<G4endl;
            
            G4int n_trajectories = 0; 
            if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
            //G4cout<<"n_trajectories "<<n_trajectories<<G4endl;
            for (G4int i=0; i < n_trajectories; i++)
            {
                //G4cout<<"num tr"<<i<<G4endl;
              //  G4Trajectory* trj=(G4Trajectory*)((*(evt->GetTrajectoryContainer()))[i]);
              //  trj->DrawTrajectory(50);

            }
        }
        */
        //if(step_action->GetRunAction()->HitInfo.IsInDetector==0)
        //if(aTrack->GetTouchableHandle()->GetVolume()->GetName()=="SenDet1")G4cout<<"HIT!!!\n";
            //if(step_action->GetRunAction()->HitInfo.TypeID==22)
            //G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
            //trajectoryContainer->clearAndDestroy();
            //fpEventManager->AbortCurrentEvent();
             //track_action->Kill();
             //track_action->Live();
             //UI->ApplyCommand("/tracking/storeTrajectory 0");

    //if(Flag_to_kill)trajectoryContainer->clearAndDestroy();
        if(Flag_to_kill)
        {
            //fpEventManager->AbortCurrentEvent();
            trajectoryContainer->clearAndDestroy();
            //track_action->GetTrackingManager()->EventAborted();
        }
   }    
}

void EventAction::KeepCurrent()
{
    //fpEventManager->GetConstCurrentEvent()->GetTrajectoryContainer()->clearAndDestroy();
    Flag_to_kill=false;
}