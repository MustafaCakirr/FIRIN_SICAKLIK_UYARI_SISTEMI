///////////////////EEM419/MİKROİŞLEMCİLER/////////////////////////////
//////ABDULKADİR YILDIRIM //MUSTAFA ÇAKIR //MUSTAFA BERKAY AYTUN /////////////////////
//////////////////I_20//FIRIN SICAKLIK UYARI SİSTEMİ//////////////////
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <LiquidCrystal.h>
Adafruit_BMP280 bmp;
#include "RTClib.h"
RTC_DS1307 RTC;
////////////////LCD DİSPLAY////////////////////////
int rs = 13, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
////////////////LEDLER//////////////////////////////////
int ledler[] = { 9, 10 };
//////////////ULTRASONİK SENSÖR////////////////////////
#define trig 7
#define echo 6
int sure;
int mesafe;
///////////////////BUZZER/////////////////////////////
#define buzzer 11

void setup() {
  Serial.begin(9600);              // 9600 BAUD HIZINDA HABERLEŞMENİN BAŞLATILMASI
  Wire.begin();                    // BMP280 VE RTC İÇİN I2C HABERLEŞME PROTOKOLÜNÜN BAŞLATILMASI
  RTC.begin();                     // DS1307(RTC) BAŞLATILMASI
  lcd.begin(16, 2);                // 16/2'lik DİSPLAY'İN BAŞLATILMASI
  bmp.begin(0x76, BMP280_CHIPID);  // BMP280(SICAKLIK SENSÖRÜ) BAŞLATILMASI
  pinMode(8, INPUT);

  for (int i = 0; i < 4; i++) {  //FOR DÖNGÜSÜ İLE LEDLERİN İ/O BAĞLANTISI
    pinMode(ledler[i], OUTPUT);
  }

  pinMode(trig, OUTPUT);  // HC-SR04(ULTRASONİK SENSÖR) TRİGGER PİNİN OUTPUT BAĞLANMASI
  pinMode(echo, INPUT);   // HC-SR04(ULTRASONİK SENSÖR) ECHO PİNİN İNPUT BAĞLANMASI

  pinMode(buzzer, OUTPUT);  // BUZZER'IN OUTPUT OLARAK BAĞLANMASI

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,  // BMP280 İÇİN ÇALIŞMA MODLARI
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);


  lcd.clear();                               //LCD EKRAN TEMİZLENDİ
  lcd.setCursor(0, 0);                       // YAZILACAK SATIRIN BELİRLENDİ
  lcd.print("FIRIN CALISIYOR!!!");           // BELİRLENEN SATIRA YAZILDI
  lcd.setCursor(0, 1);                       // YAZILACAK SATIRIN BELİRLENDİ
  lcd.print("27");                           // BELİRLENEN SATIRA YAZILDI
  lcd.print(".");                            // BELİRLENEN SATIRA YAZILDI
  lcd.print("12");                           // BELİRLENEN SATIRA YAZILDI
  lcd.print(".");                            // BELİRLENEN SATIRA YAZILDI
  lcd.print("2022");                         // BELİRLENEN SATIRA YAZILDI
  delay(3000);                               // YAZININ EKRANDA 3 SN KALMASI SAĞLANDI
  lcd.clear();                               //LCD EKRAN TEMİZLENDİ
  RTC.adjust(DateTime(__DATE__, __TIME__));  //RTC SAAT VERİSİNİN ÇEKİLMESİ
}

void loop() {
  DateTime now = RTC.now();  //RTC VERİSİNİN ÇEKİLMESİ
  dist();                    //ULTRASONİK SENSÖRDEN VERİ ALINAN ALT FONKSİYONUN ÇAĞIRILMASI

  if (bmp.readTemperature() >= 55)  //İF DÖNGÜSÜ OLUŞTURULUP 55 DERECENİN ÜZERİNE GEÇME DURUMU
  {

    if (mesafe < 50)               //55 DERECENİN ÜSTÜ DURUMLARDA BİR ALT İF DÖNGÜSÜ İLE MESAFE KONTROLÜ
    {                              // 55 DERECE VE 50CM DURUMLARINI SAĞLARSA
      digitalWrite(buzzer, HIGH);  // BUZZER AKTİF OLASMI
      led2();                      // LEDLERİN 2. YANMA DURUMUNA GEÇMESİ
      lcd.clear();                 // DİSPLAY ÜZERİNDE UYARI VERMESİ
      lcd.setCursor(0, 0);
      lcd.print("YUKSEK SICAKLIK!");
      lcd.setCursor(0, 1);
      lcd.print("UZAKLASIN!!!");
    }

    else if (mesafe > 50)  // 55 DERECE ÜSTÜ VE  MESAFENİN 50CM'DEN UZAK OLDUĞU DURUMLAR
    {
      led1();                     // LEDLERİN 1.YANMA DURUMU
      digitalWrite(buzzer, LOW);  // BUZZER PASİF DURUMDA BEKLEMESİ
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SICAKLIK: ");
      lcd.setCursor(0, 1);
      lcd.print(bmp.readTemperature());
      lcd.print("C");
    }
  }

  else if (bmp.readTemperature() < 55)  // 55 derecenin altında olduğu durumlar
  {

    if (digitalRead(8) == 1)  // BUTON ÜZERİNDEN OKUNACAK OLAN KESME
    {
      lcd.clear();
      lcd.setCursor(0, 0);  // DİSPLAY 1.SATIRDA SAAT:DAKİKA:SANİYE YAZILMASI
      lcd.print("SAAT");
      lcd.setCursor(0, 1);
      lcd.print(now.hour(), DEC);
      lcd.print(":");
      lcd.print(now.minute(), DEC);
      lcd.print(":");
      lcd.print(now.second(), DEC);
    }

    if (digitalRead(8) == 0) {
      led1();  // LEDLERİN 1.YANMA DURUMU
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SICAKLIK: ");
      lcd.setCursor(0, 1);
      lcd.print(bmp.readTemperature());  //DİSPLAY  2.SATIRDA SICAKLIK VERİSİNİN YAZILMASI
      lcd.print("C");
      digitalWrite(buzzer, LOW);  // BUZZER PASİF DURUMDA BEKLEMESİ
    }
  }
}

void dist() {  // ULTRASONİK SENSÖRDEN ALINAN VERİLERİN CM CİNSİNDEN MESAFE DEĞİŞKENİNE ATILMASI.
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(2);
  digitalWrite(trig, LOW);

  sure = pulseIn(echo, HIGH);
  mesafe = (sure / 58.2);
  delay(50);
}

void led1() {  // LED'LERİN GÜVENLİ DURUMDA ÖNCE İLERİ SONRA GERİ OLARAK SENKRON ŞEKİLDE YANMASI
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledler[i], HIGH);
    delay(80);
    digitalWrite(ledler[i], LOW);
  }

  for (int j = 4; j > -1; j--) {
    digitalWrite(ledler[j], HIGH);
    delay(80);
    digitalWrite(ledler[j], LOW);
  }
}
void led2() {  // LED'LERİN TEHLİKELİ DURUMDA 10MS ARALIKLI OLARAK AYNI ANDA YANMASI
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  delay(10);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}
