#ifndef ACTIONINITIALIZATION_H_
#define ACTIONINITIALIZATION_H_

#include "globals.hh"
#include "G4VUserActionInitialization.hh"

class ActionInitialization: public G4VUserActionInitialization
{
public:
	ActionInitialization();
	virtual ~ActionInitialization();

protected:
	void Build() const;
};

#endif /* ACTIONINITIALIZATION_H_ */
