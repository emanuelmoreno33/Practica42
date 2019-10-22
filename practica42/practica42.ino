// Incluimos librerías
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//led RGB
int pinLedR = 14;  // pin Rojo del led RGB
int pinLedV = 12;  // pin Verde del led RGB
int pinLedA = 13;   // pin Azul del led RGB

//DHT11
#define DHTPIN 10
#define DHTTYPE DHT11
//inicializacion del DHT
DHT dht(DHTPIN, DHTTYPE);

//PANTALLA LCD
LiquidCrystal_I2C lcd(0x27,16,2);
//byte para crear este simbolo -> °
byte N[8] = {
B01110,
B01010,
B01010,
B01110,
B00000,
B00000,
B00000,
B00000,
};

//RELAY
int relayInput = 2;
int contador = 0;
bool condicion = false;
 
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(pinLedR, OUTPUT);    // pone el pinLedR como output
  pinMode(pinLedV, OUTPUT);    // pone el pinLedV como output
  pinMode(pinLedA, OUTPUT);    // pone el pinLedA como output
  pinMode(relayInput, OUTPUT);
  lcd.init(); 
  lcd.backlight();
  lcd.print("Hola,iniciando");
}
 
void loop() {
  delay(500);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Espera un momento");
  lcd.setCursor(0,1);
  lcd.print("No se reciben datos");
    return;
  }
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println("°C");
  pantalla(t,h);

  if (t <25)
  {
    //azul
    color(0, 0, 255);
    digitalWrite(relayInput, LOW);
  }
  
  else if (t >=25 && t<=30)
  {
    //verde
    color(0, 255, 0);
    digitalWrite(relayInput, LOW);
  }
  else if (t>30 && t<40)
  {
    //amarillo
    color(255, 255, 0);
    digitalWrite(relayInput, LOW);
    
  }
  else if (t>=40)
  {
    //rojo
    color(255, 0, 0);
    digitalWrite(relayInput, HIGH);
  }
 
}

void pantalla(int temperatura, int humedad)
{
  lcd.clear();
  lcd.createChar (0,N);
  lcd.setCursor(0,0);
  String frase1 = "Temp.: ";
  String frase2 = " C";
  String mensaje = String(frase1 + temperatura); 
  lcd.print(mensaje);
  lcd.write(0);
  lcd.print(frase2);
  
  lcd.setCursor(0,1);
  String frase3 = "Humedad: ";
  String frase4 = " %";
  String mensaje2 = String(frase3 + humedad + frase4);
  lcd.print(mensaje2);
}

void color (int rojo, int verde, int azul) {
  analogWrite(pinLedR, rojo);
  analogWrite(pinLedV, verde);
  analogWrite(pinLedA, azul);
}

bool guardado=false;
