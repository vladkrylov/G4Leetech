#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"

#include "SteppingVerbose.hh"

SteppingVerbose::SteppingVerbose()
{}

SteppingVerbose::~SteppingVerbose()
{} 

void SteppingVerbose::StepInfo()
{
	CopyState();
  
	G4int prec = G4cout.precision(3);

	if( verboseLevel >= 1 ) {
		if( verboseLevel >= 4 ) VerboseTrack();
		if( verboseLevel >= 3 ) {
			G4cout  << G4endl;
			G4cout  << std::setw( 5) << "#Step#"     << " "
					<< std::setw( 6) << "X"          << "    "
					<< std::setw( 6) << "Y"          << "    "
					<< std::setw( 6) << "Z"          << "    "
					<< std::setw( 9) << "KineE"      << " "
					<< std::setw( 9) << "dEStep"     << " "
					<< std::setw(10) << "StepLeng"
					<< std::setw(10) << "TrakLeng"
					<< std::setw(10) << "Volume"    << "  "
					<< std::setw(10) << "Process"   << G4endl;
		}

		G4cout << std::setw( 5) << fTrack->GetCurrentStepNumber() << " "
		<< std::setw(6) << G4BestUnit(fTrack->GetPosition().x(),"Length")
		<< std::setw(6) << G4BestUnit(fTrack->GetPosition().y(),"Length")
		<< std::setw(6) << G4BestUnit(fTrack->GetPosition().z(),"Length")
		<< std::setw(6) << G4BestUnit(fTrack->GetKineticEnergy(),"Energy")
		<< std::setw(6) << G4BestUnit(fStep->GetTotalEnergyDeposit(),"Energy")
		<< std::setw(6) << G4BestUnit(fStep->GetStepLength(),"Length")
		<< std::setw(6) << G4BestUnit(fTrack->GetTrackLength(),"Length")
		<< "  ";

		// if( fStepStatus != fWorldBoundary){
		if( fTrack->GetNextVolume() != 0 ) {
		  G4cout << std::setw(10) << fTrack->GetVolume()->GetName();
		} else {
		  G4cout << std::setw(10) << "OutOfWorld";
		}

		if(fStep->GetPostStepPoint()->GetProcessDefinedStep() != NULL){
		  G4cout << "  "
				 << std::setw(10) << fStep->GetPostStepPoint()
									->GetProcessDefinedStep()->GetProcessName();
		} else {
		  G4cout << "   UserLimit";
		}

		G4cout << G4endl;

		if( verboseLevel == 2 ) {
			G4int tN2ndariesTot = fN2ndariesAtRestDoIt +
	                    fN2ndariesAlongStepDoIt +
	                    fN2ndariesPostStepDoIt;
			if(tN2ndariesTot>0) {
				G4cout << "\n    :----- List of secondaries ----------------"
						<< G4endl;

				for(size_t lp1=(*fSecondary).size()-tN2ndariesTot;
                        	lp1<(*fSecondary).size(); lp1++) {
					G4cout << "   "
							<< std::setw(13)
					<< (*fSecondary)[lp1]->GetDefinition()->GetParticleName()
					<< ":  energy ="
					<< std::setw(6)
					<< G4BestUnit((*fSecondary)[lp1]->GetKineticEnergy(),"Energy")
					<< "  time ="
					<< std::setw(6)
					<< G4BestUnit((*fSecondary)[lp1]->GetGlobalTime(),"Time");
					G4cout << G4endl;
				}
				G4cout << "    :------------------------------------------" << G4endl;
			}
		}
	}
	G4cout.precision(prec);
}

void SteppingVerbose::TrackingStarted()
{
	CopyState();
	G4int prec = G4cout.precision(3);
	if( verboseLevel > 0 ) {
		G4cout  << std::setw( 5) << "Step#"      << " "
				<< std::setw( 6) << "X"          << "    "
				<< std::setw( 6) << "Y"          << "    "
				<< std::setw( 6) << "Z"          << "    "
				<< std::setw( 9) << "KineE"      << " "
				<< std::setw( 9) << "dEStep"     << " "
				<< std::setw(10) << "StepLeng"
				<< std::setw(10) << "TrakLeng"
				<< std::setw(10) << "Volume"     << "  "
				<< std::setw(10) << "Process"    << G4endl;

		G4cout  << std::setw(5) << fTrack->GetCurrentStepNumber() << " "
				<< std::setw(6) << G4BestUnit(fTrack->GetPosition().x(),"Length")
				<< std::setw(6) << G4BestUnit(fTrack->GetPosition().y(),"Length")
				<< std::setw(6) << G4BestUnit(fTrack->GetPosition().z(),"Length")
				<< std::setw(6) << G4BestUnit(fTrack->GetKineticEnergy(),"Energy")
				<< std::setw(6) << G4BestUnit(fStep->GetTotalEnergyDeposit(),"Energy")
				<< std::setw(6) << G4BestUnit(fStep->GetStepLength(),"Length")
				<< std::setw(6) << G4BestUnit(fTrack->GetTrackLength(),"Length")
				<< "  ";

		if(fTrack->GetNextVolume()) {
			G4cout << std::setw(10) << fTrack->GetVolume()->GetName();
		} else {
			G4cout << std::setw(10) << "OutOfWorld";
		}
		G4cout  << "    initStep" << G4endl;
	}
	G4cout.precision(prec);
}

