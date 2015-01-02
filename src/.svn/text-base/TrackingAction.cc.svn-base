#include <G4RunManager.hh>
#include <G4Trajectory.hh>

#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4RunManager.hh"
#include "SteppingAction.hh"

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{

}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  //  	SteppingAction *step_action = (SteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction();
    EventAction* event_action = (EventAction*)G4RunManager::GetRunManager()->GetUserEventAction();
    //G4Event *evt = (G4Event*)G4RunManager::GetRunManager()->GetCurrentEvent();
  
  if(aTrack->GetTouchableHandle()->GetVolume()->GetName()=="SenDet1")  
    if(aTrack->GetDefinition()->GetPDGEncoding()==22)
    {
        G4cout<<"InDet & gamma\n";
        event_action->KeepCurrent();
    }
          //aTrack->SetTrackStatus(fStopAndKill);
          //G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
          //fpTrackingManager->EventAborted();
          //trajectoryContainer->clearAndDestroy();
     //if(step_action->GetRunAction()->HitInfo.IsInDetector==1)
     //fpTrackingManager->SetStoreTrajectory(true);
     //fpTrackingManager->GimmeTrajectory()->DrawTrajectory(true);
      //aTrack->SetTrackStatus(fKillTrackAndSecondaries);
}

void TrackingAction::Kill(){fpTrackingManager->SetStoreTrajectory(false);}
void TrackingAction::Live(){fpTrackingManager->SetStoreTrajectory(true);}

//Дописать что б из енд оф ивент акшена вызывался килл и лив траектори
