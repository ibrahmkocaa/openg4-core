#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

// Statik sayaçların tanımı
G4int RunAction::fissionCount = 0;
G4int RunAction::captureCount = 0;
G4int RunAction::escapeCount  = 0;
G4int RunAction::totalNeutrons = 0;
G4int RunAction::elasticCount = 0;
G4int RunAction::inelasticCount = 0;

RunAction::RunAction() {}
RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*)
{
    fissionCount = 0;
    captureCount = 0;
    escapeCount  = 0;
    totalNeutrons = 0;
    elasticCount = 0;
    inelasticCount = 0;
}

void RunAction::EndOfRunAction(const G4Run*)
{
    G4cout << "============================================" << G4endl;
    G4cout << "             SIMULATION SUMMARY             " << G4endl;
    G4cout << "============================================" << G4endl;
    G4cout << " Total neutrons simulated: " << totalNeutrons << G4endl;
    G4cout << "   Fission events        : " << fissionCount << G4endl;
    G4cout << "   Capture events        : " << captureCount << G4endl;
    G4cout << "   Escaped neutrons      : " << escapeCount  << G4endl;
    G4cout << "--------------------------------------------" << G4endl;
    G4cout << " Elastic scatterings     : " << elasticCount << G4endl;
    G4cout << " Inelastic scatterings   : " << inelasticCount << G4endl;
    G4cout << "============================================" << G4endl;
}
