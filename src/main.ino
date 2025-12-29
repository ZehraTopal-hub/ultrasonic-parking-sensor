#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 9
#define ECHO_PIN 10

#define GREEN_LED 5
#define YELLOW_LED 6
#define RED_LED 7
#define BUZZER 8

LiquidCrystal_I2C lcd(0x20, 16, 2);   

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
  Serial.println("Yanki_Suresi_us,Mesafe_cm");

  Wire.begin();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Park Sensoru");
  delay(1500);
  lcd.clear();
}

void loop() {
  long sure;
  int mesafe;

  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  sure = pulseIn(ECHO_PIN, HIGH, 30000);  

  if (sure == 0) {
    lcd.setCursor(0,0);
    lcd.print("Olcum yok     ");
    return;
  }

  mesafe = sure * 0.034 / 2;

  // CSV çıktısı
  Serial.print(sure);
  Serial.print(",");
  Serial.println(mesafe);

  
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  noTone(BUZZER);

  // PARK SENSÖRÜ KARAR MANTIĞI
  if (mesafe > 60 && mesafe <= 300) {             // Güvenli
    digitalWrite(GREEN_LED, HIGH);
  }
  else if (mesafe > 20 && mesafe <= 60) {         // Yaklaşıyor
    digitalWrite(YELLOW_LED, HIGH);
    tone(BUZZER, 1200);
    delay(150);
    noTone(BUZZER);
    delay(300);
  }
  else if (mesafe >= 6 && mesafe <= 20) {         // Çok yakın
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 2000);
    delay(100);
    noTone(BUZZER);
    delay(100);
  }
  else if (mesafe < 6 && mesafe > 0) {            // Çarpacak
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 2500);
  }

  // LCD gösterim
  lcd.setCursor(0,0);
  lcd.print("Mesafe: ");
  lcd.print(mesafe);
  lcd.print(" cm   ");

  lcd.setCursor(0,1);
  if (mesafe > 60)
    lcd.print("Guvenli       ");
  else if (mesafe > 20)
    lcd.print("Yaklasiyor    ");
  else if (mesafe >= 6)
    lcd.print("!!! COK YAKIN ");
  else
    lcd.print("CARPMA RISK! ");

  delay(50);
}
