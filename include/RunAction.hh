#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4String.hh"

class G4Run;
class RunActionMessenger;

/// Run action class

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void SetRootFile(G4String val);

private:
    RunActionMessenger* runMessenger;
    G4String rootFileName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void RunAction::SetRootFile(G4String val) {
	rootFileName = val;
}

#endif


