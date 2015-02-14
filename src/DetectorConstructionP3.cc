#include "DetectorConstructionP3.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4UnionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4SubtractionSolid.hh"

#include "G4Transform3D.hh"

//#include "TROOT.h"
#include "TMath.h"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UserLimits.hh"

//My
#include "MagneticField.hh"
//magnetic field
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4UserLimits.hh"


//C, C++
#include "assert.h"


DetectorConstructionP3::DetectorConstructionP3(DetectorConstruction* MD, MagneticField *MF)
:magField(MF),
 solidWorld(0),logicWorld(0),physiWorld(0),
 solidBeamPipe(0),logicBeamPipe(0),physiBeamPipe(0),
 solidBeamPipeV(0),logicBeamPipeV(0),physiBeamPipeV(0),
 solidCup(0),logicCup(0),physiCup(0),
 MainDet(MD)
{
  SetMagField(300);
  SetCupLenght(0.4*cm);
  SetApertureInRadius(0.1*cm);
  SetApertureLenght(7.0*cm);
  SetGeomID(3);

  SetMaxStep(8*m);
  SetMaxLength(8*mm);
  SetMaxTime(8*s);
  SetMinEkin(8*MeV);
  SetMionRang(8*m);
}

DetectorConstructionP3::~DetectorConstructionP3()
{ 
  delete stepLimit;
  //delete detectorMessenger;
}

