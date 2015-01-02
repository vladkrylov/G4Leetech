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
// $Id: G4MPIsession.cc,v 1.2 2010-05-18 06:06:21 kmura Exp $
// $Name: not supported by cvs2svn $
//
// ====================================================================
//   G4MPIsession.cc
//
//                                         2007 Q
// ====================================================================
#include "G4MPIsession.hh"
#include "G4MPImanager.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4UIcsh.hh"
#include "G4UImpish.hh"

// ====================================================================
//
// class description
//
// ====================================================================

//////////////////////////////////////////////
G4MPIsession::G4MPIsession(G4VUIshell* ashell)
  : G4VMPIsession()
//////////////////////////////////////////////
{
  G4UImanager* UI= G4UImanager::GetUIpointer();
  UI-> SetSession(this);
  UI-> SetCoutDestination(this);

  // shell
  if(isMaster) {
    if(ashell) {
      shell= ashell;
    } else {
      shell= new G4UIcsh;
    }
  } else {
    shell= new G4UImpish;
  }
}

/////////////////////////////
G4MPIsession::~G4MPIsession()
/////////////////////////////
{
  delete shell;

  G4UImanager* UI= G4UImanager::GetUIpointer();
  UI-> SetSession(0);
  UI-> SetCoutDestination(0);
}

////////////////////////////////////////////////////
void G4MPIsession::SetPrompt(const G4String& prompt)
////////////////////////////////////////////////////
{
  shell-> SetPrompt(prompt);
}


///////////////////////////////////////////////
void G4MPIsession::SetShell(G4VUIshell* ashell)
///////////////////////////////////////////////
{
  delete shell;
  shell= ashell;
}


//////////////////////////////////////////////////
G4String G4MPIsession::GetCommand(const char* msg)
//////////////////////////////////////////////////
{
  G4UImanager* UI= G4UImanager::GetUIpointer();

  G4String newCommand;
  const G4String nullString="";

  // get command from shell...
  newCommand= shell-> GetCommandLine(msg);

  G4String nC= newCommand.strip(G4String::leading);
  if( nC.length() == 0 ) {
    newCommand= nullString;

  } else if( nC(0) == '#' ) {
    G4cout << nC << G4endl;
    newCommand= nullString;

  } else if(nC=="ls" || nC(0,3)=="ls " ) {  // list commands
    ListDirectory(nC);
    newCommand= nullString;

  } else if(nC=="lc" || nC(0,3)=="lc " ) {  // ... by shell
    shell-> ListCommand(nC.remove(0,2));
    newCommand= nullString;

  } else if(nC == "pwd") { // show current directory
    G4cout << "Current Command Directory : "
     << GetCurrentWorkingDirectory() << G4endl;
    newCommand= nullString;

  } else if(nC == "cwd") { // ... by shell
    shell-> ShowCurrentDirectory();
    newCommand= nullString;

  } else if(nC == "cd" || nC(0,3) == "cd ") {  // "cd"
    ChangeDirectoryCommand(nC);
    shell-> SetCurrentDirectory(GetCurrentWorkingDirectory());
    newCommand= nullString;

  } else if(nC == "help" || nC(0,5) == "help ") {  // "help"
    TerminalHelp(nC);
    newCommand= nullString;

  } else if(nC(0) == '?') {   // "show current value of a parameter"
    ShowCurrent(nC);
    newCommand= nullString;

  } else if(nC == "hist" || nC == "history") {     // "hist/history"
    G4int nh= UI-> GetNumberOfHistory();
    for (G4int i=0; i<nh; i++) {
      G4cout << i << ": " << UI->GetPreviousCommand(i) << G4endl;
    }
    newCommand= nullString;

  } else if(nC(0) == '!') {   // "!"
    G4String ss= nC(1, nC.length()-1);
    G4int vl;
    const char* tt= ss;
    std::istringstream is(tt);
    is >> vl;
    G4int nh= UI-> GetNumberOfHistory();
    if(vl>=0 && vl<nh) {
      newCommand= UI-> GetPreviousCommand(vl);
      G4cout << newCommand << G4endl;
    } else {
      G4cerr << "history " << vl << " is not found." << G4endl;
      newCommand= nullString;
    }

  } else if( nC.empty() ){ // NULL command
    newCommand= nullString;

  } else if( nC == "exit" ){ // "exit"
    return "exit";

  } else { // ...

  }

  newCommand= TruncateCommand(newCommand);
  return ModifyToFullPathCommand(newCommand);
}


/////////////////////////////////////////
G4UIsession* G4MPIsession::SessionStart()
/////////////////////////////////////////
{
  // execute init macro
  if(g4MPI-> IsInitMacro()) {
    g4MPI-> ExecuteMacroFile(g4MPI->GetInitFileName());
  }

  // batch mode
  if(g4MPI-> IsBatchMode()) {
    g4MPI-> ExecuteMacroFile(g4MPI->GetMacroFileName(), true);
    return 0;
  }

  // interactive session
  G4String newCommand="", scommand; // newCommand is always "" in slaves

  while(1) {
    if(isMaster) newCommand = GetCommand();
    // broadcast a new G4 command
    scommand = g4MPI-> BcastCommand(newCommand);
    if(scommand == "exit") {
      G4bool qexit = TryForcedTerminate();
      if(qexit) break;
      else scommand = "";
    }
    ExecCommand(scommand);
  }

  return 0;
}


/////////////////////////////////////////
G4bool G4MPIsession::TryForcedTerminate()
/////////////////////////////////////////
{
  if(! g4MPI-> CheckThreadStatus()) {
    return true;
  }

  G4String xmessage;

  // beamOn is still running
  if(isMaster) {
    char c[1024];
    while(1) {
      G4cout << "Run is still running. Do you abort a run? [y/N]:"
             << std::flush;
      G4cin.getline(c,1024);
      G4String yesno= c;
      if(yesno=="y" || yesno=="Y" ||
         yesno=="n" || yesno=="N" || yesno == "") {
        break;
      }
    }
    if(c[0]=='y' || c[0]=='Y') {
      G4cout << "Aborting a run..." << G4endl;
      xmessage= g4MPI-> BcastCommand("kill me");
    } else {
      xmessage= g4MPI-> BcastCommand("alive");
    }
  } else {
    xmessage= g4MPI->BcastCommand("");
  }

  if(xmessage == "kill me") {
    G4RunManager* runManager= G4RunManager::GetRunManager();
    runManager-> AbortRun(true);  // soft abort
  }

  return false;
}

