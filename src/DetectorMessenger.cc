#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* detector)
: Detector(detector)
{ 
	MicromegasDir = new G4UIdirectory("/Micromegas/");
	MicromegasDir->SetGuidance("UI commands of Micromegas Geant4 app");

	detDir = new G4UIdirectory("/Micromegas/det/");
	detDir->SetGuidance("detector control");

	detDirNew = new G4UIdirectory("/Leetech/det/");
	detDirNew->SetGuidance("detector control");

	MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setField",this);
	MagFieldCmd->SetGuidance("Define magnetic field.");
	MagFieldCmd->SetGuidance("Magnetic field will be in Y direction.");
	MagFieldCmd->SetParameterName("By",false);
	MagFieldCmd->SetUnitCategory("Magnetic flux density");
	MagFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	CupThickCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setCupThick",this);
	CupThickCmd->SetGuidance("Set Thickness of the cup");
	CupThickCmd->SetParameterName("Size",false);
	CupThickCmd->SetRange("Size>=0.");
	CupThickCmd->SetUnitCategory("Length");
	CupThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	ApertureThickCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setApertureThick",this);
	ApertureThickCmd->SetGuidance("Set Thickness of the aperture");
	ApertureThickCmd->SetParameterName("Size",false);
	ApertureThickCmd->SetRange("Size>=0.");
	ApertureThickCmd->SetUnitCategory("Length");
	ApertureThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	ApertureRadiusCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setApertureRaduis",this);
	ApertureRadiusCmd->SetGuidance("Set Radius of the aperture");
	ApertureRadiusCmd->SetParameterName("Size",false);
	ApertureRadiusCmd->SetRange("Size>=0.");
	ApertureRadiusCmd->SetUnitCategory("Length");
	ApertureRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	maxStepCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setMaxStep",this);
	maxStepCmd->SetGuidance("Set maximum of the step of iteration");
	maxStepCmd->SetParameterName("Size",false);
	maxStepCmd->SetRange("Size>=0.");
	maxStepCmd->SetUnitCategory("Length");
	maxStepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	maxLengthCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setMaxLength",this);
	maxLengthCmd->SetGuidance("Set maximum flight length");
	maxLengthCmd->SetParameterName("Size",false);
	maxLengthCmd->SetRange("Size>=0.");
	maxLengthCmd->SetUnitCategory("Length");
	maxLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	maxTimeCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setMaxTime",this);
	maxTimeCmd->SetGuidance("Set maximum flight time");
	maxTimeCmd->SetParameterName("Time",false);
	maxTimeCmd->SetRange("Time>=0.");
	maxTimeCmd->SetUnitCategory("Time");
	maxTimeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	minEkinCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setMinEkin",this);
	minEkinCmd->SetGuidance("Set minimum Kinetik energy");
	minEkinCmd->SetParameterName("Energy",false);
	minEkinCmd->SetRange("Energy>=0.");
	minEkinCmd->SetUnitCategory("Energy");
	minEkinCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	mionRangCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setMionRang",this);
	mionRangCmd->SetGuidance("Set mion gang");
	mionRangCmd->SetParameterName("Size",false);
	mionRangCmd->SetRange("Size>=0.");
	mionRangCmd->SetUnitCategory("Length");
	mionRangCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	Det1InRadCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setDet1InRad",this);
	Det1InRadCmd->SetGuidance("Set detector inner radius");
	Det1InRadCmd->SetParameterName("Size",false);
	Det1InRadCmd->SetRange("Size>=0.");
	Det1InRadCmd->SetUnitCategory("Length");
	Det1InRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	Det1OutRadCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setDet1OutRad",this);
	Det1OutRadCmd->SetGuidance("Set detector outer radius");
	Det1OutRadCmd->SetParameterName("Size",false);
	Det1OutRadCmd->SetRange("Size>=0.");
	Det1OutRadCmd->SetUnitCategory("Length");
	Det1OutRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	Det1XCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setDet1X",this);
	Det1XCmd->SetGuidance("Set detector x position");
	Det1XCmd->SetParameterName("Size",false);
	Det1XCmd->SetRange("Size>=0.");
	Det1XCmd->SetUnitCategory("Length");
	Det1XCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	RotationDegCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setRotationDeg",this);
	RotationDegCmd->SetGuidance("Set detector rotation");
	RotationDegCmd->SetParameterName("Size",false);
	RotationDegCmd->SetRange("Size>=0.");
	RotationDegCmd->SetDefaultUnit("deg");
	RotationDegCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	RotationAddDistCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/det/setRotationAddDist",this);
	RotationAddDistCmd->SetGuidance("Set addition dist to calimator");
	RotationAddDistCmd->SetParameterName("Size",false);
	RotationAddDistCmd->SetRange("Size>=0.");
	RotationAddDistCmd->SetUnitCategory("Length");
	RotationAddDistCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	EntranceCollGapXCmd = CreateCommand("/Leetech/det/SetCollimatorEntranceGapX",
										"Set collimator gap along X axis at the entrance");
	EntranceCollGapYCmd = CreateCommand("/Leetech/det/SetCollimatorEntranceGapY",
										"Set collimator gap along Y axis at the entrance");
	Exit1CollGapXCmd = CreateCommand("/Leetech/det/SetCollimatorExit1GapX",
									 "Set collimator gap along X axis at the exit 1");
	Exit1CollGapYCmd = CreateCommand("/Leetech/det/SetCollimatorExit1GapY",
									 "Set collimator gap along Y axis at the exit 1");
}
DetectorMessenger::~DetectorMessenger()
{
	delete ApertureThickCmd;
	delete ApertureRadiusCmd;
	delete CupThickCmd;
	delete MagFieldCmd;
	delete detDir;
	delete detDirNew;
	delete MicromegasDir;

	delete maxStepCmd;
	delete maxLengthCmd;
	delete maxTimeCmd;
	delete minEkinCmd;
	delete mionRangCmd;

	delete Det1InRadCmd;
	delete Det1OutRadCmd;
	delete Det1XCmd;
	delete RotationDegCmd;
	delete RotationAddDistCmd;

	delete EntranceCollGapXCmd;
	delete EntranceCollGapYCmd;
	delete Exit1CollGapXCmd;
	delete Exit1CollGapYCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
	if( command == MagFieldCmd ){
		Detector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}
	if( command == CupThickCmd ){
	Detector->SetCupLenght(CupThickCmd->GetNewDoubleValue(newValue));}
	if( command == ApertureThickCmd ){
	Detector->SetApertureLenght(ApertureThickCmd->GetNewDoubleValue(newValue));}
	if( command == ApertureRadiusCmd ){
	Detector->SetApertureInRadius(ApertureRadiusCmd->GetNewDoubleValue(newValue));}
	if( command == RotationDegCmd){
	Detector->SetRotationDeg(RotationDegCmd->GetNewDoubleValue(newValue));}
	if( command == RotationAddDistCmd){
	Detector->SetRotationAddDistCmd(RotationAddDistCmd->GetNewDoubleValue(newValue));}

	if( command == maxStepCmd){
	Detector->SetMaxStep(maxStepCmd->GetNewDoubleValue(newValue));}
	if( command == maxLengthCmd){
	Detector->SetMaxLength(maxLengthCmd->GetNewDoubleValue(newValue));}
	if( command == maxTimeCmd){
	Detector->SetMaxTime(maxTimeCmd->GetNewDoubleValue(newValue));}
	if( command == minEkinCmd){
	Detector->SetMinEkin(minEkinCmd->GetNewDoubleValue(newValue));}
	if( command == mionRangCmd){
	Detector->SetMionRang(mionRangCmd->GetNewDoubleValue(newValue));}
	
	if( command == Det1InRadCmd){
	Detector->SetDet1InRad(Det1InRadCmd->GetNewDoubleValue(newValue));}
	if( command == Det1OutRadCmd){
	Detector->SetDet1OutRad(Det1OutRadCmd->GetNewDoubleValue(newValue));}
	if( command == Det1XCmd){
	Detector->SetDet1X(Det1XCmd->GetNewDoubleValue(newValue));}
	
	if( command == EntranceCollGapXCmd){
	Detector->SetCollimatorGapEntranceX(EntranceCollGapXCmd->GetNewDoubleValue(newValue));}
	if( command == EntranceCollGapYCmd){
	Detector->SetCollimatorGapEntranceY(EntranceCollGapYCmd->GetNewDoubleValue(newValue));}
	if( command == Exit1CollGapXCmd){
	Detector->SetCollimatorGapExit1X(Exit1CollGapXCmd->GetNewDoubleValue(newValue));}
	if( command == Exit1CollGapYCmd){
	Detector->SetCollimatorGapExit1Y(Exit1CollGapYCmd->GetNewDoubleValue(newValue));}
}

G4UIcmdWithADoubleAndUnit* DetectorMessenger::CreateCommand(const char *name, const char *comment)
{
	G4UIcmdWithADoubleAndUnit* command = new G4UIcmdWithADoubleAndUnit(name,this);
	command->SetGuidance(comment);
	command->SetParameterName("Size",false);
	command->SetRange("Size>=0.");
	command->SetUnitCategory("Length");
	command->AvailableForStates(G4State_PreInit,G4State_Idle);
	return command;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
