#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RunAction;
class G4UIcmdWithAString;

class RunActionMessenger: public G4UImessenger
{
  public:
    RunActionMessenger(RunAction*);
   ~RunActionMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    RunAction*   runAction;

    G4UIcmdWithAString*   RootFileCmd;
};

#endif
