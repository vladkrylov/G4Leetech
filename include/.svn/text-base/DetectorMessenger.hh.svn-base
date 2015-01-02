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
    DetectorMessenger(DetectorConstruction*,DetectorConstructionP2 *, DetectorConstructionP3 * );
   ~DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);

		  bool number_of_geom;	//true - for geom 1,2,3
											//false for 4...
    
  private:
    DetectorConstruction* Detector;
    DetectorConstructionP2* DetectorP2;
    DetectorConstructionP3* DetectorP3;
    
    G4UIdirectory*             MicromegasDir;
    G4UIdirectory*             detDir;

    G4UIcmdWithADoubleAndUnit  *MagFieldCmd;
    G4UIcmdWithADoubleAndUnit  *CupThickCmd;
    G4UIcmdWithADoubleAndUnit  *ApertureThickCmd;
    G4UIcmdWithADoubleAndUnit  *ApertureRadiusCmd;
    G4UIcmdWithAnInteger       *GeomIDCmd;
    G4UIcmdWithoutParameter    *UpdateCmd;

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
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

