// Программа для управления RGB-лентой на светодиодах типа ws281x.
// Коммерческое использование проекта без согласия автора запрещено.
// Есть вопросы? Пишите: sergey@getbyte.ru

#include <stdint.h>
#include "smartled.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <math.h>

#define PIXEL_COUNT 60
#define PIN_PIXEL 2

typedef struct AccessPoint
{
  const char* apName;
  const char* apPassword;
} AccessPoint;

const AccessPoint ap[] = {
  {"WiHome", "keenetic"},
  {"ssid2", "password2"}
};
int apCount = 2;
const char* host = "ledlight";

ESP8266WebServer server(80);
MDNSResponder mdns;
WebSocketsServer webSocket = WebSocketsServer(81);

SmartLED* smart;

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".bmp")) return "image/bmp";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool splitValueString(char* src, char* option, char* value)
{
    char* part = strtok (src, ":");
    if (part != 0)
    {
        strcpy(option, part);
        part = strtok (NULL, ":");
        if (part != 0)
            strcpy(value, part);
        else
            return false;
    } else 
    {
        return false;
    }
    return true;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * buffer, size_t len)
{
    char incoming[len+2];
    memset(incoming, 0, len+2);
    memcpy(incoming, (char*)buffer, len);
    char optionName[24];
    char optionValue[24];
    IPAddress ip = webSocket.remoteIP(num);
    switch(type) 
    {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: 
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], buffer);
            webSocket.sendTXT(num, "Connected");
            break;
        case WStype_TEXT:
            switch (buffer[0])
            {
                case '#': smart->selectMode((const char *) &incoming[1]);
                          Serial.print("New mode selected :");
                          Serial.println(smart->mode());
                          break;
                case '$': memset(optionName, 0, sizeof(optionName));
                          memset(optionValue, 0, sizeof(optionValue));
                          if (!splitValueString((char *) &incoming[1], optionName, optionValue)) 
                              return;
                          smart->setOption(optionName, optionValue);
                          break;
                case '@': memset(optionName, 0, sizeof(optionName));
                          memset(optionValue, 0, sizeof(optionValue));
                          if (!splitValueString((char *) &incoming[1], optionName, optionValue)) 
                              return;
                          smart->setOption(optionName, optionValue);
                          break;
                case '?': smart->dump();
                          break;
                default:
                          Serial.println("Unknown operation");
                          break;
            }
            break;
    }
}

void handleRoot()
{
    String path = "/index.html";
    String contentType = getContentType(path);
    if (SPIFFS.exists(path))
    {
        File file = SPIFFS.open(path, "r");
        size_t sent = server.streamFile(file, contentType);
        file.close();
    }
}

bool handleFileRead(String path)
{
    String contentType = getContentType(path);
    if (SPIFFS.exists(path))
    {
        File file = SPIFFS.open(path, "r");
        server.sendHeader("Content-Encoding", "gzip");
        server.sendHeader("Cache-Control", "max-age=533280");
        size_t sent = server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

void setup(void)
{
    Serial.begin(115200);
    Serial.print("\n");
    SPIFFS.begin();

    int apIdx = 0;
    WiFi.begin(ap[apIdx].apName, ap[apIdx].apPassword);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(" .");
        if (++attempts > 5)
        {
            delay(500);
            attempts = 0;
            WiFi.disconnect(true);
            WiFi.mode(WIFI_AP);
            delay(500);
            apIdx++;
            if (apIdx >= apCount)
            {
                apIdx = 0;
            }
            WiFi.begin(ap[apIdx].apName, ap[apIdx].apPassword);
            delay(500);
        }
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.print(ap[apIdx].apName);
    Serial.print(", IP address: ");
    Serial.println(WiFi.localIP());

    smart = new SmartLED(PIXEL_COUNT, PIN_PIXEL, NEO_GRB, true);
    

    mdns.begin(host, WiFi.localIP());
    
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    server.on("/", handleRoot);
    server.on("/index.html", handleRoot);

    server.onNotFound([]()
    {
        if (!handleFileRead(server.uri()))
            server.send(404, "text/plain", "FileNotFound");
    });

    server.begin();
}

void loop(void)
{
    server.handleClient();
    webSocket.loop();
    smart->show();
    delay(2);
}
