ESP32 Tabanlı Akıllı Işık Kontrol Sistemi

Bu projede, modern IoT teknolojilerini kullanarak WiFi üzerinden kontrol edilebilen akıllı bir ışık sistemi geliştirdim. Sistem, ESP32 mikrokontrolcü kartı ile RGB LED ve LDR sensörü entegrasyonu sayesinde otomatik ve manuel ışık kontrolü sağlamaktadır.
Proje kapsamında, kullanıcıların akıllı telefon veya bilgisayar tarayıcısı üzerinden kolayca erişebileceği bir web arayüzü tasarlanmış ve gerçek zamanlı kontrol imkanı sunulmuştur.Bu sistem özellikle ev otomasyonu uygulamaları için oldukça pratik bir çözüm sunmaktadır.

KULLANILAN MALZEMELER VE FONKSİYONLARIıI
1. ESP32 Development Board
•	Amaç: Ana kontrol ünitesi
•	Özellikler: WiFi modülü dahil mikrokontrolcü
•	Fonksiyon: Web sunucusu oluşturma, PWM sinyali üretme, sensor okuma
•	Seçim Nedeni: Dahili WiFi özelliği ve yüksek işlem kapasitesi
2. RGB LED (4 Pinli)
•	Amaç: Renkli ışık kaynağı
•	Özellikler: Kırmız ı, yeşil, mavi renk kanalları
•	Fonksiyon: Farklı renklerde ışık üretimi ve parlaklık kontrolü
•	Bağlantı: PWM çıkışları ile analog kontrol
3. LDR (Light Dependent Resistor) Modülü
•	Amaç: Ortam ışığı sensörü
•	Özellikler: Dijital çıkış (D0), analog güç girişi
•	Fonksiyon: Karanlık/aydınlık tespiti için otomatik kontrol
•	Kullanım Alanı: Akıllı ışık açma/kapama sistemi

Kullanım:
Sistemi kullanmak için öncelikle ESP32'ye kodu yükledikten sonra cihazın WiFi bağlantısını kurmanız gerekmektedir. ESP32 "Akilli_Isik" adında bir WiFi ağı oluşturacaktır. Telefonunuzun veya bilgisayarınızın WiFi ayarlarından bu ağı bulup "12345678" şifresi ile bağlanın. Bağlantı kurulduktan sonra tarayıcıyı açıp adres çubuğuna mutlaka "http://192.168.4.1" yazın - burada önemli olan "http" kısmını "https" değil "http" olarak yazmanızdır. Sayfa açıldıktan sonra tüm kontrolleri kullanabilir, LED'leri yönetebilir ve LDR sensörünün durumunu takip edebilirsiniz. Sistem otomatik olarak güncelleneceği için sayfayı yenilemenize gerek yoktur.

LDR Auto tuşu, otomatık yakıp söndürür, diger modlar ise manueldır, örn; aç kapat kıs yükselt gibi.
