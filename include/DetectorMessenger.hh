#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "DetectorConstruction.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

class DetectorMessenger: public G4UImessenger
{
public:
	DetectorMessenger(DetectorConstruction *Det);
	~DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
private:
    DetectorConstruction* Detector;
    
    G4UIdirectory*             MicromegasDir;
    G4UIdirectory*             detDir;
    G4UIdirectory*             detDirNew;

    G4UIcmdWithADoubleAndUnit  		*MagFieldCmd;
    G4UIcmdWithADoubleAndUnit  		*CupThickCmd;
    G4UIcmdWithADoubleAndUnit  		*ApertureThickCmd;
    G4UIcmdWithADoubleAndUnit  		*ApertureRadiusCmd;

	G4UIcmdWithADoubleAndUnit       *maxStepCmd;
	G4UIcmdWithADoubleAndUnit       *maxLengthCmd;
	G4UIcmdWithADoubleAndUnit       *maxTimeCmd;
	G4UIcmdWithADoubleAndUnit       *minEkinCmd;
	G4UIcmdWithADoubleAndUnit       *mionRangCmd;

	G4UIcmdWithADoubleAndUnit       *Det1InRadCmd;
	G4UIcmdWithADoubleAndUnit       *Det1OutRadCmd;
	G4UIcmdWithADoubleAndUnit       *Det1XCmd;

	G4UIcmdWithADoubleAndUnit       *RotationDegCmd;
	G4UIcmdWithADoubleAndUnit       *RotationAddDistCmd;

	G4UIcmdWithADoubleAndUnit       *EntranceCollGapXCmd;
	G4UIcmdWithADoubleAndUnit       *EntranceCollGapYCmd;
	G4UIcmdWithADoubleAndUnit       *Exit1CollGapXCmd;
	G4UIcmdWithADoubleAndUnit       *Exit1CollGapYCmd;

	G4UIcmdWithADoubleAndUnit* CreateCommand(const char *name, const char *comment);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

