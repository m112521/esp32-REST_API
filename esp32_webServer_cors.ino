#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>

const char *SSID = "fab-yota";
const char *PWD = "fablab77";
WebServer server(80);
//IPAddress local_IP(10, 2, 246, 77);

// $cmd> ipconfig
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);


StaticJsonDocument<250> jsonDocument;
char buffer[250];

const int photoPin = 34;
const int ledPinR = 35;
const int ledPinG = 32;
const int ledPinB = 33;

int sensor = 0;
int threshold = 0;

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);

  // if (!WiFi.config(local_IP, gateway, subnet)) {
  //   Serial.println("STA Failed to configure");
  // }
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void setup_routing() {
  server.enableCORS(); 
  // GET
  server.on("/sensor", getSensor); 

  // POST
  server.on("/led", HTTP_POST, handlePost);

  server.begin();
}

void create_json(int s1) {  
  jsonDocument.clear();
  jsonDocument["sensor"] = s1;
  //JsonArray data = jsonDocument.createNestedArray("data");
  //data.add(ax);
  
  serializeJson(jsonDocument, buffer);
}

void getSensor() {
  create_json(sensor);
  server.send(200, "application/json", buffer);
}

void handlePost() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  // Get RGB components
  threshold = jsonDocument["threshold"];
  //int green = jsonDocument["green"];
  
  server.send(200, "application/json", "{}");
}

void setup() {
  Serial.begin(9600);  

  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);

  connectToWiFi();  
  setup_routing();
  
  Serial.println("Sensor Begin");
  Serial.println("Initialization completed");
}
 
void loop() {
  server.handleClient();
  
  sensor = analogRead(photoPin);
  
  if (sensor < threshold) {
    digitalWrite(ledPinR, 255);
    digitalWrite(ledPinG, 255);
    digitalWrite(ledPinB, 255);
  }
  else {
    digitalWrite(ledPinR, LOW);
    digitalWrite(ledPinG, LOW);
    digitalWrite(ledPinB, LOW);
  }
  
  //Serial.println(sensor);  
  //Serial.println("A: " + String(ax) + " " + String(ay) + " " + String(az));
    
  delay(100);
}
