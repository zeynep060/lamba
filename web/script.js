/**
 * Akıllı Işık Kontrol Sistemi - JavaScript
 * ESP32 tabanlı RGB LED kontrol arayüzü
 */

class SmartLightController {
    constructor() {
        this.currentStatus = {};
        this.isUpdating = false;
        this.updateInterval = null;
        this.connectionStatus = true;
        
        this.init();
    }
    
    init() {
        this.setupEventListeners();
        this.startStatusUpdates();
        this.showInitialLoader();
    }
    
    setupEventListeners() {
        // Parlaklık slider kontrolü
        const brightnessSlider = document.getElementById('brightness');
        if (brightnessSlider) {
            brightnessSlider.addEventListener('input', (e) => {
                if (!this.isUpdating) {
                    this.updateBrightnessDisplay(e.target.value);
                    this.setBrightness(e.target.value);
                }
            });
        }
        
        // Keyboard shortcuts
        document.addEventListener('keydown', (e) => {
            this.handleKeyboardShortcuts(e);
        });
        
        // Page visibility change
        document.addEventListener('visibilitychange', () => {
            if (document.hidden) {
                this.stopStatusUpdates();
            } else {
                this.startStatusUpdates();
            }
        });
    }
    
    handleKeyboardShortcuts(e) {
        // Sadece Ctrl/Cmd tuşu ile beraber
        if (!e.ctrlKey && !e.metaKey) return;
        
        switch(e.key) {
            case '0': e.preventDefault(); this.setMode('kapali'); break;
            case '1': e.preventDefault(); this.setMode('beyaz'); break;
            case '2': e.preventDefault(); this.setMode('mavi'); break;
            case '3': e.preventDefault(); this.setMode('kirmizi'); break;
            case '4': e.preventDefault(); this.setMode('yesil'); break;
            case '5': e.preventDefault(); this.setMode('party'); break;
            case 'a': e.preventDefault(); this.toggleAuto(); break;
        }
    }
    
    showInitialLoader() {
        const elements = ['mode-text', 'brightness-display', 'ldr-status', 'auto-status'];
        elements.forEach(id => {
            const el = document.getElementById(id);
            if (el) {
                el.textContent = 'Yükleniyor...';
                el.classList.add('pulse');
            }
        });
    }
    
    startStatusUpdates() {
        this.updateStatus();
        this.updateInterval = setInterval(() => {
            this.updateStatus();
        }, 1000);
    }
    
    stopStatusUpdates() {
        if (this.updateInterval) {
            clearInterval(this.updateInterval);
            this.updateInterval = null;
        }
    }
    
    async updateStatus() {
        try {
            const response = await fetch('/status');
            if (!response.ok) throw new Error('Network response was not ok');
            
            const data = await response.json();
            this.currentStatus = data;
            this.connectionStatus = true;
            
            this.updateUI(data);
            this.removeLoadingStates();
            
        } catch (error) {
            console.error('Status güncelleme hatası:', error);
            this.connectionStatus = false;
            this.showConnectionError();
        }
    }
    
    updateUI(data) {
        this.updateModeDisplay(data.mode);
        this.updateLDRStatus(data.isDark);
        this.updateAutoButton(data.autoMode);
        this.updateBrightnessFromServer(data.brightness);
    }
    
    updateModeDisplay(mode) {
        const modeText = document.getElementById('mode-text');
        if (!modeText) return;
        
        const modeMap = {
            'kapali': '🔴 Kapalı',
            'beyaz': '⚪ Beyaz',
            'mavi': '🔵 Mavi',
            'kirmizi': '🔴 Kırmızı',
            'yesil': '🟢 Yeşil',
            'party': '🎉 Party'
        };
        
        modeText.textContent = modeMap[mode] || '❓ Bilinmiyor';
        
        // Aktif modu görsel olarak vurgula
        this.highlightActiveMode(mode);
    }
    
    highlightActiveMode(activeMode) {
        const buttons = document.querySelectorAll('.control-grid .btn');
        buttons.forEach(btn => {
            btn.classList.remove('active-mode');
            
            // Button'un hangi moda ait olduğunu kontrol et
            const onclick = btn.getAttribute('onclick');
            if (onclick && onclick.includes(`"${activeMode}"`)) {
                btn.classList.add('active-mode');
            }
        });
    }
    
    updateLDRStatus(isDark) {
        const ldrStatus = document.getElementById('ldr-status');
        if (!ldrStatus) return;
        
        if (isDark) {
            ldrStatus.textContent = '🌙 Karanlık';
            ldrStatus.className = 'value dark-indicator';
        } else {
            ldrStatus.textContent = '☀️ Aydınlık';
            ldrStatus.className = 'value light-indicator';
        }
    }
    
    updateAutoButton(autoMode) {
        const autoBtn = document.getElementById('auto-btn');
        const autoStatus = document.getElementById('auto-status');
        
        if (autoBtn) {
            if (autoMode) {
                autoBtn.classList.add('active');
                autoBtn.innerHTML = '<span class="icon">🤖</span><span>LDR Auto (Aktif)</span>';
            } else {
                autoBtn.classList.remove('active');
                autoBtn.innerHTML = '<span class="icon">🤖</span><span>LDR Otomatik Kontrol</span>';
            }
        }
        
        if (autoStatus) {
            autoStatus.textContent = autoMode ? 'Aktif' : 'Pasif';
            autoStatus.className = autoMode ? 'value status-online' : 'value';
        }
    }
    
