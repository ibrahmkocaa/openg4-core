#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    // Sayaçlar
    static G4int fissionCount;
    static G4int captureCount;
    static G4int escapeCount;

    static G4int totalNeutrons;
    static G4int elasticCount;
    static G4int inelasticCount;
};

#endif
