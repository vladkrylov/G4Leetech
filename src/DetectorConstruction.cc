#include "DetectorConstruction.hh"
#include "DetectorConstructionP2.hh"
#include "DetectorConstructionP3.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

//#include "TROOT.h"
#include "TMath.h"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UserLimits.hh"


#include "MagneticField.hh"
//magnetic field
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4UserLimits.hh"


//C, C++
#include "assert.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:magField(0),
 solidWorld(0),logicWorld(0),physiWorld(0),
 solidBeamPipe(0),logicBeamPipe(0),physiBeamPipe(0),
 solidBeamPipeV(0),logicBeamPipeV(0),physiBeamPipeV(0),
 solidCup(0),logicCup(0),physiCup(0),
 solidAperture(0),logicAperture(0),physiAperture(0),
 solidSenDet1(0),logicSenDet1(0),physiSenDet1(0),
 solidShield1(0),logicShield1(0), physiShield1(0),
 solidSenDet2(0),logicSenDet2(0),physiSenDet2(0),
 solidDipol_L(0),logicDipol_L(0),physiDipol_L(0)
{
  
  // materials
  DefineMaterials();

  //swich the magneric field
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

  //LB 08.02.2012
  magField = new MagneticField();

  // create commands for interactive definition of the calorimeter
  geom4 = new DetectorConstructionP2(this, magField);
  geom7 = new DetectorConstructionP3(this, magField);
  detectorMessenger = new DetectorMessenger(this, geom4, geom7);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ 
  delete geom4;
  delete geom7;
  delete stepLimit;
  delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	switch(_geomID)
	{
	case 12:
		return ConstructGeom12();
		break;
	case 3:
		return ConstructGeom3();
		break;
	case 2:
		return ConstructGeom2();
		break;
	case 1:
		return ConstructGeom1();
		break;
	default:
		G4cout << "Can not identity of the geometry!\n";
		assert(0);
		return 0;
		break;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials(){ 
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
  airMy = new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
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
}

G4VPhysicalVolume* DetectorConstruction::ConstructGeom12(){

  const bool ColimGeom = true;
  G4double gap_between_collimators = 3*cm;

  G4double ShiftingOfAdditionalBox = _Det1X; // Distance from the edge to the beginning of the collimators system

  G4double WorldSizeX         	= 	200.0*cm;
  G4double WorldSizeY         	= 	WorldSizeX;
  G4double WorldSizeZ         	= 	WorldSizeX;

  G4double beamSize		=	2*cm;
  const G4double beamSizeMax	=	2*cm;

  G4double rotAngle	     	= 	_RotationDeg;//rotation of the PHIL pipe

  G4double ThicknesOfChamber 	=	10*mm;
  G4double ThicknesOfSmallChamber 	=	3*mm;

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

  G4double colimatorX		=	20*mm;
  G4double colimatorY		=	40*mm;
  G4double colimatorZ		=	20*mm;
  G4double colimatorZthin	=	2*mm;

  G4double chamberX		= 	400*mm+ThicknesOfChamber*2;
  G4double chamberY		= 	40*mm+ThicknesOfChamber*2;
  //G4double chamberZ		= 	fieldBoxZ + colimatorZ*2 + ThicknesOfChamber*2 + gapSize*10;
  G4double chamberZ		= 	200*mm+ThicknesOfChamber*2;

  G4double innerBoxX         	= 	chamberX-ThicknesOfChamber*2;
  G4double innerBoxY         	= 	chamberY-ThicknesOfChamber*2;
  G4double innerBoxZ         	= 	chamberZ-ThicknesOfChamber*2;

  //Sizes of the connection from chamber to collimator system
  G4double NeckX         	= 	4*cm;
  G4double NeckY         	= 	4*cm;
  G4double NeckZ         	= 	ThicknesOfChamber + ThicknesOfSmallChamber;

  G4double fieldBoxX         	= 	chamberX-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxY         	= 	chamberY-ThicknesOfChamber*2-gapSize*2;
  G4double fieldBoxZ         	= 	chamberZ-ThicknesOfChamber*2-gapSize*2 - 6*mm;

  G4double detectorRad         	= 	_Det1OutRad;
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
  G4double fieldBox_zc = 3*mm;

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

  G4double chamber_xc		= 	chamberX/4 - (ThicknesOfChamber)/2;
  G4double chamber_yc		= 	0;
  G4double chamber_zc		= 	chamberZ/2;

  G4double ShieldBarrier_xc     = 10*cm;
  G4double ShieldBarrier_xc1     =       249*mm - 75*mm;
  G4double ShieldBarrier_xc2     =       249*mm - 75*mm - 150*mm;
  G4double ShieldBarrier_yc     =       0;
  G4double ShieldBarrier_zc     =       chamber_zc-chamberZ/2-ShieldBarrierZ/2-gapSize;

  G4double Trans_x1 = -chamberX/2+AdditionalInnerBoxX/2+ShiftingOfAdditionalBox + ThicknesOfChamber;
  G4double Trans_x2 = chamberX/2-AdditionalInnerBoxX/2-ShiftingOfAdditionalBox -ThicknesOfChamber;

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

  beamSize = _apertureInRadius;

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

  beamSize = _apertureLenght;

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
		  //GetMaterial(3), air
		  GetMaterial(5),  //myvacuum
		  "World");
  physiWorld = new G4PVPlacement(0,	 G4ThreeVector(),  logicWorld, "World", 0, false, 0);

  //
  // Beampipe
  //
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe, GetMaterial(2),"BeamPipe");
  physiBeamPipe = new G4PVPlacement(RM1, G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipe, "BeamPipe",  logicWorld,false,0);

  //
  // Beampipe Vacuum
  //
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV, GetMaterial(6),"BeamPipeV");
  physiBeamPipeV = new G4PVPlacement(RM1,G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),logicBeamPipeV,"BeamPipeV",logicWorld, false, 0);

  //
  // Beampipe cup - absorber
  //

  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup, GetMaterial(1), "Cup");
  physiCup = new G4PVPlacement(RM1,G4ThreeVector(cup_xc,cup_yc,cup_zc),logicCup,"Cup",logicWorld,false,0);

  //
  //	Chamber
  //

  G4Box *solidChamber1 = new G4Box("Chamber1",chamberX/2, chamberY/2, chamberZ/2);
  G4Box *solidAdditionalBox = new G4Box("ChamberAdd",AdditionalBoxX/2, AdditionalBoxY/2, AdditionalBoxZ/2);
  G4RotationMatrix*RMZero=new G4RotationMatrix(0,0,0);
  G4Transform3D transform11(*RMZero, zTransAddBox11);
  G4Transform3D transform12(*RMZero, zTransAddBox12);
  G4UnionSolid* solidChamber2 = new G4UnionSolid("Chamber", solidChamber1, solidAdditionalBox, transform11);
  G4UnionSolid* solidChamber = new G4UnionSolid("Chamber", solidChamber2, solidAdditionalBox, transform12);

  G4LogicalVolume *logicChamber = new G4LogicalVolume(solidChamber, GetMaterial(1), "Chamber");
  G4PVPlacement *physiChamber = new G4PVPlacement(0, G4ThreeVector(chamber_xc,chamber_yc,chamber_zc), logicChamber, "Chamber",	 logicWorld, false, 0);

  //
  //	InnerBox
  //

  G4Box *solidInnerBox = new G4Box("InnerBox",innerBoxX/2, innerBoxY/2, innerBoxZ/2);
  G4Box *solidAdditionalInnerBox = new G4Box("InnerAdd",AdditionalInnerBoxX/2, AdditionalInnerBoxY/2, AdditionalInnerBoxZ/2);
  G4Box *solidNeck = new G4Box("Neck",NeckX/2, NeckY/2, NeckZ/2);
  G4Transform3D transform21(*RMZero, zTransInnerBox21);
  G4Transform3D transform22(*RMZero, zTransInnerBox22);
  G4Transform3D transNeck1(*RMZero, zNeck1);
  G4Transform3D transNeck2(*RMZero, zNeck2);
  G4UnionSolid* solidInnerBoxUNeck = new G4UnionSolid("Chamber", solidInnerBox, solidNeck, transNeck1);
  G4UnionSolid* solidInnerBoxUNeck2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck, solidNeck, transNeck2);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox = new G4UnionSolid("Chamber", solidInnerBoxUNeck2, solidAdditionalInnerBox, transform21);
  G4UnionSolid* solidInnerBoxUNeck2UAddBox2 = new G4UnionSolid("Chamber", solidInnerBoxUNeck2UAddBox, solidAdditionalInnerBox, transform22);

  G4LogicalVolume *logicInnerBox = new G4LogicalVolume(solidInnerBoxUNeck2UAddBox2, GetMaterial(5),"InnerBox");
  G4PVPlacement *physiInnerBox = new G4PVPlacement(0, G4ThreeVector(innerBox_xc,innerBox_yc,innerBox_zc), logicInnerBox, "InnerBox", logicChamber,	 false,	 0);

  //
  //	FieldBox
  //
  G4Box *solidFieldBox = new G4Box("FieldBox",fieldBoxX/2, fieldBoxY/2, fieldBoxZ/2);
  G4LogicalVolume *logicFieldBox = new G4LogicalVolume(solidFieldBox, GetMaterial(5), "FieldBox");
  G4PVPlacement *physiFieldBox = new G4PVPlacement(0, G4ThreeVector(fieldBox_xc,fieldBox_yc,fieldBox_zc), logicFieldBox,"FieldBox", logicInnerBox,	 false,	 0);

  //
  //	Colimator 11
  //
  int ColimMat = 2;
  G4Box *solidColim11 = new G4Box("Kolimator11",colimatorX/2, colimatorY/2, colimatorZ/2);
  G4LogicalVolume *logicColim11 = new G4LogicalVolume(solidColim11, GetMaterial(ColimMat), "Kolimator11");
  G4PVPlacement *physiColim11 = new G4PVPlacement(0,	G4ThreeVector(colimator_xc11,colimator_yc11,colimator_zc11), logicColim11,"Kolimator11", logicInnerBox,  false, 0);
  //
  //	Colimator 12
  //
  G4Box *solidColim12 = new G4Box("Kolimator12",colimatorX/2, colimatorY/2, colimatorZ/2);
  G4LogicalVolume *logicColim12 = new G4LogicalVolume(solidColim12, GetMaterial(ColimMat),"Kolimator12");
  G4PVPlacement *physiColim12 = new G4PVPlacement(0, G4ThreeVector(colimator_xc12,colimator_yc12,colimator_zc12), logicColim12,"Kolimator12", logicInnerBox, false, 0);
  //
  //	Colimator 21
  //
  G4Box *solidColim21 = new G4Box("Kolimator21",colimatorY/2, colimatorX/2, colimatorZ/2);
  G4LogicalVolume *logicColim21 = new G4LogicalVolume(solidColim21, GetMaterial(ColimMat), "Kolimator21");
  G4PVPlacement *physiColim21 = new G4PVPlacement(0, G4ThreeVector(colimator_xc21,colimator_yc21,colimator_zc21), logicColim21, "Kolimator21", logicInnerBox, false, 0);
  //
  //	Colimator 22
  //
  G4Box *solidColim22 = new G4Box("Kolimator22",colimatorY/2, colimatorX/2, colimatorZ/2);
  G4LogicalVolume *logicColim22 = new G4LogicalVolume(solidColim22, GetMaterial(ColimMat), "Kolimator12");
  G4PVPlacement *physiColim22 = new G4PVPlacement(0, G4ThreeVector(colimator_xc22,colimator_yc22,colimator_zc22),  logicColim22, "Kolimator22", logicInnerBox, false, 0);

  if(ColimGeom)
  {
	  //
	  //	Colimator 31
	  //
	  G4Box *solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZ/2);
	  G4LogicalVolume *logicColim31 = new G4LogicalVolume(solidColim31, GetMaterial(ColimMat),  "Kolimator31");
	  G4PVPlacement *physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31),  logicColim11, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  G4Box *solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZ/2);
	  G4LogicalVolume *logicColim32 = new G4LogicalVolume(solidColim32, GetMaterial(ColimMat), "Kolimator32");
	  G4PVPlacement *physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  G4Box *solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZ/2);
	  G4LogicalVolume *logicColim41 = new G4LogicalVolume(solidColim41, GetMaterial(ColimMat), "Kolimator41");
	  G4PVPlacement *physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  G4Box *solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZ/2);
	  G4LogicalVolume *logicColim42 = new G4LogicalVolume(solidColim42, GetMaterial(ColimMat), "Kolimator42");
	  G4PVPlacement *physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42), logicColim42,"Kolimator42", logicInnerBox, false,0);
  }
  /**/
  else
  {
	  //
	  //	Colimator 31
	  //
	  G4Box *solidColim31 = new G4Box("Kolimator31",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  G4LogicalVolume *logicColim31 = new G4LogicalVolume(solidColim31,GetMaterial(ColimMat),  "Kolimator31");
	  G4PVPlacement *physiColim31 = new G4PVPlacement(0, G4ThreeVector(colimator_xc31,colimator_yc31,colimator_zc31thin),  logicColim31, "Kolimator31", logicInnerBox, false, 0);
	  //
	  //	Colimator 32
	  //
	  G4Box *solidColim32 = new G4Box("Kolimator32",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  G4LogicalVolume *logicColim32 = new G4LogicalVolume(solidColim32,GetMaterial(ColimMat), "Kolimator32");
	  G4PVPlacement *physiColim32 = new G4PVPlacement(0, G4ThreeVector(colimator_xc32,colimator_yc32,colimator_zc32thin), logicColim32, "Kolimator32",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 41
	  //
	  G4Box *solidColim41 = new G4Box("Kolimator41",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  G4LogicalVolume *logicColim41 = new G4LogicalVolume(solidColim41,GetMaterial(ColimMat), "Kolimator41");
	  G4PVPlacement *physiColim41 = new G4PVPlacement(0, G4ThreeVector(colimator_xc41,colimator_yc41,colimator_zc41thin), logicColim41, "Kolimator41",logicInnerBox, false, 0);
	  //
	  //	Colimator 42
	  //
	  G4Box *solidColim42 = new G4Box("Kolimator42",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  G4LogicalVolume *logicColim42 = new G4LogicalVolume(solidColim42,GetMaterial(ColimMat), "Kolimator42");
	  G4PVPlacement *physiColim42 = new G4PVPlacement(0, G4ThreeVector(colimator_xc42,colimator_yc42,colimator_zc42thin), logicColim42,"Kolimator42", logicInnerBox, false,0);

	  //
	  //	Colimator 51
	  //
	  G4Box *solidColim51 = new G4Box("Kolimator51",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  G4LogicalVolume *logicColim51 = new G4LogicalVolume(solidColim51,GetMaterial(ColimMat),  "Kolimator51");
	  G4PVPlacement *physiColim51 = new G4PVPlacement(0, G4ThreeVector(colimator_xc51,colimator_yc51,colimator_zc51thin),  logicColim51, "Kolimat51", logicInnerBox, false, 0);
	  //
	  //	Colimator 52
	  //
	  G4Box *solidColim52 = new G4Box("Kolimator52",colimatorX/2, colimatorY/2, colimatorZthin/2);
	  G4LogicalVolume *logicColim52 = new G4LogicalVolume(solidColim52,GetMaterial(ColimMat), "Kolimator52");
	  G4PVPlacement *physiColim52 = new G4PVPlacement(0, G4ThreeVector(colimator_xc52,colimator_yc52,colimator_zc52thin), logicColim52, "Kolimator52",	 logicInnerBox, false, 0);
	  //
	  //	Colimator 61
	  //
	  G4Box *solidColim61 = new G4Box("Kolimator61",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  G4LogicalVolume *logicColim61 = new G4LogicalVolume(solidColim61,GetMaterial(ColimMat), "Kolimator61");
	  G4PVPlacement *physiColim61 = new G4PVPlacement(0, G4ThreeVector(colimator_xc61,colimator_yc61,colimator_zc61thin), logicColim61, "Kolimator61",logicInnerBox, false, 0);
	  //
	  //	Colimator 62
	  //
	  G4Box *solidColim62 = new G4Box("Kolimator62",colimatorY/2, colimatorX/2, colimatorZthin/2);
	  G4LogicalVolume *logicColim62 = new G4LogicalVolume(solidColim62,GetMaterial(ColimMat), "Kolimator62");
	  G4PVPlacement *physiColim62 = new G4PVPlacement(0, G4ThreeVector(colimator_xc62,colimator_yc62,colimator_zc62thin), logicColim62,"Kolimator62", logicInnerBox, false,0);
  }

  //
  //Detector
  //
  solidSenDet1 = new G4Tubs("SenDet1", 0, detectorRad, detectorThick, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     //beamVacuum,
						GetMaterial(6),
				     "SenDet1");
  physiSenDet1 = new G4PVPlacement(0,	//rotation
				   G4ThreeVector(detector_xc,detector_yc,detector_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

//  solidShieldBarrier = new G4Box("ShieldBarrier",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
//  logicShieldBarrier = new G4LogicalVolume(solidShieldBarrier,GetMaterial(7), "ShieldBarrier");
//  physiShieldBarrier = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc,ShieldBarrier_yc,ShieldBarrier_zc),
//		  logicShieldBarrier, "ShieldBarrier",logicWorld, false, 0);

  	G4Box *solidShieldBarrier1 = new G4Box("ShieldBarrier1",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
    G4LogicalVolume *logicShieldBarrier1 = new G4LogicalVolume(solidShieldBarrier1,GetMaterial(7), "ShieldBarrier1");
    G4PVPlacement *physiShieldBarrier1 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc1,ShieldBarrier_yc,ShieldBarrier_zc),
  		  logicShieldBarrier1, "ShieldBarrier1",logicWorld, false, 0);

    G4Box *solidShieldBarrier2 = new G4Box("ShieldBarrier2",ShieldBarrierX/2, ShieldBarrierY/2, ShieldBarrierZ/2);
    G4LogicalVolume *logicShieldBarrier2 = new G4LogicalVolume(solidShieldBarrier2,GetMaterial(7), "ShieldBarrier2");
    G4PVPlacement *physiShieldBarrier2 = new G4PVPlacement(0, G4ThreeVector(ShieldBarrier_xc2,ShieldBarrier_yc,ShieldBarrier_zc),
  		  logicShieldBarrier2, "ShieldBarrier2",logicWorld, false, 0);


//  solidInnerTopShield = new G4Box("InnerTopShield",InnerTopShieldX/2, InnerTopShieldY/2, InnerTopShieldZ/2);
//  logicInnerTopShield = new G4LogicalVolume(solidInnerTopShield,GetMaterial(7), "InnerTopShield");
//  physiInnerTopShield = new G4PVPlacement(0, G4ThreeVector(InnerTopShield_xc,InnerTopShield_yc,InnerTopShield_zc),
//		  logicInnerTopShield, "InnerTopShield",logicFieldBox, false, 0);

  G4Box *solidInnerBotShield1 = new G4Box("InnerBotShield1",InnerBotShield1X/2, InnerBotShield1Y/2, InnerBotShield1Z/2);
  G4LogicalVolume *logicInnerBotShield1 = new G4LogicalVolume(solidInnerBotShield1,GetMaterial(7), "InnerBotShield1");
  G4RotationMatrix*RM11=new G4RotationMatrix;
  RM11->rotateY(0*deg);
  G4PVPlacement *physiInnerBotShield1 = new G4PVPlacement(RM11,
		  G4ThreeVector(InnerBotShield1_xc,InnerBotShield1_yc,InnerBotShield1_zc),
		  logicInnerBotShield1, "InnerBotShield1",logicFieldBox, false, 0);

  G4Box *solidInnerBotShield2 = new G4Box("InnerBotShield2",InnerBotShield2X/2, InnerBotShield2Y/2, InnerBotShield2Z/2);
  G4LogicalVolume *logicInnerBotShield2 = new G4LogicalVolume(solidInnerBotShield2,GetMaterial(7), "InnerBotShield2");
  G4RotationMatrix*RM12=new G4RotationMatrix;
  RM12->rotateY(0*deg);
  G4PVPlacement *physiInnerBotShield2 = new G4PVPlacement(RM12,
		  G4ThreeVector(InnerBotShield2_xc,InnerBotShield2_yc,InnerBotShield2_zc),
		  logicInnerBotShield2, "InnerBotShield2",logicFieldBox, false, 0);

  G4Box *solidInnerShield3 = new G4Box("InnerShield3",InnerShield3X/2, InnerShield3Y/2, InnerShield3Z/2);
  G4LogicalVolume *logicInnerShield3 = new G4LogicalVolume(solidInnerShield3,GetMaterial(7), "InnerShield3");
  G4RotationMatrix*RM13=new G4RotationMatrix;
  RM13->rotateY(0*deg);
  G4PVPlacement *physiInnerShield3 = new G4PVPlacement(RM13,
		  G4ThreeVector(InnerShield3_xc,InnerShield3_yc,InnerShield3_zc),
		  logicInnerShield3, "InnerShield3",logicFieldBox, false, 0);


  G4Box *solidInnerShield4 = new G4Box("InnerShield4",InnerShield4X/2, InnerShield4Y/2, InnerShield4Z/2);
  G4LogicalVolume *logicInnerShield4 = new G4LogicalVolume(solidInnerShield4,GetMaterial(7), "InnerShield4");
  G4RotationMatrix*RM14=new G4RotationMatrix;
  RM14->rotateY(-45*deg);
  G4PVPlacement *physiInnerShield4 = new G4PVPlacement(RM14,
		  G4ThreeVector(InnerShield4_xc,InnerShield4_yc,InnerShield4_zc),
		  logicInnerShield4, "InnerShield4",logicFieldBox, false, 0);

  G4Box *solidInnerShield5 = new G4Box("InnerShield5",InnerShield5X/2, InnerShield5Y/2, InnerShield5Z/2);
  G4LogicalVolume *logicInnerShield5 = new G4LogicalVolume(solidInnerShield5,GetMaterial(7), "InnerShield5");
  G4RotationMatrix*RM15=new G4RotationMatrix;
  RM15->rotateY(45*deg);
  G4PVPlacement *physiInnerShield5 = new G4PVPlacement(RM15,
		  G4ThreeVector(InnerShield5_xc,InnerShield5_yc,InnerShield5_zc),
		  logicInnerShield5, "InnerShield5",logicFieldBox, false, 0);

  ConstructMagnet(520*mm, 50*mm, chamberZ, gapSize, ThicknesOfChamber);
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

G4VPhysicalVolume* DetectorConstruction::ConstructGeom3(){
  
  G4double WorldSizeX         = 120.0*cm;
  G4double WorldSizeY         = WorldSizeX;
  G4double WorldSizeZ         = 170.0*cm;  

  G4double gapSize            = 1.0*mm;

  G4double beamPipeLenght     = 20.0*cm;
  G4double beamPipeOutRadius  = 3.00*cm;
  G4double beamPipeInRadius   = 2.70*cm;
  G4double beamPipeVOutRadius = beamPipeInRadius - 0.1*cm;

  G4double cupOutRadius       = beamPipeOutRadius;
  G4double cupLenght          = _cupLenght;

  G4double apertureOutRadius  = 3*cm;
  G4double apertureInRadius   = _apertureInRadius;
  G4double apertureLenght     = _apertureLenght;

G4cout<<"DET In RADIUS = "<<_Det1InRad<<std::endl;
G4cout<<"DET Out RADIUS = "<<_Det1OutRad<<std::endl;

  G4double senDet1OutRadius   = WorldSizeY/2 - 0.2*cm;
  G4double senDet1InRadius    = apertureOutRadius + 0.1*cm;
  G4double senDet1Lenght      = 0.1*cm;
  G4double shield1OutRadius   = apertureOutRadius;//senDet1OutRadius;
  G4double shield1InRadius    = _apertureInRadius;//senDet1InRadius;
  G4double shield1Lenght      = 7.0*cm;

  G4double senDet2_X          = WorldSizeX - 0.5*cm;
  G4double senDet2_Y          = WorldSizeY - 0.5*cm;
  G4double senDet2_Z          = senDet1Lenght;

  //Razmer geometricheskij v ploskosti zakruchivanija electrona:  ~53 cm x ~31 cm.
  //Razmer (oblast"  polya) v ploskosti zakruchivanija electrona: ~50 cm x ~25 cm.
  //Maximal"noe pole: 570 G, eto znachit, kak uchit Alessandro, chto mozhno podnimat"  pole do 450-470 G.
  //Ponyatno, chto pole neodnorodno po krajam, no dlya Geanta eto poka, navernoe, ne nuzhno, da my i ne znaem 
  G4double dipol_LR_X         = 5.00*cm;
  G4double dipol_LR_Y         = 42.0*cm;
  G4double dipol_LR_Z         = 53.0*cm;
  G4double dipol_TB_X         = 55.0*cm;//in realyty 62 = 31 + 31 but effective size of the field + gap between two magnets
  G4double dipol_TB_Y         = 5.00*cm;
  G4double dipol_TB_Z         = 53.0*cm;

  G4double fieldBox_X         = 25.0*cm; 
  G4double fieldBox_Y         = 30.0*cm;
  G4double fieldBox_Z         = 50.0*cm;

  G4double x_tot = dipol_TB_X + gapSize + gapSize + dipol_LR_X + dipol_LR_X;
  G4double y_tot = dipol_LR_Y;
  G4double z_tot = beamPipeLenght + gapSize + cupLenght + gapSize + apertureLenght + gapSize + dipol_LR_Z;

	//Checking validity of set value
  if(WorldSizeX<x_tot){
    G4cout<<"WorldSizeX < x_tot"<<G4endl
	  <<"WorldSizeX = "<<WorldSizeX<<G4endl
	  <<"x_tot      = "<<x_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeY<y_tot){
    G4cout<<"WorldSizeY < y_tot"<<G4endl
	  <<"WorldSizeY = "<<WorldSizeY<<G4endl
	  <<"y_tot      = "<<y_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeZ<z_tot){
    G4cout<<"WorldSizeZ < z_tot"<<G4endl
	  <<"WorldSizeZ = "<<WorldSizeZ<<G4endl
	  <<"z_tot      = "<<z_tot<<G4endl;
    assert(0);
  }
  if(dipol_TB_X<fieldBox_X*2){
    G4cout<<"dipol_TB_X   < fieldBox_X*2"<<G4endl
	  <<"dipol_TB_X   = "<<dipol_TB_X<<G4endl
	  <<"fieldBox_X*2 = "<<fieldBox_X*2<<G4endl;
    assert(0);
  }
  if(dipol_LR_Z<fieldBox_Z){
    G4cout<<"dipol_LR_Z < fieldBox_Z"<<G4endl
	  <<"dipol_LR_Z = "<<dipol_LR_Z<<G4endl
	  <<"fieldBox_Z = "<<fieldBox_Z<<G4endl;
    assert(0);
  }
  if((dipol_LR_Y - dipol_TB_Y*2)<fieldBox_Y){
    G4cout<<"(dipol_LR_Y - dipol_TB_Y*2) < fieldBox_Y"<<G4endl
	  <<" dipol_LR_Y - dipol_TB_Y*2  = "<<dipol_LR_Y - dipol_TB_Y*2<<G4endl
	  <<" fieldBox_Y                 = "<<fieldBox_Y<<G4endl;
    assert(0);
  }
  
// setting constans of geometry
  G4double beamPipe_xc = 0.0;
  G4double beamPipe_yc = 0.0;
  G4double beamPipe_zc = -cupLenght/2.0 - gapSize - beamPipeLenght/2.0;

  G4double cup_xc      = 0.0;
  G4double cup_yc      = 0.0;
  G4double cup_zc      = 0.0;

  G4double aperture_xc = 0.0;
  G4double aperture_yc = 0.0;
  G4double aperture_zc = cupLenght/2.0 + gapSize + apertureLenght/2.0;

  G4double dipol_L_xc  = beamPipeOutRadius + gapSize + dipol_LR_X/2.0;
  G4double dipol_L_yc  = 0.0;
  G4double dipol_L_zc  = cupLenght/2.0 + gapSize + apertureLenght + gapSize + dipol_LR_Z/2.0;

  G4double dipol_R_xc  = dipol_L_xc - dipol_LR_X/2.0 - gapSize - dipol_TB_X - gapSize - dipol_LR_X/2.0;
  G4double dipol_R_yc  = 0.0;
  G4double dipol_R_zc  = dipol_L_zc;

  G4double dipol_T_xc  = dipol_L_xc - gapSize - dipol_LR_X/2.0 - dipol_TB_X/2.0;
  G4double dipol_T_yc  = dipol_LR_Y/2.0 - dipol_TB_Y/2.0;
  G4double dipol_T_zc  = dipol_L_zc;

  G4double dipol_B_xc  = dipol_T_xc;
  G4double dipol_B_yc  = -dipol_LR_Y/2.0 + dipol_TB_Y/2.0;
  G4double dipol_B_zc  = dipol_L_zc;

  G4double fieldBox_L_xc = dipol_L_xc - dipol_LR_X/2.0 - gapSize - fieldBox_X/2.0;
  G4double fieldBox_L_yc = 0.0;
  G4double fieldBox_L_zc = cupLenght/2.0 + gapSize + apertureLenght + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;

  G4double fieldBox_R_xc = dipol_R_xc + dipol_LR_X/2.0 + gapSize + fieldBox_X/2.0;
  G4double fieldBox_R_yc = 0.0;
  G4double fieldBox_R_zc = cupLenght/2.0 + gapSize + apertureLenght + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;

  G4double senDet1_xc    = 0.0;
  G4double senDet1_yc    = 0.0;
  G4double senDet1_zc    = aperture_zc + apertureLenght/2.0 - senDet1Lenght/2.0;

  G4double shield1_xc    = 0.0;
  G4double shield1_yc    = 0.0;
  G4double shield1_zc    = senDet1_zc - gapSize*3 - shield1Lenght/2.0;

  G4double senDet2_xc    = 0.0;
  G4double senDet2_yc    = 0.0;
  G4double senDet2_zc    = dipol_T_zc + dipol_TB_Z/2 + gapSize + senDet2_Z/2.0;

  

  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox1(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc, 
			 fieldBox_X, fieldBox_Y, fieldBox_Z);
  magField->setFieldBox2(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc, 
			 fieldBox_X, fieldBox_Y, fieldBox_Z);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
   
  //     
  // World
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"World material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);                     
  logicWorld = new G4LogicalVolume(solidWorld,	    //its solid
				   vacuumMy,        //its material
				   "World");	    //its name                                   
  physiWorld = new G4PVPlacement(0,		    //no rotation
  				 G4ThreeVector(),   //at (0,0,0)
				 logicWorld,	    //its logical volume				 
				 "World",	    //its name
				 0,		    //its mother  volume
				 false,		    //no boolean operation
				 0);		    //copy number

  //
  // Beampipe
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Beampipe material"<<G4endl
	//<<steelMy<<G4endl;
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,	//its solid
				      steelMy,         	//its material
				      "BeamPipe");	//its name
  physiBeamPipe = new G4PVPlacement(0,			//no rotation
				    G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),	//its position
				    logicBeamPipe,	//its logical volume				 
				    "BeamPipe",		//its name
				    logicWorld,		//its mother  volume
				    false,		//no boolean operation
				    0);			//copy number

  //
  // Beampipe Vacuum
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Beampipe Vacuum material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,	//its solid
				       beamVacuum,      //its material
				       "BeamPipeV");	//its name
  physiBeamPipeV = new G4PVPlacement(0,			//no rotation
				     G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc), //its position
				     logicBeamPipeV,	//its logical volume				 
				     "BeamPipeV",	//its name
				     logicWorld,	//its mother  volume
				     false,		//no boolean operation
				     0);		//copy number

  //
  // Beampipe cup - absorber
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Cup"<<G4endl
	//<<alMy<<G4endl;
  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,		//its solid
				 alMy,	//its material
				 "Cup");		//its name  
  physiCup = new G4PVPlacement(0,			//no rotation
			       G4ThreeVector(cup_xc,cup_yc,cup_zc),//its position
			       logicCup,		//its logical volume				 
			       "Cup",		        //its name
			       logicWorld,		//its mother  volume
			       false,			//no boolean operation
			       0);		        //copy number

  //
  //Aperture -colim
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"aperture"<<G4endl
	//<<alMy<<G4endl;
  solidAperture = new G4Tubs("Aperture", apertureInRadius, apertureOutRadius, apertureLenght/2.0, 0, 360.0*deg);
  logicAperture = new G4LogicalVolume(solidAperture,	    //its solid
				      alMy,	            //its material
				      "Aperture");          //its name  
  physiAperture = new G4PVPlacement(0,			    //no rotation
				    G4ThreeVector(aperture_xc,aperture_yc,aperture_zc),//its position
				    logicAperture,	    //its logical volume				 
				    "Cup",		    //its name
				    logicWorld,		    //its mother  volume
				    false,		    //no boolean operation
				    0);		            //copy number



  //     
  // Dipol left - blue
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol left  material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_L = new G4Box("Dipol_L",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_L = new G4LogicalVolume(solidDipol_L,  //its solid
				     alMy,          //its material
				     "Dipol_L");    //its name
  physiDipol_L = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_L_xc, dipol_L_yc, dipol_L_zc),   //at (0,0,0)
				   logicDipol_L,	    //its logical volume
				   "Dipol_L",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol right
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol right material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_R = new G4Box("Dipol_R",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_R = new G4LogicalVolume(solidDipol_R,  //its solid
				     alMy,          //its material
				     "Dipol_R");    //its name
  physiDipol_R = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_R_xc, dipol_R_yc, dipol_R_zc),   //at (0,0,0)
				   logicDipol_R,	    //its logical volume
				   "Dipol_R",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol top
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol top material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_T = new G4Box("Dipol_T",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_T = new G4LogicalVolume(solidDipol_T,  //its solid
				     alMy,          //its material
				     "Dipol_T");    //its name
  physiDipol_T = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_T_xc, dipol_T_yc, dipol_T_zc),   //at (0,0,0)
				   logicDipol_T,    //its logical volume
				   "Dipol_T",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol bottom
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol bottom material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_B = new G4Box("Dipol_B",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_B = new G4LogicalVolume(solidDipol_B,  //its solid
				     alMy,          //its material
				     "Dipol_B");    //its name
  physiDipol_B = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_B_xc, dipol_B_yc, dipol_B_zc),   //at (0,0,0)
				   logicDipol_B,    //its logical volume
				   "Dipol_B",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Field box left
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Field box left  material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidFieldBox_L = new G4Box("FieldBox_L",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_L = new G4LogicalVolume(solidFieldBox_L,    //its solid
					vacuumMy,           //its material
					"FieldBox_L");      //its name
  physiFieldBox_L = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc),   //its position
				      logicFieldBox_L,	    //its logical volume
				      "FieldBox_L",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  //     
  // Field box right
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Field box left  material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidFieldBox_R = new G4Box("FieldBox_R",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_R = new G4LogicalVolume(solidFieldBox_R,    //its solid
					vacuumMy,           //its material
					"FieldBox_R");      //its name
  physiFieldBox_R = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc),   //its position
				      logicFieldBox_R,	    //its logical volume
				      "FieldBox_R",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  
  //     
  // Sensative detector 1
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Sensative detector 1 material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidSenDet1 = new G4Tubs("SenDet1", senDet1InRadius, senDet1OutRadius, senDet1Lenght/2.0, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     beamVacuum,
				     "SenDet1");
  
  physiSenDet1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(senDet1_xc,senDet1_yc,senDet1_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume				 
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

   
  // Shield 1
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Shield 1 1 material"<<G4endl
	//<<alMy<<G4endl;

  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Shield size"<<G4endl
	//<<shield1InRadius<<"  "<<shield1OutRadius<<G4endl;

  solidShield1 = new G4Tubs("Shield1", shield1InRadius, shield1OutRadius, shield1Lenght/2.0, 0, 360.0*deg);
  logicShield1 = new G4LogicalVolume(solidShield1,
				     alMy,
				     "Shield1");
  
  physiShield1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(shield1_xc,shield1_yc,shield1_zc), //at (0,0,0)
				   logicShield1,	//its logical volume				 
				   "Shield1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number


  //     
  // Sensative detector 2
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Sensative detector 2 material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidSenDet2 = new G4Box("SenDet2", senDet2_X/2, senDet2_Y/2, senDet2_Z/2);
  logicSenDet2 = new G4LogicalVolume(solidSenDet2,  //its solid
				     beamVacuum,    //its material
				     "SenDet2");    //its name
  physiSenDet2 = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(senDet2_xc, senDet2_yc, senDet2_zc),   //its position
				   logicSenDet2,    //its logical volume
				   "SenDet2",	    //its name
				   logicWorld,	    //its mother  volume
				   false,     	    //no boolean operation
				   0);		    //copy number

  //                                        
  // Visualization attributes
  //

  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);

  G4VisAttributes* alVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicBeamPipe->SetVisAttributes(alVisAtt);
  logicDipol_L->SetVisAttributes(alVisAtt);
  logicDipol_R->SetVisAttributes(alVisAtt);
  logicDipol_T->SetVisAttributes(alVisAtt);
  logicDipol_B->SetVisAttributes(alVisAtt);

  G4VisAttributes* appVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicAperture->SetVisAttributes(appVisAtt);

  G4VisAttributes* magVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicFieldBox_L->SetVisAttributes(magVisAtt);
  logicFieldBox_R->SetVisAttributes(magVisAtt);

  G4VisAttributes* beamVacuumVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  logicBeamPipeV->SetVisAttributes(beamVacuumVisAtt);

  //Slate Blue
  G4VisAttributes* senDetVisAtt = new G4VisAttributes(G4Colour(106.0/255.0, 90.0/255.0, 205.0/255.0));
  logicSenDet1->SetVisAttributes(senDetVisAtt);
  logicSenDet2->SetVisAttributes(senDetVisAtt);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

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

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  logicWorld->SetUserLimits(stepLimit);
  logicFieldBox_L->SetUserLimits(stepLimit);
  logicFieldBox_R->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);

  //
  //always return the physical World
  //
  return physiWorld;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume* DetectorConstruction::ConstructGeom2(){

	//magField = new MagneticField();

  G4double WorldSizeX         = 120.0*cm;
  G4double WorldSizeY         = WorldSizeX;
  G4double WorldSizeZ         = 170.0*cm;  

  G4double gapSize            = 1.0*mm;

  G4double beamPipeLenght     = 20.0*cm;
  G4double beamPipeOutRadius  = 3.00*cm;
  G4double beamPipeInRadius   = 2.70*cm;
  G4double beamPipeVOutRadius = beamPipeInRadius - 0.1*cm;

  G4double cupOutRadius       = beamPipeOutRadius;
  G4double cupLenght          = _cupLenght;

  G4double apertureOutRadius  = beamPipeOutRadius;
  G4double apertureInRadius   = _apertureInRadius;
  G4double apertureLenght     = _apertureLenght;

  G4double senDet1OutRadius   = _Det1OutRad;//WorldSizeY/2 - 0.2*cm;
  G4double senDet1InRadius    = _Det1InRad;
  G4double senDet1Lenght      = 0.1*cm;

  G4double shield1OutRadius   = beamPipeOutRadius;
  G4double shield1InRadius    = senDet1OutRadius;//_Det1OutRad;
  G4double shield1Lenght      = 1.0*cm;

  //Razmer geometricheskij v ploskosti zakruchivanija electrona:  ~53 cm x ~31 cm.
  //Razmer (oblast"  polya) v ploskosti zakruchivanija electrona: ~50 cm x ~25 cm.
  //Maximal"noe pole: 570 G, eto znachit, kak uchit Alessandro, chto mozhno podnimat"  pole do 450-470 G.
  //Ponyatno, chto pole neodnorodno po krajam, no dlya Geanta eto poka, navernoe, ne nuzhno, da my i ne znaem 
  G4double dipol_LR_X         = 5.00*cm;
  G4double dipol_LR_Y         = 42.0*cm;
  G4double dipol_LR_Z         = 53.0*cm;
  G4double dipol_TB_X         = 55.0*cm;//in realyty 62 = 31 + 31 but effective size of the field + gap between two magnets
  G4double dipol_TB_Y         = 5.00*cm;
  G4double dipol_TB_Z         = 53.0*cm;

  G4double fieldBox_X         = 25.0*cm; 
  G4double fieldBox_Y         = 30.0*cm;
  G4double fieldBox_Z         = 50.0*cm;

  G4double x_tot = dipol_TB_X + gapSize + gapSize + dipol_LR_X + dipol_LR_X;
  G4double y_tot = dipol_LR_Y;
  G4double z_tot = beamPipeLenght + gapSize + cupLenght + gapSize + apertureLenght + gapSize + dipol_LR_Z;

	//Checking validity of set value
  if (WorldSizeX<x_tot) {
    G4cout<<"WorldSizeX < x_tot"<<G4endl
	  <<"WorldSizeX = "<<WorldSizeX<<G4endl
	  <<"x_tot      = "<<x_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeY<y_tot){
    G4cout<<"WorldSizeY < y_tot"<<G4endl
	  <<"WorldSizeY = "<<WorldSizeY<<G4endl
	  <<"y_tot      = "<<y_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeZ<z_tot){
    G4cout<<"WorldSizeZ < z_tot"<<G4endl
	  <<"WorldSizeZ = "<<WorldSizeZ<<G4endl
	  <<"z_tot      = "<<z_tot<<G4endl;
    assert(0);
  }
  if(dipol_TB_X<fieldBox_X*2){
    G4cout<<"dipol_TB_X   < fieldBox_X*2"<<G4endl
	  <<"dipol_TB_X   = "<<dipol_TB_X<<G4endl
	  <<"fieldBox_X*2 = "<<fieldBox_X*2<<G4endl;
    assert(0);
  }
  if(dipol_LR_Z<fieldBox_Z){
    G4cout<<"dipol_LR_Z < fieldBox_Z"<<G4endl
	  <<"dipol_LR_Z = "<<dipol_LR_Z<<G4endl
	  <<"fieldBox_Z = "<<fieldBox_Z<<G4endl;
    assert(0);
  }
  if((dipol_LR_Y - dipol_TB_Y*2)<fieldBox_Y){
    G4cout<<"(dipol_LR_Y - dipol_TB_Y*2) < fieldBox_Y"<<G4endl
	  <<" dipol_LR_Y - dipol_TB_Y*2  = "<<dipol_LR_Y - dipol_TB_Y*2<<G4endl
	  <<" fieldBox_Y                 = "<<fieldBox_Y<<G4endl;
    assert(0);
  }
  
// setting constans of geometry
  G4double beamPipe_xc = 0.0;
  G4double beamPipe_yc = 0.0;
  G4double beamPipe_zc = -cupLenght/2.0 - gapSize - beamPipeLenght/2.0;

  G4double cup_xc      = 0.0;
  G4double cup_yc      = 0.0;
  G4double cup_zc      = 0.0;

  G4double aperture_xc = 0.0;
  G4double aperture_yc = 0.0;
  G4double aperture_zc = cupLenght/2.0 + gapSize + apertureLenght/2.0;

  G4double dipol_L_xc  = beamPipeOutRadius + gapSize + dipol_LR_X/2.0;
  G4double dipol_L_yc  = 0.0;
  G4double dipol_L_zc  = cupLenght/2.0 + gapSize + apertureLenght + gapSize + dipol_LR_Z/2.0;

  G4double dipol_R_xc  = dipol_L_xc - dipol_LR_X/2.0 - gapSize - dipol_TB_X - gapSize - dipol_LR_X/2.0;
  G4double dipol_R_yc  = 0.0;
  G4double dipol_R_zc  = dipol_L_zc;

  G4double dipol_T_xc  = dipol_L_xc - gapSize - dipol_LR_X/2.0 - dipol_TB_X/2.0;
  G4double dipol_T_yc  = dipol_LR_Y/2.0 - dipol_TB_Y/2.0;
  G4double dipol_T_zc  = dipol_L_zc;

  G4double dipol_B_xc  = dipol_T_xc;
  G4double dipol_B_yc  = -dipol_LR_Y/2.0 + dipol_TB_Y/2.0;
  G4double dipol_B_zc  = dipol_L_zc;

  G4double fieldBox_L_xc = dipol_L_xc - dipol_LR_X/2.0 - gapSize - fieldBox_X/2.0;
  G4double fieldBox_L_yc = 0.0;
  G4double fieldBox_L_zc = cupLenght/2.0 + gapSize + apertureLenght + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;

  G4double fieldBox_R_xc = dipol_R_xc + dipol_LR_X/2.0 + gapSize + fieldBox_X/2.0;
  G4double fieldBox_R_yc = 0.0;
  G4double fieldBox_R_zc = cupLenght/2.0 + gapSize + apertureLenght + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;

  G4double senDet1_xc    = -_Det1X;
  G4double senDet1_yc    = 0.0;
  G4double senDet1_zc    = gapSize - shield1Lenght + cupLenght/2 + apertureLenght - senDet1Lenght/2.0;

  G4double shield1_xc    = -_Det1X;
  G4double shield1_yc    = 0.0;
  G4double shield1_zc    = gapSize - shield1Lenght/2.0 + cupLenght/2 + apertureLenght;


  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox1(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc, 
			 fieldBox_X, fieldBox_Y, fieldBox_Z);
  magField->setFieldBox2(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc, 
			 fieldBox_X, fieldBox_Y, fieldBox_Z);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
   
  //     
  // World
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"World material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);                     
  logicWorld = new G4LogicalVolume(solidWorld,	    //its solid
				   vacuumMy,        //its material
				   "World");	    //its name                                   
  physiWorld = new G4PVPlacement(0,		    //no rotation
  				 G4ThreeVector(),   //at (0,0,0)
				 logicWorld,	    //its logical volume				 
				 "World",	    //its name
				 0,		    //its mother  volume
				 false,		    //no boolean operation
				 0);		    //copy number

  //
  // Beampipe
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Beampipe material"<<G4endl
	//<<steelMy<<G4endl;
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,	//its solid
				      steelMy,         	//its material
				      "BeamPipe");	//its name
  physiBeamPipe = new G4PVPlacement(0,			//no rotation
				    G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),	//its position
				    logicBeamPipe,	//its logical volume				 
				    "BeamPipe",		//its name
				    logicWorld,		//its mother  volume
				    false,		//no boolean operation
				    0);			//copy number

  //
  // Beampipe Vacuum
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Beampipe Vacuum material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,	//its solid
				       beamVacuum,      //its material
				       "BeamPipeV");	//its name
  physiBeamPipeV = new G4PVPlacement(0,			//no rotation
				     G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc), //its position
				     logicBeamPipeV,	//its logical volume				 
				     "BeamPipeV",	//its name
				     logicWorld,	//its mother  volume
				     false,		//no boolean operation
				     0);		//copy number

  //
  // Beampipe cup - absorber
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Cup"<<G4endl
	//<<alMy<<G4endl;
  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,		//its solid
				 alMy,	//its material
				 "Cup");		//its name  
  physiCup = new G4PVPlacement(0,			//no rotation
			       G4ThreeVector(cup_xc,cup_yc,cup_zc),//its position
			       logicCup,		//its logical volume				 
			       "Cup",		        //its name
			       logicWorld,		//its mother  volume
			       false,			//no boolean operation
			       0);		        //copy number

  //
  //Aperture -colim
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"aperture"<<G4endl
	//<<alMy<<G4endl;
  solidAperture = new G4Tubs("Aperture", apertureInRadius, apertureOutRadius, apertureLenght/2.0, 0, 360.0*deg);
  logicAperture = new G4LogicalVolume(solidAperture,	    //its solid
				      alMy,	            //its material
				      "Aperture");          //its name  
  physiAperture = new G4PVPlacement(0,			    //no rotation
				    G4ThreeVector(aperture_xc,aperture_yc,aperture_zc),//its position
				    logicAperture,	    //its logical volume				 
				    "Cup",		    //its name
				    logicWorld,		    //its mother  volume
				    false,		    //no boolean operation
				    0);		            //copy number



  //     
  // Dipol left - blue
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol left  material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_L = new G4Box("Dipol_L",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_L = new G4LogicalVolume(solidDipol_L,  //its solid
				     alMy,          //its material
				     "Dipol_L");    //its name
  physiDipol_L = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_L_xc, dipol_L_yc, dipol_L_zc),   //at (0,0,0)
				   logicDipol_L,	    //its logical volume
				   "Dipol_L",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol right
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol right material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_R = new G4Box("Dipol_R",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_R = new G4LogicalVolume(solidDipol_R,  //its solid
				     alMy,          //its material
				     "Dipol_R");    //its name
  physiDipol_R = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_R_xc, dipol_R_yc, dipol_R_zc),   //at (0,0,0)
				   logicDipol_R,	    //its logical volume
				   "Dipol_R",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol top
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol top material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_T = new G4Box("Dipol_T",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_T = new G4LogicalVolume(solidDipol_T,  //its solid
				     alMy,          //its material
				     "Dipol_T");    //its name
  physiDipol_T = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_T_xc, dipol_T_yc, dipol_T_zc),   //at (0,0,0)
				   logicDipol_T,    //its logical volume
				   "Dipol_T",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol bottom
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol bottom material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_B = new G4Box("Dipol_B",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_B = new G4LogicalVolume(solidDipol_B,  //its solid
				     alMy,          //its material
				     "Dipol_B");    //its name
  physiDipol_B = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_B_xc, dipol_B_yc, dipol_B_zc),   //at (0,0,0)
				   logicDipol_B,    //its logical volume
				   "Dipol_B",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Field box left
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Field box left  material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidFieldBox_L = new G4Box("FieldBox_L",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_L = new G4LogicalVolume(solidFieldBox_L,    //its solid
					vacuumMy,           //its material
					"FieldBox_L");      //its name
  physiFieldBox_L = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc),   //its position
				      logicFieldBox_L,	    //its logical volume
				      "FieldBox_L",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  //     
  // Field box right
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Field box left  material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidFieldBox_R = new G4Box("FieldBox_R",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_R = new G4LogicalVolume(solidFieldBox_R,    //its solid
					vacuumMy,           //its material
					"FieldBox_R");      //its name
  physiFieldBox_R = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc),   //its position
				      logicFieldBox_R,	    //its logical volume
				      "FieldBox_R",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  
  //     
  // Sensative detector 1
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Sensative detector 1 material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidSenDet1 = new G4Tubs("SenDet1", senDet1InRadius, senDet1OutRadius, senDet1Lenght/2.0, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     beamVacuum,
				     "SenDet1");
  
  physiSenDet1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(senDet1_xc,senDet1_yc,senDet1_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume				 
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

  //     
  // Shield 1
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Shield 1 1 material"<<G4endl
	//<<alMy<<G4endl;
  solidShield1 = new G4Tubs("Shield1", shield1InRadius, shield1OutRadius, shield1Lenght/2.0, 0, 360.0*deg);
  logicShield1 = new G4LogicalVolume(solidShield1,
				     alMy,
				     "Shield1");
  
  physiShield1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(shield1_xc,shield1_yc,shield1_zc), //at (0,0,0)
				   logicShield1,	//its logical volume				 
				   "Shield1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

/*
  //     
  // Sensative detector 2
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Sensative detector 2 material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidSenDet2 = new G4Box("SenDet2", senDet2_X/2, senDet2_Y/2, senDet2_Z/2);
  logicSenDet2 = new G4LogicalVolume(solidSenDet2,  //its solid
				     beamVacuum,    //its material
				     "SenDet2");    //its name
  physiSenDet2 = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(senDet2_xc, senDet2_yc, senDet2_zc),   //its position
				   logicSenDet2,    //its logical volume
				   "SenDet2",	    //its name
				   logicWorld,	    //its mother  volume
				   false,     	    //no boolean operation
				   0);		    //copy number
*/
  //                                        
  // Visualization attributes
  //

  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);

  G4VisAttributes* alVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicBeamPipe->SetVisAttributes(alVisAtt);
  logicDipol_L->SetVisAttributes(alVisAtt);
  logicDipol_R->SetVisAttributes(alVisAtt);
  logicDipol_T->SetVisAttributes(alVisAtt);
  logicDipol_B->SetVisAttributes(alVisAtt);

  G4VisAttributes* appVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicAperture->SetVisAttributes(appVisAtt);

  G4VisAttributes* magVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicFieldBox_L->SetVisAttributes(magVisAtt);
  logicFieldBox_R->SetVisAttributes(magVisAtt);

  G4VisAttributes* beamVacuumVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  logicBeamPipeV->SetVisAttributes(beamVacuumVisAtt);

  //Slate Blue
  G4VisAttributes* senDetVisAtt = new G4VisAttributes(G4Colour(106.0/255.0, 90.0/255.0, 205.0/255.0));
  logicSenDet1->SetVisAttributes(senDetVisAtt);
 // logicSenDet2->SetVisAttributes(senDetVisAtt);

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
  logicFieldBox_L->SetUserLimits(stepLimit);
  logicFieldBox_R->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}

