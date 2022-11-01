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

int sensor = 0;

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
  //Serial.println("Get sensor");
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
  int red = jsonDocument["red"];
  int green = jsonDocument["green"];
  int blue = jsonDocument["blue"];
  Serial.print("Red: ");
  Serial.print(red);
  
  // Respond to the client
  server.send(200, "application/json", "{}");
}

void setup() {
  Serial.begin(9600);  

  connectToWiFi();  
  setup_routing();
  
  Serial.println("Sensor Begin");
  Serial.println("Initialization completed");
}
 
void loop() {
  server.handleClient();
  
  sensor = sensor + 1;
  //Serial.println(sensor);  
  //Serial.println("A: " + String(ax) + " " + String(ay) + " " + String(az));
    
  delay(100);
}
