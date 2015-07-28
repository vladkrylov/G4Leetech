#ifndef PHYSICSLIST_HH_
#define PHYSICSLIST_HH_

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

/// Modular physics list
///
/// It includes the folowing physics builders
/// - G4DecayPhysics
/// - G4RadioactiveDecayPhysics
/// - G4EmStandardPhysics
class PhysicsList : public G4VModularPhysicsList
{
public:
	PhysicsList();
	virtual ~PhysicsList();

	virtual void SetCuts();

private:
	// Construct particle and physics process
//	void ConstructParticle();
//	void ConstructProcess();

	void ConstructEM();
	void ConstructDecay();
};

#endif /* PHYSICSLIST_HH_ */