G4VPhysicalVolume* DetectorConstruction::ConstructGeom1(){

	//magField = new MagneticField();
  
  G4double WorldSizeX         = 120.0*cm;
  G4double WorldSizeY         = WorldSizeX;
  G4double WorldSizeZ         = 170.0*cm;  

  G4double gapSize            = 1.0*mm;

  G4double beamPipeLenght     = 20.0*cm;
  G4double beamPipeOutRadius  = 3.00*cm;
  G4double beamPipeInRadius   = 2.70*cm;
  G4double beamPipeVOutRadius = beamPipeInRadius - 0.1*cm;

  G4double cupOutRadius       = beamPipeOutRadius;
  //G4double cupLenght          = 0.4*cm;
  G4double cupLenght          = _cupLenght;

  G4double apertureLenght     = _apertureLenght;

  G4double senDet1OutRadius   =_Det1OutRad;//WorldSizeY/2 - 0.2*cm; 
  G4double senDet1InRadius    = _Det1InRad;
  G4double senDet1Lenght      = 10*cm;

  //Razmer geometricheskij v ploskosti zakruchivanija electrona:  ~53 cm x ~31 cm.
  //Razmer (oblast"  polya) v ploskosti zakruchivanija electrona: ~50 cm x ~25 cm.
  //Maximal"noe pole: 570 G, eto znachit, kak uchit Alessandro, chto mozhno podnimat"  pole do 450-470 G.
  //Ponyatno, chto pole neodnorodno po krajam, no dlya Geanta eto poka, navernoe, ne nuzhno, da my i ne znaem 
  G4double dipol_LR_X         = 5.00*cm;
  G4double dipol_LR_Y         = 42.0*cm;
  G4double dipol_LR_Z         = 53.0*cm;
  G4double dipol_TB_X         = 55.0*cm;//in realyty 62 = 31 + 31 but effective size of the field + gap between two magnets
  G4double dipol_TB_Y         = 5.00*cm;
  G4double dipol_TB_Z         = 53.0*cm;

  G4double fieldBox_X         = 25.0*cm; 
  G4double fieldBox_Y         = 30.0*cm;
  G4double fieldBox_Z         = 50.0*cm;

  G4double x_tot = dipol_TB_X + gapSize + gapSize + dipol_LR_X + dipol_LR_X;
  G4double y_tot = dipol_LR_Y;
  G4double z_tot = beamPipeLenght + gapSize + cupLenght + gapSize + apertureLenght + gapSize + dipol_LR_Z;

	//Checking validity of set value
  if(WorldSizeX<x_tot){
    G4cout<<"WorldSizeX < x_tot"<<G4endl
	  <<"WorldSizeX = "<<WorldSizeX<<G4endl
	  <<"x_tot      = "<<x_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeY<y_tot){
    G4cout<<"WorldSizeY < y_tot"<<G4endl
	  <<"WorldSizeY = "<<WorldSizeY<<G4endl
	  <<"y_tot      = "<<y_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeZ<z_tot){
    G4cout<<"WorldSizeZ < z_tot"<<G4endl
	  <<"WorldSizeZ = "<<WorldSizeZ<<G4endl
	  <<"z_tot      = "<<z_tot<<G4endl;
    assert(0);
  }
  if(dipol_TB_X<fieldBox_X*2){
    G4cout<<"dipol_TB_X   < fieldBox_X*2"<<G4endl
	  <<"dipol_TB_X   = "<<dipol_TB_X<<G4endl
	  <<"fieldBox_X*2 = "<<fieldBox_X*2<<G4endl;
    assert(0);
  }
  if(dipol_LR_Z<fieldBox_Z){
    G4cout<<"dipol_LR_Z < fieldBox_Z"<<G4endl
	  <<"dipol_LR_Z = "<<dipol_LR_Z<<G4endl
	  <<"fieldBox_Z = "<<fieldBox_Z<<G4endl;
    assert(0);
  }
  if((dipol_LR_Y - dipol_TB_Y*2)<fieldBox_Y){
    G4cout<<"(dipol_LR_Y - dipol_TB_Y*2) < fieldBox_Y"<<G4endl
	  <<" dipol_LR_Y - dipol_TB_Y*2  = "<<dipol_LR_Y - dipol_TB_Y*2<<G4endl
	  <<" fieldBox_Y                 = "<<fieldBox_Y<<G4endl;
    assert(0);
  }
  
// setting constans of geometry
//  G4double beamPipe_xc = 0.0;
//  G4double beamPipe_yc = 0.0;
//  G4double beamPipe_zc = -cupLenght/2.0 - gapSize - beamPipeLenght/2.0;
//
//  G4double cup_xc      = 0.0;
//  G4double cup_yc      = 0.0;
//  G4double cup_zc      = 0.0;
//
//  //G4double aperture_xc = -_Det1X;//16.7*cm;
//  //G4double aperture_yc = 0.0;
//  G4double aperture_zc = - apertureLenght/2.0 + gapSize + cupLenght/2.0;
//
//  G4double dipol_L_xc  = beamPipeOutRadius + gapSize + dipol_LR_X/2.0;
//  G4double dipol_L_yc  = 0.0;
//  G4double dipol_L_zc  = cupLenght/2.0 + gapSize + dipol_LR_Z/2.0;
//
//  G4double dipol_R_xc  = dipol_L_xc - dipol_LR_X/2.0 - gapSize - dipol_TB_X - gapSize - dipol_LR_X/2.0;
//  G4double dipol_R_yc  = 0.0;
//  G4double dipol_R_zc  = dipol_L_zc;
//
//  G4double dipol_T_xc  = dipol_L_xc - gapSize - dipol_LR_X/2.0 - dipol_TB_X/2.0;
//  G4double dipol_T_yc  = dipol_LR_Y/2.0 - dipol_TB_Y/2.0;
//  G4double dipol_T_zc  = dipol_L_zc;
//
//  G4double dipol_B_xc  = dipol_T_xc;
//  G4double dipol_B_yc  = -dipol_LR_Y/2.0 + dipol_TB_Y/2.0;
//  G4double dipol_B_zc  = dipol_L_zc;
//
//  G4double fieldBox_L_xc = dipol_L_xc - dipol_LR_X/2.0 - gapSize - fieldBox_X/2.0;
//  G4double fieldBox_L_yc = 0.0;
//  G4double fieldBox_L_zc = cupLenght/2.0 + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;
//
//  G4double fieldBox_R_xc = dipol_R_xc + dipol_LR_X/2.0 + gapSize + fieldBox_X/2.0;
//  G4double fieldBox_R_yc = 0.0;
//  G4double fieldBox_R_zc = cupLenght/2.0 + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;
//
  G4double targetThickness = 2*mm;
  G4double targetRadius = 2*mm;

  G4double senDetThickness = 5*mm;
  G4double senDetRadius = 30*mm;

  G4double l1 = 0*mm;
  G4double l2 = 5*mm;

  G4double target_xc    = 0;
  G4double target_yc    = 0;
  G4double target_zc    = targetThickness/2 + l1;
  G4double senDet1_zc    = l1 + targetThickness + l2 + senDetThickness/2;


  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
   
  //     
  // World
  //
  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);                     
  logicWorld = new G4LogicalVolume(solidWorld,	    //its solid
				   vacuumMy,        //its material
				   "World");	    //its name                                   
  physiWorld = new G4PVPlacement(0,		    //no rotation
  				 G4ThreeVector(),   //at (0,0,0)
				 logicWorld,	    //its logical volume				 
				 "World",	    //its name
				 0,		    //its mother  volume
				 false,		    //no boolean operation
				 0);		    //copy number


  G4Tubs* targetTube = new G4Tubs("Target", 0, targetRadius, targetThickness/2, 0, 360.0*deg);
  G4LogicalVolume* logicTarget = new G4LogicalVolume(targetTube, alMy, "Target");
  G4VPhysicalVolume* physiTarget = new G4PVPlacement(0,
		  	  	  	  	  	  	  	G4ThreeVector(target_xc,target_yc,target_zc),
		  	  	  	  	  	  	  	logicTarget,
		  	  	  	  	  	  	  	"Target",
		  	  	  	  	  	  	  	logicWorld,
		  	  	  	  	  	  	  	false,
		  	  	  	  	  	  	  	0);
  G4VisAttributes* senDetVisAtt = new G4VisAttributes(G4Colour::Green());
  logicTarget->SetVisAttributes(senDetVisAtt);

  //////////////////////// Detector ///////////////////////////
  solidSenDet1 = new G4Tubs("SenDet1", 0, senDetRadius, senDetThickness/2, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     beamVacuum,
				     "SenDet1");
  
  physiSenDet1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(target_xc,target_yc,senDet1_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume				 
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number
  //Slate Blue
  G4VisAttributes* senDetVisAtt1 = new G4VisAttributes(G4Colour(106.0/255.0, 90.0/255.0, 205.0/255.0));
  logicSenDet1->SetVisAttributes(senDetVisAtt1);

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

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
  G4cout<<"DetectorConstruction::UpdateGeometry()"<<G4endl;
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

G4Material *DetectorConstruction::GetMaterial(G4int t)
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


void DetectorConstruction::ConstructMagnet(double refX, double refY, double refZ, double gapSize, double thicknessOfChamber)
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


	G4Box *solidMagnetTopMain = new G4Box("MagnetTopMain",magnetMainX/2, magnetMainY/2, magnetMainZ/2);
	G4LogicalVolume *logicMagnetTopMain = new G4LogicalVolume(solidMagnetTopMain, GetMaterial(2), "MagnetTopMain");
	G4PVPlacement *physiMagnetTopMain = new G4PVPlacement(0, G4ThreeVector(magnetTopMain_xc,magnetTopMain_yc,magnetTopMain_zc),
												logicMagnetTopMain,"MagnetTopMain", logicWorld, false,0);

	G4Box *solidMagnetTopLeft = new G4Box("MagnetTopLeft",magnetSideX/2, magnetSideY/2, magnetSideZ/2);
	G4LogicalVolume *logicMagnetTopLeft = new G4LogicalVolume(solidMagnetTopLeft, GetMaterial(2), "MagnetTopLeft");
	G4PVPlacement *physiMagnetTopLeft = new G4PVPlacement(0, G4ThreeVector(magnetTopLeft_xc,magnetTopLeft_yc,magnetTopLeft_zc),
												logicMagnetTopLeft,"MagnetTopLeft", logicWorld, false,0);

	G4Box *solidMagnetTopRight = new G4Box("MagnetTopRight",magnetSideX/2, magnetSideY/2, magnetSideZ/2);
	G4LogicalVolume *logicMagnetTopRight = new G4LogicalVolume(solidMagnetTopRight, GetMaterial(2), "MagnetTopRight");
	G4PVPlacement *physiMagnetTopRight = new G4PVPlacement(0, G4ThreeVector(magnetTopRight_xc,magnetTopRight_yc,magnetTopRight_zc),
												logicMagnetTopRight,"MagnetTopRight", logicWorld, false,0);

	G4Box *solidMagnetTopMid = new G4Box("MagnetTopMid",MagnetMidX/2, MagnetMidY/2, MagnetMidZ/2);
	G4LogicalVolume *logicMagnetTopMid = new G4LogicalVolume(solidMagnetTopMid, GetMaterial(2), "MagnetTopMid");
	G4PVPlacement *physiMagnetTopMid = new G4PVPlacement(0, G4ThreeVector(MagnetTopMid_xc,MagnetTopMid_yc,MagnetTopMid_zc),
												logicMagnetTopMid,"MagnetTopMid", logicWorld, false,0);


	G4Box *solidMagnetBotMain = new G4Box("MagnetBotMain",magnetMainX/2, magnetMainY/2, magnetMainZ/2);
	G4LogicalVolume *logicMagnetBotMain = new G4LogicalVolume(solidMagnetBotMain, GetMaterial(2), "MagnetBotMain");
	G4PVPlacement *physiMagnetBotMain = new G4PVPlacement(0, G4ThreeVector(magnetBotMain_xc,magnetBotMain_yc,magnetBotMain_zc),
												logicMagnetBotMain,"MagnetBotMain", logicWorld, false,0);

	G4Box *solidMagnetBotMid = new G4Box("MagnetBotMid",MagnetMidX/2, MagnetMidY/2, MagnetMidZ/2);
	G4LogicalVolume *logicMagnetBotMid = new G4LogicalVolume(solidMagnetBotMid, GetMaterial(2), "MagnetBotMid");
	G4PVPlacement *physiMagnetBotMid = new G4PVPlacement(0, G4ThreeVector(MagnetBotMid_xc,MagnetBotMid_yc,MagnetBotMid_zc),
												logicMagnetBotMid,"MagnetBotMid", logicWorld, false,0);

	  G4VisAttributes* magnetVisAtt = new G4VisAttributes(G4Colour(0.8,0.7,0.3));
	  logicMagnetTopMain->SetVisAttributes(magnetVisAtt);
	  logicMagnetTopLeft->SetVisAttributes(magnetVisAtt);
	  logicMagnetTopRight->SetVisAttributes(magnetVisAtt);
	  logicMagnetTopMid->SetVisAttributes(magnetVisAtt);
	  logicMagnetBotMain->SetVisAttributes(magnetVisAtt);
	  logicMagnetBotMid->SetVisAttributes(magnetVisAtt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
