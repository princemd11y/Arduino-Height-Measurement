#include<DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define Trig 3
#define Echo 2
#define DHTPIN 13

DHT dht(DHTPIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  dht.begin();
  lcd.init();           // initialise le LCD
  lcd.backlight();      // allume le rétroéclairage
}

void loop() {
  // put your main code here, to run repeatedly:
  //lecture de la temprature
  float t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println("Erreur lecture DHT11 !");
    t = 20; 
    return;
  }
  //Calcul de la vitesse du son
  float v = 331.3 + (0.606*(t));
  Serial.print("Temperature : ");
  Serial.println(t);

  //Affichage sur LCD
  lcd.setCursor(0, 1);  // col 0, ligne 0
  lcd.print("T: ");
  lcd.setCursor(2, 1);  // col 2, ligne 0
  lcd.print(t);
  lcd.setCursor(8, 1);  // col 8, ligne 0
  lcd.print("V: ");
  lcd.setCursor(10, 1);  // col 10, ligne 0
  lcd.print(v);


  Serial.print("Vitesse du son : ");
  Serial.println(v);
  v = v*0.0001; //Conversion de m/s en cm/microsec
  //caclul de la distance
  float distance = mesure(v);

  Serial.print("Distance : ");
  Serial.println(distance);

  lcd.setCursor(0, 0);  // col 0, ligne 0
  lcd.print("Taille: ");
  lcd.setCursor(8, 0);  // col 8, ligne 0
  lcd.print(distance);
  lcd.setCursor(14, 0);  // col 14, ligne 0
  lcd.print("cm");
}

float mesure(float v_son)
{
  float distance_total;  //Aller-retour (2X la distance mesurer)
  float duree;
  digitalWrite(Trig, LOW);

  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);

  duree = pulseIn(Echo, HIGH); //donne la valeur en microsec
  
  distance_total = duree*v_son;
  float distance = distance_total/2;
  return distance;
}


