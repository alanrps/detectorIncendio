#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 12 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

const int dhtPin = 12;
const int gasPin = A0; //GAS sensor output pin to Arduino analog A0 pin 
const int buzzerPin = 4;
const int chamaPin = 5;
String apiKey = "T35YEB5NXNSD60LS";
const char* ssid = "LIGUE-8963";
const char* password = "6592218867";
const char* server = "api.thingspeak.com";
 
WiFiClient client;
DHT dht(dhtPin, DHTTYPE);
 
void setup(){
  Serial.begin(115200);
  delay(10);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  dht.begin();
}
 
void loop() {
  int valorGas = analogRead(gasPin);
  int valorChama = digitalRead(chamaPin);
  float valorUmidade = dht.readHumidity();
  float valorTemperatura = dht.readTemperature();

  Serial.print("Valor Sensor Gas: ");
  Serial.println(valorGas);
  
  Serial.print("Valor Sensor Chama: ");
  Serial.println(valorChama);
  
  Serial.print("Valor Sensor Umidade: ");
  Serial.println(valorUmidade);
  
  Serial.print("Valor Sensor Temperatura: ");
  Serial.println(valorTemperatura);

  Serial.println("----------------------------");
  
  if(valorGas > 400 || valorChama == 0){
    tone(buzzerPin,1000);
  }
  else
    noTone(buzzerPin);
  
  if (client.connect(server,80)) {
    String postStr = apiKey;
    
    postStr +="&field1=";
    postStr += valorGas;
    postStr +="&field2=";
    postStr += valorChama;
    postStr += "\r\n\r\n";
    postStr +="&field3=";
    postStr += valorUmidade;
    postStr +="&field4=";
    postStr += valorTemperatura;
     
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop(); 
  
  delay(10000);
}
