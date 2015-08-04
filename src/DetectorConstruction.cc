#include "assert.h"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4UserLimits.hh"
#include "G4RunManager.hh"
#include "G4AutoDelete.hh"

#include "TMath.h"

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "MagneticField.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//G4ThreadLocal G4UniformMagField* DetectorConstruction::fMagneticField = 0;
//G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
, _RotationDeg(0)
, detectorMessenger(NULL)
{
//	magField = new MagneticField();
	detectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
	if (detectorMessenger) delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	DefineMaterials();

	G4double gap_between_collimators = 3*cm;

	//	  G4double ShiftingOfAdditionalBox = _Det1X; // Distance from the edge to the beginning of the collimators system

	G4double WorldSizeX         	= 	200.0*cm;
	G4double WorldSizeY         	= 	WorldSizeX;
	G4double WorldSizeZ         	= 	WorldSizeX;

	G4double beamSize		=	2*cm;
	const G4double beamSizeMax	=	2*cm;

	G4double rotAngle	     	= 	_RotationDeg;//rotation of the PHIL pipe

	G4double ThicknesOfChamber 	=	8*mm;
	G4double ThicknesOfSmallChamber 	=	3*mm;

	G4double gapSize            	= 	1*mm;
	G4double bigGap            	= 	1*cm;

	G4double beamPipeLenght     	= 	20.0*cm;
	G4double beamPipeOutRadius  	= 	3.00*cm;
	G4double beamPipeInRadius   	= 	2.70*cm;
	G4double beamPipeVOutRadius 	= 	beamPipeInRadius - gapSize;

	G4double cupOutRadius       	= 	beamPipeOutRadius;
	//	  G4double cupLenght          	= 	_cupLenght;


	G4double chamberX		= 	52*cm+ThicknesOfChamber*2;
	G4double chamberY		= 	4*cm+ThicknesOfChamber*2;
	G4double chamberZ		= 	25*cm+ThicknesOfChamber*2;

	G4double electronsRadius = 20.3*cm;

	//	  G4double beamCorectionX	=	-electronsRadius - (beamPipeLenght/2+gapSize)*sin(rotAngle);
	//	  G4double beamCorectionZ	=	(beamPipeLenght/2+gapSize)*(1-cos(rotAngle));

	//G4double windowRad 		= 	0.5*cm;
	G4double windowThick 		= 	20*um;

	G4double colimatorX		=	20*mm;
	G4double colimatorY		=	40*mm;
	G4double colimatorZ		=	20*mm;
	G4double colimatorZthin	=	2*mm;

	G4double innerBoxX         	= 	chamberX-ThicknesOfChamber*2;
	G4double innerBoxY         	= 	chamberY-ThicknesOfChamber*2;
	G4double innerBoxZ         	= 	chamberZ-ThicknesOfChamber*2;

	//Sizes of the connection from chamber to collimator system
	G4double NeckX         	= 	4*cm;
	G4double NeckY         	= 	4*cm;
	G4double NeckZ         	= 	ThicknesOfChamber;

	G4double fieldBoxX         	= 	chamberX-ThicknesOfChamber*2-gapSize*2;
	G4double fieldBoxY         	= 	chamberY-ThicknesOfChamber*2-gapSize*2;
	G4double fieldBoxZ         	= 	chamberZ-ThicknesOfChamber*2-gapSize*2 - 6*mm;

	//	  G4double detectorRad         	= 	_Det1OutRad;
	G4double detectorThick        = 	1*mm;

	G4double AdditionalBoxX	=	colimatorX*2+beamSizeMax+gapSize*2 + ThicknesOfSmallChamber*2;
	G4double AdditionalBoxY	=	colimatorY+beamSizeMax+gapSize*2 + ThicknesOfSmallChamber*2 + bigGap;
	G4double AdditionalBoxZ	=	colimatorZ*2+bigGap*0.5 + 2*ThicknesOfSmallChamber;

	//  G4double AdditionalBoxX	=	colimatorX*2+beamSizeMax+gapSize*2 + ThicknesOfChamberOther*2;
	//  G4double AdditionalBoxY	=	colimatorY+beamSizeMax+gapSize*2 + ThicknesOfChamberOther*2 + bigGap;
	//  G4double AdditionalBoxZ	=	colimatorZ*2+bigGap*0.5 + 2*ThicknesOfChamberOther;

	G4double AdditionalInnerBoxX	=	AdditionalBoxX-ThicknesOfSmallChamber*2;
	G4double AdditionalInnerBoxY	=	AdditionalBoxY-ThicknesOfSmallChamber*2;
	G4double AdditionalInnerBoxZ	=	AdditionalBoxZ-ThicknesOfSmallChamber;

	G4double  windowToCup 	= 	3*cm;
	G4RotationMatrix*RM1 = new G4RotationMatrix(89.9*deg, rotAngle, 0);

	//distance between system of cordiantes of beampipe and chamber
	G4double fromOriginToOrigin 	=	AdditionalBoxZ+2*cm;

	G4double ShieldBarrierX       =       16*cm;
	G4double ShieldBarrierY       =       40*cm;
	G4double ShieldBarrierZ       =       30*cm;

	G4double InnerTopShieldX      =       20*mm;
	G4double InnerTopShieldY      =       fieldBoxY-gapSize;
	G4double InnerTopShieldZ      =       25*mm;

	G4double InnerBotShield1X      =       InnerTopShieldX;
	G4double InnerBotShield1Y      =       InnerTopShieldY;
	G4double InnerBotShield1Z      =       140*mm;

	G4double InnerBotShield2X      =       InnerTopShieldX;
	G4double InnerBotShield2Y      =       InnerTopShieldY;
	G4double InnerBotShield2Z      =       60*mm;

	G4double InnerShield3X      =       	180*mm;
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

	G4double InnerBotShield1_xc    =       90*mm;
	G4double InnerBotShield1_yc    =       0;
	G4double InnerBotShield1_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield1Z/2;

	G4double InnerBotShield2_xc    =       -90*mm;
	G4double InnerBotShield2_yc    =       0;
	G4double InnerBotShield2_zc    =       -fieldBoxZ/2+gapSize+InnerBotShield2Z/2;

	G4double InnerShield3_xc	    =       InnerBotShield1_xc - InnerBotShield1X/2 - InnerShield3X/2;
	G4double InnerShield3_yc 	    =       0;
	G4double InnerShield3_zc   	=       -fieldBoxZ/2+gapSize+InnerBotShield1Z - InnerShield3Z/2;

	G4double InnerShield4_xc	    =       180*mm;
	G4double InnerShield4_yc 	    =       0;
	G4double InnerShield4_zc   	=       62*mm;

	G4double InnerShield5_xc	    =       -180*mm;
	G4double InnerShield5_yc 	    =       0;
	G4double InnerShield5_zc   	=       62*mm;

	G4double fieldBox_xc         	= 	0;
	G4double fieldBox_yc         	= 	0;
	G4double fieldBox_zc = 3*mm;

	G4double innerBox_xc         	= 	0.0*cm;
	G4double innerBox_yc         	= 	0.0*cm;
	G4double innerBox_zc         	= 	0.0*cm;

	//	  G4double beamPipe_xc          =       beamCorectionX;
	G4double beamPipe_yc          =       0;
	//	  G4double beamPipe_zc          =       - beamPipeLenght/2.0 + beamCorectionZ - 10*cm;

	G4double cup_xc               =       0;
	G4double cup_yc               =       0;
	G4double cup_zc               =       0;

	G4double window_xc            =       0;
	//G4double window_yc  	=       0;
	//	  G4double window_zc            =       -cupLenght/2.0 - gapSize- windowToCup;

	//Move to other coordinat system
	//	  beamPipe_zc	-=	fromOriginToOrigin;
	cup_zc	-=	fromOriginToOrigin;
	//	  window_zc	-=	fromOriginToOrigin;

	//	  beamPipe_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
	//	  cup_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
	//	  window_xc	+=	-chamberX/4+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox;
	//Finish

	//std::cout<<std::endl<<"Coordinates of initial position of electrons must be: "<<beamPipe_xc<<" "<<beamPipe_yc<<" "<<beamPipe_zc<<std::endl<<std::endl;

	G4double chamber_xc		= 	0;
	G4double chamber_yc		= 	0;
	G4double chamber_zc		= 	chamberZ/2;

	G4double ShieldBarrier_xc     = 10*cm;
	G4double ShieldBarrier_xc1     =       249*mm - 75*mm;
	G4double ShieldBarrier_xc2     =       249*mm - 75*mm - 150*mm;
	G4double ShieldBarrier_yc     =       0;
	G4double ShieldBarrier_zc     =       chamber_zc-chamberZ/2-ShieldBarrierZ/2-gapSize;

	G4double Trans_x1 = -chamberX/2;
	G4double Trans_x2 = chamberX/2;

	G4double Trans_z11            =       -chamberZ/2-AdditionalBoxZ/2;
	G4double Trans_z12            =       -chamber_zc-AdditionalBoxZ/2;
	G4double Trans_z21            =       -chamberZ/2-AdditionalBoxZ/2-ThicknesOfSmallChamber/2+windowThick;
	G4double Trans_z22            =       -chamber_zc-AdditionalBoxZ/2-ThicknesOfSmallChamber/2+windowThick;

	G4ThreeVector zTransAddBox11	(Trans_x1, 0, Trans_z11);
	G4ThreeVector zTransAddBox12	(Trans_x2, 0, Trans_z12);

	G4ThreeVector zTransInnerBox21	(Trans_x1, 0, Trans_z21);
	G4ThreeVector zTransInnerBox22	(Trans_x2, 0, Trans_z22);

	G4ThreeVector zNeck1	(Trans_x1, 0, -innerBoxZ/2-NeckZ/2);
	G4ThreeVector zNeck2	(Trans_x2, 0, -innerBoxZ/2-NeckZ/2);

	G4double shiftDet             =       1*cm;
	G4double detector_xc         	= 	Trans_x2+chamber_xc;
	G4double detector_yc         	= 	0;
	G4double detector_zc         	= 	-AdditionalBoxZ - gapSize - shiftDet + 10*mm;

	//	  beamSize = _apertureInRadius;

	G4double colimator_xc11	=	Trans_x1+colimatorX/2+beamSize/2;
	G4double colimator_yc11	=	0*mm;
	G4double colimator_zc11	=	-chamber_zc-colimatorZ/2-gapSize-2*ThicknesOfSmallChamber;

	G4double colimator_xc12	=	Trans_x1-colimatorX/2-beamSize/2;
	G4double colimator_yc12	=	0*mm;
	G4double colimator_zc12	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfSmallChamber*2;

	G4double colimator_xc21	=	Trans_x1;
	G4double colimator_yc21	=	colimatorX/2+beamSize/2;
	G4double colimator_zc21	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfSmallChamber*2;

	G4double colimator_xc22	=	Trans_x1;
	G4double colimator_yc22	=	-colimatorX/2-beamSize/2;
	G4double colimator_zc22	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfSmallChamber*2;

	//	  beamSize = _apertureLenght;

	G4double colimator_xc31	=	Trans_x2+colimatorX/2+beamSize/2;
	G4double colimator_yc31	=	0*mm;
	G4double colimator_zc31	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfSmallChamber*2;
	G4double colimator_zc31thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfSmallChamber*2;

	G4double colimator_xc32	=	Trans_x2-colimatorX/2-beamSize/2;
	G4double colimator_yc32	=	0*mm;
	G4double colimator_zc32	=	-chamber_zc-colimatorZ/2-gapSize-ThicknesOfSmallChamber*2;
	G4double colimator_zc32thin	=	-chamber_zc-colimatorZthin/2-gapSize-ThicknesOfSmallChamber*2;

	G4double colimator_xc41	=	Trans_x2;
	G4double colimator_yc41	=	colimatorX/2+beamSize/2;
	G4double colimator_zc41	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfSmallChamber*2;
	G4double colimator_zc41thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfSmallChamber*2;

	G4double colimator_xc42	=	Trans_x2;
	G4double colimator_yc42	=	-colimatorX/2-beamSize/2;
	G4double colimator_zc42	=	-chamber_zc-colimatorZ*1.5-gapSize*2-ThicknesOfSmallChamber*2;
	G4double colimator_zc42thin	=	-chamber_zc-colimatorZthin*1.5-gapSize*2-ThicknesOfSmallChamber*2;

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

	// Clean old geometry, if any
	//
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	G4RotationMatrix RMZero(0,0,0);

	G4Box* solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);
	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,
		  //GetMaterial(3), air
		  GetMaterial(6),  //myvacuum
		  "World");
	G4VPhysicalVolume* physWorld = new G4PVPlacement(0,	 G4ThreeVector(),  logicWorld, "World", 0, false, 0);

	//
	//	Chamber
	//
	G4Box *solidChamberMain = new G4Box("Chamber1",chamberX/2, chamberY/2, chamberZ/2);

	G4double radNeck = 1.4*cm;
	G4double lengthNeck = 3*cm;

	G4double radNeckRing = 2*cm;
	G4double lengthNeckRing = 0.4*cm;

	G4double neckHoleSide = 2*cm;
	G4double lengthNeckHole = lengthNeck + ThicknesOfChamber;

	G4Tubs *neckSolid = new G4Tubs("Neck", 0, radNeck, lengthNeck/2, 0, 360.0*deg);

	G4Tubs *neckOuterRing = new G4Tubs("Neck", radNeck, radNeckRing, lengthNeckRing/2, 0, 360.0*deg);
	G4Transform3D transNeckOuterRing(RMZero, G4ThreeVector(0,0,-(lengthNeck-lengthNeckRing)/2));
	G4UnionSolid* neckWithOuterRing  = new G4UnionSolid("Chamber", neckSolid, neckOuterRing, transNeckOuterRing);

	G4Box  *neckHole  = new G4Box("NeckHole", neckHoleSide/2, neckHoleSide/2, lengthNeckHole/2);
	G4Transform3D transNeckHole(RMZero, G4ThreeVector(0,0,ThicknesOfChamber/2));
	G4SubtractionSolid* neck = new G4SubtractionSolid("Neck", neckWithOuterRing, neckHole, transNeckHole);

	//  G4Box *solidAdditionalBox = new G4Box("ChamberAdd",AdditionalBoxX/2, AdditionalBoxY/2, AdditionalBoxZ/2);

	G4Transform3D transEntranceNeck(RMZero, G4ThreeVector(-electronsRadius,0,-lengthNeck/2-chamberZ/2));
	G4Transform3D transExit1Neck(RMZero, G4ThreeVector(electronsRadius,0,-lengthNeck/2-chamberZ/2));
	G4Transform3D transExit2Neck(RMZero, G4ThreeVector(0,0,-lengthNeck/2-chamberZ/2));

	G4UnionSolid* solidChamberEntranceNeck  = new G4UnionSolid("Chamber", solidChamberMain, neck, transEntranceNeck);
	G4UnionSolid* solidChamberEnEx1Necks    = new G4UnionSolid("Chamber", solidChamberEntranceNeck, neck, transExit1Neck);
	G4UnionSolid* solidChamberEnEx1Ex2Necks = new G4UnionSolid("Chamber", solidChamberEnEx1Necks, neck, transExit2Neck);

	G4LogicalVolume* logicChamber = new G4LogicalVolume(solidChamberEnEx1Ex2Necks, GetMaterial(1), "Chamber");
	G4VPhysicalVolume* physiChamber = new G4PVPlacement(0, G4ThreeVector(chamber_xc,chamber_yc,chamber_zc), logicChamber, "Chamber",	 logicWorld, false, 0);

	//
	//	InnerBox
	//
	G4Box *innerMainBox = new G4Box("InnerBox", innerBoxX/2, innerBoxY/2, innerBoxZ/2);
	//  G4Box *solidAdditionalInnerBox = new G4Box("InnerAdd",AdditionalInnerBoxX/2, AdditionalInnerBoxY/2, AdditionalInnerBoxZ/2);

	G4Transform3D transEntranceNeckHole(RMZero, G4ThreeVector(-electronsRadius,0,-(chamberZ+lengthNeckHole)/2+ThicknesOfChamber));
	G4Transform3D transExit1NeckHole(RMZero, G4ThreeVector(electronsRadius,0,-(chamberZ+lengthNeckHole)/2+ThicknesOfChamber));
	G4Transform3D transExit2NeckHole(RMZero, G4ThreeVector(0,0,-(chamberZ+lengthNeckHole)/2+ThicknesOfChamber));

	G4UnionSolid *innerMainEntranceBox = new G4UnionSolid("InnerBox", innerMainBox, neckHole, transEntranceNeckHole);
	G4UnionSolid *innerMainEnEx1Box    = new G4UnionSolid("InnerBox", innerMainEntranceBox, neckHole, transExit1NeckHole);
	G4UnionSolid *innerMainEnEx1Ex2Box = new G4UnionSolid("InnerBox", innerMainEnEx1Box, neckHole, transExit2NeckHole);

	logicInnerBox = new G4LogicalVolume(innerMainEnEx1Ex2Box, GetMaterial(5),"InnerBox");
	G4VPhysicalVolume *physiInnerBox = new G4PVPlacement(0, G4ThreeVector(innerBox_xc,innerBox_yc,innerBox_zc), logicInnerBox, "InnerBox", logicChamber, false,	 0);

	G4double thicknessOfColBox = 8*mm;
	G4double sideOfColBox = 12.2*cm + 2*thicknessOfColBox;
	G4double heightOfColBox = 10.7*cm + 2*thicknessOfColBox;

	G4double collNeckInnerRad = 11.5*mm;
	G4double collInputNeckLength = 1.6*cm;
	G4double collOutputNeckLength = 4.2*cm;

	/* Box with holes */
	G4Box *colBoxMain = new G4Box("ColBox", sideOfColBox/2, sideOfColBox/2, heightOfColBox/2);
	G4Box *colInnerBox = new G4Box("ColBox", sideOfColBox/2 - thicknessOfColBox, sideOfColBox/2 - thicknessOfColBox, heightOfColBox/2 - thicknessOfColBox);
	G4Transform3D transColBox(RMZero, G4ThreeVector(0,0,0));
	G4SubtractionSolid *colBoxWithoutHoles = new G4SubtractionSolid("ColBox", colBoxMain, colInnerBox, transColBox);

	G4Tubs *cutColBoxHole = new G4Tubs("CutColHole", 0, collNeckInnerRad, thicknessOfColBox/2, 0, 360*deg);
	G4Transform3D transColHoleOut(RMZero, G4ThreeVector(0,0,
												  colBoxMain->GetZHalfLength() - cutColBoxHole->GetZHalfLength()));
	G4Transform3D transColHoleIn(RMZero, G4ThreeVector(0,0,
												  -(colBoxMain->GetZHalfLength() - cutColBoxHole->GetZHalfLength())));
	G4SubtractionSolid *colBoxWith1Hole = new G4SubtractionSolid("ColBox", colBoxWithoutHoles, cutColBoxHole, transColHoleOut);
	G4SubtractionSolid *colBox = new G4SubtractionSolid("ColBox", colBoxWith1Hole, cutColBoxHole, transColHoleIn);

	/* Add output neck */
	G4Tubs *collimOutNeckMain = new G4Tubs("CollimOutNeck", collNeckInnerRad, radNeck, collOutputNeckLength/2, 0, 360*deg);
	G4Tubs *collimOutNeckRing = new G4Tubs("CollimNeckRing", radNeck, radNeckRing, lengthNeckRing/2, 0, 360*deg);
	G4Transform3D transColOutRing(RMZero, G4ThreeVector(0,0,(collOutputNeckLength-lengthNeckRing)/2));
	G4UnionSolid *collimOutNeck = new G4UnionSolid("CollimOutNeck", collimOutNeckMain, collimOutNeckRing, transColOutRing);

	G4Transform3D transOutNeckToColBox(RMZero, G4ThreeVector(0,0,
										  colBoxMain->GetZHalfLength() + collimOutNeckMain->GetZHalfLength()));
	G4UnionSolid *collimBoxOutNeck = new G4UnionSolid("CollimOutNeck", colBox, collimOutNeck, transOutNeckToColBox);

	/* Add input neck */
	G4Tubs *collimInNeckMain = new G4Tubs("CollimInNeck", collNeckInnerRad, radNeck, collInputNeckLength/2, 0, 360*deg);
	G4Tubs *collimInNeckRing = new G4Tubs("CollimNeckRing", radNeck, radNeckRing, lengthNeckRing/2, 0, 360*deg);
	G4Transform3D transColInRing(RMZero, G4ThreeVector(0,0,-(collInputNeckLength-lengthNeckRing)/2));
	G4UnionSolid *collimInNeck = new G4UnionSolid("CollimOutNeck", collimInNeckMain, collimInNeckRing, transColInRing);

	G4Transform3D transInNeckToColBox(RMZero, G4ThreeVector(0,0,
										  -(colBoxMain->GetZHalfLength() + collimInNeckMain->GetZHalfLength())));
	G4UnionSolid *collimBoxOutNecks = new G4UnionSolid("ColBox", collimBoxOutNeck, collimInNeck, transInNeckToColBox);

	/* construct the same exit1 collimator box */
	G4Transform3D trans1stEntranceColBox(RMZero, G4ThreeVector(2*electronsRadius,0,0));
	G4UnionSolid *twoColBoxes = new G4UnionSolid("ColBoxes", collimBoxOutNecks, collimBoxOutNecks, trans1stEntranceColBox);

	G4LogicalVolume *logicColBoxes = new G4LogicalVolume(twoColBoxes, GetMaterial(1),"ColBoxes");
	G4VPhysicalVolume *phyColBoxes = new G4PVPlacement(0, G4ThreeVector(-electronsRadius,0,
																-(2*neckSolid->GetZHalfLength() + 2*collimOutNeckMain->GetZHalfLength() + colBoxMain->GetZHalfLength())),
											  logicColBoxes, "ColBoxes", logicWorld, false, 0);

	/* construct target */
	G4Tubs *target = new G4Tubs("Target", 0, radNeckRing, _targetThickness/2, 0, 360*deg);
	G4LogicalVolume *logicTarget = new G4LogicalVolume(target, GetMaterial(1), "Target");
	phyTarget = new G4PVPlacement(0, G4ThreeVector(-electronsRadius,0,
																-(2*neckSolid->GetZHalfLength()
																  + 2*collimOutNeckMain->GetZHalfLength()
																  + 2*colBoxMain->GetZHalfLength()
																  + 2*collimInNeckMain->GetZHalfLength()
																  + target->GetZHalfLength())),
											  logicTarget, "Target", logicWorld, false, 0);


	/* construct collimators */
	/* 1) collimator base plate */
	G4double baseSide = sideOfColBox - 2*thicknessOfColBox;
	G4double baseWidth = 10*mm;
	G4double baseHoleSize = 2.5*cm;

	//  collEntranceGapX = 1.5*cm;
	//  collEntranceGapY = 1*cm;
	//  collExit1GapX = 1*cm;
	//  collExit1GapY = 2*cm;

	G4Box *colBasePlateMain = new G4Box("ColBasePlate", baseSide/2, baseSide/2, baseWidth/2);
	G4Box *baseHole = new G4Box("BaseHole", baseHoleSize/2, baseHoleSize/2, baseWidth/2);
	G4Transform3D transBaseHole(RMZero, G4ThreeVector(0,0,0));
	G4SubtractionSolid *colBasePlate = new G4SubtractionSolid("colBasePlate", colBasePlateMain, baseHole, transBaseHole);

	/* construct the base plate at exit 1 */
	G4Transform3D transColBasePlateEx1(RMZero, G4ThreeVector(2*electronsRadius,0,0));
	G4UnionSolid *colBasePlates = new G4UnionSolid("ColBox", colBasePlate, colBasePlate, transColBasePlateEx1);

	G4LogicalVolume *logicColBasePlates = new G4LogicalVolume(colBasePlates, GetMaterial(1), "ColBasePlates");
	G4VPhysicalVolume *phyColBasePlates     = new G4PVPlacement(0, G4ThreeVector(-electronsRadius,0,
																	-(2*neckSolid->GetZHalfLength()
																  + 2*collimOutNeckMain->GetZHalfLength()
																  + 2*colBoxMain->GetZHalfLength()
																  - thicknessOfColBox
																  - colBasePlateMain->GetZHalfLength())),
													  logicColBasePlates, "colBasePlates", logicWorld, false, 0);

	/* 2) collimators */

	int ColimMaterial = 1;

	/* Entrance */
	//
	//	Collimator 11
	//
	G4Box *solidColim11 = new G4Box("Kolimator11",colimatorX/2, colimatorY/2, colimatorZ/2);
	G4LogicalVolume *logicColim11 = new G4LogicalVolume(solidColim11, GetMaterial(ColimMaterial), "Kolimator11");
	G4VPhysicalVolume *physiColim11 = new G4PVPlacement(0, phyColBasePlates->GetObjectTranslation() +
													 G4ThreeVector((colimatorX+_collEntranceGapX)/2,
															 0,
															 (colimatorZ + baseWidth)/2 + 4*cm),
												  logicColim11,"Kolimator11", logicWorld,  false, 0);
	//
	//	Colimator 12
	//
	G4Box *solidColim12 = new G4Box("Kolimator12",colimatorX/2, colimatorY/2, colimatorZ/2);
	G4LogicalVolume *logicColim12 = new G4LogicalVolume(solidColim12, GetMaterial(ColimMaterial), "Kolimator12");
	G4VPhysicalVolume *physiColim12 = new G4PVPlacement(0, phyColBasePlates->GetObjectTranslation() +
													 G4ThreeVector(-(colimatorX+_collEntranceGapX)/2,
															 0,
															 (colimatorZ + baseWidth)/2 + 4*cm),
												  logicColim12,"Kolimator12", logicWorld,  false, 0);
	//
	//	Colimator 21
	//
	G4Box *solidColim21 = new G4Box("Kolimator21",colimatorY/2, colimatorX/2, colimatorZ/2);
	G4LogicalVolume *logicColim21 = new G4LogicalVolume(solidColim21, GetMaterial(ColimMaterial), "Kolimator21");
	G4VPhysicalVolume *physiColim21 = new G4PVPlacement(0, phyColBasePlates->GetObjectTranslation() +
													 G4ThreeVector(0,
															 (colimatorX+_collEntranceGapY)/2,
															 (colimatorZ + baseWidth)/2 + 1.8*cm),
												  logicColim21,"Kolimator21", logicWorld,  false, 0);
	//
	//	Colimator 22
	//
	G4Box *solidColim22 = new G4Box("Kolimator22",colimatorY/2, colimatorX/2, colimatorZ/2);
	G4LogicalVolume *logicColim22 = new G4LogicalVolume(solidColim22, GetMaterial(ColimMaterial), "Kolimator22");
	G4VPhysicalVolume *physiColim22 = new G4PVPlacement(0, phyColBasePlates->GetObjectTranslation() +
													 G4ThreeVector(0,
															 -(colimatorX+_collEntranceGapY)/2,
															 (colimatorZ + baseWidth)/2 + 1.8*cm),
												  logicColim22,"Kolimator22", logicWorld,  false, 0);


	/* Exit1 */
	//
	//	Collimator 11
	//
	G4Box *solidColim33 = new G4Box("Kolimator33",colimatorX/2, colimatorY/2, colimatorZ/2);
	G4LogicalVolume *logicColim33 = new G4LogicalVolume(solidColim33, GetMaterial(ColimMaterial), "Kolimator33");
	G4VPhysicalVolume *physiColim33 = new G4PVPlacement(0, phyColBasePlates->GetObjectTranslation() +
													 G4ThreeVector((colimatorX+_collExit1GapX)/2 + 2*electronsRadius,
															 0,
															 (colimatorZ + baseWidth)/2 + 4*cm),
												  logicColim33,"Kolimator33", logicWorld,  false, 0);
	//
	//	Colimator 12
	//
	G4Box *solidColim34 = new G4Box("Kolimator34",colimatorX/2, colimatorY/2, colimatorZ/2);
	G4LogicalVolume *logicColim34 = new G4LogicalVolume(solidColim34, GetMaterial(ColimMaterial), "Kolimator34");
	G4VPhysicalVolume *physiColim34 = new G4PVPlacement(0, phyColBasePlates->GetObjectTranslation() +
													 G4ThreeVector(-(colimatorX+_collExit1GapX)/2 + 2*electronsRadius,
															 0,
															 (colimatorZ + baseWidth)/2 + 4*cm),
													 logicColim34,"Kolimator43", logicWorld,  false, 0);
	//
	//	Colimator 21
	//
	G4Box *solidColim43 = new G4Box("Kolimator43",colimatorY/2, colimatorX/2, colimatorZ/2);
	G4LogicalVolume *logicColim43 = new G4LogicalVolume(solidColim43, GetMaterial(ColimMaterial), "Kolimator43");
	G4VPhysicalVolume *physiColim43 = new G4PVPlacement(0, phyColBasePlates->GetObjectTranslation() +
													 G4ThreeVector(2*electronsRadius,
															 (colimatorX+_collExit1GapY)/2,
															 (colimatorZ + baseWidth)/2 + 1.8*cm),
												  logicColim43,"Kolimator43", logicWorld,  false, 0);
	//
	//	Colimator 22
	//
	G4Box *solidColim44 = new G4Box("Kolimator44",colimatorY/2, colimatorX/2, colimatorZ/2);
	G4LogicalVolume *logicColim44 = new G4LogicalVolume(solidColim44, GetMaterial(ColimMaterial), "Kolimator44");
	G4VPhysicalVolume *physiColim44 = new G4PVPlacement(0, phyColBasePlates->GetObjectTranslation() +
													 G4ThreeVector(2*electronsRadius,
															 -(colimatorX+_collExit1GapY)/2,
															 (colimatorZ + baseWidth)/2 + 1.8*cm),
												  logicColim44,"Kolimator44", logicWorld,  false, 0);

	//
	// Beampipe
	//
	G4double distPipeToTarget = 10*cm;
	G4ThreeVector beamPipeCenter = phyTarget->GetObjectTranslation()
								 + G4ThreeVector(-(beamPipeLenght/2+distPipeToTarget)*sin(rotAngle), 0,
											   -target->GetZHalfLength() - (beamPipeLenght/2+distPipeToTarget)*cos(rotAngle));

	G4Tubs* solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
	G4LogicalVolume* logicBeamPipe = new G4LogicalVolume(solidBeamPipe, GetMaterial(2),"BeamPipe");
	G4VPhysicalVolume* physiBeamPipe = new G4PVPlacement(RM1, beamPipeCenter, logicBeamPipe, "BeamPipe",  logicWorld,false,0);

	//
	// Beampipe Vacuum
	//
	G4Tubs* solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
	G4LogicalVolume* logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV, GetMaterial(6),"BeamPipeV");
	physiBeamPipeV = new G4PVPlacement(RM1, beamPipeCenter, logicBeamPipeV,"BeamPipeV",logicWorld, false, 0);

	//
	// Detector shielding
	//

	G4Box *solidShieldBarrier1 = new G4Box("ShieldBarrier1",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
	G4LogicalVolume* logicShieldBarrier1 = new G4LogicalVolume(solidShieldBarrier1,GetMaterial(7), "ShieldBarrier1");

	G4ThreeVector detShieldCenter(0, 0, -solidShieldBarrier1->GetZHalfLength()
									  - 2*neckSolid->GetZHalfLength());
	G4VPhysicalVolume* physiShieldBarrier1 = new G4PVPlacement(0, detShieldCenter,
					 logicShieldBarrier1, "ShieldBarrier1",logicWorld, false, 0);

	//
	// Exit window
	//
	G4double exitWinRad = radNeckRing;
	G4double exitWinWidth = 100*um;
	G4ThreeVector exitWinCenter = phyTarget->GetObjectTranslation() + G4ThreeVector(2*electronsRadius, 0, target->GetZHalfLength() - exitWinWidth/2);

	G4Tubs *exitWin = new G4Tubs("ExitWindow", 0, exitWinRad, exitWinWidth/2, 0, 360*deg);
	G4LogicalVolume *logicExitWin = new G4LogicalVolume(exitWin, GetMaterial(1), "ExitWindow");
	G4VPhysicalVolume *phyExitWin = new G4PVPlacement(0, exitWinCenter, logicExitWin, "ExitWindow", logicWorld, false, 0);

	//
	// Detector
	//
	G4double detectorGap = 1*mm;
	G4ThreeVector detectorCenter = phyExitWin->GetObjectTranslation() + G4ThreeVector(0, 0, - exitWin->GetZHalfLength() - detectorGap - detectorThick/2);

	G4Tubs* solidSenDet1 = new G4Tubs("SenDet1", 0, radNeckRing, detectorThick, 0, 360.0*deg);
	G4LogicalVolume* logicSenDet1 = new G4LogicalVolume(solidSenDet1,
					 //beamVacuum,
						GetMaterial(6),
					 "SenDet1");
	G4VPhysicalVolume* physiSenDet1 = new G4PVPlacement(0,	//rotation
				   detectorCenter,
				   logicSenDet1,	//its logical volume
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

	//
	// Inner shielding
	//
	G4Box *solidInnerBotShield1 = new G4Box("InnerBotShield1",InnerBotShield1X/2, InnerBotShield1Y/2, InnerBotShield1Z/2);
	G4LogicalVolume *logicInnerBotShield1 = new G4LogicalVolume(solidInnerBotShield1,GetMaterial(7), "InnerBotShield1");
	G4RotationMatrix*RM11 = new G4RotationMatrix;
	RM11->rotateY(0*deg);
	G4VPhysicalVolume *physiInnerBotShield1 = new G4PVPlacement(RM11,
		  G4ThreeVector(InnerBotShield1_xc,InnerBotShield1_yc,InnerBotShield1_zc),
		  logicInnerBotShield1, "InnerBotShield1", logicInnerBox, false, 0);

	G4Box *solidInnerBotShield2 = new G4Box("InnerBotShield2",InnerBotShield2X/2, InnerBotShield2Y/2, InnerBotShield2Z/2);
	G4LogicalVolume *logicInnerBotShield2 = new G4LogicalVolume(solidInnerBotShield2,GetMaterial(7), "InnerBotShield2");
	G4RotationMatrix*RM12=new G4RotationMatrix;
	RM12->rotateY(0*deg);
	G4VPhysicalVolume *physiInnerBotShield2 = new G4PVPlacement(RM12,
		  G4ThreeVector(InnerBotShield2_xc,InnerBotShield2_yc,InnerBotShield2_zc),
		  logicInnerBotShield2, "InnerBotShield2", logicInnerBox, false, 0);

	G4Box *solidInnerShield3 = new G4Box("InnerShield3",InnerShield3X/2, InnerShield3Y/2, InnerShield3Z/2);
	G4LogicalVolume *logicInnerShield3 = new G4LogicalVolume(solidInnerShield3,GetMaterial(7), "InnerShield3");
	G4RotationMatrix*RM13=new G4RotationMatrix;
	RM13->rotateY(0*deg);
	G4VPhysicalVolume *physiInnerShield3 = new G4PVPlacement(RM13,
		  G4ThreeVector(InnerShield3_xc,InnerShield3_yc,InnerShield3_zc),
		  logicInnerShield3, "InnerShield3", logicInnerBox, false, 0);


	G4Box *solidInnerShield4 = new G4Box("InnerShield4",InnerShield4X/2, InnerShield4Y/2, InnerShield4Z/2);
	G4LogicalVolume *logicInnerShield4 = new G4LogicalVolume(solidInnerShield4,GetMaterial(7), "InnerShield4");
	G4RotationMatrix*RM14=new G4RotationMatrix;
	RM14->rotateY(-45*deg);
	G4VPhysicalVolume *physiInnerShield4 = new G4PVPlacement(RM14,
		  G4ThreeVector(InnerShield4_xc,InnerShield4_yc,InnerShield4_zc),
		  logicInnerShield4, "InnerShield4", logicInnerBox, false, 0);

	G4Box *solidInnerShield5 = new G4Box("InnerShield5",InnerShield5X/2, InnerShield5Y/2, InnerShield5Z/2);
	G4LogicalVolume *logicInnerShield5 = new G4LogicalVolume(solidInnerShield5,GetMaterial(7), "InnerShield5");
	G4RotationMatrix*RM15=new G4RotationMatrix;
	RM15->rotateY(45*deg);
	G4VPhysicalVolume *physiInnerShield5 = new G4PVPlacement(RM15,
		  G4ThreeVector(InnerShield5_xc,InnerShield5_yc,InnerShield5_zc),
		  logicInnerShield5, "InnerShield5", logicInnerBox, false, 0);

	//
	// Visualization attributes
	//
	G4VisAttributes* detVisAtt = new G4VisAttributes(G4Colour(0.25,0.7,0.0));
	logicSenDet1->SetVisAttributes(detVisAtt);

	G4VisAttributes* pipeVisAtt = new G4VisAttributes(G4Colour(0.5,0.0,0.6));
	logicBeamPipe->SetVisAttributes(pipeVisAtt);

	G4VisAttributes* pipeVVisAtt = new G4VisAttributes(G4Colour(0.4,0.3,0.0));
	logicBeamPipeV->SetVisAttributes(pipeVVisAtt);

	G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
	logicTarget->SetVisAttributes(cupVisAtt);
	G4VisAttributes* exitWinVisAtt = new G4VisAttributes(G4Colour(1.0,0.5,0.0));
	logicExitWin->SetVisAttributes(exitWinVisAtt);
//
//  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
//  logicChamber->SetVisAttributes(chamberVisAtt);
//
	G4VisAttributes* fieldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
	logicInnerBox->SetVisAttributes(fieldVisAtt);
//
//
//  G4VisAttributes* detVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
//  logicSenDet1->SetVisAttributes(detVisAtt);

	G4UserLimits* stepLimit = new G4UserLimits(_maxStep, _maxLength, _maxTime, _minEkin, _mionRang);
	logicWorld->SetUserLimits(stepLimit);
	logicTarget->SetUserLimits(stepLimit);
	logicChamber->SetUserLimits(stepLimit);
	logicInnerBox->SetUserLimits(stepLimit);




	//always return the physical World
	//
	return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
	fMagneticField = new G4UniformMagField(G4ThreeVector(0., _MagFieldVal, 0.));
	fFieldMgr = new G4FieldManager();
	fFieldMgr->SetDetectorField(fMagneticField);
	fFieldMgr->CreateChordFinder(fMagneticField);
	fFieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

	logicInnerBox->SetFieldManager(fFieldMgr, true);
}

void DetectorConstruction::DefineMaterials()
{
  //This function illustrates the possible ways to define materials

  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;
//  G4int iz, n;                 //iz=number of protons  in an isotope;
                               // n=number of nucleons in an isotope;

  G4int ncomponents;
  G4double fractionmass;

  //
  // define Elements and simple materials
  //

  //G4Element* H  = new G4Element("Hydrogen",  symbol="H",  z= 1.00, a= 1.0100*g/mole);
  //G4Element* C  = new G4Element("Carbon",    symbol="C",  z= 6.00, a= 12.010*g/mole);
  G4Element* N  = new G4Element("Nitrogen",  symbol="N",  z= 7.00, a= 14.010*g/mole);
  G4Element* O  = new G4Element("Oxygen",    symbol="O",  z= 8.00, a= 16.000*g/mole);
  //G4Element* Si = new G4Element("Silicon",   symbol="Si", z= 14.0, a= 28.090*g/mole);
  G4Element* Cr = new G4Element("Chromium",  symbol="Cr", z= 24.0, a= 51.996*g/mole);
  G4Element* Mn = new G4Element("Manganese", symbol="Mn", z= 25.0, a= 54.938*g/mole);
  G4Element* Fe = new G4Element("Ferum",     symbol="Fe", z= 26.0, a= 55.845*g/mole);
  G4Element* Ni = new G4Element("Nickel",    symbol="Ni", z= 28.0, a= 58.693*g/mole);

  alMy          = new G4Material("Aluminium", z=13.0, a=26.980*g/mole, density=2.700*g/cm3);
  titanMy       = new G4Material("Titan",     z=22.0, a=47.867*g/mole, density=4.506*g/cm3);

  G4Lead = new G4Material("Lead",     z=82.0, a=207.2*g/mole, density=11.3*g/cm3);

  G4NistManager* nistMan = G4NistManager::Instance();
  U = nistMan->FindOrBuildMaterial("G4_U");

  //////////////////////////Stainless Steel///////////////////////////////
  steelMy = new G4Material("steelMy", density= 7.9*g/cm3, ncomponents=4);
  steelMy->AddElement(Fe, fractionmass=0.72);
  steelMy->AddElement(Cr, fractionmass=0.18);
  steelMy->AddElement(Ni, fractionmass=0.08);
  steelMy->AddElement(Mn, fractionmass=0.02);
  ////////////////////////////////////////////////////////////////////////

  /////////////////////////////////Air////////////////////////////////////
  airMy = new G4Material("Air1", density= 1.290*mg/cm3, ncomponents=2);
  airMy->AddElement(N, fractionmass=0.7);
  airMy->AddElement(O, fractionmass=0.3);
  ///////////////////////////////////////////////////////////////////////

  /////////////////////////////beamVacuum////////////////////////////////
  beamVacuum = new G4Material("beamVacuum", density= 1.290*mg/(1.0e11)/cm3, ncomponents=2);
  beamVacuum->AddElement(N, fractionmass=0.7);
  beamVacuum->AddElement(O, fractionmass=0.3);
  ///////////////////////////////////////////////////////////////////////

  /////////////////////////////vacuumMy//////////////////////////////////
  //vacuumMy = new G4Material("vacuumMy", density= 1.290*mg/(1.0e3)/cm3, ncomponents=2);
  vacuumMy = new G4Material("vacuumMy", density= 1.290*mg/(1.0e5)/cm3, ncomponents=2);
  //vacuumMy = new G4Material("vacuumMy", density= 1.290*mg/(1.0e60)/cm3, ncomponents=2);
  vacuumMy->AddElement(N, fractionmass=0.7);
  vacuumMy->AddElement(O, fractionmass=0.3);
  ///////////////////////////////////////////////////////////////////////

  // print table
  //
  //G4cout <<*(G4Material::GetMaterialTable()) << G4endl;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}

G4Material* DetectorConstruction::GetMaterial(G4int t)
{
  switch(t)
  {
	  //test = alMy;
	case 1:
		return alMy;
		break;
	case 2:
		return steelMy;
		break;
	case 3:
		return airMy;
		break;
	case 4:
		return titanMy;
		break;
	case 5:
		return vacuumMy;
		break;
	case 6:
		return beamVacuum;
		break;
	case 7:
		return G4Lead;
		break;
	case 8:
		return U;
			break;
  default:
	  return beamVacuum;
		break;
  }

}

G4ThreeVector DetectorConstruction::GetBeamPipeCenter()
{
	return physiBeamPipeV->GetObjectTranslation();
}

G4ThreeVector DetectorConstruction::GetTargetFaceCenter()
{
	return phyTarget->GetObjectTranslation() - G4ThreeVector(0, 0, _targetThickness/2);
}


