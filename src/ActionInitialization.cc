#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"


ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction());

}
void ActionInitialization::BuildForMaster() const
{
    // Master için sadece toplama işleri olabilir (şimdilik boş)
}