    updateBrightnessFromServer(brightness) {
        if (!this.isUpdating) {
            const slider = document.getElementById('brightness');
            if (slider) {
                slider.value = brightness;
            }
            this.updateBrightnessDisplay(brightness);
        }
    }
    
    updateBrightnessDisplay(value) {
        const brightnessValue = document.getElementById('brightness-value');
        const brightnessDisplay = document.getElementById('brightness-display');
        
        if (brightnessValue) {
            brightnessValue.textContent = value + '%';
        }
        if (brightnessDisplay) {
            brightnessDisplay.textContent = value + '%';
        }
    }
    
    removeLoadingStates() {
        const elements = document.querySelectorAll('.pulse');
        elements.forEach(el => {
            el.classList.remove('pulse');
        });
    }
    
    showConnectionError() {
        const statusCard = document.querySelector('.status-card');
        if (statusCard) {
            statusCard.style.borderColor = 'var(--danger-color)';
            statusCard.style.background = 'rgba(220, 53, 69, 0.1)';
        }
        
        const modeText = document.getElementById('mode-text');
        if (modeText) {
            modeText.textContent = '❌ Bağlantı Hatası';
        }
    }
    
    async sendCommand(endpoint, data) {
        try {
            const response = await fetch(endpoint, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(data)
            });
            
            if (!response.ok) throw new Error('Command failed');
            
            // Komut başarılı, durumu hemen güncelle
            setTimeout(() => this.updateStatus(), 100);
            
            return true;
        } catch (error) {
            console.error('Komut gönderme hatası:', error);
            this.showNotification('Komut gönderilemedi!', 'error');
            return false;
        }
    }
    
    async setMode(mode) {
        const success = await this.sendCommand('/control', {
            action: 'setMode',
            mode: mode
        });
        
        if (success) {
            this.showNotification(`Mod değiştirildi: ${mode}`, 'success');
        }
    }
    
    async toggleAuto() {
        const success = await this.sendCommand('/control', {
            action: 'toggleAuto'
        });
        
        if (success) {
            const newState = !this.currentStatus.autoMode;
            this.showNotification(
                `Otomatik mod: ${newState ? 'Aktif' : 'Pasif'}`, 
                'info'
            );
        }
    }
    
    async setBrightness(value) {
        this.isUpdating = true;
        
        const success = await this.sendCommand('/control', {
            action: 'setBrightness',
            brightness: parseInt(value)
        });
        
        // Güncelleme bayrağını kısa süre sonra sıfırla
        setTimeout(() => {
            this.isUpdating = false;
        }, 200);
        
        if (success) {
            this.showNotification(`Parlaklık: ${value}%`, 'info');
        }
    }
    
    showNotification(message, type = 'info') {
        // Basit notification sistemi
        const notification = document.createElement('div');
        notification.className = `notification notification-${type}`;
        notification.textContent = message;
        
        // Notification stillerini ekle
        Object.assign(notification.style, {
            position: 'fixed',
            top: '20px',
            right: '20px',
            padding: '12px 20px',
            borderRadius: '8px',
            color: 'white',
            fontWeight: '600',
            zIndex: '9999',
            opacity: '0',
            transform: 'translateX(100%)',
            transition: 'all 0.3s ease',
            maxWidth: '300px'
        });
        
        // Type'a göre renk
        const colors = {
            success: '#28a745',
            error: '#dc3545',
            info: '#007bff',
            warning: '#ffc107'
        };
        notification.style.background = colors[type] || colors.info;
        
        document.body.appendChild(notification);
        
        // Animasyon
        setTimeout(() => {
            notification.style.opacity = '1';
            notification.style.transform = 'translateX(0)';
        }, 10);
        
        // Otomatik kaldırma
        setTimeout(() => {
            notification.style.opacity = '0';
            notification.style.transform = 'translateX(100%)';
            setTimeout(() => {
                if (notification.parentNode) {
                    notification.parentNode.removeChild(notification);
                }
            }, 300);
        }, 3000);
    }
}

// Global fonksiyonlar (HTML'den çağrılabilir)
let lightController;

function setMode(mode) {
    if (lightController) {
        lightController.setMode(mode);
    }
}

function toggleAuto() {
    if (lightController) {
        lightController.toggleAuto();
    }
}

// Sayfa yüklendiğinde controller'ı başlat
document.addEventListener('DOMContentLoaded', () => {
    lightController = new SmartLightController();
    
    // Keyboard shortcuts bilgisini göster
    console.log('🎮 Klavye Kısayolları:');
    console.log('Ctrl+0: Kapat');
    console.log('Ctrl+1: Beyaz');
    console.log('Ctrl+2: Mavi');
    console.log('Ctrl+3: Kırmızı');
    console.log('Ctrl+4: Yeşil');
    console.log('Ctrl+5: Party');
    console.log('Ctrl+A: Auto Toggle');
});
