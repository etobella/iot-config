/*
  Simple Wifi Switch
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "wifi_ssid";
const char* password = "wifi_password";

const char* WSUser = "ws_user";
const char* WSPass = "ws_password";

String response_json_on = "{\"status\" : \"on\"}";
String response_json_off = "{\"status\" : \"off\"}";

int gpio_13_led = 13;
int gpio_12_relay = 12;

ESP8266WebServer server(80);

void setup(void){  
  //  Init
  pinMode(gpio_13_led, OUTPUT);
  digitalWrite(gpio_13_led, HIGH);
  
  pinMode(gpio_12_relay, OUTPUT);
  digitalWrite(gpio_12_relay, HIGH);
 
  Serial.begin(115200); 
  delay(5000);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to wifi");
  Serial.println(ssid);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(gpio_13_led, LOW);
    delay(500);
    Serial.print(".");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.status());
    digitalWrite(gpio_13_led, HIGH);
    delay(500);
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    if (!server.authenticate(WSUser, WSPass))
    {
      return server.requestAuthentication();
    }
    
    if(digitalRead(gpio_12_relay)==HIGH)
    {
      server.send(200, "application/json", response_json_on);
    }
    else
    {
      server.send(200, "application/json", response_json_off);
    }
  });
  
  server.on("/on", [](){
    if (!server.authenticate(WSUser, WSPass))
    {
      return server.requestAuthentication();
    }
    
    server.send(200, "application/json", response_json_on);
    digitalWrite(gpio_13_led, LOW);
    digitalWrite(gpio_12_relay, HIGH);
    delay(1000);
  });
  
  server.on("/off", [](){
    if (!server.authenticate(WSUser, WSPass))
    {
      return server.requestAuthentication();
    }
    
    server.send(200, "application/json", response_json_off);
    digitalWrite(gpio_13_led, HIGH);
    digitalWrite(gpio_12_relay, LOW);
    delay(1000); 
  });
  
  server.begin();
  Serial.println("Server ready..");
}
 
void loop(void){
  server.handleClient();
} 
