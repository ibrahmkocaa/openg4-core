#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"


ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(new SteppingAction());
    SetUserAction(new TrackingAction());


}
void ActionInitialization::BuildForMaster() const
{
    // Master için sadece toplama işleri olabilir (şimdilik boş)
}