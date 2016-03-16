#ifndef SENSITIVEDETECTOR
#define SENSITIVEDETECTOR

#include<G4VSensitiveDetector.hh>
#include<fstream>
class G4Step;
class G4TouchableHistory;


class SensitiveDetector: public G4VSensitiveDetector
{
private:

public:
  SensitiveDetector(G4String name);
  ~SensitiveDetector();
  G4bool ProcessHits(G4Step *step, G4TouchableHistory *hist);
  void EndOfEvent(G4HCofThisEvent*);
  std::ofstream myfile;
};

#endif /* SENSITIVEDETECTOR */
