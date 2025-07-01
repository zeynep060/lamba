#include "web_interface.h"
#include <WebServer.h>

extern WebServer server;

const char* webPageHTML = R"(
<!DOCTYPE html>
<html lang='tr'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Akilli Isik Kontrolu</title>
    <link rel='stylesheet' href='style.css'>
</head>
<body>
    <div class='container'>
        <header>
            <h1>💡 Akıllı Işık Kontrol Sistemi</h1>
            <p>ESP32 Tabanlı RGB LED Kontrolü</p>
        </header>
        
        <div class='status-card'>
            <h3>📊 Sistem Durumu</h3>
            <div class='status-grid'>
                <div class='status-item'>
                    <span class='label'>Mod:</span>
                    <span id='mode-text' class='value'>Yükleniyor...</span>
                </div>
                <div class='status-item'>
                    <span class='label'>Parlaklık:</span>
                    <span id='brightness-display' class='value'>50%</span>
                </div>
                <div class='status-item'>
                    <span class='label'>Ortam:</span>
                    <span id='ldr-status' class='value'>Yükleniyor...</span>
                </div>
                <div class='status-item'>
                    <span class='label'>Auto Mod:</span>
                    <span id='auto-status' class='value'>Pasif</span>
                </div>
            </div>
        </div>
        
        <div class='control-section'>
            <h3>🎮 Kontrol Paneli</h3>
            <div class='control-grid'>
                <button class='btn btn-off' onclick='setMode("kapali")'>
                    <span class='icon'>🔴</span>
                    <span>Kapat</span>
                </button>
                <button class='btn btn-white' onclick='setMode("beyaz")'>
                    <span class='icon'>⚪</span>
                    <span>Beyaz</span>
                </button>
                <button class='btn btn-blue' onclick='setMode("mavi")'>
                    <span class='icon'>🔵</span>
                    <span>Mavi</span>
                </button>
                <button class='btn btn-red' onclick='setMode("kirmizi")'>
                    <span class='icon'>🔴</span>
                    <span>Kırmızı</span>
                </button>
                <button class='btn btn-green' onclick='setMode("yesil")'>
                    <span class='icon'>🟢</span>
                    <span>Yeşil</span>
                </button>
                <button class='btn btn-party' onclick='setMode("party")'>
                    <span class='icon'>🎉</span>
                    <span>Party</span>
                </button>
            </div>
            
            <button class='btn btn-auto full-width' id='auto-btn' onclick='toggleAuto()'>
                <span class='icon'>🤖</span>
                <span>LDR Otomatik Kontrol</span>
            </button>
        </div>
        
        <div class='brightness-section'>
            <h3>🔆 Parlaklık Kontrolü</h3>
            <div class='brightness-control'>
                <input type='range' min='1' max='100' value='50' class='brightness-slider' id='brightness'>
                <div class='brightness-labels'>
                    <span>1%</span>
                    <span id='brightness-value'>50%</span>
                    <span>100%</span>
                </div>
            </div>
        </div>
        
        <footer>
            <p>🚀 ESP32 Akıllı Işık Sistemi v1.0</p>
            <div class='connection-info'>
                <small>WiFi: Akilli_Isik | IP: 192.168.4.1</small>
            </div>
        </footer>
    </div>

    <script src='script.js'></script>
</body>
</html>
)";

void handleRoot() {
  server.send(200, "text/html", webPageHTML);
}
