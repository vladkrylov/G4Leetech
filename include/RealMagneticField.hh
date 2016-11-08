//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B5MagneticField.hh 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file B5MagneticField.hh
/// \brief Definition of the B5MagneticField class

#ifndef RealMagneticField_H
#define RealMagneticField_H 1

#include <vector>

#include "globals.hh"
#include "G4MagneticField.hh"

class G4GenericMessenger;
class DetectorConstruction;

/// Magnetic field

class RealMagneticField : public G4MagneticField
{
public:
	RealMagneticField();
    virtual ~RealMagneticField();
    
    virtual void GetFieldValue(const G4double point[4], G4double* bField ) const;
    
    void SetField(G4double val) { By = val; }
    G4double GetField() const { return By; }
    
private:
    void DefineCommands();

    G4GenericMessenger* fMessenger;
    DetectorConstruction* detector;
    G4double By;

    G4double z0, zmin, zmax;
    std::vector<G4double> zmap;
    std::vector<G4double> Bzmap;

    void AddBZPoint(G4double z, G4double B);
    void CreateBZMap();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
