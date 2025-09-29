#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

RunAction::RunAction() : G4UserRunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);     // Çok çekirdek desteği
    analysisManager->SetDefaultFileType("root"); // ROOT formatı
}

RunAction::~RunAction()
{
    // Burada delete çağırma! Geant4 kendisi temizler.
}

void RunAction::BeginOfRunAction(const G4Run *)
{
    auto analysisManager = G4AnalysisManager::Instance();

    // Çıktı dosyasını aç
    analysisManager->OpenFile("results"); // sonuç: results.root

    // --- Energy deposition ---
    analysisManager->CreateNtuple("energy", "Energy deposition");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleSColumn("particle");
    analysisManager->CreateNtupleSColumn("volume");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->FinishNtuple();

    // --- Neutron history ---
    analysisManager->CreateNtuple("history", "Neutron history");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("stepNumber");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleDColumn("ekin");
    analysisManager->FinishNtuple();

    // --- Fission ---
    analysisManager->CreateNtuple("fission", "Fission events");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleSColumn("secondaryType");
    analysisManager->CreateNtupleSColumn("particle");
    analysisManager->CreateNtupleIColumn("Z");
    analysisManager->CreateNtupleIColumn("A");
    analysisManager->CreateNtupleDColumn("ekin");
    analysisManager->FinishNtuple();

    // --- Capture ---
    analysisManager->CreateNtuple("capture", "Capture events");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleSColumn("processName");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->FinishNtuple();

    // --- Tracking (neutron summary) ---
    analysisManager->CreateNtuple("tracking", "Neutron tracking summary");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("nSteps");
    analysisManager->CreateNtupleDColumn("finalEnergy");
    analysisManager->CreateNtupleDColumn("tof");
    analysisManager->CreateNtupleSColumn("escaped");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->FinishNtuple();
}

void RunAction::EndOfRunAction(const G4Run *)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();     // tüm ntuple verilerini dosyaya yaz
    analysisManager->CloseFile(); // dosyayı kapat
}
