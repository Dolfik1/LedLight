#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "FlashStream.h"

ESP8266WebServer server(80);
const __FlashStringHelper *html = reinterpret_cast<const __FlashStringHelper *>(&__html[0]);

void setup() {
 
  Serial.begin(115200);
  WiFi.begin("Network name", "Password");  // Connect to the WiFi network
 
  while (WiFi.status() != WL_CONNECTED) {  // Wait for connection
    delay(500);
    Serial.println("Waiting to connect…");
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Print the local IP
 
  server.on("/", handleRootPath);  // Associate the handler function to the path
  server.begin(); // Start the server
  Serial.println("Server listening");
}
 
void loop() {
 
  server.handleClient(); //Handling of incoming requests
 
}
 
void handleRootPath() { // Handler for the rooth path
  FlashStream htmlStream((PGM_P)html, "index.html");
  server.streamFile(htmlStream, "text/html");
}