#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class EventActionMessenger;
class EventAction;

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();

  void  BeginOfEventAction(const G4Event*);
  void    EndOfEventAction(const G4Event*);
  
  void    KeepCurrent();
                         
  void SetPrintModulo(G4int    val)  {printModulo = val;};

  G4int _evtNb;
    
private:
   RunAction*  runAct;
                        
   G4int     printModulo;
   bool      Flag_to_kill;
                             
   EventActionMessenger*  eventMessenger;
};

#endif

    
