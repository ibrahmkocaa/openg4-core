// Gerekli Geant4 sınıfları ve C++ kütüphaneleri dahil ediliyor.
#include "G4RunManagerFactory.hh"       
#include "G4UImanager.hh"               
#include "G4UIExecutive.hh"             
#include "G4VisExecutive.hh"            
#include "QGSP_BERT_HP.hh"           
#include "DetectorConstruction.hh"  
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    // Kullanıcı tanımlı sınıflar RunManager'a atanıyor.
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BERT_HP());
    runManager->SetUserInitialization(new ActionInitialization());

    // Görselleştirme yöneticisi başlatılıyor.
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // UI yöneticisi alınıyor
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc == 1) {
        // UI mod: grafik arayüzlü çalıştırma
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute macros/vis.mac"); // Görselleştirme makrosu çalıştırılıyor.
        ui->SessionStart();
        delete ui;
    } else {
        // Batch mod: komut satırından makro dosyası ile çalıştırma.
        G4String command = "/control/execute ";
        G4String filename = argv[1];
        UImanager->ApplyCommand(command + filename); // Belirtilen makro çalıştırılıyor.
    }

    delete visManager;
    delete runManager;
}
