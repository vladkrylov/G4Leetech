#include "DetectorConstructionP2.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4SubtractionSolid.hh"

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


DetectorConstructionP2::DetectorConstructionP2(DetectorConstruction* MD, MagneticField *MF)
:magField(MF),
 solidWorld(0),logicWorld(0),physiWorld(0),
 solidBeamPipe(0),logicBeamPipe(0),physiBeamPipe(0),
 solidBeamPipeV(0),logicBeamPipeV(0),physiBeamPipeV(0),
 solidCup(0),logicCup(0),physiCup(0),
 solidAperture(0),logicAperture(0),physiAperture(0),
 solidSenDet1(0),logicSenDet1(0),physiSenDet1(0),
 solidShield1(0),logicShield1(0), physiShield1(0),
 //solidSenDet2(0),logicSenDet2(0),physiSenDet2(0),
 solidDipol_L(0),logicDipol_L(0),physiDipol_L(0), MainDet(MD)
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

DetectorConstructionP2::~DetectorConstructionP2()
{ 
  delete stepLimit;
  //delete detectorMessenger;
}

G4VPhysicalVolume* DetectorConstructionP2::ConstructGeom4(){

  G4double WorldSizeX         = 200.0*cm;
  G4double WorldSizeY         = WorldSizeX;
  G4double WorldSizeZ         = 200.0*cm;  

  G4double gapSize            = 1.0*mm;

  G4double beamPipeLenght     = 20.0*cm;
  G4double beamPipeOutRadius  = 3.00*cm;
  G4double beamPipeInRadius   = 2.70*cm;
  G4double beamPipeVOutRadius = beamPipeInRadius - 0.1*cm;

  G4double cupOutRadius       = beamPipeOutRadius;
  //G4double cupLenght          = 0.4*cm;
  G4double cupLenght          = _cupLenght;

  G4double apertureOutRadius  = beamPipeOutRadius;
  G4double apertureInRadius   = _apertureInRadius;
  G4double apertureLenght     = _apertureLenght;
  G4double senDet1OutRadius   = _Det1OutRad;//WorldSizeY/2 - 0.2*cm; 
  G4double senDet1InRadius    = _Det1InRad;
  G4double senDet1Lenght      = 0.1*cm;

  G4double shield1OutRadius   = apertureOutRadius+gapSize+1*cm;
  G4double shield1InRadius    = apertureOutRadius+gapSize;
  G4double shield1Lenght      = 20.0*cm;

 // G4double shield2_X = 10*cm;
 // G4double shield2_Y = 50*cm;
 // G4double shield2_Z = 20*cm;
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
  //G4double rotAngle = 45*deg;
  G4double rotAngle = _RotationDeg;
  G4double RotAddDist = _RotAddDist;

  G4double beamPipe_xc = 0.0;
  G4double beamPipe_yc = 0.0;
  G4double beamPipe_zc = -cupLenght/2.0 - gapSize - beamPipeLenght/2.0;

  G4double cup_xc      = 0.0;
  G4double cup_yc      = 0.0;
  G4double cup_zc      = 0.0;

  G4double aperture_xc = -_Det1X - (RotAddDist*2 + apertureLenght +2*gapSize)/2*sin(rotAngle);// - ()*sin(rotAngle);
  G4double aperture_yc = 0.0;
  G4double aperture_zc = cupLenght/2.0 - apertureLenght/2 - RotAddDist -gapSize
	  + (RotAddDist*2 + gapSize*2 + apertureLenght )*(1-cos(rotAngle))/2;

  G4double shift = 0*cm;

  G4double dipol_L_xc  = beamPipeOutRadius + gapSize + dipol_LR_X/2.0;
  G4double dipol_L_yc  = 0.0;
  G4double dipol_L_zc  = cupLenght/2.0 + gapSize + dipol_LR_Z/2.0;

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
  G4double fieldBox_L_zc = cupLenght/2.0 + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;

  G4double fieldBox_R_xc = dipol_R_xc + dipol_LR_X/2.0 + gapSize + fieldBox_X/2.0;
  G4double fieldBox_R_yc = 0.0;
  G4double fieldBox_R_zc = cupLenght/2.0 + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;


//  G4double aperture_xc = -_Det1X - (RotAddDist*2 + apertureLenght +2*gapSize)/2*sin(rotAngle);// - ()*sin(rotAngle);
  //G4double aperture_yc = 0.0;
  //G4double aperture_zc = cupLenght/2.0 - apertureLenght/2 - RotAddDist -gapSize
	//  + (RotAddDist*2 + gapSize*2 + apertureLenght )*(1-cos(rotAngle))/2;

			double dCol_Det = apertureLenght/cm;

			double tempX = -_Det1X - (RotAddDist*2 + dCol_Det*cm +2*gapSize)/2*sin(rotAngle);

		G4double senDet1_xc    = tempX//aperture_xc
			 -  gapSize
			//- (apertureLenght/2*2
			-(dCol_Det*cm/2*2
			    + senDet1Lenght + gapSize*2)/2*sin(rotAngle);//-16.7*cm;
		G4double senDet1_yc    = 0;

		double tempZ =  cupLenght/2.0 - dCol_Det*cm/2 - RotAddDist -gapSize
	  + (RotAddDist*2 + gapSize*2 + dCol_Det*cm )*(1-cos(rotAngle))/2;

		G4double senDet1_zc    = tempZ//aperture_zc
			 //- apertureLenght/2.0
			- dCol_Det*cm/2
			 - senDet1Lenght/2 - gapSize*2//*1
			 //+ (apertureLenght/2*2
			+(dCol_Det*cm/2*2
				 + senDet1Lenght + gapSize*2)/2*(1-cos(rotAngle));
				
				
				//senDet1_xc = senDet1_xc - (apertureLenght+gapSize+senDet1Lenght)*sin(rotAngle)/2;
				//senDet1_zc = senDet1_zc + (apertureLenght+gapSize+senDet1Lenght)*(1-cos(rotAngle))/2;
  
  G4double shield1_xc    = senDet1_xc;
  G4double shield1_yc    = senDet1_yc;
  G4double shield1_zc    = senDet1_zc;

//  G4double shield2_xc    = -(beamPipeOutRadius + gapSize + shield2_X/2);
//  G4double shield2_yc    = 0;
//  G4double shield2_zc    = - shield2_Z/2 + cupLenght/2;

	G4RotationMatrix*RM1=new G4RotationMatrix(89.9*deg,rotAngle,0*deg);
  
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
   
  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);                     
  logicWorld = new G4LogicalVolume(solidWorld,	    //its solid
				  // vacuumMy,        //its material
					MainDet->GetMaterial(6),
								   //*&testMat,
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

  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,	//its solid
				      //steelMy,         	//its material
					MainDet->GetMaterial(2),
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
//	<<"--------------------"<<G4endl
//	<<"Beampipe Vacuum material"<<G4endl;
	//<<beamVacuum<<G4endl;
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,	//its solid
				       //beamVacuum,      //its material
						MainDet->GetMaterial(6),
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

  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,		//its solid
				 //alMy,	//its material
				MainDet->GetMaterial(1),
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

  solidAperture = new G4Tubs("Aperture", apertureInRadius, apertureOutRadius, apertureLenght/2.0, 0, 360.0*deg);
  logicAperture = new G4LogicalVolume(solidAperture,	    //its solid
				      //alMy,	            //its material
						MainDet->GetMaterial(7),
				      "Aperture");          //its name  
  physiAperture = new G4PVPlacement(RM1,    //rotation
				    G4ThreeVector(aperture_xc,aperture_yc,aperture_zc),//its position
				    logicAperture,	    //its logical volume				 
				    "Cup",		    //its name
				    logicWorld,		    //its mother  volume
				    false,		    //no boolean operation
				    0);		            //copy number

  //     
  // Dipol left - blue
  //

  solidDipol_L = new G4Box("Dipol_L",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_L = new G4LogicalVolume(solidDipol_L,  //its solid
				     //alMy,          //its material
					MainDet->GetMaterial(1),
				     "Dipol_L");    //its name
  physiDipol_L = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_L_xc + shift, dipol_L_yc, dipol_L_zc),   //at (0,0,0)
				   logicDipol_L,	    //its logical volume
				   "Dipol_L",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number
  //     
  // Dipol right
  //

  solidDipol_R = new G4Box("Dipol_R",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_R = new G4LogicalVolume(solidDipol_R,  //its solid
				    // alMy,          //its material
					MainDet->GetMaterial(1),
				     "Dipol_R");    //its name
  physiDipol_R = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_R_xc + shift, dipol_R_yc, dipol_R_zc),   //at (0,0,0)
				   logicDipol_R,	    //its logical volume
				   "Dipol_R",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol top
  //

  solidDipol_T = new G4Box("Dipol_T",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_T = new G4LogicalVolume(solidDipol_T,  //its solid
				     //alMy,          //its material
					MainDet->GetMaterial(1),
				     "Dipol_T");    //its name
  physiDipol_T = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_T_xc + shift, dipol_T_yc, dipol_T_zc),   //at (0,0,0)
				   logicDipol_T,    //its logical volume
				   "Dipol_T",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol bottom
  //

  solidDipol_B = new G4Box("Dipol_B",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_B = new G4LogicalVolume(solidDipol_B,  //its solid
				     //alMy,          //its material
						MainDet->GetMaterial(1),
				     "Dipol_B");    //its name
  physiDipol_B = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_B_xc + shift, dipol_B_yc, dipol_B_zc),   //at (0,0,0)
				   logicDipol_B,    //its logical volume
				   "Dipol_B",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number
  solidFieldBox_L = new G4Box("FieldBox_L",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_L = new G4LogicalVolume(solidFieldBox_L,    //its solid
					//vacuumMy,           //its material
					MainDet->GetMaterial(6),
					"FieldBox_L");      //its name
  physiFieldBox_L = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc),   //its position
				      logicFieldBox_L,	    //its logical volume
				      "FieldBox_L",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  solidFieldBox_R = new G4Box("FieldBox_R",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_R = new G4LogicalVolume(solidFieldBox_R,    //its solid
					//vacuumMy,           //its material
						MainDet->GetMaterial(6),
					"FieldBox_R");      //its name
  physiFieldBox_R = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc),   //its position
				      logicFieldBox_R,	    //its logical volume
				      "FieldBox_R",	    //its name
				      logicWorld,	    //its mother  volume
						false,     	    //no boolean operation
							0);		    //copy number


  solidSenDet1 = new G4Tubs("SenDet1", senDet1InRadius, senDet1OutRadius, senDet1Lenght/2.0, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     //beamVacuum,
						MainDet->GetMaterial(6),
				     "SenDet1");
  
  physiSenDet1 = new G4PVPlacement(RM1,	//rotation
				   G4ThreeVector(senDet1_xc,senDet1_yc,senDet1_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume				 
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number
  
  solidShield1 = new G4Tubs("Shield1", shield1InRadius, shield1OutRadius, shield1Lenght/2.0, 0, 360.0*deg);
  logicShield1 = new G4LogicalVolume(solidShield1,
				     MainDet->GetMaterial(7),
				     "Shield1");
  
  physiShield1 = new G4PVPlacement(RM1,		       	//rotation
				   G4ThreeVector(shield1_xc,shield1_yc,shield1_zc), //at (0,0,0)
				   logicShield1,	//its logical volume				 
				   "Shield1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number
  
 G4double main_Rmin = 1.7*cm;
 G4double main_Rmax = 2*cm;
 G4double main_Z = 15*cm;
 G4double main_xc = 0; 
 G4double main_yc = 0;
 G4double main_zc = 2*gapSize + cupLenght/2;
 G4double dirka_D = 3*mm;//senDet1OutRadius*(main_Rmin+(2*gapsize+cupLenght/2)/cos(rotAngle))/(19.69*cm)+0.1*mm;//ychot razshirenia puchka

  G4double popravka = 2*gapSize*sin(rotAngle); //popravka na to chto koliamtor ne v upor k nochke 0 0 0

 G4RotationMatrix*RMM=new G4RotationMatrix(0,90*deg , 0);
										   //rotAngle,0*deg);
 G4RotationMatrix *RotDirka = new G4RotationMatrix(rotAngle,90*deg , 0);
 const G4ThreeVector *RotTr = new G4ThreeVector(0,0,0);

  solidShieldMain = new G4Tubs("ShieldMain", main_Rmin, main_Rmax, main_Z, 0*deg,180*deg);
  solidDirka = new G4Tubs("Dirka", 0, dirka_D, main_Rmax, 0, 360*deg);
  solidShieldRes = new G4SubtractionSolid ("ShieldRes", solidShieldMain, solidDirka, RotDirka, *RotTr);
  logicShieldMain = new G4LogicalVolume(solidShieldRes,MainDet->GetMaterial(7),"ShieldRes");
  physiShieldMain = new G4PVPlacement(RMM,	    	//rotation
					G4ThreeVector(main_xc - popravka,main_yc,main_zc),
				   logicShieldMain,	//its logical volume				 
				   "ShieldRes",		//its name
								   logicWorld,
				   false,      		//no boolean operation
				   0);			//copy number

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

  //G4VisAttributes* magVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  //logicFieldBox_L->SetVisAttributes(magVisAtt);
  //logicFieldBox_R->SetVisAttributes(magVisAtt);

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
  //logicFieldBox_L->SetUserLimits(stepLimit);
  //logicFieldBox_R->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}


G4VPhysicalVolume* DetectorConstructionP2::ConstructGeom5(){
    

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
    G4FieldManager* fieldMgr  = G4TransportationManager::GetTransportationManager()->GetFieldManager();
//  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  int n = 1;
  magField->setFieldBox1(0*cm, 0*cm, 0*cm, n*m, n*m, n*m);

  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  solidWorld = new G4Box("World",n*m,n*m, n*m);                     
  logicWorld = new G4LogicalVolume(solidWorld,MainDet->GetMaterial(3),"World");	
  physiWorld = new G4PVPlacement(0,G4ThreeVector(),logicWorld,"World",0,false, 0);

  
  G4RotationMatrix*RMM=new G4RotationMatrix(90*deg,90*deg , 0);										   //rotAngle,0*deg);
 //G4RotationMatrix *RotDirka = new G4RotationMatrix(rotAngle,90*deg , 0);
 //const G4ThreeVector *RotTr = new G4ThreeVector(0,0,0);
  solidDipol_L = new G4Box("Dipol_L",10*cm,10*cm,0.5*cm);
  //solidShieldMain = new G4Tubs("ShieldMain", main_Rmin, main_Rmax, main_Z, 0*deg,180*deg);
  solidDirka = new G4Tubs("Dirka", 0, 0.2*cm, 3*cm, 0, 360*deg);
  solidShieldRes = new G4SubtractionSolid ("ShieldRes", solidDipol_L, solidDirka);
  logicShieldMain = new G4LogicalVolume(solidShieldRes,MainDet->GetMaterial(7),"ShieldRes");
  physiShieldMain = new G4PVPlacement(RMM,G4ThreeVector(1*cm,0*cm,0),
			logicShieldMain,"ShieldRes",logicWorld, false, 0);
  
  G4RotationMatrix*RMM2=new G4RotationMatrix(90*deg,-45*deg , 0);										   //rotAngle,0*deg);
 //G4RotationMatrix *RotDirka = new G4RotationMatrix(rotAngle,90*deg , 0);
 //const G4ThreeVector *RotTr = new G4ThreeVector(0,0,0);
  solidDipol_R = new G4Box("Dipol_R",10*cm,10*cm,0.5*cm);
  //solidShieldMain = new G4Tubs("ShieldMain", main_Rmin, main_Rmax, main_Z, 0*deg,180*deg);
  G4Tubs *solidDirkaR = new G4Tubs("Dirka2", 0,0.4*cm, 3*cm, 0, 360*deg);
  G4SubtractionSolid *solidShieldRes = new G4SubtractionSolid ("ShieldRes2", solidDipol_R, solidDirkaR);
  G4LogicalVolume *logicShieldMain2 = new G4LogicalVolume(solidShieldRes,MainDet->GetMaterial(7),"ShieldRes2");
  G4PVPlacement *physiShieldMain2 = new G4PVPlacement(RMM2,G4ThreeVector(12.4*cm,0,-6.5*cm),
			logicShieldMain2,"ShieldRes2",logicWorld, false, 0);
  
  solidSenDet1 = new G4Tubs("SenDet1", 0, 0.5*cm, 0.5*cm, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,MainDet->GetMaterial(6),"SenDet1");  
  physiSenDet1 = new G4PVPlacement(new G4RotationMatrix(89.9*deg, -45*deg,0*deg),G4ThreeVector(13.5*cm,0,-8*cm),logicSenDet1,"SenDet1",logicWorld,false,0);
//  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
//  logicCup->SetVisAttributes(cupVisAtt);

  //G4VisAttributes* alVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  //logicDipol_L->SetVisAttributes(alVisAtt);

 // G4VisAttributes* beamVacuumVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
 // logicBeamPipeV->SetVisAttributes(beamVacuumVisAtt);

  //Slate Blue
  G4VisAttributes* senDetVisAtt = new G4VisAttributes(G4Colour(106.0/255.0, 90.0/255.0, 205.0/255.0));
  logicSenDet1->SetVisAttributes(senDetVisAtt);
 // logicSenDet2->SetVisAttributes(senDetVisAtt);

  //logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  return physiWorld;
}


G4VPhysicalVolume* DetectorConstructionP2::ConstructGeom6(){

  G4double WorldSizeX         = 200.0*cm;
  G4double WorldSizeY         = WorldSizeX;
  G4double WorldSizeZ         = 200.0*cm;  

  G4double gapSize            = 1.0*mm;

  G4double beamPipeLenght     = 20.0*cm;
  G4double beamPipeOutRadius  = 3.00*cm;
  G4double beamPipeInRadius   = 2.70*cm;
  G4double beamPipeVOutRadius = beamPipeInRadius - 0.1*cm;

  G4double cupOutRadius       = beamPipeOutRadius;
  //G4double cupLenght          = 0.4*cm;
  G4double cupLenght          = _cupLenght;

//  G4double apertureOutRadius  = beamPipeOutRadius;
 // G4double apertureInRadius   = _apertureInRadius;
 // G4double apertureLenght     = _apertureLenght;
  G4double senDet1OutRadius   = _Det1OutRad;//WorldSizeY/2 - 0.2*cm; 
  G4double senDet1InRadius    = _Det1InRad;
  G4double senDet1Lenght      = gapSize;

//  G4double shield1OutRadius   = apertureOutRadius+gapSize+1*cm;
//  G4double shield1InRadius    = apertureOutRadius+gapSize;
//  G4double shield1Lenght      = 20.0*cm;

  G4double fieldBox_X         = 10.0*cm; 
  G4double fieldBox_Y         = 5.0*cm;
  G4double fieldBox_Z         = 10.0*cm;

  G4double beamPipe_xc = 0.0;
  G4double beamPipe_yc = 0.0;
  G4double beamPipe_zc = -cupLenght/2.0 - gapSize - beamPipeLenght/2.0;

  G4double cup_xc      = 0.0*cm;
  G4double cup_yc      = 0.0;
  G4double cup_zc      = 0.0;

  G4double rotAngle = _RotationDeg;	

//  G4double shift = 0*cm;

  G4double fieldBox_L_xc = -18*cm;
  G4double fieldBox_L_yc = 0*cm;
  G4double fieldBox_L_zc = 15*cm;

  G4double senDet1_xc = -_Det1X;
  G4double senDet1_yc = 0;
  G4double senDet1_zc = 0*cm;
  
//  G4double shield1_xc    = senDet1_xc;
//  G4double shield1_yc    = senDet1_yc;
//  G4double shield1_zc    = senDet1_zc;
  
  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
//  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox2(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc, 
			 fieldBox_X, fieldBox_Y, fieldBox_Z);
//  magField->setFieldBox2(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc, 
//			 fieldBox_X, fieldBox_Y, fieldBox_Z);
  magField->setFieldBox1(-100*cm, -100*cm, -100*cm,
							0,0,0);

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
  logicWorld = new G4LogicalVolume(solidWorld,	    //its solid
				  // vacuumMy,        //its material
					MainDet->GetMaterial(6),
								   //*&testMat,
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

  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,	//its solid
				      //steelMy,         	//its material
					MainDet->GetMaterial(2),
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
//	<<"--------------------"<<G4endl
//	<<"Beampipe Vacuum material"<<G4endl;
	//<<beamVacuum<<G4endl;
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,	//its solid
				       //beamVacuum,      //its material
						MainDet->GetMaterial(6),
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

  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,		//its solid
				 //alMy,	//its material
				MainDet->GetMaterial(1),
				 "Cup");		//its name  
  physiCup = new G4PVPlacement(0,			//no rotation
			       G4ThreeVector(cup_xc,cup_yc,cup_zc),//its position
			       logicCup,		//its logical volume				 
			       "Cup",		        //its name
			       logicWorld,		//its mother  volume
			       false,			//no boolean operation
			       0);		        //copy number
  //     
  // Field box left
  //

  solidFieldBox_L = new G4Box("FieldBox_L",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_L = new G4LogicalVolume(solidFieldBox_L,    //its solid
					//vacuumMy,           //its material
					MainDet->GetMaterial(6),
					"FieldBox_L");      //its name
  physiFieldBox_L = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc),   //its position
				      logicFieldBox_L,	    //its logical volume
				      "FieldBox_L",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  //     
  // Sensative detector 1
  //

  solidSenDet1 = new G4Tubs("SenDet1", senDet1InRadius, senDet1OutRadius, senDet1Lenght/2.0, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     //beamVacuum,
						MainDet->GetMaterial(6),
				     "SenDet1");
  
  physiSenDet1 = new G4PVPlacement(0,	//rotation
				   G4ThreeVector(senDet1_xc,senDet1_yc,senDet1_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume				 
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number


 G4double main_Rmin = 1.7*cm;
 G4double main_Rmax = 2*cm;
 G4double main_Z = 15*cm;
 G4double main_xc = 0; 
 G4double main_yc = 0;
 G4double main_zc = 2*gapSize + cupLenght/2;
 G4double dirka_D = 3*mm;//senDet1OutRadius*(main_Rmin+(2*gapsize+cupLenght/2)/cos(rotAngle))/(19.69*cm)+0.1*mm;//ychot razshirenia puchka

  G4double popravka = 2*gapSize*sin(rotAngle); //popravka na to chto koliamtor ne v upor k nochke 0 0 0

 G4RotationMatrix*RMM=new G4RotationMatrix(0,90*deg , 0);
										   //rotAngle,0*deg);
 G4RotationMatrix *RotDirka = new G4RotationMatrix(rotAngle,90*deg , 0);
 const G4ThreeVector *RotTr = new G4ThreeVector(0,0,0);

  solidShieldMain = new G4Tubs("ShieldMain", main_Rmin, main_Rmax, main_Z, 0*deg,180*deg);
  solidDirka = new G4Tubs("Dirka", 0, dirka_D, main_Rmax, 0, 360*deg);
  solidShieldRes = new G4SubtractionSolid ("ShieldRes", solidShieldMain, solidDirka, RotDirka, *RotTr);
  logicShieldMain = new G4LogicalVolume(solidShieldRes,MainDet->GetMaterial(7),"ShieldRes");
  physiShieldMain = new G4PVPlacement(RMM,	    	//rotation

					G4ThreeVector(main_xc - popravka,main_yc,main_zc),
				   logicShieldMain,	//its logical volume				 
				   "ShieldRes",		//its name
								   logicWorld,
				   false,      		//no boolean operation
				   0);			//copy number

  //                                        
  // Visualization attributes
  //

  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);

  G4VisAttributes* alVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicBeamPipe->SetVisAttributes(alVisAtt);

  G4VisAttributes* fieldVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicFieldBox_L->SetVisAttributes(fieldVisAtt);
//std::cout<<"MARK!!\n";

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

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  logicWorld->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}
