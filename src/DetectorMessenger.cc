#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

DetectorMessenger::DetectorMessenger(
				     DetectorConstruction* Det, DetectorConstructionP2* DetP2, DetectorConstructionP3* DetP3)
 :Detector(Det),DetectorP2(DetP2),DetectorP3(DetP3)
{ 
	number_of_geom = true;
  MicromegasDir = new G4UIdirectory("/Micromegas/");
  MicromegasDir->SetGuidance("UI commands of Micromegas Geant4 app");
  
  detDir = new G4UIdirectory("/Micromegas/det/");
  detDir->SetGuidance("detector control");

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

  GeomIDCmd = new G4UIcmdWithAnInteger("/Micromegas/det/SetGeomID",this);
  GeomIDCmd->SetGuidance("Set geom ID.");  
  GeomIDCmd->SetParameterName("GeomID",false);
  GeomIDCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/Micromegas/det/update",this);
  UpdateCmd->SetGuidance("Update detector geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);

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
}
DetectorMessenger::~DetectorMessenger()
{
  delete UpdateCmd;
  delete ApertureThickCmd;
  delete ApertureRadiusCmd;
  delete CupThickCmd;
  delete MagFieldCmd;
  delete GeomIDCmd;
  delete detDir;
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
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{   
	if(number_of_geom)
		{
	  if( command == UpdateCmd ){
		Detector->UpdateGeometry();}
	  if( command == MagFieldCmd ){ 
		Detector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}
	  if( command == CupThickCmd ){
		Detector->SetCupLenght(CupThickCmd->GetNewDoubleValue(newValue));}
	  if( command == ApertureThickCmd ){
		Detector->SetApertureLenght(ApertureThickCmd->GetNewDoubleValue(newValue));}
	  if( command == ApertureRadiusCmd ){
		Detector->SetApertureInRadius(ApertureRadiusCmd->GetNewDoubleValue(newValue));}
	  if( command == GeomIDCmd){
		Detector->SetGeomID(GeomIDCmd->GetNewIntValue(newValue));}
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
	}

	if(number_of_geom)
		{
//	  if( command == UpdateCmd ){
	//    Detector->UpdateGeometry();}
	  if( command == MagFieldCmd ){ 
		DetectorP2->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}
	  if( command == CupThickCmd ){
		DetectorP2->SetCupLenght(CupThickCmd->GetNewDoubleValue(newValue));}
	  if( command == ApertureThickCmd ){
		DetectorP2->SetApertureLenght(ApertureThickCmd->GetNewDoubleValue(newValue));}
	  if( command == ApertureRadiusCmd ){
		DetectorP2->SetApertureInRadius(ApertureRadiusCmd->GetNewDoubleValue(newValue));}
	  if( command == GeomIDCmd){
		DetectorP2->SetGeomID(GeomIDCmd->GetNewIntValue(newValue));}
	
	  if( command == maxStepCmd){
		DetectorP2->SetMaxStep(maxStepCmd->GetNewDoubleValue(newValue));}
	  if( command == maxLengthCmd){
		DetectorP2->SetMaxLength(maxLengthCmd->GetNewDoubleValue(newValue));}
	  if( command == maxTimeCmd){
		DetectorP2->SetMaxTime(maxTimeCmd->GetNewDoubleValue(newValue));}
	  if( command == minEkinCmd){
		DetectorP2->SetMinEkin(minEkinCmd->GetNewDoubleValue(newValue));}
	  if( command == mionRangCmd){
		DetectorP2->SetMionRang(mionRangCmd->GetNewDoubleValue(newValue));}
	
		if( command == Det1InRadCmd){
		DetectorP2->SetDet1InRad(Det1InRadCmd->GetNewDoubleValue(newValue));}
		if( command == Det1OutRadCmd){
		DetectorP2->SetDet1OutRad(Det1OutRadCmd->GetNewDoubleValue(newValue));}
		if( command == Det1XCmd){
		DetectorP2->SetDet1X(Det1XCmd->GetNewDoubleValue(newValue));}
		if( command == RotationDegCmd){
		DetectorP2->SetRotationDeg(RotationDegCmd->GetNewDoubleValue(newValue));}
		if( command == RotationAddDistCmd){
		DetectorP2->SetRotationAddDistCmd(RotationAddDistCmd->GetNewDoubleValue(newValue));}
	}
	
		if(number_of_geom)
		{
//	  if( command == UpdateCmd ){
	//    Detector->UpdateGeometry();}
	  if( command == MagFieldCmd ){ 
		DetectorP3->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}
	  if( command == CupThickCmd ){
		DetectorP3->SetCupLenght(CupThickCmd->GetNewDoubleValue(newValue));}
	  if( command == ApertureThickCmd ){
		DetectorP3->SetApertureLenght(ApertureThickCmd->GetNewDoubleValue(newValue));}
	  if( command == ApertureRadiusCmd ){
		DetectorP3->SetApertureInRadius(ApertureRadiusCmd->GetNewDoubleValue(newValue));}
	  if( command == GeomIDCmd){
		DetectorP3->SetGeomID(GeomIDCmd->GetNewIntValue(newValue));}
	
	  if( command == maxStepCmd){
		DetectorP3->SetMaxStep(maxStepCmd->GetNewDoubleValue(newValue));}
	  if( command == maxLengthCmd){
		DetectorP3->SetMaxLength(maxLengthCmd->GetNewDoubleValue(newValue));}
	  if( command == maxTimeCmd){
		DetectorP3->SetMaxTime(maxTimeCmd->GetNewDoubleValue(newValue));}
	  if( command == minEkinCmd){
		DetectorP3->SetMinEkin(minEkinCmd->GetNewDoubleValue(newValue));}
	  if( command == mionRangCmd){
		DetectorP3->SetMionRang(mionRangCmd->GetNewDoubleValue(newValue));}
	
		if( command == Det1InRadCmd){
		DetectorP3->SetDet1InRad(Det1InRadCmd->GetNewDoubleValue(newValue));}
		if( command == Det1OutRadCmd){
		DetectorP3->SetDet1OutRad(Det1OutRadCmd->GetNewDoubleValue(newValue));}
		if( command == Det1XCmd){
		DetectorP3->SetDet1X(Det1XCmd->GetNewDoubleValue(newValue));}
		if( command == RotationDegCmd){
		DetectorP3->SetRotationDeg(RotationDegCmd->GetNewDoubleValue(newValue));}
		if( command == RotationAddDistCmd){
		DetectorP3->SetRotationAddDistCmd(RotationAddDistCmd->GetNewDoubleValue(newValue));}
	}


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
