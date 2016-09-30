#ifndef RunAction_h
#define RunAction_h 1

#include <vector>

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class RunActionMessenger;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void SetOutRootFile(G4String newValue);

private:
    G4String rootFileName;
    RunActionMessenger* runMessenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

