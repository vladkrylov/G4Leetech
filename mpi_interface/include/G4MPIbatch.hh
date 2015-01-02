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
// $Id: G4MPIbatch.hh,v 1.1 2007-11-16 14:05:39 kmura Exp $
// $Name: not supported by cvs2svn $
//
// ====================================================================
//   G4MPIbatch.hh
//
//                                         2007 Q
// ====================================================================
#ifndef G4MPI_BATCH_H
#define G4MPI_BATCH_H

#include "G4VMPIsession.hh"
#include <fstream>

// ====================================================================
//
// class definition
//
// ====================================================================

class G4MPIbatch : public G4VMPIsession {
protected:
  std::ifstream batchStream;
  G4bool isOpened;
  G4bool isBatchMode;

  // get command from a batch script file
  G4String ReadCommand();

public:
  G4MPIbatch(const G4String& fname="", G4bool qbatch=false);
  ~G4MPIbatch();

  // concrete implementations
  virtual G4UIsession* SessionStart();
};

#endif
