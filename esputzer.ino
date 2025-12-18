// =======================================================
// ESPutzer.ino
// EEPROM komplett löschen + ESP8266 Diagnose
//
// Programmiert von: Roland Mainka
// Gecodet von: Deutschland GPT
// GitHub: https://github.com/dummbold/ESPutzer
// 
// Verwendung:
// 1. Dieses Programm flashen
// 2. Seriellen Monitor öffnen (115200 Baud)
// 3. Warten bis "FERTIG" erscheint
// 4. Lesen und freuen😄
// =======================================================

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <user_interface.h>

#define EEPROM_SIZE 4096

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n");
  Serial.println("+====================================================+");
  Serial.println("|           🧹 ESPutzer v1.0 gestartet              |");
  Serial.println("|     EEPROM Reiniger + ESP8266 Diagnose Tool       |");
  Serial.println("+====================================================+");
  Serial.println();
  
  // =======================================================
  // TEIL 1: ESP8266 INFORMATIONEN AUSLESEN
  // =======================================================
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📋 CHIP INFORMATIONEN");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Chip ID
  uint32_t chipId = ESP.getChipId();
  Serial.print("Chip ID:           0x");
  Serial.println(chipId, HEX);
  
  // Flash Chip ID
  uint32_t flashChipId = ESP.getFlashChipId();
  Serial.print("Flash Chip ID:     0x");
  Serial.println(flashChipId, HEX);
  
  // CPU Frequenz
  Serial.print("CPU Frequenz:      ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
  
  // SDK Version
  Serial.print("SDK Version:       ");
  Serial.println(ESP.getSdkVersion());
  
  // Core Version
  Serial.print("Core Version:      ");
  Serial.println(ESP.getCoreVersion());
  
  // Boot Version
  Serial.print("Boot Version:      ");
  Serial.println(ESP.getBootVersion());
  
  // Boot Mode
  Serial.print("Boot Mode:         ");
  Serial.println(ESP.getBootMode());
  
  Serial.println();
  
  // =======================================================
  // SPEICHER INFORMATIONEN
  // =======================================================
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("💾 SPEICHER INFORMATIONEN");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Flash Größe
  uint32_t flashSize = ESP.getFlashChipSize();
  Serial.print("Flash Größe:       ");
  Serial.print(flashSize / 1024);
  Serial.println(" KB");
  
  // Flash Real Größe (vom Chip)
  uint32_t flashRealSize = ESP.getFlashChipRealSize();
  Serial.print("Flash Real Größe:  ");
  Serial.print(flashRealSize / 1024);
  Serial.println(" KB");
  
  // Flash Speed
  uint32_t flashSpeed = ESP.getFlashChipSpeed();
  Serial.print("Flash Speed:       ");
  Serial.print(flashSpeed / 1000000);
  Serial.println(" MHz");
  
  // Flash Mode
  FlashMode_t flashMode = ESP.getFlashChipMode();
  Serial.print("Flash Mode:        ");
  switch(flashMode) {
    case FM_QIO:  Serial.println("QIO");  break;
    case FM_QOUT: Serial.println("QOUT"); break;
    case FM_DIO:  Serial.println("DIO");  break;
    case FM_DOUT: Serial.println("DOUT"); break;
    default:      Serial.println("UNKNOWN"); break;
  }
  
  // Sketch Größe
  uint32_t sketchSize = ESP.getSketchSize();
  Serial.print("Sketch Größe:      ");
  Serial.print(sketchSize / 1024);
  Serial.println(" KB");
  
  // Freier Sketch Space
  uint32_t freeSketchSpace = ESP.getFreeSketchSpace();
  Serial.print("Freier Sketch:     ");
  Serial.print(freeSketchSpace / 1024);
  Serial.println(" KB");
  
  // Sketch MD5
  Serial.print("Sketch MD5:        ");
  Serial.println(ESP.getSketchMD5());
  
  Serial.println();
  
  // =======================================================
  // RAM INFORMATIONEN
  // =======================================================
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🧠 RAM INFORMATIONEN");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Freier Heap
  uint32_t freeHeap = ESP.getFreeHeap();
  Serial.print("Freier Heap:       ");
  Serial.print(freeHeap / 1024);
  Serial.print(" KB (");
  Serial.print(freeHeap);
  Serial.println(" Bytes)");
  
  // Heap Fragmentierung
  uint8_t heapFrag = ESP.getHeapFragmentation();
  Serial.print("Heap Fragment.:    ");
  Serial.print(heapFrag);
  Serial.println(" %");
  
  // Max Free Block Size
  uint32_t maxFreeBlock = ESP.getMaxFreeBlockSize();
  Serial.print("Max Free Block:    ");
  Serial.print(maxFreeBlock / 1024);
  Serial.print(" KB (");
  Serial.print(maxFreeBlock);
  Serial.println(" Bytes)");
  
  Serial.println();
  
  // =======================================================
  // WLAN INFORMATIONEN
  // =======================================================
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📡 WLAN INFORMATIONEN");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // MAC Adresse
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC Adresse (STA): ");
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  
  // MAC Adresse AP
  WiFi.softAPmacAddress(mac);
  Serial.print("MAC Adresse (AP):  ");
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  
  // WiFi Mode
  Serial.print("WiFi Mode:         ");
  WiFiMode_t mode = WiFi.getMode();
  switch(mode) {
    case WIFI_OFF:     Serial.println("OFF"); break;
    case WIFI_STA:     Serial.println("STA (Client)"); break;
    case WIFI_AP:      Serial.println("AP (Access Point)"); break;
    case WIFI_AP_STA:  Serial.println("AP+STA (Beide)"); break;
    default:           Serial.println("UNKNOWN"); break;
  }
  
  // WiFi Auto Connect
  Serial.print("Auto Connect:      ");
  Serial.println(WiFi.getAutoConnect() ? "EIN" : "AUS");
  
  // WiFi Sleep Mode
  Serial.print("Sleep Mode:        ");
  WiFiSleepType_t sleepMode = WiFi.getSleepMode();
  switch(sleepMode) {
    case WIFI_NONE_SLEEP:   Serial.println("NONE"); break;
    case WIFI_LIGHT_SLEEP:  Serial.println("LIGHT"); break;
    case WIFI_MODEM_SLEEP:  Serial.println("MODEM"); break;
    default:                Serial.println("UNKNOWN"); break;
  }
  
  // WiFi Phy Mode
  Serial.print("PHY Mode:          ");
  WiFiPhyMode_t phyMode = WiFi.getPhyMode();
  switch(phyMode) {
    case WIFI_PHY_MODE_11B: Serial.println("802.11b"); break;
    case WIFI_PHY_MODE_11G: Serial.println("802.11g"); break;
    case WIFI_PHY_MODE_11N: Serial.println("802.11n"); break;
    default:                Serial.println("UNKNOWN"); break;
  }
  
  Serial.println();
  
  // =======================================================
  // RESET INFORMATIONEN
  // =======================================================
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔄 RESET INFORMATIONEN");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Reset Grund
  Serial.print("Reset Grund:       ");
  String resetReason = ESP.getResetReason();
  Serial.println(resetReason);
  
  // Reset Info
  Serial.print("Reset Info:        ");
  String resetInfo = ESP.getResetInfo();
  Serial.println(resetInfo);
  
  // Cycle Count
  Serial.print("Cycle Count:       ");
  Serial.println(ESP.getCycleCount());
  
  Serial.println();
  
  // =======================================================
  // VCC (Spannungsmessung - nur wenn ADC frei)
  // =======================================================
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("⚡ SPANNUNGSMESSUNG");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Hinweis: VCC Messung funktioniert nur wenn ADC_MODE(ADC_VCC) gesetzt ist
  // und kein externer Sensor am ADC hängt
  Serial.println("VCC Messung:       Nicht verfügbar");
  Serial.println("                   (ADC für externe Sensoren verwendet)");
  
  Serial.println();
  
  // =======================================================
  // TEIL 2: EEPROM LÖSCHEN
  // =======================================================
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🧹 EEPROM WIRD GELÖSCHT");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  Serial.print("EEPROM Größe:      ");
  Serial.print(EEPROM_SIZE);
  Serial.println(" Bytes");
  Serial.println();
  
  EEPROM.begin(EEPROM_SIZE);
  
  Serial.print("Lösche EEPROM:     [");
  
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0xFF);
    
    // Fortschrittsbalken (50 Zeichen)
    if (i % (EEPROM_SIZE / 50) == 0) {
      Serial.print("█");
    }
  }
  
  Serial.println("]");
  
  EEPROM.commit();
  
  Serial.println("✅ EEPROM erfolgreich gelöscht!");
  Serial.println();
  
  // =======================================================
  // TEIL 3: EEPROM VERIFIZIEREN
  // =======================================================
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔍 EEPROM VERIFIZIERUNG");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  bool allFF = true;
  int errorCount = 0;
  
  for (int i = 0; i < EEPROM_SIZE; i++) {
    uint8_t val = EEPROM.read(i);
    if (val != 0xFF) {
      allFF = false;
      errorCount++;
      if (errorCount <= 10) {  // Nur erste 10 Fehler ausgeben
        Serial.print("❌ Byte ");
        Serial.print(i);
        Serial.print(" = 0x");
        Serial.println(val, HEX);
      }
    }
  }
  
  if (allFF) {
    Serial.println("✅ EEPROM vollständig gelöscht (alle Bytes = 0xFF)");
  } else {
    Serial.print("⚠️  WARNUNG: ");
    Serial.print(errorCount);
    Serial.println(" Bytes nicht gelöscht!");
  }
  
  Serial.println();
  
  // =======================================================
  // ABSCHLUSS
  // =======================================================
  
  Serial.println("+====================================================+");
  Serial.println("|                  ✅ FERTIG!                        |");
  Serial.println("|                                                    |");
  Serial.println("|  Jetzt kannst du das Hauptprogramm flashen!       |");
  Serial.println("+====================================================+");
  Serial.println();
  Serial.println("💡 Tipp: Speichere diese Ausgabe für Debugging-Zwecke!");
  Serial.println();
}

void loop() {
  // Nichts tun - nur warten
  delay(1000);
}
