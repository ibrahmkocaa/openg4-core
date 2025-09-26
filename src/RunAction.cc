#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

// Sayaçların tanımı
G4int RunAction::fissionCount = 0;
G4int RunAction::captureCount = 0;
G4int RunAction::escapeCount  = 0;
G4int RunAction::totalNeutrons = 0;
G4int RunAction::elasticCount = 0;
G4int RunAction::inelasticCount = 0;

std::map<std::string, G4double> RunAction::volumeEnergy;
std::map<std::string, G4int>    RunAction::volumeFission;
std::map<std::string, G4int>    RunAction::volumeCapture;

G4int RunAction::producedNeutrons  = 0;
G4int RunAction::producedGammas    = 0;
G4int RunAction::producedElectrons = 0;
G4int RunAction::producedFragments = 0;

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

    volumeEnergy.clear();
    volumeFission.clear();
    volumeCapture.clear();

    producedNeutrons  = 0;
    producedGammas    = 0;
    producedElectrons = 0;
    producedFragments = 0;
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
    G4cout << "--------------------------------------------" << G4endl;

    // Enerji depozisyonu özeti
    G4cout << " Energy deposition by volume: " << G4endl;
    for (auto& kv : volumeEnergy) {
        G4cout << "   " << kv.first << " : "
               << kv.second / MeV << " MeV" << G4endl;
    }

    // Fission özeti
    G4cout << " Fission events by volume: " << G4endl;
    for (auto& kv : volumeFission) {
        G4cout << "   " << kv.first << " : "
               << kv.second << G4endl;
    }

    // Capture özeti
    G4cout << " Capture events by volume: " << G4endl;
    for (auto& kv : volumeCapture) {
        G4cout << "   " << kv.first << " : "
               << kv.second << G4endl;
    }

    G4cout << "--------------------------------------------" << G4endl;
    G4cout << " Particles produced in fission: " << G4endl;
    G4cout << "   Neutrons   : " << producedNeutrons << G4endl;
    G4cout << "   Gammas     : " << producedGammas << G4endl;
    G4cout << "   Electrons  : " << producedElectrons << G4endl;
    G4cout << "   Fragments  : " << producedFragments << G4endl;
    G4cout << "============================================" << G4endl;
}