G4VPhysicalVolume* DetectorConstructionP3::ConstructGeom7(){

	const bool ColimGeom = true;
	G4double gap_between_collimators = 3*cm;

  G4double ShiftingOfAdditionalBox = _Det1X; // Distance from the edge to the beginning of the collimators system

  G4double WorldSizeX         	= 	200.0*cm;
  G4double WorldSizeY         	= 	WorldSizeX;
  G4double WorldSizeZ         	= 	WorldSizeX;
  
  G4double beamSize		=	2*cm;
  const G4double beamSizeMax	=	2*cm;

  G4double rotAngle	     	= 	_RotationDeg;//rotation of the PHIL pipe
  
  G4double ThicknesOfChamber 	=	0.01*mm;
    
  G4double gapSize            	= 	1*mm;
  G4double bigGap            	= 	1*cm;
  
  G4double beamPipeLenght     	= 	20.0*cm;
  G4double beamPipeOutRadius  	= 	3.00*cm;
  G4double beamPipeInRadius   	= 	2.70*cm;
  G4double beamPipeVOutRadius 	= 	beamPipeInRadius - gapSize;
  
  G4double cupOutRadius       	= 	beamPipeOutRadius;
  G4double cupLenght          	= 	_cupLenght;
  
  G4double beamCorectionX	=	(beamPipeLenght/2+gapSize+cupLenght/2)*sin(rotAngle);
  G4double beamCorectionZ	=	(beamPipeLenght/2+gapSize+cupLenght/2)*(1-cos(rotAngle));
    
  //G4double windowRad 		= 	0.5*cm;
  G4double windowThick 		= 	20*um;
  //G4double windowThick 		= 	1*nm;
  
  G4double colimatorX		=	20*mm;
  G4double colimatorY		=	40*mm;
  G4double colimatorZ		=	20*mm;
  G4double colimatorZthin	=	2*mm;
    
  G4double chamberX		= 	400*mm;
  G4double chamberY		= 	40*mm;
  //G4double chamberZ		= 	fieldBoxZ + colimatorZ*2 + ThicknesOfChamber*2 + gapSize*10;
  G4double chamberZ		= 	200*mm;
  
  G4double innerBoxX         	= 	chamberX-ThicknesOfChamber*2; 
  G4double innerBoxY         	= 	chamberY-ThicknesOfChamber*2;
  G4double innerBoxZ         	= 	chamberZ-ThicknesOfChamber*2;
  
  //Sizes of the connection from chamber to collimator system
  G4double NeckX         	= 	4*cm;
  G4double NeckY         	= 	4*cm;
  G4double NeckZ         	= 	ThicknesOfChamber*2;

  G4double fieldBoxX         	= 	chamberX-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxY         	= 	chamberY-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxZ         	= 	chamberZ-ThicknesOfChamber*2-gapSize*2 - 6*mm;
  
  G4double detectorRad         	= 	_Det1OutRad; 
  G4double detectorThick        = 	1*mm; 
  
  G4double AdditionalBoxX	=	colimatorX*2+beamSizeMax+gapSize*2 + ThicknesOfChamber*2;
  G4double AdditionalBoxY	=	colimatorY+beamSizeMax+gapSize*2 + ThicknesOfChamber*2 + bigGap;
  G4double AdditionalBoxZ	=	colimatorZ*2+bigGap*0.5 + 2*ThicknesOfChamber;
  
  G4double AdditionalInnerBoxX	=	AdditionalBoxX-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxY	=	AdditionalBoxY-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxZ	=	AdditionalBoxZ-ThicknesOfChamber;
  
  G4double  windowToCup 	= 	3*cm;
  G4RotationMatrix*RM1=new G4RotationMatrix(89.9*deg,-rotAngle,0);
  
  //distance between system of cordiantes of beampipe and chamber
  G4double fromOriginToOrigin 	=	AdditionalBoxZ+2*cm;

  G4double ShieldBarrierX       =       0.22*m;
  G4double ShieldBarrierY       =       0.4*m;
  G4double ShieldBarrierZ       =       0.4*m;
  
  G4double InnerTopShieldX      =       20*mm;
  G4double InnerTopShieldY      =       fieldBoxY-gapSize;
  G4double InnerTopShieldZ      =       25*mm;
  
  G4double InnerBotShieldX      =       InnerTopShieldX;
  G4double InnerBotShieldY      =       InnerTopShieldY;
  G4double InnerBotShieldZ      =       135*mm;
  
  //
  //	Volimes position
  //
  
  G4double InnerTopShield_xc    =       0*mm;
  G4double InnerTopShield_yc    =       0;
  G4double InnerTopShield_zc    =       fieldBoxZ/2-gapSize-InnerTopShieldZ/2;
  
  G4double InnerBotShield_xc    =       0*mm;
  G4double InnerBotShield_yc    =       0;
  G4double InnerBotShield_zc    =       -fieldBoxZ/2+gapSize+InnerBotShieldZ/2; 
  
  G4double fieldBox_xc         	= 	0;
  G4double fieldBox_yc         	= 	0;
  G4double fieldBox_zc         	= 	3*mm;
  
  G4double innerBox_xc         	= 	0.0*cm; 
  G4double innerBox_yc         	= 	0.0*cm;
  G4double innerBox_zc         	= 	0.0*cm;
  
  G4double beamPipe_xc          =       beamCorectionX;
  G4double beamPipe_yc          =       0;
  G4double beamPipe_zc          =       -cupLenght/2.0 - gapSize - beamPipeLenght/2.0 + beamCorectionZ;

  G4double cup_xc               =       0;
  G4double cup_yc               =       0;
  G4double cup_zc               =       0;
  
  G4double window_xc            =       0;
  //G4double window_yc  	=       0;
  G4double window_zc            =       -cupLenght/2.0 - gapSize- windowToCup;

  //Move to other coordinat system
  beamPipe_zc	-=	fromOriginToOrigin;
  cup_zc	-=	fromOriginToOrigin;
  window_zc	-=	fromOriginToOrigin;

  beamPipe_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  cup_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  window_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
 //Finish
  
  //std::cout<<std::endl<<"Coordinates of initial position of electrons must be: "<<beamPipe_xc<<" "<<beamPipe_yc<<" "<<beamPipe_zc<<std::endl<<std::endl;

  G4double chamber_xc		= 	chamberX/4;
  G4double chamber_yc		= 	0;
  G4double chamber_zc		= 	chamberZ/2;
  
  G4double ShieldBarrier_xc     =       chamber_xc;
  G4double ShieldBarrier_yc     =       0;
  G4double ShieldBarrier_zc     =       chamber_zc-chamberZ/2-ShieldBarrierZ/2-gapSize;

  G4double Trans_x1             =       -chamberX/2+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  G4double Trans_x2             =       chamberX/2-AdditionalInnerBoxX/2-ShiftingOfAdditionalBox;

  G4double Trans_z11            =       -chamberZ/2-AdditionalBoxZ/2;
  G4double Trans_z12            =       -chamber_zc-AdditionalBoxZ/2;
  G4double Trans_z21            =       -chamberZ/2-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;
  G4double Trans_z22            =       -chamber_zc-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;

  G4ThreeVector zTransAddBox11	(Trans_x1, 0, Trans_z11);
  G4ThreeVector zTransAddBox12	(Trans_x2, 0, Trans_z12);
  
  G4ThreeVector zTransInnerBox21	(Trans_x1, 0, Trans_z21);
  G4ThreeVector zTransInnerBox22	(Trans_x2, 0, Trans_z22);
  
  G4ThreeVector zNeck1	(Trans_x1, 0, -innerBoxZ/2-NeckZ/2);
  G4ThreeVector zNeck2	(Trans_x2, 0, -innerBoxZ/2-NeckZ/2);

  G4double shiftDet             =       1*cm;
  G4double detector_xc         	= 	Trans_x2+chamber_xc;
  G4double detector_yc         	= 	0;
  G4double detector_zc         	= 	-AdditionalBoxZ - gapSize + ThicknesOfChamber*2 - shiftDet;

  beamSize = _apertureInRadius;

  G4double colimator_xc11	=	Trans_x1+colimatorX/2+beamSize/2;
  G4double colimator_yc11	=	0*mm;
  G4double colimator_zc11	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  
  G4double colimator_xc12	=	Trans_x1-colimatorX/2-beamSize/2;
  G4double colimator_yc12	=	0*mm;			
  G4double colimator_zc12	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  
  G4double colimator_xc21	=	Trans_x1;
  G4double colimator_yc21	=	colimatorX/2+beamSize/2;			
  G4double colimator_zc21	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  
  G4double colimator_xc22	=	Trans_x1;
  G4double colimator_yc22	=	-colimatorX/2-beamSize/2;				
  G4double colimator_zc22	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  
  beamSize = _apertureLenght;
  
  G4double colimator_xc31	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc31	=	0*mm;				
  G4double colimator_zc31	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc31thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;
  
  G4double colimator_xc32	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc32	=	0*mm;			
  G4double colimator_zc32	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc32thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;
  
  G4double colimator_xc41	=	Trans_x2;
  G4double colimator_yc41	=	colimatorX/2+beamSize/2;			
  G4double colimator_zc41	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc41thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;
  
  G4double colimator_xc42	=	Trans_x2;
  G4double colimator_yc42	=	-colimatorX/2-beamSize/2;				
  G4double colimator_zc42	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc42thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc51	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc51	=	0*mm;				
  G4double colimator_zc51thin	=	colimator_zc31thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc52	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc52	=	0*mm;
  G4double colimator_zc52thin	=	colimator_zc32thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc61	=	Trans_x2;
  G4double colimator_yc61	=	colimatorX/2+beamSize/2;
  G4double colimator_zc61thin	=	colimator_zc41thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc62	=	Trans_x2;
  G4double colimator_yc62	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc62thin	=	colimator_zc42thin -gap_between_collimators-ThicknesOfChamber*2;
   
  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox1(fieldBox_xc + chamber_xc, fieldBox_yc + chamber_yc, fieldBox_zc + chamber_zc, 
			 fieldBoxX, fieldBoxY, fieldBoxZ);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
  
  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);                     
  logicWorld = new G4LogicalVolume(solidWorld,
		  //MainDet->GetMaterial(3), air
		  MainDet->GetMaterial(5),  //myvacuum
		  "World");
  physiWorld = new G4PVPlacement(0,	 G4ThreeVector(),  logicWorld, "World", 0, false, 0);

  //
  // Beampipe
  //
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,MainDet->GetMaterial(2),"BeamPipe");
  physiBeamPipe = new G4PVPlacement(RM1, G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipe, "BeamPipe",  logicWorld,false,0);

  //
  // Beampipe Vacuum
  //
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,MainDet->GetMaterial(6),"BeamPipeV");
  physiBeamPipeV = new G4PVPlacement(RM1,G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipeV,"BeamPipeV",logicWorld, false, 0);

  //
  // Beampipe cup - absorber
  //

  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,MainDet->GetMaterial(1), "Cup");
  physiCup = new G4PVPlacement(RM1,G4ThreeVector(cup_xc,cup_yc,cup_zc),logicCup,"Cup",logicWorld,false,0);
  
  //
  //	Chamber
  //
  
  solidChamber1 = new G4Box("Chamber1",chamberX/2, chamberY/2, chamberZ/2);
  solidAdditionalBox = new G4Box("ChamberAdd",AdditionalBoxX/2, AdditionalBoxY/2, AdditionalBoxZ/2);
  G4RotationMatrix*RMZero=new G4RotationMatrix(0,0,0);
  G4Transform3D transform11(*RMZero, zTransAddBox11);
  G4Transform3D transform12(*RMZero, zTransAddBox12);
  G4UnionSolid* solidChamber2 = new G4UnionSolid("Chamber", solidChamber1, solidAdditionalBox, transform11);
  G4UnionSolid* solidChamber = new G4UnionSolid("Chamber", solidChamber2, solidAdditionalBox, transform12);
 
  logicChamber = new G4LogicalVolume(solidChamber,MainDet->GetMaterial(1), "Chamber");
  physiChamber = new G4PVPlacement(0, G4ThreeVector(chamber_xc,chamber_yc,chamber_zc), logicChamber, "Chamber",	 logicWorld, false, 0);	

  //
  //	InnerBox
  //

  solidInnerBox = new G4Box("InnerBox",innerBoxX/2, innerBoxY/2, innerBoxZ/2);
  solidAdditionalInnerBox = new G4Box("InnerAdd",AdditionalInnerBoxX/2, AdditionalInnerBoxY/2, AdditionalInnerBoxZ/2);
  solidNeck = new G4Box("Neck",NeckX/2, NeckY/2, NeckZ/2);
  G4Transform3D transform21(*RMZero, zTransInnerBox21);
  G4Transform3D transform22(*RMZero, zTransInnerBox22);
  G4Transform3D transNeck1(*RMZero, zNeck1);
  G4Transform3D transNeck2(*RMZero, zNeck2);
  G4UnionSolid* solidInnerBoxUNeck = new G4UnionSolid("Chamber", solidInnerBox, solidNeck, transNeck1);
  G4UnionSolid* solidInnerBoxUNeck2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck, solidNeck, transNeck2);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox = new G4UnionSolid("Chamber", solidInnerBoxUNeck2, solidAdditionalInnerBox, transform21);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck2UAddBox, solidAdditionalInnerBox, transform22);
  
  logicInnerBox = new G4LogicalVolume(solidInnerBoxUNeck2UAddBox2,MainDet->GetMaterial(5),"InnerBox");
  physiInnerBox = new G4PVPlacement(0, G4ThreeVector(innerBox_xc,innerBox_yc,innerBox_zc), logicInnerBox, "InnerBox", logicChamber,	 false,	 0);
  
  //
  //	FieldBox
  //
  solidFieldBox = new G4Box("FieldBox",fieldBoxX/2, fieldBoxY/2, fieldBoxZ/2);                     
  logicFieldBox = new G4LogicalVolume(solidFieldBox,MainDet->GetMaterial(5), "FieldBox");
  physiFieldBox = new G4PVPlacement(0, G4ThreeVector(fieldBox_xc,fieldBox_yc,fieldBox_zc), logicFieldBox,"FieldBox", logicInnerBox,	 false,	 0);

  //
  //	Colimator 11
  //
  int ColimMat = 2;
  solidColim11 = new G4Box("Kolimator11",colimatorX/2, colimatorY/2, colimatorZ/2);                     
  logicColim11 = new G4LogicalVolume(solidColim11,MainDet->GetMaterial(ColimMat), "Kolimator11");
  physiColim11 = new G4PVPlacement(0,	G4ThreeVector(colimator_xc11,colimator_yc11,colimator_zc11), logicColim11,"Kolimator11", logicInnerBox,  false, 0);
  //
  //	Colimator 12
  //
  solidColim12 = new G4Box("Kolimator12",colimatorX/2, colimatorY/2, colimatorZ/2);                     
  logicColim12 = new G4LogicalVolume(solidColim12,MainDet->GetMaterial(ColimMat),"Kolimator12");
  physiColim12 = new G4PVPlacement(0, G4ThreeVector(colimator_xc12,colimator_yc12,colimator_zc12), logicColim12,"Kolimator12", logicInnerBox, false, 0);	
  //
  //	Colimator 21
  //
  solidColim21 = new G4Box("Kolimator21",colimatorY/2, colimatorX/2, colimatorZ/2);                     
  logicColim21 = new G4LogicalVolume(solidColim21,MainDet->GetMaterial(ColimMat), "Kolimator21");
  physiColim21 = new G4PVPlacement(0, G4ThreeVector(colimator_xc21,colimator_yc21,colimator_zc21), logicColim21, "Kolimator21", logicInnerBox, false, 0);	
  //
  //	Colimator 22
  //
  solidColim22 = new G4Box("Kolimator22",colimatorY/2, colimatorX/2, colimatorZ/2);                     
  logicColim22 = new G4LogicalVolume(solidColim22,MainDet->GetMaterial(ColimMat), "Kolimator12");
  physiColim22 = new G4PVPlacement(0, G4ThreeVector(colimator_xc22,colimator_yc22,colimator_zc22),  logicColim22, "Kolimator22", logicInnerBox, false, 0);	

  if(ColimGeom)
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31),  logicColim11, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42), logicColim42,"Kolimator42", logicInnerBox, false,0);
  }
  /**/
  else
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31thin),  logicColim31, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32thin), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41thin), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42thin), logicColim42,"Kolimator42", logicInnerBox, false,0);

	  //
	  //	Colimator 51
	  //
	  solidColim51 = new G4Box("Kolimator51",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim51 = new G4LogicalVolume(solidColim51,MainDet->GetMaterial(ColimMat),  "Kolimator51");
	  physiColim51 = new G4PVPlacement(0, G4ThreeVector(colimator_xc51,colimator_yc51,colimator_zc51thin),  logicColim51, "Kolimat51", logicInnerBox, false, 0);
	  //
	  //	Colimator 52
	  //
	  solidColim52 = new G4Box("Kolimator52",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim52 = new G4LogicalVolume(solidColim52,MainDet->GetMaterial(ColimMat), "Kolimator52");
	  physiColim52 = new G4PVPlacement(0, G4ThreeVector(colimator_xc52,colimator_yc52,colimator_zc52thin), logicColim52, "Kolimator52",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 61
	  //
	  solidColim61 = new G4Box("Kolimator61",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim61 = new G4LogicalVolume(solidColim61,MainDet->GetMaterial(ColimMat), "Kolimator61");
	  physiColim61 = new G4PVPlacement(0, G4ThreeVector(colimator_xc61,colimator_yc61,colimator_zc61thin), logicColim61, "Kolimator61",logicInnerBox, false, 0);
	  //
	  //	Colimator 62
	  //
	  solidColim62 = new G4Box("Kolimator62",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim62 = new G4LogicalVolume(solidColim62,MainDet->GetMaterial(ColimMat), "Kolimator62");
	  physiColim62 = new G4PVPlacement(0, G4ThreeVector(colimator_xc62,colimator_yc62,colimator_zc62thin), logicColim62,"Kolimator62", logicInnerBox, false,0);
  }

  //
  //Detector
  //
  solidSenDet1 = new G4Tubs("SenDet1", 0, detectorRad, detectorThick, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     //beamVacuum,
						MainDet->GetMaterial(6),
				     "SenDet1");
  physiSenDet1 = new G4PVPlacement(0,	//rotation
				   G4ThreeVector(detector_xc,detector_yc,detector_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume				 
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

  solidShieldBarrier = new G4Box("ShieldBarrier",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
  logicShieldBarrier = new G4LogicalVolume(solidShieldBarrier,MainDet->GetMaterial(7), "ShieldBarrier");
  physiShieldBarrier = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc,ShieldBarrier_yc,ShieldBarrier_zc),
		  logicShieldBarrier, "ShieldBarrier",logicWorld, false, 0);
  
  solidInnerTopShield = new G4Box("InnerTopShield",InnerTopShieldX/2, InnerTopShieldY/2, InnerTopShieldZ/2);
  logicInnerTopShield = new G4LogicalVolume(solidInnerTopShield,MainDet->GetMaterial(7), "InnerTopShield");
  physiInnerTopShield = new G4PVPlacement(0, G4ThreeVector(InnerTopShield_xc,InnerTopShield_yc,InnerTopShield_zc),
		  logicInnerTopShield, "InnerTopShield",logicFieldBox, false, 0);
  
  solidInnerBotShield1 = new G4Box("InnerBotShield1",InnerBotShieldX/2, InnerBotShieldY/2, InnerBotShieldZ/2);
  logicInnerBotShield1 = new G4LogicalVolume(solidInnerBotShield1,MainDet->GetMaterial(7), "InnerBotShield1");
  physiInnerBotShield1 = new G4PVPlacement(0, G4ThreeVector(InnerBotShield_xc,InnerBotShield_yc,InnerBotShield_zc),
		  logicInnerBotShield1, "InnerBotShield1",logicFieldBox, false, 0);

  ConstructMagnet(520*mm, 50*mm, chamberZ, gapSize);
  //                                        
  // Visualization attributes
  //

  
  G4VisAttributes* pipeVisAtt = new G4VisAttributes(G4Colour(0.5,0.0,0.6));
  logicBeamPipe->SetVisAttributes(pipeVisAtt);
  
  G4VisAttributes* pipeVVisAtt = new G4VisAttributes(G4Colour(0.4,0.3,0.0));
  logicBeamPipeV->SetVisAttributes(pipeVVisAtt);
  
  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);
  
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicChamber->SetVisAttributes(chamberVisAtt);
  
  G4VisAttributes* innerVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicInnerBox->SetVisAttributes(innerVisAtt);
  
  G4VisAttributes* fieldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicFieldBox->SetVisAttributes(fieldVisAtt);
      
  G4VisAttributes* detVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicSenDet1->SetVisAttributes(detVisAtt);

  G4double maxStep   = _maxStep;
  G4double maxLength = _maxLength;
  G4double maxTime   = _maxTime; 
  G4double minEkin   = _minEkin;
  G4double mionRang  = _mionRang;  

/*
  G4double maxStep   = 0.1*mm;
  G4double maxLength = 5.0*m;
  G4double maxTime   = 20.0*ns; 
  G4double minEkin   = 1.0/100*MeV;
  G4double mionRang  = 0.01*mm;  
*/
G4cout<<std::endl<<"maxStep = "<<std::setw(14)<<_maxStep<<std::endl;
G4cout<<"maxLength = "<<std::setw(14)<<_maxLength<<std::endl;
G4cout<<"maxTime = "<<std::setw(14)<<_maxTime<<std::endl;
G4cout<<"minEkin = "<<std::setw(14)<<_minEkin<<std::endl;
G4cout<<"mionRangHERE = "<<std::setw(14)<<_mionRang<<std::endl<<std::endl;

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  logicWorld->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);
  logicChamber->SetUserLimits(stepLimit);
  logicInnerBox->SetUserLimits(stepLimit);
  logicFieldBox->SetUserLimits(stepLimit);
  
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}

G4VPhysicalVolume* DetectorConstructionP3::ConstructGeom8(){

	const bool ColimGeom = true;
	G4double gap_between_collimators = 3*cm;

  G4double ShiftingOfAdditionalBox = _Det1X; // Distance from the edge to the beginning of the collimators system

  G4double WorldSizeX         	= 	200.0*cm;
  G4double WorldSizeY         	= 	WorldSizeX;
  G4double WorldSizeZ         	= 	WorldSizeX;

  G4double beamSize		=	2*cm;
  const G4double beamSizeMax	=	2*cm;

  G4double rotAngle	     	= 	_RotationDeg;//rotation of the PHIL pipe

  G4double ThicknesOfChamber 	=	3*mm;
//  G4double ThicknesOfChamberOther 	=	3*mm;

  G4double gapSize            	= 	1*mm;
  G4double bigGap            	= 	1*cm;

  G4double beamPipeLenght     	= 	20.0*cm;
  G4double beamPipeOutRadius  	= 	3.00*cm;
  G4double beamPipeInRadius   	= 	2.70*cm;
  G4double beamPipeVOutRadius 	= 	beamPipeInRadius - gapSize;

  G4double cupOutRadius       	= 	beamPipeOutRadius;
  G4double cupLenght          	= 	_cupLenght;

  G4double beamCorectionX	=	(beamPipeLenght/2+gapSize+cupLenght/2)*sin(rotAngle);
  G4double beamCorectionZ	=	(beamPipeLenght/2+gapSize+cupLenght/2)*(1-cos(rotAngle));

  //G4double windowRad 		= 	0.5*cm;
  G4double windowThick 		= 	20*um;
  //G4double windowThick 		= 	1*nm;

  G4double colimatorX		=	20*mm;
  G4double colimatorY		=	40*mm;
  G4double colimatorZ		=	20*mm;
  G4double colimatorZthin	=	2*mm;

  G4double chamberX		= 	400*mm;
  G4double chamberY		= 	40*mm;
  //G4double chamberZ		= 	fieldBoxZ + colimatorZ*2 + ThicknesOfChamber*2 + gapSize*10;
  G4double chamberZ		= 	200*mm;

  G4double innerBoxX         	= 	chamberX-ThicknesOfChamber*2;
  G4double innerBoxY         	= 	chamberY-ThicknesOfChamber*2;
  G4double innerBoxZ         	= 	chamberZ-ThicknesOfChamber*2;

  //Sizes of the connection from chamber to collimator system
  G4double NeckX         	= 	4*cm;
  G4double NeckY         	= 	4*cm;
  G4double NeckZ         	= 	ThicknesOfChamber*2;

  G4double fieldBoxX         	= 	chamberX-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxY         	= 	chamberY-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxZ         	= 	chamberZ-ThicknesOfChamber*2-gapSize*2 - 6*mm;

  G4double detectorRad         	= 	_Det1OutRad;
  G4double detectorThick        = 	1*mm;

  G4double AdditionalBoxX	=	colimatorX*2+beamSizeMax+gapSize*2 + ThicknesOfChamber*2;
  G4double AdditionalBoxY	=	colimatorY+beamSizeMax+gapSize*2 + ThicknesOfChamber*2 + bigGap;
  G4double AdditionalBoxZ	=	colimatorZ*2+bigGap*0.5 + 2*ThicknesOfChamber;

//  G4double AdditionalBoxX	=	colimatorX*2+beamSizeMax+gapSize*2 + ThicknesOfChamberOther*2;
//  G4double AdditionalBoxY	=	colimatorY+beamSizeMax+gapSize*2 + ThicknesOfChamberOther*2 + bigGap;
//  G4double AdditionalBoxZ	=	colimatorZ*2+bigGap*0.5 + 2*ThicknesOfChamberOther;

  G4double AdditionalInnerBoxX	=	AdditionalBoxX-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxY	=	AdditionalBoxY-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxZ	=	AdditionalBoxZ-ThicknesOfChamber;

  G4double  windowToCup 	= 	3*cm;
  G4RotationMatrix*RM1=new G4RotationMatrix(89.9*deg,-rotAngle,0);

  //distance between system of cordiantes of beampipe and chamber
  G4double fromOriginToOrigin 	=	AdditionalBoxZ+2*cm;

  G4double ShieldBarrierX       =       0.07*m;
  G4double ShieldBarrierY       =       0.4*m;
  G4double ShieldBarrierZ       =       0.3*m;

  G4double InnerTopShieldX      =       20*mm;
  G4double InnerTopShieldY      =       fieldBoxY-gapSize;
  G4double InnerTopShieldZ      =       25*mm;

  G4double InnerBotShield1X      =       InnerTopShieldX;
  G4double InnerBotShield1Y      =       InnerTopShieldY;
  G4double InnerBotShield1Z      =       110*mm;

  G4double InnerBotShield2X      =       InnerTopShieldX;
  G4double InnerBotShield2Y      =       InnerTopShieldY;
  G4double InnerBotShield2Z      =       60*mm;

  G4double InnerShield3X      =       	140*mm;
  G4double InnerShield3Y      =       	InnerTopShieldY;
  G4double InnerShield3Z      =       	InnerTopShieldX;

  G4double InnerShield4X      =  	     InnerTopShieldX;
  G4double InnerShield4Y      =    	 	 InnerTopShieldY;
  G4double InnerShield4Z      =     	 60*mm;

  G4double InnerShield5X      =  	     InnerTopShieldX;
  G4double InnerShield5Y      =    	 	 InnerTopShieldY;
  G4double InnerShield5Z      =     	 60*mm;
  //
  //	Volumes position
  //

  G4double InnerTopShield_xc    =       0*mm;
  G4double InnerTopShield_yc    =       0;
  G4double InnerTopShield_zc    =       fieldBoxZ/2-gapSize-InnerTopShieldZ/2;

  G4double InnerBotShield1_xc    =       70*mm;
  G4double InnerBotShield1_yc    =       0;
  G4double InnerBotShield1_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield1Z/2;

  G4double InnerBotShield2_xc    =       -70*mm;
  G4double InnerBotShield2_yc    =       0;
  G4double InnerBotShield2_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield2Z/2;

  G4double InnerShield3_xc	    =       InnerBotShield1_xc - InnerBotShield1X/2 - InnerShield3X/2;
  G4double InnerShield3_yc 	    =       0;
  G4double InnerShield3_zc   	=       -fieldBoxZ/2+gapSize+InnerBotShield1Z - InnerShield3Z/2;

  G4double InnerShield4_xc	    =       120*mm;
  G4double InnerShield4_yc 	    =       0;
  G4double InnerShield4_zc   	=       62*mm;

  G4double InnerShield5_xc	    =       -120*mm;
  G4double InnerShield5_yc 	    =       0;
  G4double InnerShield5_zc   	=       62*mm;

  G4double fieldBox_xc         	= 	0;
  G4double fieldBox_yc         	= 	0;
  G4double fieldBox_zc         	= 	3*mm;

  G4double innerBox_xc         	= 	0.0*cm;
  G4double innerBox_yc         	= 	0.0*cm;
  G4double innerBox_zc         	= 	0.0*cm;

  G4double beamPipe_xc          =       beamCorectionX;
  G4double beamPipe_yc          =       0;
  G4double beamPipe_zc          =       -cupLenght/2.0 - gapSize - beamPipeLenght/2.0 + beamCorectionZ;

  G4double cup_xc               =       0;
  G4double cup_yc               =       0;
  G4double cup_zc               =       0;

  G4double window_xc            =       0;
  //G4double window_yc  	=       0;
  G4double window_zc            =       -cupLenght/2.0 - gapSize- windowToCup;

  //Move to other coordinat system
  beamPipe_zc	-=	fromOriginToOrigin;
  cup_zc	-=	fromOriginToOrigin;
  window_zc	-=	fromOriginToOrigin;

  beamPipe_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  cup_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  window_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
 //Finish

  //std::cout<<std::endl<<"Coordinates of initial position of electrons must be: "<<beamPipe_xc<<" "<<beamPipe_yc<<" "<<beamPipe_zc<<std::endl<<std::endl;

  G4double chamber_xc		= 	chamberX/4;
  G4double chamber_yc		= 	0;
  G4double chamber_zc		= 	chamberZ/2;

  G4double ShieldBarrier_xc     =       0.175*m;
  G4double ShieldBarrier_yc     =       0;
  G4double ShieldBarrier_zc     =       chamber_zc-chamberZ/2-ShieldBarrierZ/2-gapSize;

  G4double Trans_x1             =       -chamberX/2+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  G4double Trans_x2             =       chamberX/2-AdditionalInnerBoxX/2-ShiftingOfAdditionalBox;

  G4double Trans_z11            =       -chamberZ/2-AdditionalBoxZ/2;
  G4double Trans_z12            =       -chamber_zc-AdditionalBoxZ/2;
  G4double Trans_z21            =       -chamberZ/2-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;
  G4double Trans_z22            =       -chamber_zc-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;

  G4ThreeVector zTransAddBox11	(Trans_x1, 0, Trans_z11);
  G4ThreeVector zTransAddBox12	(Trans_x2, 0, Trans_z12);

  G4ThreeVector zTransInnerBox21	(Trans_x1, 0, Trans_z21);
  G4ThreeVector zTransInnerBox22	(Trans_x2, 0, Trans_z22);

  G4ThreeVector zNeck1	(Trans_x1, 0, -innerBoxZ/2-NeckZ/2);
  G4ThreeVector zNeck2	(Trans_x2, 0, -innerBoxZ/2-NeckZ/2);

  G4double shiftDet             =       1*cm;
  G4double detector_xc         	= 	Trans_x2+chamber_xc;
  G4double detector_yc         	= 	0;
  G4double detector_zc         	= 	-AdditionalBoxZ - gapSize /*+ ThicknesOfChamber*2*/ - shiftDet + 6*mm;

  beamSize = _apertureInRadius;

  G4double colimator_xc11	=	Trans_x1+colimatorX/2+beamSize/2;
  G4double colimator_yc11	=	0*mm;
  G4double colimator_zc11	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc12	=	Trans_x1-colimatorX/2-beamSize/2;
  G4double colimator_yc12	=	0*mm;
  G4double colimator_zc12	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc21	=	Trans_x1;
  G4double colimator_yc21	=	colimatorX/2+beamSize/2;
  G4double colimator_zc21	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc22	=	Trans_x1;
  G4double colimator_yc22	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc22	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;

  beamSize = _apertureLenght;

  G4double colimator_xc31	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc31	=	0*mm;
  G4double colimator_zc31	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc31thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc32	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc32	=	0*mm;
  G4double colimator_zc32	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc32thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc41	=	Trans_x2;
  G4double colimator_yc41	=	colimatorX/2+beamSize/2;
  G4double colimator_zc41	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc41thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc42	=	Trans_x2;
  G4double colimator_yc42	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc42	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc42thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc51	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc51	=	0*mm;
  G4double colimator_zc51thin	=	colimator_zc31thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc52	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc52	=	0*mm;
  G4double colimator_zc52thin	=	colimator_zc32thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc61	=	Trans_x2;
  G4double colimator_yc61	=	colimatorX/2+beamSize/2;
  G4double colimator_zc61thin	=	colimator_zc41thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc62	=	Trans_x2;
  G4double colimator_yc62	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc62thin	=	colimator_zc42thin -gap_between_collimators-ThicknesOfChamber*2;

  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox1(fieldBox_xc + chamber_xc, fieldBox_yc + chamber_yc, fieldBox_zc + chamber_zc,
			 fieldBoxX, fieldBoxY, fieldBoxZ);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();


  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);
  logicWorld = new G4LogicalVolume(solidWorld,
		  //MainDet->GetMaterial(3), air
		  MainDet->GetMaterial(5),  //myvacuum
		  "World");
  physiWorld = new G4PVPlacement(0,	 G4ThreeVector(),  logicWorld, "World", 0, false, 0);

  //
  // Beampipe
  //
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,MainDet->GetMaterial(2),"BeamPipe");
  physiBeamPipe = new G4PVPlacement(RM1, G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipe, "BeamPipe",  logicWorld,false,0);

  //
  // Beampipe Vacuum
  //
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,MainDet->GetMaterial(6),"BeamPipeV");
  physiBeamPipeV = new G4PVPlacement(RM1,G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipeV,"BeamPipeV",logicWorld, false, 0);

  //
  // Beampipe cup - absorber
  //

  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,MainDet->GetMaterial(1), "Cup");
  physiCup = new G4PVPlacement(RM1,G4ThreeVector(cup_xc,cup_yc,cup_zc),logicCup,"Cup",logicWorld,false,0);

  //
  //	Chamber
  //

  solidChamber1 = new G4Box("Chamber1",chamberX/2, chamberY/2, chamberZ/2);
  solidAdditionalBox = new G4Box("ChamberAdd",AdditionalBoxX/2, AdditionalBoxY/2, AdditionalBoxZ/2);
  G4RotationMatrix*RMZero=new G4RotationMatrix(0,0,0);
  G4Transform3D transform11(*RMZero, zTransAddBox11);
  G4Transform3D transform12(*RMZero, zTransAddBox12);
  G4UnionSolid* solidChamber2 = new G4UnionSolid("Chamber", solidChamber1, solidAdditionalBox, transform11);
  G4UnionSolid* solidChamber = new G4UnionSolid("Chamber", solidChamber2, solidAdditionalBox, transform12);

  logicChamber = new G4LogicalVolume(solidChamber,MainDet->GetMaterial(1), "Chamber");
  physiChamber = new G4PVPlacement(0, G4ThreeVector(chamber_xc,chamber_yc,chamber_zc), logicChamber, "Chamber",	 logicWorld, false, 0);

  //
  //	InnerBox
  //

  solidInnerBox = new G4Box("InnerBox",innerBoxX/2, innerBoxY/2, innerBoxZ/2);
  solidAdditionalInnerBox = new G4Box("InnerAdd",AdditionalInnerBoxX/2, AdditionalInnerBoxY/2, AdditionalInnerBoxZ/2);
  solidNeck = new G4Box("Neck",NeckX/2, NeckY/2, NeckZ/2);
  G4Transform3D transform21(*RMZero, zTransInnerBox21);
  G4Transform3D transform22(*RMZero, zTransInnerBox22);
  G4Transform3D transNeck1(*RMZero, zNeck1);
  G4Transform3D transNeck2(*RMZero, zNeck2);
  G4UnionSolid* solidInnerBoxUNeck = new G4UnionSolid("Chamber", solidInnerBox, solidNeck, transNeck1);
  G4UnionSolid* solidInnerBoxUNeck2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck, solidNeck, transNeck2);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox = new G4UnionSolid("Chamber", solidInnerBoxUNeck2, solidAdditionalInnerBox, transform21);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck2UAddBox, solidAdditionalInnerBox, transform22);

  logicInnerBox = new G4LogicalVolume(solidInnerBoxUNeck2UAddBox2,MainDet->GetMaterial(5),"InnerBox");
  physiInnerBox = new G4PVPlacement(0, G4ThreeVector(innerBox_xc,innerBox_yc,innerBox_zc), logicInnerBox, "InnerBox", logicChamber,	 false,	 0);

  //
  //	FieldBox
  //
  solidFieldBox = new G4Box("FieldBox",fieldBoxX/2, fieldBoxY/2, fieldBoxZ/2);
  logicFieldBox = new G4LogicalVolume(solidFieldBox,MainDet->GetMaterial(5), "FieldBox");
  physiFieldBox = new G4PVPlacement(0, G4ThreeVector(fieldBox_xc,fieldBox_yc,fieldBox_zc), logicFieldBox,"FieldBox", logicInnerBox,	 false,	 0);

  //
  //	Colimator 11
  //
  int ColimMat = 2;
  solidColim11 = new G4Box("Kolimator11",colimatorX/2, colimatorY/2, colimatorZ/2);
  logicColim11 = new G4LogicalVolume(solidColim11,MainDet->GetMaterial(ColimMat), "Kolimator11");
  physiColim11 = new G4PVPlacement(0,	G4ThreeVector(colimator_xc11,colimator_yc11,colimator_zc11), logicColim11,"Kolimator11", logicInnerBox,  false, 0);
  //
  //	Colimator 12
  //
  solidColim12 = new G4Box("Kolimator12",colimatorX/2, colimatorY/2, colimatorZ/2);
  logicColim12 = new G4LogicalVolume(solidColim12,MainDet->GetMaterial(ColimMat),"Kolimator12");
  physiColim12 = new G4PVPlacement(0, G4ThreeVector(colimator_xc12,colimator_yc12,colimator_zc12), logicColim12,"Kolimator12", logicInnerBox, false, 0);
  //
  //	Colimator 21
  //
  solidColim21 = new G4Box("Kolimator21",colimatorY/2, colimatorX/2, colimatorZ/2);
  logicColim21 = new G4LogicalVolume(solidColim21,MainDet->GetMaterial(ColimMat), "Kolimator21");
  physiColim21 = new G4PVPlacement(0, G4ThreeVector(colimator_xc21,colimator_yc21,colimator_zc21), logicColim21, "Kolimator21", logicInnerBox, false, 0);
  //
  //	Colimator 22
  //
  solidColim22 = new G4Box("Kolimator22",colimatorY/2, colimatorX/2, colimatorZ/2);
  logicColim22 = new G4LogicalVolume(solidColim22,MainDet->GetMaterial(ColimMat), "Kolimator12");
  physiColim22 = new G4PVPlacement(0, G4ThreeVector(colimator_xc22,colimator_yc22,colimator_zc22),  logicColim22, "Kolimator22", logicInnerBox, false, 0);

  if(ColimGeom)
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31),  logicColim11, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42), logicColim42,"Kolimator42", logicInnerBox, false,0);
  }
  /**/
  else
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31thin),  logicColim31, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32thin), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41thin), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42thin), logicColim42,"Kolimator42", logicInnerBox, false,0);

	  //
	  //	Colimator 51
	  //
	  solidColim51 = new G4Box("Kolimator51",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim51 = new G4LogicalVolume(solidColim51,MainDet->GetMaterial(ColimMat),  "Kolimator51");
	  physiColim51 = new G4PVPlacement(0, G4ThreeVector(colimator_xc51,colimator_yc51,colimator_zc51thin),  logicColim51, "Kolimat51", logicInnerBox, false, 0);
	  //
	  //	Colimator 52
	  //
	  solidColim52 = new G4Box("Kolimator52",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim52 = new G4LogicalVolume(solidColim52,MainDet->GetMaterial(ColimMat), "Kolimator52");
	  physiColim52 = new G4PVPlacement(0, G4ThreeVector(colimator_xc52,colimator_yc52,colimator_zc52thin), logicColim52, "Kolimator52",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 61
	  //
	  solidColim61 = new G4Box("Kolimator61",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim61 = new G4LogicalVolume(solidColim61,MainDet->GetMaterial(ColimMat), "Kolimator61");
	  physiColim61 = new G4PVPlacement(0, G4ThreeVector(colimator_xc61,colimator_yc61,colimator_zc61thin), logicColim61, "Kolimator61",logicInnerBox, false, 0);
	  //
	  //	Colimator 62
	  //
	  solidColim62 = new G4Box("Kolimator62",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim62 = new G4LogicalVolume(solidColim62,MainDet->GetMaterial(ColimMat), "Kolimator62");
	  physiColim62 = new G4PVPlacement(0, G4ThreeVector(colimator_xc62,colimator_yc62,colimator_zc62thin), logicColim62,"Kolimator62", logicInnerBox, false,0);
  }

  //
  //Detector
  //
  solidSenDet1 = new G4Tubs("SenDet1", 0, detectorRad, detectorThick, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     //beamVacuum,
						MainDet->GetMaterial(6),
				     "SenDet1");
  physiSenDet1 = new G4PVPlacement(0,	//rotation
				   G4ThreeVector(detector_xc,detector_yc,detector_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

//  solidShieldBarrier = new G4Box("ShieldBarrier",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
//  logicShieldBarrier = new G4LogicalVolume(solidShieldBarrier,MainDet->GetMaterial(7), "ShieldBarrier");
//  physiShieldBarrier = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc,ShieldBarrier_yc,ShieldBarrier_zc),
//		  logicShieldBarrier, "ShieldBarrier",logicWorld, false, 0);

    solidShieldBarrier1 = new G4Box("ShieldBarrier1",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
    logicShieldBarrier1 = new G4LogicalVolume(solidShieldBarrier1,MainDet->GetMaterial(7), "ShieldBarrier1");
    physiShieldBarrier1 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc,ShieldBarrier_yc,ShieldBarrier_zc),
  		  logicShieldBarrier1, "ShieldBarrier1",logicWorld, false, 0);

    solidShieldBarrier2 = new G4Box("ShieldBarrier2",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
    logicShieldBarrier2 = new G4LogicalVolume(solidShieldBarrier2,MainDet->GetMaterial(7), "ShieldBarrier2");
    physiShieldBarrier2 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc-0.15*m,ShieldBarrier_yc,ShieldBarrier_zc),
  		  logicShieldBarrier2, "ShieldBarrier2",logicWorld, false, 0);


//  solidInnerTopShield = new G4Box("InnerTopShield",InnerTopShieldX/2, InnerTopShieldY/2, InnerTopShieldZ/2);
//  logicInnerTopShield = new G4LogicalVolume(solidInnerTopShield,MainDet->GetMaterial(7), "InnerTopShield");
//  physiInnerTopShield = new G4PVPlacement(0, G4ThreeVector(InnerTopShield_xc,InnerTopShield_yc,InnerTopShield_zc),
//		  logicInnerTopShield, "InnerTopShield",logicFieldBox, false, 0);

  solidInnerBotShield1 = new G4Box("InnerBotShield1",InnerBotShield1X/2, InnerBotShield1Y/2, InnerBotShield1Z/2);
  logicInnerBotShield1 = new G4LogicalVolume(solidInnerBotShield1,MainDet->GetMaterial(7), "InnerBotShield1");
  G4RotationMatrix*RM11=new G4RotationMatrix;
  RM11->rotateY(0*deg);
  physiInnerBotShield1 = new G4PVPlacement(RM11,
		  G4ThreeVector(InnerBotShield1_xc,InnerBotShield1_yc,InnerBotShield1_zc),
		  logicInnerBotShield1, "InnerBotShield1",logicFieldBox, false, 0);

  solidInnerBotShield2 = new G4Box("InnerBotShield2",InnerBotShield2X/2, InnerBotShield2Y/2, InnerBotShield2Z/2);
  logicInnerBotShield2 = new G4LogicalVolume(solidInnerBotShield2,MainDet->GetMaterial(7), "InnerBotShield2");
  G4RotationMatrix*RM12=new G4RotationMatrix;
  RM12->rotateY(0*deg);
  physiInnerBotShield2 = new G4PVPlacement(RM12,
		  G4ThreeVector(InnerBotShield2_xc,InnerBotShield2_yc,InnerBotShield2_zc),
		  logicInnerBotShield2, "InnerBotShield2",logicFieldBox, false, 0);

  solidInnerShield3 = new G4Box("InnerShield3",InnerShield3X/2, InnerShield3Y/2, InnerShield3Z/2);
  logicInnerShield3 = new G4LogicalVolume(solidInnerShield3,MainDet->GetMaterial(7), "InnerShield3");
  G4RotationMatrix*RM13=new G4RotationMatrix;
  RM13->rotateY(0*deg);
  physiInnerShield3 = new G4PVPlacement(RM13,
		  G4ThreeVector(InnerShield3_xc,InnerShield3_yc,InnerShield3_zc),
		  logicInnerShield3, "InnerShield3",logicFieldBox, false, 0);


  solidInnerShield4 = new G4Box("InnerShield4",InnerShield4X/2, InnerShield4Y/2, InnerShield4Z/2);
  logicInnerShield4 = new G4LogicalVolume(solidInnerShield4,MainDet->GetMaterial(7), "InnerShield4");
  G4RotationMatrix*RM14=new G4RotationMatrix;
  RM14->rotateY(-45*deg);
  physiInnerShield4 = new G4PVPlacement(RM14,
		  G4ThreeVector(InnerShield4_xc,InnerShield4_yc,InnerShield4_zc),
		  logicInnerShield4, "InnerShield4",logicFieldBox, false, 0);

  solidInnerShield5 = new G4Box("InnerShield5",InnerShield5X/2, InnerShield5Y/2, InnerShield5Z/2);
  logicInnerShield5 = new G4LogicalVolume(solidInnerShield5,MainDet->GetMaterial(7), "InnerShield5");
  G4RotationMatrix*RM15=new G4RotationMatrix;
  RM15->rotateY(45*deg);
  physiInnerShield5 = new G4PVPlacement(RM15,
		  G4ThreeVector(InnerShield5_xc,InnerShield5_yc,InnerShield5_zc),
		  logicInnerShield5, "InnerShield5",logicFieldBox, false, 0);

  ConstructMagnet(520*mm, 50*mm, chamberZ, gapSize);
  //
  // Visualization attributes
  //


  G4VisAttributes* pipeVisAtt = new G4VisAttributes(G4Colour(0.5,0.0,0.6));
  logicBeamPipe->SetVisAttributes(pipeVisAtt);

  G4VisAttributes* pipeVVisAtt = new G4VisAttributes(G4Colour(0.4,0.3,0.0));
  logicBeamPipeV->SetVisAttributes(pipeVVisAtt);

  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);

  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicChamber->SetVisAttributes(chamberVisAtt);

  G4VisAttributes* innerVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicInnerBox->SetVisAttributes(innerVisAtt);

  G4VisAttributes* fieldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicFieldBox->SetVisAttributes(fieldVisAtt);

  G4VisAttributes* detVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicSenDet1->SetVisAttributes(detVisAtt);

  G4double maxStep   = _maxStep;
  G4double maxLength = _maxLength;
  G4double maxTime   = _maxTime;
  G4double minEkin   = _minEkin;
  G4double mionRang  = _mionRang;

/*
  G4double maxStep   = 0.1*mm;
  G4double maxLength = 5.0*m;
  G4double maxTime   = 20.0*ns;
  G4double minEkin   = 1.0/100*MeV;
  G4double mionRang  = 0.01*mm;
*/
G4cout<<std::endl<<"maxStep = "<<std::setw(14)<<_maxStep<<std::endl;
G4cout<<"maxLength = "<<std::setw(14)<<_maxLength<<std::endl;
G4cout<<"maxTime = "<<std::setw(14)<<_maxTime<<std::endl;
G4cout<<"minEkin = "<<std::setw(14)<<_minEkin<<std::endl;
G4cout<<"mionRangHERE = "<<std::setw(14)<<_mionRang<<std::endl<<std::endl;

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  logicWorld->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);
  logicChamber->SetUserLimits(stepLimit);
  logicInnerBox->SetUserLimits(stepLimit);
  logicFieldBox->SetUserLimits(stepLimit);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}

G4VPhysicalVolume* DetectorConstructionP3::ConstructGeom9(){

	const bool ColimGeom = true;
	G4double gap_between_collimators = 3*cm;

  G4double ShiftingOfAdditionalBox = _Det1X; // Distance from the edge to the beginning of the collimators system

  G4double WorldSizeX         	= 	200.0*cm;
  G4double WorldSizeY         	= 	WorldSizeX;
  G4double WorldSizeZ         	= 	WorldSizeX;

  G4double beamSize		=	2*cm;
  const G4double beamSizeMax	=	2*cm;

  G4double rotAngle	     	= 	_RotationDeg;//rotation of the PHIL pipe

  G4double ThicknesOfChamber 	=	3*mm;

  G4double gapSize            	= 	1*mm;
  G4double bigGap            	= 	1*cm;

  G4double beamPipeLenght     	= 	20.0*cm;
  G4double beamPipeOutRadius  	= 	3.00*cm;
  G4double beamPipeInRadius   	= 	2.70*cm;
  G4double beamPipeVOutRadius 	= 	beamPipeInRadius - gapSize;

  G4double cupOutRadius       	= 	beamPipeOutRadius;
  G4double cupLenght          	= 	_cupLenght;

  G4double beamCorectionX	=	(beamPipeLenght/2+gapSize+cupLenght/2)*sin(rotAngle);
  G4double beamCorectionZ	=	(beamPipeLenght/2+gapSize+cupLenght/2)*(1-cos(rotAngle));

  //G4double windowRad 		= 	0.5*cm;
  G4double windowThick 		= 	20*um;
  //G4double windowThick 		= 	1*nm;

  G4double colimatorX		=	20*mm;
  G4double colimatorY		=	40*mm;
  G4double colimatorZ		=	20*mm;
  G4double colimatorZthin	=	2*mm;

  G4double chamberX		= 	400*mm;
  G4double chamberY		= 	40*mm;
  //G4double chamberZ		= 	fieldBoxZ + colimatorZ*2 + ThicknesOfChamber*2 + gapSize*10;
  G4double chamberZ		= 	200*mm;

  G4double innerBoxX         	= 	chamberX-ThicknesOfChamber*2;
  G4double innerBoxY         	= 	chamberY-ThicknesOfChamber*2;
  G4double innerBoxZ         	= 	chamberZ-ThicknesOfChamber*2;

  //Sizes of the connection from chamber to collimator system
  G4double NeckX         	= 	4*cm;
  G4double NeckY         	= 	4*cm;
  G4double NeckZ         	= 	ThicknesOfChamber*2;

  G4double fieldBoxX         	= 	chamberX-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxY         	= 	chamberY-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxZ         	= 	chamberZ-ThicknesOfChamber*2-gapSize*2 - 6*mm;

  G4double detectorRad         	= 	_Det1OutRad;
  G4double detectorThick        = 	1*mm;

  G4double AdditionalBoxX	=	colimatorX*2+beamSizeMax+gapSize*2 + ThicknesOfChamber*2;
  G4double AdditionalBoxY	=	colimatorY+beamSizeMax+gapSize*2 + ThicknesOfChamber*2 + bigGap;
  G4double AdditionalBoxZ	=	colimatorZ*2+bigGap*0.5 + 2*ThicknesOfChamber;

  G4double AdditionalInnerBoxX	=	AdditionalBoxX-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxY	=	AdditionalBoxY-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxZ	=	AdditionalBoxZ-ThicknesOfChamber;

  G4double  windowToCup 	= 	3*cm;
  G4RotationMatrix*RM1=new G4RotationMatrix(89.9*deg,-rotAngle,0);

  //distance between system of cordiantes of beampipe and chamber
  G4double fromOriginToOrigin 	=	AdditionalBoxZ+2*cm;

  G4double ShieldBarrierX       =       0.07*m;
  G4double ShieldBarrierY       =       0.4*m;
  G4double ShieldBarrierZ       =       0.4*m;

  G4double InnerTopShieldX      =       20*mm;
  G4double InnerTopShieldY      =       fieldBoxY-gapSize;
  G4double InnerTopShieldZ      =       25*mm;

  G4double InnerBotShield1X      =       InnerTopShieldX;
  G4double InnerBotShield1Y      =       InnerTopShieldY;
  G4double InnerBotShield1Z      =       110*mm;

  G4double InnerBotShield2X      =       InnerTopShieldX;
  G4double InnerBotShield2Y      =       InnerTopShieldY;
  G4double InnerBotShield2Z      =       60*mm;

  G4double InnerShield3X      =       	140*mm;
  G4double InnerShield3Y      =       	InnerTopShieldY;
  G4double InnerShield3Z      =       	InnerTopShieldX;

  G4double InnerShield4X      =  	     InnerTopShieldX;
  G4double InnerShield4Y      =    	 	 InnerTopShieldY;
  G4double InnerShield4Z      =     	 60*mm;

  G4double InnerShield5X      =  	     InnerTopShieldX;
  G4double InnerShield5Y      =    	 	 InnerTopShieldY;
  G4double InnerShield5Z      =     	 60*mm;
  //
  //	Volumes position
  //

  G4double InnerTopShield_xc    =       0*mm;
  G4double InnerTopShield_yc    =       0;
  G4double InnerTopShield_zc    =       fieldBoxZ/2-gapSize-InnerTopShieldZ/2;

  G4double InnerBotShield1_xc    =       70*mm;
  G4double InnerBotShield1_yc    =       0;
  G4double InnerBotShield1_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield1Z/2;

  G4double InnerBotShield2_xc    =       -70*mm;
  G4double InnerBotShield2_yc    =       0;
  G4double InnerBotShield2_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield2Z/2;

  G4double InnerShield3_xc	    =       InnerBotShield1_xc - InnerBotShield1X/2 - InnerShield3X/2;
  G4double InnerShield3_yc 	    =       0;
  G4double InnerShield3_zc   	=       -fieldBoxZ/2+gapSize+InnerBotShield1Z - InnerShield3Z/2;

  G4double InnerShield4_xc	    =       120*mm;
  G4double InnerShield4_yc 	    =       0;
  G4double InnerShield4_zc   	=       62*mm;

  G4double InnerShield5_xc	    =       -120*mm;
  G4double InnerShield5_yc 	    =       0;
  G4double InnerShield5_zc   	=       62*mm;

  G4double fieldBox_xc         	= 	0;
  G4double fieldBox_yc         	= 	0;
  G4double fieldBox_zc         	= 	3*mm;

  G4double innerBox_xc         	= 	0.0*cm;
  G4double innerBox_yc         	= 	0.0*cm;
  G4double innerBox_zc         	= 	0.0*cm;

  G4double beamPipe_xc          =       beamCorectionX;
  G4double beamPipe_yc          =       0;
  G4double beamPipe_zc          =       -cupLenght/2.0 - gapSize - beamPipeLenght/2.0 + beamCorectionZ;

  G4double cup_xc               =       0;
  G4double cup_yc               =       0;
  G4double cup_zc               =       0;

  G4double window_xc            =       0;
  //G4double window_yc  	=       0;
  G4double window_zc            =       -cupLenght/2.0 - gapSize- windowToCup;

  //Move to other coordinat system
  beamPipe_zc	-=	fromOriginToOrigin;
  cup_zc	-=	fromOriginToOrigin;
  window_zc	-=	fromOriginToOrigin;

  beamPipe_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  cup_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  window_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
 //Finish

  //std::cout<<std::endl<<"Coordinates of initial position of electrons must be: "<<beamPipe_xc<<" "<<beamPipe_yc<<" "<<beamPipe_zc<<std::endl<<std::endl;

  G4double chamber_xc		= 	chamberX/4;
  G4double chamber_yc		= 	0;
  G4double chamber_zc		= 	chamberZ/2;

  G4double ShieldBarrier_xc     =       0.175*m;
  G4double ShieldBarrier_yc     =       0;
  G4double ShieldBarrier_zc     =       chamber_zc-chamberZ/2-ShieldBarrierZ/2-gapSize;

  G4double Trans_x1             =       -chamberX/2+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  G4double Trans_x2             =       chamberX/2-AdditionalInnerBoxX/2-ShiftingOfAdditionalBox;

  G4double Trans_z11            =       -chamberZ/2-AdditionalBoxZ/2;
  G4double Trans_z12            =       -chamber_zc-AdditionalBoxZ/2;
  G4double Trans_z21            =       -chamberZ/2-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;
  G4double Trans_z22            =       -chamber_zc-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;

  G4ThreeVector zTransAddBox11	(Trans_x1, 0, Trans_z11);
  G4ThreeVector zTransAddBox12	(Trans_x2, 0, Trans_z12);

  G4ThreeVector zTransInnerBox21	(Trans_x1, 0, Trans_z21);
  G4ThreeVector zTransInnerBox22	(Trans_x2, 0, Trans_z22);

  G4ThreeVector zNeck1	(Trans_x1, 0, -innerBoxZ/2-NeckZ/2);
  G4ThreeVector zNeck2	(Trans_x2, 0, -innerBoxZ/2-NeckZ/2);

  G4double shiftDet             =       1*cm;
  G4double detector_xc         	= 	Trans_x2+chamber_xc;
  G4double detector_yc         	= 	0;
  G4double detector_zc         	= 	-AdditionalBoxZ - gapSize + ThicknesOfChamber*2 - shiftDet;

  beamSize = _apertureInRadius;

  G4double colimator_xc11	=	Trans_x1+colimatorX/2+beamSize/2;
  G4double colimator_yc11	=	0*mm;
  G4double colimator_zc11	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc12	=	Trans_x1-colimatorX/2-beamSize/2;
  G4double colimator_yc12	=	0*mm;
  G4double colimator_zc12	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc21	=	Trans_x1;
  G4double colimator_yc21	=	colimatorX/2+beamSize/2;
  G4double colimator_zc21	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc22	=	Trans_x1;
  G4double colimator_yc22	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc22	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;

  beamSize = _apertureLenght;

  G4double colimator_xc31	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc31	=	0*mm;
  G4double colimator_zc31	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc31thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc32	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc32	=	0*mm;
  G4double colimator_zc32	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc32thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc41	=	Trans_x2;
  G4double colimator_yc41	=	colimatorX/2+beamSize/2;
  G4double colimator_zc41	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc41thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc42	=	Trans_x2;
  G4double colimator_yc42	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc42	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc42thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc51	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc51	=	0*mm;
  G4double colimator_zc51thin	=	colimator_zc31thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc52	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc52	=	0*mm;
  G4double colimator_zc52thin	=	colimator_zc32thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc61	=	Trans_x2;
  G4double colimator_yc61	=	colimatorX/2+beamSize/2;
  G4double colimator_zc61thin	=	colimator_zc41thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc62	=	Trans_x2;
  G4double colimator_yc62	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc62thin	=	colimator_zc42thin -gap_between_collimators-ThicknesOfChamber*2;

  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox1(fieldBox_xc + chamber_xc, fieldBox_yc + chamber_yc, fieldBox_zc + chamber_zc,
			 fieldBoxX, fieldBoxY, fieldBoxZ);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();


  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);
  logicWorld = new G4LogicalVolume(solidWorld,
		  //MainDet->GetMaterial(3), air
		  MainDet->GetMaterial(5),  //myvacuum
		  "World");
  physiWorld = new G4PVPlacement(0,	 G4ThreeVector(),  logicWorld, "World", 0, false, 0);

  //
  // Beampipe
  //
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,MainDet->GetMaterial(2),"BeamPipe");
  physiBeamPipe = new G4PVPlacement(RM1, G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipe, "BeamPipe",  logicWorld,false,0);

  //
  // Beampipe Vacuum
  //
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,MainDet->GetMaterial(6),"BeamPipeV");
  physiBeamPipeV = new G4PVPlacement(RM1,G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipeV,"BeamPipeV",logicWorld, false, 0);

  //
  // Beampipe cup - absorber
  //

  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,MainDet->GetMaterial(1), "Cup");
  physiCup = new G4PVPlacement(RM1,G4ThreeVector(cup_xc,cup_yc,cup_zc),logicCup,"Cup",logicWorld,false,0);

  //
  //	Chamber
  //

  solidChamber1 = new G4Box("Chamber1",chamberX/2, chamberY/2, chamberZ/2);
  solidAdditionalBox = new G4Box("ChamberAdd",AdditionalBoxX/2, AdditionalBoxY/2, AdditionalBoxZ/2);
  G4RotationMatrix*RMZero=new G4RotationMatrix(0,0,0);
  G4Transform3D transform11(*RMZero, zTransAddBox11);
  G4Transform3D transform12(*RMZero, zTransAddBox12);
  G4UnionSolid* solidChamber2 = new G4UnionSolid("Chamber", solidChamber1, solidAdditionalBox, transform11);
  G4UnionSolid* solidChamber = new G4UnionSolid("Chamber", solidChamber2, solidAdditionalBox, transform12);

  logicChamber = new G4LogicalVolume(solidChamber,MainDet->GetMaterial(1), "Chamber");
  physiChamber = new G4PVPlacement(0, G4ThreeVector(chamber_xc,chamber_yc,chamber_zc), logicChamber, "Chamber",	 logicWorld, false, 0);

  //
  //	InnerBox
  //

  solidInnerBox = new G4Box("InnerBox",innerBoxX/2, innerBoxY/2, innerBoxZ/2);
  solidAdditionalInnerBox = new G4Box("InnerAdd",AdditionalInnerBoxX/2, AdditionalInnerBoxY/2, AdditionalInnerBoxZ/2);
  solidNeck = new G4Box("Neck",NeckX/2, NeckY/2, NeckZ/2);
  G4Transform3D transform21(*RMZero, zTransInnerBox21);
  G4Transform3D transform22(*RMZero, zTransInnerBox22);
  G4Transform3D transNeck1(*RMZero, zNeck1);
  G4Transform3D transNeck2(*RMZero, zNeck2);
  G4UnionSolid* solidInnerBoxUNeck = new G4UnionSolid("Chamber", solidInnerBox, solidNeck, transNeck1);
  G4UnionSolid* solidInnerBoxUNeck2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck, solidNeck, transNeck2);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox = new G4UnionSolid("Chamber", solidInnerBoxUNeck2, solidAdditionalInnerBox, transform21);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck2UAddBox, solidAdditionalInnerBox, transform22);

  logicInnerBox = new G4LogicalVolume(solidInnerBoxUNeck2UAddBox2,MainDet->GetMaterial(5),"InnerBox");
  physiInnerBox = new G4PVPlacement(0, G4ThreeVector(innerBox_xc,innerBox_yc,innerBox_zc), logicInnerBox, "InnerBox", logicChamber,	 false,	 0);

  //
  //	FieldBox
  //
  solidFieldBox = new G4Box("FieldBox",fieldBoxX/2, fieldBoxY/2, fieldBoxZ/2);
  logicFieldBox = new G4LogicalVolume(solidFieldBox,MainDet->GetMaterial(5), "FieldBox");
  physiFieldBox = new G4PVPlacement(0, G4ThreeVector(fieldBox_xc,fieldBox_yc,fieldBox_zc), logicFieldBox,"FieldBox", logicInnerBox,	 false,	 0);

  //
  //	Colimator 11
  //
  int ColimMat = 2;
  solidColim11 = new G4Box("Kolimator11",colimatorX/2, colimatorY/2, colimatorZ/2);
  logicColim11 = new G4LogicalVolume(solidColim11,MainDet->GetMaterial(ColimMat), "Kolimator11");
  physiColim11 = new G4PVPlacement(0,	G4ThreeVector(colimator_xc11,colimator_yc11,colimator_zc11), logicColim11,"Kolimator11", logicInnerBox,  false, 0);
  //
  //	Colimator 12
  //
  solidColim12 = new G4Box("Kolimator12",colimatorX/2, colimatorY/2, colimatorZ/2);
  logicColim12 = new G4LogicalVolume(solidColim12,MainDet->GetMaterial(ColimMat),"Kolimator12");
  physiColim12 = new G4PVPlacement(0, G4ThreeVector(colimator_xc12,colimator_yc12,colimator_zc12), logicColim12,"Kolimator12", logicInnerBox, false, 0);
  //
  //	Colimator 21
  //
  solidColim21 = new G4Box("Kolimator21",colimatorY/2, colimatorX/2, colimatorZ/2);
  logicColim21 = new G4LogicalVolume(solidColim21,MainDet->GetMaterial(ColimMat), "Kolimator21");
  physiColim21 = new G4PVPlacement(0, G4ThreeVector(colimator_xc21,colimator_yc21,colimator_zc21), logicColim21, "Kolimator21", logicInnerBox, false, 0);
  //
  //	Colimator 22
  //
  solidColim22 = new G4Box("Kolimator22",colimatorY/2, colimatorX/2, colimatorZ/2);
  logicColim22 = new G4LogicalVolume(solidColim22,MainDet->GetMaterial(ColimMat), "Kolimator12");
  physiColim22 = new G4PVPlacement(0, G4ThreeVector(colimator_xc22,colimator_yc22,colimator_zc22),  logicColim22, "Kolimator22", logicInnerBox, false, 0);

  if(ColimGeom)
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31),  logicColim11, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42), logicColim42,"Kolimator42", logicInnerBox, false,0);
  }
  /**/
  else
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31thin),  logicColim31, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32thin), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41thin), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42thin), logicColim42,"Kolimator42", logicInnerBox, false,0);

	  //
	  //	Colimator 51
	  //
	  solidColim51 = new G4Box("Kolimator51",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim51 = new G4LogicalVolume(solidColim51,MainDet->GetMaterial(ColimMat),  "Kolimator51");
	  physiColim51 = new G4PVPlacement(0, G4ThreeVector(colimator_xc51,colimator_yc51,colimator_zc51thin),  logicColim51, "Kolimat51", logicInnerBox, false, 0);
	  //
	  //	Colimator 52
	  //
	  solidColim52 = new G4Box("Kolimator52",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim52 = new G4LogicalVolume(solidColim52,MainDet->GetMaterial(ColimMat), "Kolimator52");
	  physiColim52 = new G4PVPlacement(0, G4ThreeVector(colimator_xc52,colimator_yc52,colimator_zc52thin), logicColim52, "Kolimator52",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 61
	  //
	  solidColim61 = new G4Box("Kolimator61",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim61 = new G4LogicalVolume(solidColim61,MainDet->GetMaterial(ColimMat), "Kolimator61");
	  physiColim61 = new G4PVPlacement(0, G4ThreeVector(colimator_xc61,colimator_yc61,colimator_zc61thin), logicColim61, "Kolimator61",logicInnerBox, false, 0);
	  //
	  //	Colimator 62
	  //
	  solidColim62 = new G4Box("Kolimator62",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim62 = new G4LogicalVolume(solidColim62,MainDet->GetMaterial(ColimMat), "Kolimator62");
	  physiColim62 = new G4PVPlacement(0, G4ThreeVector(colimator_xc62,colimator_yc62,colimator_zc62thin), logicColim62,"Kolimator62", logicInnerBox, false,0);
  }

  //
  //Detector
  //
  solidSenDet1 = new G4Tubs("SenDet1", 0, detectorRad, detectorThick, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     //beamVacuum,
						MainDet->GetMaterial(6),
				     "SenDet1");
  physiSenDet1 = new G4PVPlacement(0,	//rotation
				   G4ThreeVector(detector_xc,detector_yc,detector_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

  solidShieldBarrier1 = new G4Box("ShieldBarrier1",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
  logicShieldBarrier1 = new G4LogicalVolume(solidShieldBarrier1,MainDet->GetMaterial(7), "ShieldBarrier1");
  physiShieldBarrier1 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc,ShieldBarrier_yc,ShieldBarrier_zc),
		  logicShieldBarrier1, "ShieldBarrier1",logicWorld, false, 0);

  solidShieldBarrier2 = new G4Box("ShieldBarrier2",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
  logicShieldBarrier2 = new G4LogicalVolume(solidShieldBarrier2,MainDet->GetMaterial(7), "ShieldBarrier2");
  physiShieldBarrier2 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc-0.15*m,ShieldBarrier_yc,ShieldBarrier_zc),
		  logicShieldBarrier2, "ShieldBarrier2",logicWorld, false, 0);

//  solidInnerTopShield = new G4Box("InnerTopShield",InnerTopShieldX/2, InnerTopShieldY/2, InnerTopShieldZ/2);
//  logicInnerTopShield = new G4LogicalVolume(solidInnerTopShield,MainDet->GetMaterial(7), "InnerTopShield");
//  physiInnerTopShield = new G4PVPlacement(0, G4ThreeVector(InnerTopShield_xc,InnerTopShield_yc,InnerTopShield_zc),
//		  logicInnerTopShield, "InnerTopShield",logicFieldBox, false, 0);

  solidInnerBotShield1 = new G4Box("InnerBotShield1",InnerBotShield1X/2, InnerBotShield1Y/2, InnerBotShield1Z/2);
  logicInnerBotShield1 = new G4LogicalVolume(solidInnerBotShield1,MainDet->GetMaterial(7), "InnerBotShield1");
  G4RotationMatrix*RM11=new G4RotationMatrix;
  RM11->rotateY(0*deg);
  physiInnerBotShield1 = new G4PVPlacement(RM11,
		  G4ThreeVector(InnerBotShield1_xc,InnerBotShield1_yc,InnerBotShield1_zc),
		  logicInnerBotShield1, "InnerBotShield1",logicFieldBox, false, 0);

  solidInnerBotShield2 = new G4Box("InnerBotShield2",InnerBotShield2X/2, InnerBotShield2Y/2, InnerBotShield2Z/2);
  logicInnerBotShield2 = new G4LogicalVolume(solidInnerBotShield2,MainDet->GetMaterial(7), "InnerBotShield2");
  G4RotationMatrix*RM12=new G4RotationMatrix;
  RM12->rotateY(0*deg);
  physiInnerBotShield2 = new G4PVPlacement(RM12,
		  G4ThreeVector(InnerBotShield2_xc,InnerBotShield2_yc,InnerBotShield2_zc),
		  logicInnerBotShield2, "InnerBotShield2",logicFieldBox, false, 0);

//  solidInnerShield3 = new G4Box("InnerShield3",InnerShield3X/2, InnerShield3Y/2, InnerShield3Z/2);
//  logicInnerShield3 = new G4LogicalVolume(solidInnerShield3,MainDet->GetMaterial(7), "InnerShield3");
//  G4RotationMatrix*RM13=new G4RotationMatrix;
//  RM13->rotateY(0*deg);
//  physiInnerShield3 = new G4PVPlacement(RM13,
//		  G4ThreeVector(InnerShield3_xc,InnerShield3_yc,InnerShield3_zc),
//		  logicInnerShield3, "InnerShield3",logicFieldBox, false, 0);

  solidInnerShield4 = new G4Box("InnerShield4",InnerShield4X/2, InnerShield4Y/2, InnerShield4Z/2);
  logicInnerShield4 = new G4LogicalVolume(solidInnerShield4,MainDet->GetMaterial(7), "InnerShield4");
  G4RotationMatrix*RM14=new G4RotationMatrix;
  RM14->rotateY(-45*deg);
  physiInnerShield4 = new G4PVPlacement(RM14,
		  G4ThreeVector(InnerShield4_xc,InnerShield4_yc,InnerShield4_zc),
		  logicInnerShield4, "InnerShield4",logicFieldBox, false, 0);

  solidInnerShield5 = new G4Box("InnerShield5",InnerShield5X/2, InnerShield5Y/2, InnerShield5Z/2);
  logicInnerShield5 = new G4LogicalVolume(solidInnerShield5,MainDet->GetMaterial(7), "InnerShield5");
  G4RotationMatrix*RM15=new G4RotationMatrix;
  RM15->rotateY(45*deg);
  physiInnerShield5 = new G4PVPlacement(RM15,
		  G4ThreeVector(InnerShield5_xc,InnerShield5_yc,InnerShield5_zc),
		  logicInnerShield5, "InnerShield5",logicFieldBox, false, 0);

//  solidInnerBotShield2 = new G4Box("InnerBotShield2",InnerBotShield2X/2, InnerBotShield2Y/2, InnerBotShield2Z/2);
//  logicInnerBotShield2 = new G4LogicalVolume(solidInnerBotShield2,MainDet->GetMaterial(7), "InnerBotShield2");
//  G4RotationMatrix*RM12=new G4RotationMatrix;
//  RM11->rotateY(0*deg);
//  physiInnerBotShield2 = new G4PVPlacement(RM12,
//		  G4ThreeVector(InnerBotShield2_xc,InnerBotShield2_yc,InnerBotShield2_zc),
//		  logicInnerBotShield2, "InnerBotShield2",logicFieldBox, false, 0);

  ConstructMagnet(520*mm, 50*mm, chamberZ, gapSize);
  //
  // Visualization attributes
  //


  G4VisAttributes* pipeVisAtt = new G4VisAttributes(G4Colour(0.5,0.0,0.6));
  logicBeamPipe->SetVisAttributes(pipeVisAtt);

  G4VisAttributes* pipeVVisAtt = new G4VisAttributes(G4Colour(0.4,0.3,0.0));
  logicBeamPipeV->SetVisAttributes(pipeVVisAtt);

  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);

  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicChamber->SetVisAttributes(chamberVisAtt);

  G4VisAttributes* innerVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicInnerBox->SetVisAttributes(innerVisAtt);

  G4VisAttributes* fieldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicFieldBox->SetVisAttributes(fieldVisAtt);

  G4VisAttributes* detVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicSenDet1->SetVisAttributes(detVisAtt);

  G4double maxStep   = _maxStep;
  G4double maxLength = _maxLength;
  G4double maxTime   = _maxTime;
  G4double minEkin   = _minEkin;
  G4double mionRang  = _mionRang;

/*
  G4double maxStep   = 0.1*mm;
  G4double maxLength = 5.0*m;
  G4double maxTime   = 20.0*ns;
  G4double minEkin   = 1.0/100*MeV;
  G4double mionRang  = 0.01*mm;
*/
G4cout<<std::endl<<"maxStep = "<<std::setw(14)<<_maxStep<<std::endl;
G4cout<<"maxLength = "<<std::setw(14)<<_maxLength<<std::endl;
G4cout<<"maxTime = "<<std::setw(14)<<_maxTime<<std::endl;
G4cout<<"minEkin = "<<std::setw(14)<<_minEkin<<std::endl;
G4cout<<"mionRangHERE = "<<std::setw(14)<<_mionRang<<std::endl<<std::endl;

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  logicWorld->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);
  logicChamber->SetUserLimits(stepLimit);
  logicInnerBox->SetUserLimits(stepLimit);
  logicFieldBox->SetUserLimits(stepLimit);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}

G4VPhysicalVolume* DetectorConstructionP3::ConstructGeom10(){

	const bool ColimGeom = true;
	G4double gap_between_collimators = 3*cm;

  G4double ShiftingOfAdditionalBox = _Det1X; // Distance from the edge to the beginning of the collimators system

  G4double WorldSizeX         	= 	200.0*cm;
  G4double WorldSizeY         	= 	WorldSizeX;
  G4double WorldSizeZ         	= 	WorldSizeX;

  G4double beamSize		=	2*cm;
  const G4double beamSizeMax	=	2*cm;

  G4double rotAngle	     	= 	_RotationDeg;//rotation of the PHIL pipe

  G4double ThicknesOfChamber 	=	3*mm;

  G4double gapSize            	= 	1*mm;
  G4double bigGap            	= 	1*cm;

  G4double beamPipeLenght     	= 	20.0*cm;
  G4double beamPipeOutRadius  	= 	3.00*cm;
  G4double beamPipeInRadius   	= 	2.70*cm;
  G4double beamPipeVOutRadius 	= 	beamPipeInRadius - gapSize;

  G4double cupOutRadius       	= 	beamPipeOutRadius;
  G4double cupLenght          	= 	_cupLenght;

  G4double beamCorectionX	=	(beamPipeLenght/2+gapSize+cupLenght/2)*sin(rotAngle);
  G4double beamCorectionZ	=	(beamPipeLenght/2+gapSize+cupLenght/2)*(1-cos(rotAngle));

  //G4double windowRad 		= 	0.5*cm;
  G4double windowThick 		= 	20*um;
  //G4double windowThick 		= 	1*nm;

  G4double colimatorX		=	20*mm;
  G4double colimatorY		=	40*mm;
  G4double colimatorZ		=	20*mm;
  G4double colimatorZthin	=	2*mm;

  G4double chamberX		= 	400*mm;
  G4double chamberY		= 	40*mm;
  //G4double chamberZ		= 	fieldBoxZ + colimatorZ*2 + ThicknesOfChamber*2 + gapSize*10;
  G4double chamberZ		= 	200*mm;

  G4double innerBoxX         	= 	chamberX-ThicknesOfChamber*2;
  G4double innerBoxY         	= 	chamberY-ThicknesOfChamber*2;
  G4double innerBoxZ         	= 	chamberZ-ThicknesOfChamber*2;

  //Sizes of the connection from chamber to collimator system
  G4double NeckX         	= 	4*cm;
  G4double NeckY         	= 	4*cm;
  G4double NeckZ         	= 	ThicknesOfChamber*2;

  G4double fieldBoxX         	= 	chamberX-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxY         	= 	chamberY-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxZ         	= 	chamberZ-ThicknesOfChamber*2-gapSize*2 - 6*mm;

  G4double detectorRad         	= 	_Det1OutRad;
  G4double detectorThick        = 	1*mm;

  G4double AdditionalBoxX	=	colimatorX*2+beamSizeMax+gapSize*2 + ThicknesOfChamber*2;
  G4double AdditionalBoxY	=	colimatorY+beamSizeMax+gapSize*2 + ThicknesOfChamber*2 + bigGap;
  G4double AdditionalBoxZ	=	colimatorZ*2+bigGap*0.5 + 2*ThicknesOfChamber;

  G4double AdditionalInnerBoxX	=	AdditionalBoxX-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxY	=	AdditionalBoxY-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxZ	=	AdditionalBoxZ-ThicknesOfChamber;

  G4double  windowToCup 	= 	3*cm;
  G4RotationMatrix*RM1=new G4RotationMatrix(89.9*deg,-rotAngle,0);

  //distance between system of cordiantes of beampipe and chamber
  G4double fromOriginToOrigin 	=	AdditionalBoxZ+2*cm;

  G4double ShieldBarrierX       =       0.07*m;
  G4double ShieldBarrierY       =       0.4*m;
  G4double ShieldBarrierZ       =       0.4*m;

  G4double InnerTopShieldX      =       20*mm;
  G4double InnerTopShieldY      =       fieldBoxY-gapSize;
  G4double InnerTopShieldZ      =       25*mm;

  G4double InnerBotShield1X      =       InnerTopShieldX;
  G4double InnerBotShield1Y      =       InnerTopShieldY;
  G4double InnerBotShield1Z      =       110*mm;

  G4double InnerBotShield2X      =       InnerTopShieldX;
  G4double InnerBotShield2Y      =       InnerTopShieldY;
  G4double InnerBotShield2Z      =       60*mm;

  G4double InnerShield3X      =       	140*mm;
  G4double InnerShield3Y      =       	InnerTopShieldY;
  G4double InnerShield3Z      =       	InnerTopShieldX;

  G4double InnerShield4X      =  	     InnerTopShieldX;
  G4double InnerShield4Y      =    	 	 InnerTopShieldY;
  G4double InnerShield4Z      =     	 60*mm;

  G4double InnerShield5X      =  	     InnerTopShieldX;
  G4double InnerShield5Y      =    	 	 InnerTopShieldY;
  G4double InnerShield5Z      =     	 60*mm;
  //
  //	Volumes position
  //

  G4double InnerTopShield_xc    =       0*mm;
  G4double InnerTopShield_yc    =       0;
  G4double InnerTopShield_zc    =       fieldBoxZ/2-gapSize-InnerTopShieldZ/2;

  G4double InnerBotShield1_xc    =       70*mm;
  G4double InnerBotShield1_yc    =       0;
  G4double InnerBotShield1_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield1Z/2;

  G4double InnerBotShield2_xc    =       -70*mm;
  G4double InnerBotShield2_yc    =       0;
  G4double InnerBotShield2_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield2Z/2;

  G4double InnerShield3_xc	    =       InnerBotShield1_xc - InnerBotShield1X/2 - InnerShield3X/2;
  G4double InnerShield3_yc 	    =       0;
  G4double InnerShield3_zc   	=       -fieldBoxZ/2+gapSize+InnerBotShield1Z - InnerShield3Z/2;

  G4double InnerShield4_xc	    =       120*mm;
  G4double InnerShield4_yc 	    =       0;
  G4double InnerShield4_zc   	=       62*mm;

  G4double InnerShield5_xc	    =       -120*mm;
  G4double InnerShield5_yc 	    =       0;
  G4double InnerShield5_zc   	=       62*mm;

  G4double fieldBox_xc         	= 	0;
  G4double fieldBox_yc         	= 	0;
  G4double fieldBox_zc         	= 	3*mm;

  G4double innerBox_xc         	= 	0.0*cm;
  G4double innerBox_yc         	= 	0.0*cm;
  G4double innerBox_zc         	= 	0.0*cm;

  G4double beamPipe_xc          =       beamCorectionX;
  G4double beamPipe_yc          =       0;
  G4double beamPipe_zc          =       -cupLenght/2.0 - gapSize - beamPipeLenght/2.0 + beamCorectionZ;

  G4double cup_xc               =       0;
  G4double cup_yc               =       0;
  G4double cup_zc               =       0;

  G4double window_xc            =       0;
  //G4double window_yc  	=       0;
  G4double window_zc            =       -cupLenght/2.0 - gapSize- windowToCup;

  //Move to other coordinat system
  beamPipe_zc	-=	fromOriginToOrigin;
  cup_zc	-=	fromOriginToOrigin;
  window_zc	-=	fromOriginToOrigin;

  beamPipe_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  cup_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  window_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
 //Finish

  //std::cout<<std::endl<<"Coordinates of initial position of electrons must be: "<<beamPipe_xc<<" "<<beamPipe_yc<<" "<<beamPipe_zc<<std::endl<<std::endl;

  G4double chamber_xc		= 	chamberX/4;
  G4double chamber_yc		= 	0;
  G4double chamber_zc		= 	chamberZ/2;

  G4double ShieldBarrier_xc     =       0.175*m;
  G4double ShieldBarrier_yc     =       0;
  G4double ShieldBarrier_zc     =       chamber_zc-chamberZ/2-ShieldBarrierZ/2-gapSize;

  G4double Trans_x1             =       -chamberX/2+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  G4double Trans_x2             =       chamberX/2-AdditionalInnerBoxX/2-ShiftingOfAdditionalBox;

  G4double Trans_z11            =       -chamberZ/2-AdditionalBoxZ/2;
  G4double Trans_z12            =       -chamber_zc-AdditionalBoxZ/2;
  G4double Trans_z21            =       -chamberZ/2-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;
  G4double Trans_z22            =       -chamber_zc-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;

  G4ThreeVector zTransAddBox11	(Trans_x1, 0, Trans_z11);
  G4ThreeVector zTransAddBox12	(Trans_x2, 0, Trans_z12);

  G4ThreeVector zTransInnerBox21	(Trans_x1, 0, Trans_z21);
  G4ThreeVector zTransInnerBox22	(Trans_x2, 0, Trans_z22);

  G4ThreeVector zNeck1	(Trans_x1, 0, -innerBoxZ/2-NeckZ/2);
  G4ThreeVector zNeck2	(Trans_x2, 0, -innerBoxZ/2-NeckZ/2);

  G4double shiftDet             =       1*cm;
  G4double detector_xc         	= 	Trans_x2+chamber_xc;
  G4double detector_yc         	= 	0;
  G4double detector_zc         	= 	-AdditionalBoxZ - gapSize + ThicknesOfChamber*2 - shiftDet;

  beamSize = _apertureInRadius;

  G4double colimator_xc11	=	Trans_x1+colimatorX/2+beamSize/2;
  G4double colimator_yc11	=	0*mm;
  G4double colimator_zc11	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc12	=	Trans_x1-colimatorX/2-beamSize/2;
  G4double colimator_yc12	=	0*mm;
  G4double colimator_zc12	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc21	=	Trans_x1;
  G4double colimator_yc21	=	colimatorX/2+beamSize/2;
  G4double colimator_zc21	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc22	=	Trans_x1;
  G4double colimator_yc22	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc22	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;

  beamSize = _apertureLenght;

  G4double colimator_xc31	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc31	=	0*mm;
  G4double colimator_zc31	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc31thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc32	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc32	=	0*mm;
  G4double colimator_zc32	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc32thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc41	=	Trans_x2;
  G4double colimator_yc41	=	colimatorX/2+beamSize/2;
  G4double colimator_zc41	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc41thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc42	=	Trans_x2;
  G4double colimator_yc42	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc42	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc42thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc51	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc51	=	0*mm;
  G4double colimator_zc51thin	=	colimator_zc31thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc52	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc52	=	0*mm;
  G4double colimator_zc52thin	=	colimator_zc32thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc61	=	Trans_x2;
  G4double colimator_yc61	=	colimatorX/2+beamSize/2;
  G4double colimator_zc61thin	=	colimator_zc41thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc62	=	Trans_x2;
  G4double colimator_yc62	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc62thin	=	colimator_zc42thin -gap_between_collimators-ThicknesOfChamber*2;

  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox1(fieldBox_xc + chamber_xc, fieldBox_yc + chamber_yc, fieldBox_zc + chamber_zc,
			 fieldBoxX, fieldBoxY, fieldBoxZ);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();


  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);
  logicWorld = new G4LogicalVolume(solidWorld,
		  //MainDet->GetMaterial(3), air
		  MainDet->GetMaterial(5),  //myvacuum
		  "World");
  physiWorld = new G4PVPlacement(0,	 G4ThreeVector(),  logicWorld, "World", 0, false, 0);

  //
  // Beampipe
  //
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,MainDet->GetMaterial(2),"BeamPipe");
  physiBeamPipe = new G4PVPlacement(RM1, G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipe, "BeamPipe",  logicWorld,false,0);

  //
  // Beampipe Vacuum
  //
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,MainDet->GetMaterial(6),"BeamPipeV");
  physiBeamPipeV = new G4PVPlacement(RM1,G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipeV,"BeamPipeV",logicWorld, false, 0);

  //
  // Beampipe cup - absorber
  //

  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,MainDet->GetMaterial(1), "Cup");
  physiCup = new G4PVPlacement(RM1,G4ThreeVector(cup_xc,cup_yc,cup_zc),logicCup,"Cup",logicWorld,false,0);

  //
  //	Chamber
  //

  solidChamber1 = new G4Box("Chamber1",chamberX/2, chamberY/2, chamberZ/2);
  solidAdditionalBox = new G4Box("ChamberAdd",AdditionalBoxX/2, AdditionalBoxY/2, AdditionalBoxZ/2);
  G4RotationMatrix*RMZero=new G4RotationMatrix(0,0,0);
  G4Transform3D transform11(*RMZero, zTransAddBox11);
  G4Transform3D transform12(*RMZero, zTransAddBox12);
  G4UnionSolid* solidChamber2 = new G4UnionSolid("Chamber", solidChamber1, solidAdditionalBox, transform11);
  G4UnionSolid* solidChamber = new G4UnionSolid("Chamber", solidChamber2, solidAdditionalBox, transform12);

  logicChamber = new G4LogicalVolume(solidChamber,MainDet->GetMaterial(1), "Chamber");
  physiChamber = new G4PVPlacement(0, G4ThreeVector(chamber_xc,chamber_yc,chamber_zc), logicChamber, "Chamber",	 logicWorld, false, 0);

  //
  //	InnerBox
  //

  solidInnerBox = new G4Box("InnerBox",innerBoxX/2, innerBoxY/2, innerBoxZ/2);
  solidAdditionalInnerBox = new G4Box("InnerAdd",AdditionalInnerBoxX/2, AdditionalInnerBoxY/2, AdditionalInnerBoxZ/2);
  solidNeck = new G4Box("Neck",NeckX/2, NeckY/2, NeckZ/2);
  G4Transform3D transform21(*RMZero, zTransInnerBox21);
  G4Transform3D transform22(*RMZero, zTransInnerBox22);
  G4Transform3D transNeck1(*RMZero, zNeck1);
  G4Transform3D transNeck2(*RMZero, zNeck2);
  G4UnionSolid* solidInnerBoxUNeck = new G4UnionSolid("Chamber", solidInnerBox, solidNeck, transNeck1);
  G4UnionSolid* solidInnerBoxUNeck2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck, solidNeck, transNeck2);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox = new G4UnionSolid("Chamber", solidInnerBoxUNeck2, solidAdditionalInnerBox, transform21);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck2UAddBox, solidAdditionalInnerBox, transform22);

  logicInnerBox = new G4LogicalVolume(solidInnerBoxUNeck2UAddBox2,MainDet->GetMaterial(5),"InnerBox");
  physiInnerBox = new G4PVPlacement(0, G4ThreeVector(innerBox_xc,innerBox_yc,innerBox_zc), logicInnerBox, "InnerBox", logicChamber,	 false,	 0);

  //
  //	FieldBox
  //
  solidFieldBox = new G4Box("FieldBox",fieldBoxX/2, fieldBoxY/2, fieldBoxZ/2);
  logicFieldBox = new G4LogicalVolume(solidFieldBox,MainDet->GetMaterial(5), "FieldBox");
  physiFieldBox = new G4PVPlacement(0, G4ThreeVector(fieldBox_xc,fieldBox_yc,fieldBox_zc), logicFieldBox,"FieldBox", logicInnerBox,	 false,	 0);

  //
  //	Colimator 11
  //
  int ColimMat = 2;
  solidColim11 = new G4Box("Kolimator11",colimatorX/2, colimatorY/2, colimatorZ/2);
  logicColim11 = new G4LogicalVolume(solidColim11,MainDet->GetMaterial(ColimMat), "Kolimator11");
  physiColim11 = new G4PVPlacement(0,	G4ThreeVector(colimator_xc11,colimator_yc11,colimator_zc11), logicColim11,"Kolimator11", logicInnerBox,  false, 0);
  //
  //	Colimator 12
  //
  solidColim12 = new G4Box("Kolimator12",colimatorX/2, colimatorY/2, colimatorZ/2);
  logicColim12 = new G4LogicalVolume(solidColim12,MainDet->GetMaterial(ColimMat),"Kolimator12");
  physiColim12 = new G4PVPlacement(0, G4ThreeVector(colimator_xc12,colimator_yc12,colimator_zc12), logicColim12,"Kolimator12", logicInnerBox, false, 0);
  //
  //	Colimator 21
  //
  solidColim21 = new G4Box("Kolimator21",colimatorY/2, colimatorX/2, colimatorZ/2);
  logicColim21 = new G4LogicalVolume(solidColim21,MainDet->GetMaterial(ColimMat), "Kolimator21");
  physiColim21 = new G4PVPlacement(0, G4ThreeVector(colimator_xc21,colimator_yc21,colimator_zc21), logicColim21, "Kolimator21", logicInnerBox, false, 0);
  //
  //	Colimator 22
  //
  solidColim22 = new G4Box("Kolimator22",colimatorY/2, colimatorX/2, colimatorZ/2);
  logicColim22 = new G4LogicalVolume(solidColim22,MainDet->GetMaterial(ColimMat), "Kolimator12");
  physiColim22 = new G4PVPlacement(0, G4ThreeVector(colimator_xc22,colimator_yc22,colimator_zc22),  logicColim22, "Kolimator22", logicInnerBox, false, 0);

  if(ColimGeom)
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31),  logicColim11, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42), logicColim42,"Kolimator42", logicInnerBox, false,0);
  }
  /**/
  else
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31thin),  logicColim31, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32thin), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41thin), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42thin), logicColim42,"Kolimator42", logicInnerBox, false,0);

	  //
	  //	Colimator 51
	  //
	  solidColim51 = new G4Box("Kolimator51",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim51 = new G4LogicalVolume(solidColim51,MainDet->GetMaterial(ColimMat),  "Kolimator51");
	  physiColim51 = new G4PVPlacement(0, G4ThreeVector(colimator_xc51,colimator_yc51,colimator_zc51thin),  logicColim51, "Kolimat51", logicInnerBox, false, 0);
	  //
	  //	Colimator 52
	  //
	  solidColim52 = new G4Box("Kolimator52",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim52 = new G4LogicalVolume(solidColim52,MainDet->GetMaterial(ColimMat), "Kolimator52");
	  physiColim52 = new G4PVPlacement(0, G4ThreeVector(colimator_xc52,colimator_yc52,colimator_zc52thin), logicColim52, "Kolimator52",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 61
	  //
	  solidColim61 = new G4Box("Kolimator61",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim61 = new G4LogicalVolume(solidColim61,MainDet->GetMaterial(ColimMat), "Kolimator61");
	  physiColim61 = new G4PVPlacement(0, G4ThreeVector(colimator_xc61,colimator_yc61,colimator_zc61thin), logicColim61, "Kolimator61",logicInnerBox, false, 0);
	  //
	  //	Colimator 62
	  //
	  solidColim62 = new G4Box("Kolimator62",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim62 = new G4LogicalVolume(solidColim62,MainDet->GetMaterial(ColimMat), "Kolimator62");
	  physiColim62 = new G4PVPlacement(0, G4ThreeVector(colimator_xc62,colimator_yc62,colimator_zc62thin), logicColim62,"Kolimator62", logicInnerBox, false,0);
  }

  //
  //Detector
  //
  solidSenDet1 = new G4Tubs("SenDet1", 0, detectorRad, detectorThick, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     //beamVacuum,
						MainDet->GetMaterial(6),
				     "SenDet1");
  physiSenDet1 = new G4PVPlacement(0,	//rotation
				   G4ThreeVector(detector_xc,detector_yc,detector_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

  solidShieldBarrier1 = new G4Box("ShieldBarrier1",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
  logicShieldBarrier1 = new G4LogicalVolume(solidShieldBarrier1,MainDet->GetMaterial(7), "ShieldBarrier1");
  physiShieldBarrier1 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc,ShieldBarrier_yc,ShieldBarrier_zc),
		  logicShieldBarrier1, "ShieldBarrier1",logicWorld, false, 0);

  solidShieldBarrier2 = new G4Box("ShieldBarrier2",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
  logicShieldBarrier2 = new G4LogicalVolume(solidShieldBarrier2,MainDet->GetMaterial(7), "ShieldBarrier2");
  physiShieldBarrier2 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc-0.15*m,ShieldBarrier_yc,ShieldBarrier_zc),
		  logicShieldBarrier2, "ShieldBarrier2",logicWorld, false, 0);

//  solidInnerTopShield = new G4Box("InnerTopShield",InnerTopShieldX/2, InnerTopShieldY/2, InnerTopShieldZ/2);
//  logicInnerTopShield = new G4LogicalVolume(solidInnerTopShield,MainDet->GetMaterial(7), "InnerTopShield");
//  physiInnerTopShield = new G4PVPlacement(0, G4ThreeVector(InnerTopShield_xc,InnerTopShield_yc,InnerTopShield_zc),
//		  logicInnerTopShield, "InnerTopShield",logicFieldBox, false, 0);

  solidInnerBotShield1 = new G4Box("InnerBotShield1",InnerBotShield1X/2, InnerBotShield1Y/2, InnerBotShield1Z/2);
  logicInnerBotShield1 = new G4LogicalVolume(solidInnerBotShield1,MainDet->GetMaterial(7), "InnerBotShield1");
  G4RotationMatrix*RM11=new G4RotationMatrix;
  RM11->rotateY(0*deg);
  physiInnerBotShield1 = new G4PVPlacement(RM11,
		  G4ThreeVector(InnerBotShield1_xc,InnerBotShield1_yc,InnerBotShield1_zc),
		  logicInnerBotShield1, "InnerBotShield1",logicFieldBox, false, 0);

  solidInnerBotShield2 = new G4Box("InnerBotShield2",InnerBotShield2X/2, InnerBotShield2Y/2, InnerBotShield2Z/2);
  logicInnerBotShield2 = new G4LogicalVolume(solidInnerBotShield2,MainDet->GetMaterial(7), "InnerBotShield2");
  G4RotationMatrix*RM12=new G4RotationMatrix;
  RM12->rotateY(0*deg);
  physiInnerBotShield2 = new G4PVPlacement(RM12,
		  G4ThreeVector(InnerBotShield2_xc,InnerBotShield2_yc,InnerBotShield2_zc),
		  logicInnerBotShield2, "InnerBotShield2",logicFieldBox, false, 0);

  solidInnerShield3 = new G4Box("InnerShield3",InnerShield3X/2, InnerShield3Y/2, InnerShield3Z/2);
  logicInnerShield3 = new G4LogicalVolume(solidInnerShield3,MainDet->GetMaterial(7), "InnerShield3");
  G4RotationMatrix*RM13=new G4RotationMatrix;
  RM13->rotateY(0*deg);
  physiInnerShield3 = new G4PVPlacement(RM13,
		  G4ThreeVector(InnerShield3_xc,InnerShield3_yc,InnerShield3_zc),
		  logicInnerShield3, "InnerShield3",logicFieldBox, false, 0);

  solidInnerShield4 = new G4Box("InnerShield4",InnerShield4X/2, InnerShield4Y/2, InnerShield4Z/2);
  logicInnerShield4 = new G4LogicalVolume(solidInnerShield4,MainDet->GetMaterial(7), "InnerShield4");
  G4RotationMatrix*RM14=new G4RotationMatrix;
  RM14->rotateY(-45*deg);
  physiInnerShield4 = new G4PVPlacement(RM14,
		  G4ThreeVector(InnerShield4_xc,InnerShield4_yc,InnerShield4_zc),
		  logicInnerShield4, "InnerShield4",logicFieldBox, false, 0);

//  solidInnerShield5 = new G4Box("InnerShield5",InnerShield5X/2, InnerShield5Y/2, InnerShield5Z/2);
//  logicInnerShield5 = new G4LogicalVolume(solidInnerShield5,MainDet->GetMaterial(7), "InnerShield5");
//  G4RotationMatrix*RM15=new G4RotationMatrix;
//  RM15->rotateY(45*deg);
//  physiInnerShield5 = new G4PVPlacement(RM15,
//		  G4ThreeVector(InnerShield5_xc,InnerShield5_yc,InnerShield5_zc),
//		  logicInnerShield5, "InnerShield5",logicFieldBox, false, 0);

  ConstructMagnet(520*mm, 50*mm, chamberZ, gapSize);
  //
  // Visualization attributes
  //


  G4VisAttributes* pipeVisAtt = new G4VisAttributes(G4Colour(0.5,0.0,0.6));
  logicBeamPipe->SetVisAttributes(pipeVisAtt);

  G4VisAttributes* pipeVVisAtt = new G4VisAttributes(G4Colour(0.4,0.3,0.0));
  logicBeamPipeV->SetVisAttributes(pipeVVisAtt);

  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);

  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicChamber->SetVisAttributes(chamberVisAtt);

  G4VisAttributes* innerVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicInnerBox->SetVisAttributes(innerVisAtt);

  G4VisAttributes* fieldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicFieldBox->SetVisAttributes(fieldVisAtt);

  G4VisAttributes* detVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicSenDet1->SetVisAttributes(detVisAtt);

  G4double maxStep   = _maxStep;
  G4double maxLength = _maxLength;
  G4double maxTime   = _maxTime;
  G4double minEkin   = _minEkin;
  G4double mionRang  = _mionRang;

/*
  G4double maxStep   = 0.1*mm;
  G4double maxLength = 5.0*m;
  G4double maxTime   = 20.0*ns;
  G4double minEkin   = 1.0/100*MeV;
  G4double mionRang  = 0.01*mm;
*/
G4cout<<std::endl<<"maxStep = "<<std::setw(14)<<_maxStep<<std::endl;
G4cout<<"maxLength = "<<std::setw(14)<<_maxLength<<std::endl;
G4cout<<"maxTime = "<<std::setw(14)<<_maxTime<<std::endl;
G4cout<<"minEkin = "<<std::setw(14)<<_minEkin<<std::endl;
G4cout<<"mionRangHERE = "<<std::setw(14)<<_mionRang<<std::endl<<std::endl;

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  logicWorld->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);
  logicChamber->SetUserLimits(stepLimit);
  logicInnerBox->SetUserLimits(stepLimit);
  logicFieldBox->SetUserLimits(stepLimit);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}

G4VPhysicalVolume* DetectorConstructionP3::ConstructGeom11(){

	const bool ColimGeom = true;
	G4double gap_between_collimators = 3*cm;

  G4double ShiftingOfAdditionalBox = _Det1X; // Distance from the edge to the beginning of the collimators system

  G4double WorldSizeX         	= 	200.0*cm;
  G4double WorldSizeY         	= 	WorldSizeX;
  G4double WorldSizeZ         	= 	WorldSizeX;

  G4double beamSize		=	2*cm;
  const G4double beamSizeMax	=	2*cm;

  G4double rotAngle	     	= 	_RotationDeg;//rotation of the PHIL pipe

  G4double ThicknesOfChamber 	=	3*mm;

  G4double gapSize            	= 	1*mm;
  G4double bigGap            	= 	1*cm;

  G4double beamPipeLenght     	= 	20.0*cm;
  G4double beamPipeOutRadius  	= 	3.00*cm;
  G4double beamPipeInRadius   	= 	2.70*cm;
  G4double beamPipeVOutRadius 	= 	beamPipeInRadius - gapSize;

  G4double cupOutRadius       	= 	beamPipeOutRadius;
  G4double cupLenght          	= 	_cupLenght;

  G4double beamCorectionX	=	(beamPipeLenght/2+gapSize+cupLenght/2)*sin(rotAngle);
  G4double beamCorectionZ	=	(beamPipeLenght/2+gapSize+cupLenght/2)*(1-cos(rotAngle));

  //G4double windowRad 		= 	0.5*cm;
  G4double windowThick 		= 	20*um;
  //G4double windowThick 		= 	1*nm;

  G4double colimatorX		=	20*mm;
  G4double colimatorY		=	40*mm;
  G4double colimatorZ		=	20*mm;
  G4double colimatorZthin	=	2*mm;

  G4double chamberX		= 	400*mm;
  G4double chamberY		= 	40*mm;
  //G4double chamberZ		= 	fieldBoxZ + colimatorZ*2 + ThicknesOfChamber*2 + gapSize*10;
  G4double chamberZ		= 	200*mm;

  G4double innerBoxX         	= 	chamberX-ThicknesOfChamber*2;
  G4double innerBoxY         	= 	chamberY-ThicknesOfChamber*2;
  G4double innerBoxZ         	= 	chamberZ-ThicknesOfChamber*2;

  //Sizes of the connection from chamber to collimator system
  G4double NeckX         	= 	4*cm;
  G4double NeckY         	= 	4*cm;
  G4double NeckZ         	= 	ThicknesOfChamber*2;

  G4double fieldBoxX         	= 	chamberX-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxY         	= 	chamberY-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxZ         	= 	chamberZ-ThicknesOfChamber*2-gapSize*2 - 6*mm;

  G4double detectorRad         	= 	_Det1OutRad;
  G4double detectorThick        = 	1*mm;

  G4double AdditionalBoxX	=	colimatorX*2+beamSizeMax+gapSize*2 + ThicknesOfChamber*2;
  G4double AdditionalBoxY	=	colimatorY+beamSizeMax+gapSize*2 + ThicknesOfChamber*2 + bigGap;
  G4double AdditionalBoxZ	=	colimatorZ*2+bigGap*0.5 + 2*ThicknesOfChamber;

  G4double AdditionalInnerBoxX	=	AdditionalBoxX-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxY	=	AdditionalBoxY-ThicknesOfChamber*2;
  G4double AdditionalInnerBoxZ	=	AdditionalBoxZ-ThicknesOfChamber;

  G4double  windowToCup 	= 	3*cm;
  G4RotationMatrix*RM1=new G4RotationMatrix(89.9*deg,-rotAngle,0);

  //distance between system of cordiantes of beampipe and chamber
  G4double fromOriginToOrigin 	=	AdditionalBoxZ+2*cm;

  G4double ShieldBarrierX       =       0.07*m;
  G4double ShieldBarrierY       =       0.4*m;
  G4double ShieldBarrierZ       =       0.4*m;

  G4double InnerTopShieldX      =       20*mm;
  G4double InnerTopShieldY      =       fieldBoxY-gapSize;
  G4double InnerTopShieldZ      =       25*mm;

  G4double InnerBotShield1X      =       InnerTopShieldX;
  G4double InnerBotShield1Y      =       InnerTopShieldY;
  G4double InnerBotShield1Z      =       110*mm;

  G4double InnerBotShield2X      =       InnerTopShieldX;
  G4double InnerBotShield2Y      =       InnerTopShieldY;
  G4double InnerBotShield2Z      =       60*mm;

  G4double InnerShield3X      =       	140*mm;
  G4double InnerShield3Y      =       	InnerTopShieldY;
  G4double InnerShield3Z      =       	InnerTopShieldX;

  G4double InnerShield4X      =  	     InnerTopShieldX;
  G4double InnerShield4Y      =    	 	 InnerTopShieldY;
  G4double InnerShield4Z      =     	 60*mm;

  G4double InnerShield5X      =  	     InnerTopShieldX;
  G4double InnerShield5Y      =    	 	 InnerTopShieldY;
  G4double InnerShield5Z      =     	 60*mm;
  //
  //	Volumes position
  //

  G4double InnerTopShield_xc    =       0*mm;
  G4double InnerTopShield_yc    =       0;
  G4double InnerTopShield_zc    =       fieldBoxZ/2-gapSize-InnerTopShieldZ/2;

  G4double InnerBotShield1_xc    =       70*mm;
  G4double InnerBotShield1_yc    =       0;
  G4double InnerBotShield1_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield1Z/2;

  G4double InnerBotShield2_xc    =       -70*mm;
  G4double InnerBotShield2_yc    =       0;
  G4double InnerBotShield2_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield2Z/2;

  G4double InnerShield3_xc	    =       InnerBotShield1_xc - InnerBotShield1X/2 - InnerShield3X/2;
  G4double InnerShield3_yc 	    =       0;
  G4double InnerShield3_zc   	=       -fieldBoxZ/2+gapSize+InnerBotShield1Z - InnerShield3Z/2;

  G4double InnerShield4_xc	    =       120*mm;
  G4double InnerShield4_yc 	    =       0;
  G4double InnerShield4_zc   	=       62*mm;

  G4double InnerShield5_xc	    =       -120*mm;
  G4double InnerShield5_yc 	    =       0;
  G4double InnerShield5_zc   	=       62*mm;

  G4double fieldBox_xc         	= 	0;
  G4double fieldBox_yc         	= 	0;
  G4double fieldBox_zc         	= 	3*mm;

  G4double innerBox_xc         	= 	0.0*cm;
  G4double innerBox_yc         	= 	0.0*cm;
  G4double innerBox_zc         	= 	0.0*cm;

  G4double beamPipe_xc          =       beamCorectionX;
  G4double beamPipe_yc          =       0;
  G4double beamPipe_zc          =       -cupLenght/2.0 - gapSize - beamPipeLenght/2.0 + beamCorectionZ;

  G4double cup_xc               =       0;
  G4double cup_yc               =       0;
  G4double cup_zc               =       0;

  G4double window_xc            =       0;
  //G4double window_yc  	=       0;
  G4double window_zc            =       -cupLenght/2.0 - gapSize- windowToCup;

  //Move to other coordinat system
  beamPipe_zc	-=	fromOriginToOrigin;
  cup_zc	-=	fromOriginToOrigin;
  window_zc	-=	fromOriginToOrigin;

  beamPipe_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  cup_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  window_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
 //Finish

  //std::cout<<std::endl<<"Coordinates of initial position of electrons must be: "<<beamPipe_xc<<" "<<beamPipe_yc<<" "<<beamPipe_zc<<std::endl<<std::endl;

  G4double chamber_xc		= 	chamberX/4;
  G4double chamber_yc		= 	0;
  G4double chamber_zc		= 	chamberZ/2;

  G4double ShieldBarrier_xc     =       0.175*m;
  G4double ShieldBarrier_yc     =       0;
  G4double ShieldBarrier_zc     =       chamber_zc-chamberZ/2-ShieldBarrierZ/2-gapSize;

  G4double Trans_x1             =       -chamberX/2+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
  G4double Trans_x2             =       chamberX/2-AdditionalInnerBoxX/2-ShiftingOfAdditionalBox;

  G4double Trans_z11            =       -chamberZ/2-AdditionalBoxZ/2;
  G4double Trans_z12            =       -chamber_zc-AdditionalBoxZ/2;
  G4double Trans_z21            =       -chamberZ/2-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;
  G4double Trans_z22            =       -chamber_zc-AdditionalBoxZ/2-ThicknesOfChamber/2+windowThick;

  G4ThreeVector zTransAddBox11	(Trans_x1, 0, Trans_z11);
  G4ThreeVector zTransAddBox12	(Trans_x2, 0, Trans_z12);

  G4ThreeVector zTransInnerBox21	(Trans_x1, 0, Trans_z21);
  G4ThreeVector zTransInnerBox22	(Trans_x2, 0, Trans_z22);

  G4ThreeVector zNeck1	(Trans_x1, 0, -innerBoxZ/2-NeckZ/2);
  G4ThreeVector zNeck2	(Trans_x2, 0, -innerBoxZ/2-NeckZ/2);

  G4double shiftDet             =       1*cm;
  G4double detector_xc         	= 	Trans_x2+chamber_xc;
  G4double detector_yc         	= 	0;
  G4double detector_zc         	= 	-AdditionalBoxZ - gapSize + ThicknesOfChamber*2 - shiftDet;

  beamSize = _apertureInRadius;

  G4double colimator_xc11	=	Trans_x1+colimatorX/2+beamSize/2;
  G4double colimator_yc11	=	0*mm;
  G4double colimator_zc11	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc12	=	Trans_x1-colimatorX/2-beamSize/2;
  G4double colimator_yc12	=	0*mm;
  G4double colimator_zc12	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc21	=	Trans_x1;
  G4double colimator_yc21	=	colimatorX/2+beamSize/2;
  G4double colimator_zc21	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc22	=	Trans_x1;
  G4double colimator_yc22	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc22	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;

  beamSize = _apertureLenght;

  G4double colimator_xc31	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc31	=	0*mm;
  G4double colimator_zc31	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc31thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc32	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc32	=	0*mm;
  G4double colimator_zc32	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfChamber*2;
  G4double colimator_zc32thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfChamber*2;

  G4double colimator_xc41	=	Trans_x2;
  G4double colimator_yc41	=	colimatorX/2+beamSize/2;
  G4double colimator_zc41	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc41thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc42	=	Trans_x2;
  G4double colimator_yc42	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc42	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfChamber*2;
  G4double colimator_zc42thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfChamber*2;

  G4double colimator_xc51	=	Trans_x2+colimatorX/2+beamSize/2;
  G4double colimator_yc51	=	0*mm;
  G4double colimator_zc51thin	=	colimator_zc31thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc52	=	Trans_x2-colimatorX/2-beamSize/2;
  G4double colimator_yc52	=	0*mm;
  G4double colimator_zc52thin	=	colimator_zc32thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc61	=	Trans_x2;
  G4double colimator_yc61	=	colimatorX/2+beamSize/2;
  G4double colimator_zc61thin	=	colimator_zc41thin -gap_between_collimators-ThicknesOfChamber*2;

  G4double colimator_xc62	=	Trans_x2;
  G4double colimator_yc62	=	-colimatorX/2-beamSize/2;
  G4double colimator_zc62thin	=	colimator_zc42thin -gap_between_collimators-ThicknesOfChamber*2;

  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox1(fieldBox_xc + chamber_xc, fieldBox_yc + chamber_yc, fieldBox_zc + chamber_zc,
			 fieldBoxX, fieldBoxY, fieldBoxZ);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();


  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);
  logicWorld = new G4LogicalVolume(solidWorld,
		  //MainDet->GetMaterial(3), air
		  MainDet->GetMaterial(5),  //myvacuum
		  "World");
  physiWorld = new G4PVPlacement(0,	 G4ThreeVector(),  logicWorld, "World", 0, false, 0);

  //
  // Beampipe
  //
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,MainDet->GetMaterial(2),"BeamPipe");
  physiBeamPipe = new G4PVPlacement(RM1, G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipe, "BeamPipe",  logicWorld,false,0);

  //
  // Beampipe Vacuum
  //
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,MainDet->GetMaterial(6),"BeamPipeV");
  physiBeamPipeV = new G4PVPlacement(RM1,G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipeV,"BeamPipeV",logicWorld, false, 0);

  //
  // Beampipe cup - absorber
  //

  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,MainDet->GetMaterial(1), "Cup");
  physiCup = new G4PVPlacement(RM1,G4ThreeVector(cup_xc,cup_yc,cup_zc),logicCup,"Cup",logicWorld,false,0);

  //
  //	Chamber
  //

  solidChamber1 = new G4Box("Chamber1",chamberX/2, chamberY/2, chamberZ/2);
  solidAdditionalBox = new G4Box("ChamberAdd",AdditionalBoxX/2, AdditionalBoxY/2, AdditionalBoxZ/2);
  G4RotationMatrix*RMZero=new G4RotationMatrix(0,0,0);
  G4Transform3D transform11(*RMZero, zTransAddBox11);
  G4Transform3D transform12(*RMZero, zTransAddBox12);
  G4UnionSolid* solidChamber2 = new G4UnionSolid("Chamber", solidChamber1, solidAdditionalBox, transform11);
  G4UnionSolid* solidChamber = new G4UnionSolid("Chamber", solidChamber2, solidAdditionalBox, transform12);

  logicChamber = new G4LogicalVolume(solidChamber,MainDet->GetMaterial(1), "Chamber");
  physiChamber = new G4PVPlacement(0, G4ThreeVector(chamber_xc,chamber_yc,chamber_zc), logicChamber, "Chamber",	 logicWorld, false, 0);

  //
  //	InnerBox
  //

  solidInnerBox = new G4Box("InnerBox",innerBoxX/2, innerBoxY/2, innerBoxZ/2);
  solidAdditionalInnerBox = new G4Box("InnerAdd",AdditionalInnerBoxX/2, AdditionalInnerBoxY/2, AdditionalInnerBoxZ/2);
  solidNeck = new G4Box("Neck",NeckX/2, NeckY/2, NeckZ/2);
  G4Transform3D transform21(*RMZero, zTransInnerBox21);
  G4Transform3D transform22(*RMZero, zTransInnerBox22);
  G4Transform3D transNeck1(*RMZero, zNeck1);
  G4Transform3D transNeck2(*RMZero, zNeck2);
  G4UnionSolid* solidInnerBoxUNeck = new G4UnionSolid("Chamber", solidInnerBox, solidNeck, transNeck1);
  G4UnionSolid* solidInnerBoxUNeck2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck, solidNeck, transNeck2);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox = new G4UnionSolid("Chamber", solidInnerBoxUNeck2, solidAdditionalInnerBox, transform21);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck2UAddBox, solidAdditionalInnerBox, transform22);

  logicInnerBox = new G4LogicalVolume(solidInnerBoxUNeck2UAddBox2,MainDet->GetMaterial(5),"InnerBox");
  physiInnerBox = new G4PVPlacement(0, G4ThreeVector(innerBox_xc,innerBox_yc,innerBox_zc), logicInnerBox, "InnerBox", logicChamber,	 false,	 0);

  //
  //	FieldBox
  //
  solidFieldBox = new G4Box("FieldBox",fieldBoxX/2, fieldBoxY/2, fieldBoxZ/2);
  logicFieldBox = new G4LogicalVolume(solidFieldBox,MainDet->GetMaterial(5), "FieldBox");
  physiFieldBox = new G4PVPlacement(0, G4ThreeVector(fieldBox_xc,fieldBox_yc,fieldBox_zc), logicFieldBox,"FieldBox", logicInnerBox,	 false,	 0);

  //
  //	Colimator 11
  //
  int ColimMat = 2;
  solidColim11 = new G4Box("Kolimator11",colimatorX/2, colimatorY/2, colimatorZ/2);
  logicColim11 = new G4LogicalVolume(solidColim11,MainDet->GetMaterial(ColimMat), "Kolimator11");
  physiColim11 = new G4PVPlacement(0,	G4ThreeVector(colimator_xc11,colimator_yc11,colimator_zc11), logicColim11,"Kolimator11", logicInnerBox,  false, 0);
  //
  //	Colimator 12
  //
  solidColim12 = new G4Box("Kolimator12",colimatorX/2, colimatorY/2, colimatorZ/2);
  logicColim12 = new G4LogicalVolume(solidColim12,MainDet->GetMaterial(ColimMat),"Kolimator12");
  physiColim12 = new G4PVPlacement(0, G4ThreeVector(colimator_xc12,colimator_yc12,colimator_zc12), logicColim12,"Kolimator12", logicInnerBox, false, 0);
  //
  //	Colimator 21
  //
  solidColim21 = new G4Box("Kolimator21",colimatorY/2, colimatorX/2, colimatorZ/2);
  logicColim21 = new G4LogicalVolume(solidColim21,MainDet->GetMaterial(ColimMat), "Kolimator21");
  physiColim21 = new G4PVPlacement(0, G4ThreeVector(colimator_xc21,colimator_yc21,colimator_zc21), logicColim21, "Kolimator21", logicInnerBox, false, 0);
  //
  //	Colimator 22
  //
  solidColim22 = new G4Box("Kolimator22",colimatorY/2, colimatorX/2, colimatorZ/2);
  logicColim22 = new G4LogicalVolume(solidColim22,MainDet->GetMaterial(ColimMat), "Kolimator12");
  physiColim22 = new G4PVPlacement(0, G4ThreeVector(colimator_xc22,colimator_yc22,colimator_zc22),  logicColim22, "Kolimator22", logicInnerBox, false, 0);

  if(ColimGeom)
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31),  logicColim11, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZ/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZ/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42), logicColim42,"Kolimator42", logicInnerBox, false,0);
  }
  /**/
  else
  {
	  //
	  //	Colimator 31
	  //
	  solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim31 = new G4LogicalVolume(solidColim31,MainDet->GetMaterial(ColimMat),  "Kolimator31");
	  physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31thin),  logicColim31, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim32 = new G4LogicalVolume(solidColim32,MainDet->GetMaterial(ColimMat), "Kolimator32");
	  physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32thin), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim41 = new G4LogicalVolume(solidColim41,MainDet->GetMaterial(ColimMat), "Kolimator41");
	  physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41thin), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim42 = new G4LogicalVolume(solidColim42,MainDet->GetMaterial(ColimMat), "Kolimator42");
	  physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42thin), logicColim42,"Kolimator42", logicInnerBox, false,0);

	  //
	  //	Colimator 51
	  //
	  solidColim51 = new G4Box("Kolimator51",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim51 = new G4LogicalVolume(solidColim51,MainDet->GetMaterial(ColimMat),  "Kolimator51");
	  physiColim51 = new G4PVPlacement(0, G4ThreeVector(colimator_xc51,colimator_yc51,colimator_zc51thin),  logicColim51, "Kolimat51", logicInnerBox, false, 0);
	  //
	  //	Colimator 52
	  //
	  solidColim52 = new G4Box("Kolimator52",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  logicColim52 = new G4LogicalVolume(solidColim52,MainDet->GetMaterial(ColimMat), "Kolimator52");
	  physiColim52 = new G4PVPlacement(0, G4ThreeVector(colimator_xc52,colimator_yc52,colimator_zc52thin), logicColim52, "Kolimator52",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 61
	  //
	  solidColim61 = new G4Box("Kolimator61",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim61 = new G4LogicalVolume(solidColim61,MainDet->GetMaterial(ColimMat), "Kolimator61");
	  physiColim61 = new G4PVPlacement(0, G4ThreeVector(colimator_xc61,colimator_yc61,colimator_zc61thin), logicColim61, "Kolimator61",logicInnerBox, false, 0);
	  //
	  //	Colimator 62
	  //
	  solidColim62 = new G4Box("Kolimator62",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  logicColim62 = new G4LogicalVolume(solidColim62,MainDet->GetMaterial(ColimMat), "Kolimator62");
	  physiColim62 = new G4PVPlacement(0, G4ThreeVector(colimator_xc62,colimator_yc62,colimator_zc62thin), logicColim62,"Kolimator62", logicInnerBox, false,0);
  }

  //
  //Detector
  //
  solidSenDet1 = new G4Tubs("SenDet1", 0, detectorRad, detectorThick, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     //beamVacuum,
						MainDet->GetMaterial(6),
				     "SenDet1");
  physiSenDet1 = new G4PVPlacement(0,	//rotation
				   G4ThreeVector(detector_xc,detector_yc,detector_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

  solidShieldBarrier1 = new G4Box("ShieldBarrier1",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
  logicShieldBarrier1 = new G4LogicalVolume(solidShieldBarrier1,MainDet->GetMaterial(7), "ShieldBarrier1");
  physiShieldBarrier1 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc,ShieldBarrier_yc,ShieldBarrier_zc),
		  logicShieldBarrier1, "ShieldBarrier1",logicWorld, false, 0);

  solidShieldBarrier2 = new G4Box("ShieldBarrier2",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
  logicShieldBarrier2 = new G4LogicalVolume(solidShieldBarrier2,MainDet->GetMaterial(7), "ShieldBarrier2");
  physiShieldBarrier2 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc-0.15*m,ShieldBarrier_yc,ShieldBarrier_zc),
		  logicShieldBarrier2, "ShieldBarrier2",logicWorld, false, 0);

//  solidInnerTopShield = new G4Box("InnerTopShield",InnerTopShieldX/2, InnerTopShieldY/2, InnerTopShieldZ/2);
//  logicInnerTopShield = new G4LogicalVolume(solidInnerTopShield,MainDet->GetMaterial(7), "InnerTopShield");
//  physiInnerTopShield = new G4PVPlacement(0, G4ThreeVector(InnerTopShield_xc,InnerTopShield_yc,InnerTopShield_zc),
//		  logicInnerTopShield, "InnerTopShield",logicFieldBox, false, 0);

  solidInnerBotShield1 = new G4Box("InnerBotShield1",InnerBotShield1X/2, InnerBotShield1Y/2, InnerBotShield1Z/2);
  logicInnerBotShield1 = new G4LogicalVolume(solidInnerBotShield1,MainDet->GetMaterial(7), "InnerBotShield1");
  G4RotationMatrix*RM11=new G4RotationMatrix;
  RM11->rotateY(0*deg);
  physiInnerBotShield1 = new G4PVPlacement(RM11,
		  G4ThreeVector(InnerBotShield1_xc,InnerBotShield1_yc,InnerBotShield1_zc),
		  logicInnerBotShield1, "InnerBotShield1",logicFieldBox, false, 0);

  solidInnerBotShield2 = new G4Box("InnerBotShield2",InnerBotShield2X/2, InnerBotShield2Y/2, InnerBotShield2Z/2);
  logicInnerBotShield2 = new G4LogicalVolume(solidInnerBotShield2,MainDet->GetMaterial(7), "InnerBotShield2");
  G4RotationMatrix*RM12=new G4RotationMatrix;
  RM12->rotateY(0*deg);
  physiInnerBotShield2 = new G4PVPlacement(RM12,
		  G4ThreeVector(InnerBotShield2_xc,InnerBotShield2_yc,InnerBotShield2_zc),
		  logicInnerBotShield2, "InnerBotShield2",logicFieldBox, false, 0);

  solidInnerShield3 = new G4Box("InnerShield3",InnerShield3X/2, InnerShield3Y/2, InnerShield3Z/2);
  logicInnerShield3 = new G4LogicalVolume(solidInnerShield3,MainDet->GetMaterial(7), "InnerShield3");
  G4RotationMatrix*RM13=new G4RotationMatrix;
  RM13->rotateY(0*deg);
  physiInnerShield3 = new G4PVPlacement(RM13,
		  G4ThreeVector(InnerShield3_xc,InnerShield3_yc,InnerShield3_zc),
		  logicInnerShield3, "InnerShield3",logicFieldBox, false, 0);

//  solidInnerShield4 = new G4Box("InnerShield4",InnerShield4X/2, InnerShield4Y/2, InnerShield4Z/2);
//  logicInnerShield4 = new G4LogicalVolume(solidInnerShield4,MainDet->GetMaterial(7), "InnerShield4");
//  G4RotationMatrix*RM14=new G4RotationMatrix;
//  RM14->rotateY(-45*deg);
//  physiInnerShield4 = new G4PVPlacement(RM14,
//		  G4ThreeVector(InnerShield4_xc,InnerShield4_yc,InnerShield4_zc),
//		  logicInnerShield4, "InnerShield4",logicFieldBox, false, 0);

  solidInnerShield5 = new G4Box("InnerShield5",InnerShield5X/2, InnerShield5Y/2, InnerShield5Z/2);
  logicInnerShield5 = new G4LogicalVolume(solidInnerShield5,MainDet->GetMaterial(7), "InnerShield5");
  G4RotationMatrix*RM15=new G4RotationMatrix;
  RM15->rotateY(45*deg);
  physiInnerShield5 = new G4PVPlacement(RM15,
		  G4ThreeVector(InnerShield5_xc,InnerShield5_yc,InnerShield5_zc),
		  logicInnerShield5, "InnerShield5",logicFieldBox, false, 0);

  ConstructMagnet(520*mm, 50*mm, chamberZ, gapSize);
  //
  // Visualization attributes
  //


  G4VisAttributes* pipeVisAtt = new G4VisAttributes(G4Colour(0.5,0.0,0.6));
  logicBeamPipe->SetVisAttributes(pipeVisAtt);

  G4VisAttributes* pipeVVisAtt = new G4VisAttributes(G4Colour(0.4,0.3,0.0));
  logicBeamPipeV->SetVisAttributes(pipeVVisAtt);

  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);

  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicChamber->SetVisAttributes(chamberVisAtt);

  G4VisAttributes* innerVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicInnerBox->SetVisAttributes(innerVisAtt);

  G4VisAttributes* fieldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicFieldBox->SetVisAttributes(fieldVisAtt);

  G4VisAttributes* detVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicSenDet1->SetVisAttributes(detVisAtt);

  G4double maxStep   = _maxStep;
  G4double maxLength = _maxLength;
  G4double maxTime   = _maxTime;
  G4double minEkin   = _minEkin;
  G4double mionRang  = _mionRang;

/*
  G4double maxStep   = 0.1*mm;
  G4double maxLength = 5.0*m;
  G4double maxTime   = 20.0*ns;
  G4double minEkin   = 1.0/100*MeV;
  G4double mionRang  = 0.01*mm;
*/
G4cout<<std::endl<<"maxStep = "<<std::setw(14)<<_maxStep<<std::endl;
G4cout<<"maxLength = "<<std::setw(14)<<_maxLength<<std::endl;
G4cout<<"maxTime = "<<std::setw(14)<<_maxTime<<std::endl;
G4cout<<"minEkin = "<<std::setw(14)<<_minEkin<<std::endl;
G4cout<<"mionRangHERE = "<<std::setw(14)<<_mionRang<<std::endl<<std::endl;

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  logicWorld->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);
  logicChamber->SetUserLimits(stepLimit);
  logicInnerBox->SetUserLimits(stepLimit);
  logicFieldBox->SetUserLimits(stepLimit);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}



void DetectorConstructionP3::ConstructMagnet(double refX, double refY, double refZ, double gapSize, double thicknessOfChamber)
{
	double popravka = 10*mm + thicknessOfChamber - 3*mm;
	double gapBetweenSideAndMid = 50*mm;

	double magnetSideX = 50 * mm;
	double magnetSideY = 120 * mm + 2*gapSize + 2*(thicknessOfChamber - 3*mm);
	double magnetSideZ = 250 * mm;

	double magnetMainX = refX + 2*magnetSideX + 2*gapSize + gapBetweenSideAndMid*2;
	double magnetMainY = 50 * mm;
	double magnetMainZ = magnetSideZ;

	double MagnetMidX = refX * mm;
	double MagnetMidY = 35 * mm;
	double MagnetMidZ = magnetSideZ;

	double magnetTopMain_xc = 100*mm; //equal to ChamberX/4
	double magnetTopMain_yc = refY/2 + magnetMainY/2 + gapSize*2 + MagnetMidY + thicknessOfChamber - 3*mm;
	double magnetTopMain_zc = magnetMainZ/2 + popravka;

	double magnetTopLeft_xc = magnetTopMain_xc - magnetMainX/2 + magnetSideX/2;
	double magnetTopLeft_yc = magnetTopMain_yc - magnetMainY/2 - magnetSideY/2 - gapSize;
	double magnetTopLeft_zc = magnetTopMain_zc;

	double magnetTopRight_xc = -magnetTopLeft_xc + 2* magnetTopMain_xc;
	double magnetTopRight_yc = magnetTopLeft_yc;
	double magnetTopRight_zc = magnetTopLeft_zc;

	double MagnetTopMid_xc = magnetTopMain_xc;
	double MagnetTopMid_yc = magnetTopMain_yc - magnetMainY/2 - MagnetMidY/2 - gapSize;
	double MagnetTopMid_zc = magnetTopMain_zc;

	double magnetBotMain_xc = magnetTopMain_xc;
	double magnetBotMain_yc = -magnetTopMain_yc;
	double magnetBotMain_zc = magnetTopMain_zc;

	double MagnetBotMid_xc = MagnetTopMid_xc;
	double MagnetBotMid_yc = -MagnetTopMid_yc;
	double MagnetBotMid_zc = MagnetTopMid_zc;


	solidMagnetTopMain = new G4Box("MagnetTopMain",magnetMainX/2, magnetMainY/2, magnetMainZ/2);
	logicMagnetTopMain = new G4LogicalVolume(solidMagnetTopMain,MainDet->GetMaterial(2), "MagnetTopMain");
	physiMagnetTopMain = new G4PVPlacement(0, G4ThreeVector(magnetTopMain_xc,magnetTopMain_yc,magnetTopMain_zc),
												logicMagnetTopMain,"MagnetTopMain", logicWorld, false,0);

	solidMagnetTopLeft = new G4Box("MagnetTopLeft",magnetSideX/2, magnetSideY/2, magnetSideZ/2);
	logicMagnetTopLeft = new G4LogicalVolume(solidMagnetTopLeft,MainDet->GetMaterial(2), "MagnetTopLeft");
	physiMagnetTopLeft = new G4PVPlacement(0, G4ThreeVector(magnetTopLeft_xc,magnetTopLeft_yc,magnetTopLeft_zc),
												logicMagnetTopLeft,"MagnetTopLeft", logicWorld, false,0);

	solidMagnetTopRight = new G4Box("MagnetTopRight",magnetSideX/2, magnetSideY/2, magnetSideZ/2);
	logicMagnetTopRight = new G4LogicalVolume(solidMagnetTopRight,MainDet->GetMaterial(2), "MagnetTopRight");
	physiMagnetTopRight = new G4PVPlacement(0, G4ThreeVector(magnetTopRight_xc,magnetTopRight_yc,magnetTopRight_zc),
												logicMagnetTopRight,"MagnetTopRight", logicWorld, false,0);

	solidMagnetTopMid = new G4Box("MagnetTopMid",MagnetMidX/2, MagnetMidY/2, MagnetMidZ/2);
	logicMagnetTopMid = new G4LogicalVolume(solidMagnetTopMid,MainDet->GetMaterial(2), "MagnetTopMid");
	physiMagnetTopMid = new G4PVPlacement(0, G4ThreeVector(MagnetTopMid_xc,MagnetTopMid_yc,MagnetTopMid_zc),
												logicMagnetTopMid,"MagnetTopMid", logicWorld, false,0);


	solidMagnetBotMain = new G4Box("MagnetBotMain",magnetMainX/2, magnetMainY/2, magnetMainZ/2);
	logicMagnetBotMain = new G4LogicalVolume(solidMagnetBotMain,MainDet->GetMaterial(2), "MagnetBotMain");
	physiMagnetBotMain = new G4PVPlacement(0, G4ThreeVector(magnetBotMain_xc,magnetBotMain_yc,magnetBotMain_zc),
												logicMagnetBotMain,"MagnetBotMain", logicWorld, false,0);

	solidMagnetBotMid = new G4Box("MagnetBotMid",MagnetMidX/2, MagnetMidY/2, MagnetMidZ/2);
	logicMagnetBotMid = new G4LogicalVolume(solidMagnetBotMid,MainDet->GetMaterial(2), "MagnetBotMid");
	physiMagnetBotMid = new G4PVPlacement(0, G4ThreeVector(MagnetBotMid_xc,MagnetBotMid_yc,MagnetBotMid_zc),
												logicMagnetBotMid,"MagnetBotMid", logicWorld, false,0);

	  G4VisAttributes* magnetVisAtt = new G4VisAttributes(G4Colour(0.8,0.7,0.3));
	  logicMagnetTopMain->SetVisAttributes(magnetVisAtt);
	  logicMagnetTopLeft->SetVisAttributes(magnetVisAtt);
	  logicMagnetTopRight->SetVisAttributes(magnetVisAtt);
	  logicMagnetTopMid->SetVisAttributes(magnetVisAtt);
	  logicMagnetBotMain->SetVisAttributes(magnetVisAtt);
	  logicMagnetBotMid->SetVisAttributes(magnetVisAtt);
}

void DetectorConstructionP3::ConstructMagnet(double refX, double refY, double refZ, double gapSize)
{
	double popravka = 10 *mm;
	double gapBetweenSideAndMid = 50*mm;

	double magnetSideX = 50 * mm;
	double magnetSideY = 120 * mm + 2*gapSize;
	double magnetSideZ = 250 * mm;

	double magnetMainX = refX + 2*magnetSideX + 2*gapSize + gapBetweenSideAndMid*2;
	double magnetMainY = 50 * mm;
	double magnetMainZ = magnetSideZ;

	double MagnetMidX = refX * mm;
	double MagnetMidY = 35 * mm;
	double MagnetMidZ = magnetSideZ;

	double magnetTopMain_xc = 100*mm; //equal to ChamberX/4
	double magnetTopMain_yc = refY/2 + magnetMainY/2 + gapSize*2 + MagnetMidY;
	double magnetTopMain_zc = magnetMainZ/2 + popravka;

	double magnetTopLeft_xc = magnetTopMain_xc - magnetMainX/2 + magnetSideX/2;
	double magnetTopLeft_yc = magnetTopMain_yc - magnetMainY/2 - magnetSideY/2 - gapSize;
	double magnetTopLeft_zc = magnetTopMain_zc;

	double magnetTopRight_xc = -magnetTopLeft_xc + 2* magnetTopMain_xc;
	double magnetTopRight_yc = magnetTopLeft_yc;
	double magnetTopRight_zc = magnetTopLeft_zc;

	double MagnetTopMid_xc = magnetTopMain_xc;
	double MagnetTopMid_yc = magnetTopMain_yc - magnetMainY/2 - MagnetMidY/2 - gapSize;
	double MagnetTopMid_zc = magnetTopMain_zc;

	double magnetBotMain_xc = magnetTopMain_xc;
	double magnetBotMain_yc = -magnetTopMain_yc;
	double magnetBotMain_zc = magnetTopMain_zc;

	double MagnetBotMid_xc = MagnetTopMid_xc;
	double MagnetBotMid_yc = -MagnetTopMid_yc;
	double MagnetBotMid_zc = MagnetTopMid_zc;


	solidMagnetTopMain = new G4Box("MagnetTopMain",magnetMainX/2, magnetMainY/2, magnetMainZ/2);
	logicMagnetTopMain = new G4LogicalVolume(solidMagnetTopMain,MainDet->GetMaterial(2), "MagnetTopMain");
	physiMagnetTopMain = new G4PVPlacement(0, G4ThreeVector(magnetTopMain_xc,magnetTopMain_yc,magnetTopMain_zc),
												logicMagnetTopMain,"MagnetTopMain", logicWorld, false,0);

	solidMagnetTopLeft = new G4Box("MagnetTopLeft",magnetSideX/2, magnetSideY/2, magnetSideZ/2);
	logicMagnetTopLeft = new G4LogicalVolume(solidMagnetTopLeft,MainDet->GetMaterial(2), "MagnetTopLeft");
	physiMagnetTopLeft = new G4PVPlacement(0, G4ThreeVector(magnetTopLeft_xc,magnetTopLeft_yc,magnetTopLeft_zc),
												logicMagnetTopLeft,"MagnetTopLeft", logicWorld, false,0);

	solidMagnetTopRight = new G4Box("MagnetTopRight",magnetSideX/2, magnetSideY/2, magnetSideZ/2);
	logicMagnetTopRight = new G4LogicalVolume(solidMagnetTopRight,MainDet->GetMaterial(2), "MagnetTopRight");
	physiMagnetTopRight = new G4PVPlacement(0, G4ThreeVector(magnetTopRight_xc,magnetTopRight_yc,magnetTopRight_zc),
												logicMagnetTopRight,"MagnetTopRight", logicWorld, false,0);

	solidMagnetTopMid = new G4Box("MagnetTopMid",MagnetMidX/2, MagnetMidY/2, MagnetMidZ/2);
	logicMagnetTopMid = new G4LogicalVolume(solidMagnetTopMid,MainDet->GetMaterial(2), "MagnetTopMid");
	physiMagnetTopMid = new G4PVPlacement(0, G4ThreeVector(MagnetTopMid_xc,MagnetTopMid_yc,MagnetTopMid_zc),
												logicMagnetTopMid,"MagnetTopMid", logicWorld, false,0);


	solidMagnetBotMain = new G4Box("MagnetBotMain",magnetMainX/2, magnetMainY/2, magnetMainZ/2);
	logicMagnetBotMain = new G4LogicalVolume(solidMagnetBotMain,MainDet->GetMaterial(2), "MagnetBotMain");
	physiMagnetBotMain = new G4PVPlacement(0, G4ThreeVector(magnetBotMain_xc,magnetBotMain_yc,magnetBotMain_zc),
												logicMagnetBotMain,"MagnetBotMain", logicWorld, false,0);


	solidMagnetBotMid = new G4Box("MagnetBotMid",MagnetMidX/2, MagnetMidY/2, MagnetMidZ/2);
	logicMagnetBotMid = new G4LogicalVolume(solidMagnetBotMid,MainDet->GetMaterial(2), "MagnetBotMid");
	physiMagnetBotMid = new G4PVPlacement(0, G4ThreeVector(MagnetBotMid_xc,MagnetBotMid_yc,MagnetBotMid_zc),
												logicMagnetBotMid,"MagnetBotMid", logicWorld, false,0);

	  G4VisAttributes* magnetVisAtt = new G4VisAttributes(G4Colour(0.8,0.7,0.3));
	  logicMagnetTopMain->SetVisAttributes(magnetVisAtt);
	  logicMagnetTopLeft->SetVisAttributes(magnetVisAtt);
	  logicMagnetTopRight->SetVisAttributes(magnetVisAtt);
	  logicMagnetTopMid->SetVisAttributes(magnetVisAtt);
	  logicMagnetBotMain->SetVisAttributes(magnetVisAtt);
	  logicMagnetBotMid->SetVisAttributes(magnetVisAtt);
}
