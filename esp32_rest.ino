#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>

const char *SSID = "fablab";
const char *PWD = "fablab77";
WebServer server(80);
//IPAddress local_IP(10, 2, 246, 77);

// $cmd> ipconfig
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);


StaticJsonDocument<250> jsonDocument;
char buffer[250];

const int photoPin = 34;
const int ledPinR = 21;
const int ledPinG = 22;
const int ledPinB = 23;

const int frequency = 5000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
const int resolution = 8;

int sensor = 0;
int sensor2 = 0;
int threshold = 0;
int R = 255;
int G = 0;
int B = 0;

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

  Serial.print(body);
  
  // Get RGB components
  threshold = jsonDocument["threshold"];
  R = jsonDocument["R"];
  G = jsonDocument["G"];
  B = jsonDocument["B"];
  //int green = jsonDocument["green"];
  
  server.send(200, "application/json", "{}");
}

void setup() {
  ledcSetup(redChannel, frequency, resolution);
  ledcSetup(greenChannel, frequency, resolution);
  ledcSetup(blueChannel, frequency, resolution);
 
  ledcAttachPin(ledPinR, redChannel);
  ledcAttachPin(ledPinG, greenChannel);
  ledcAttachPin(ledPinB, blueChannel);

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
  
  sensor = sensor + 1; //analogRead(photoPin);
  sensor2 = sensor2 - 1;

  int R255 = map(R, 0, 255, 0, 1023);
  int G255 = map(G, 0, 255, 0, 1023);
  int B255 = map(B, 0, 255, 0, 1023);

  ledcWrite(redChannel, 1023 - R255);
  ledcWrite(greenChannel, 1023 - G255);
  ledcWrite(blueChannel, 1023 - B255);
  
  // if (sensor < threshold) {
  //   digitalWrite(ledPinR, 255);
  //   digitalWrite(ledPinG, 255);
  //   digitalWrite(ledPinB, 255);
  // }
  // else {
  //   digitalWrite(ledPinR, LOW);
  //   digitalWrite(ledPinG, LOW);
  //   digitalWrite(ledPinB, LOW);
  // }
  
  //Serial.println(sensor);  
  //Serial.println("A: " + String(ax) + " " + String(ay) + " " + String(az));
    
  delay(100);
}