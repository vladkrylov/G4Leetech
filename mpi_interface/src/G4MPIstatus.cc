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
// $Id: G4MPIstatus.cc,v 1.2 2010-05-18 06:06:21 kmura Exp $
// $Name: not supported by cvs2svn $
//
// ====================================================================
//   G4MPIstatus.cc
//
//                                         2007 Q
// ====================================================================
#include "G4MPIstatus.hh"
#include "G4ApplicationState.hh"

// ====================================================================
//
// class description
//
// ====================================================================

//////////////////////////
G4MPIstatus::G4MPIstatus()
  : rank(0), runID(0),
    nEventToBeProcessed(0),
    eventID(0),
    cputime(0.),
    g4state(G4State_Quit)
//////////////////////////
{
  timer= new G4Timer;
}


//////////////////////////
G4MPIstatus::~G4MPIstatus()
//////////////////////////
{
  delete timer;
}


/////////////////////////////////////////////////////
void G4MPIstatus::SetStatus(G4int arank, G4int runid,
                            G4int noe, G4int evtid,
                            G4ApplicationState state)
/////////////////////////////////////////////////////
{
  rank= arank;
  runID= runid;
  nEventToBeProcessed= noe;
  eventID= evtid;
  g4state= state;
  if (timer-> IsValid()) cputime= timer-> GetUserElapsed();
  else cputime = 0.;
}


/////////////////////////////////////////
void G4MPIstatus::Pack(G4int* data) const
/////////////////////////////////////////
{
  data[0]= rank;
  data[1]= runID;
  data[2]= nEventToBeProcessed;
  data[3]= eventID;
  data[4]= g4state;

  G4double* ddata= (G4double*)(data+5);
  ddata[0]= cputime;
}


/////////////////////////////////////
void G4MPIstatus::UnPack(G4int* data)
/////////////////////////////////////
{
  rank= data[0];
  runID= data[1];
  nEventToBeProcessed= data[2];
  eventID= data[3];
  g4state= (G4ApplicationState)data[4];

  G4double* ddata= (G4double*)(data+5);
  cputime= ddata[0];
}


///////////////////////////////
void G4MPIstatus::Print() const
///////////////////////////////
{
  // * rank= 001 run= 10002 event= 00001 / 100000 state= Idle"
  G4cout << "* rank= " << rank
         << " run= " << runID
         << " event= " << eventID << " / " << nEventToBeProcessed
         << " state= " << GetStateString(g4state)
         << " time= " << cputime << "s"
         << G4endl;
}


/////////////////////////////////////////////////////////////////////
G4String G4MPIstatus::GetStateString(G4ApplicationState astate) const
/////////////////////////////////////////////////////////////////////
{
  G4String sname;

  switch(astate) {
  case G4State_PreInit:
    sname = "PreInit";
    break;
  case G4State_Init:
    sname = "Init";
    break;
  case G4State_Idle:
    sname = "Idle";
    break;
  case G4State_GeomClosed:
    sname = "GeomClosed";
    break;
  case G4State_EventProc:
    sname = "EventProc";
    break;
  case G4State_Quit:
    sname = "Quit";
    break;
  case G4State_Abort:
    sname = "Abort";
    break;
  default:
    sname = "Unknown";
    break;
  }

  return sname;
}

