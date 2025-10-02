#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "RunAction.hh"

ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(new SteppingAction());
    SetUserAction(new RunAction());   // Worker’larda
}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());   // Master’da
}
