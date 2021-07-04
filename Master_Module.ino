/*This is the master module i.e. the device that detects the 
presence of unwanted motion at the entrance of the house during night time*/


#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
// ESP8266 Mac address (first peer) or the Mac address of the slave device
uint8_t mac_peer1[] = {0xCC, 0x50, 0xE3, 0x56, 0x53, 0x5F};//CC:50:E3:56:53:5F
esp_now_peer_info_t peer1;
int i = 0;
typedef struct message {
   bool detect;
};
struct message myMessage;
int trig=13,echo=2,pir=25;
double dur=0,d=0;
void setup() {
  Serial.begin(115200);
   pinMode (trig,OUTPUT);
   pinMode (echo,INPUT);
   pinMode (pir,INPUT);
  WiFi.mode(WIFI_STA);
  // Get Mac Add
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println("ESP32 ESP-Now Broadcast");
  // Initializing the ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Problem during ESP-NOW init");
    return;
  }
  memcpy(peer1.peer_addr, mac_peer1, 6);
  peer1.channel = 1;
  peer1.encrypt = 0;
  // Register the peer
  Serial.println("Registering a peer 1");
  if ( esp_now_add_peer(&peer1) == ESP_OK) {
    Serial.println("Peer 1 added");
  }  
  
  }  


double dist ()
{
  digitalWrite (trig, LOW);
  delayMicroseconds (2);
  digitalWrite (trig, HIGH);
  delayMicroseconds (10);
  digitalWrite (trig, LOW);
  dur=pulseIn (echo,HIGH);
  d=(0.0343*dur)/2;
  return (d);
}

void loop() {
  if (dist()<30&&digitalRead(pir))
  {
    myMessage.detect=true;
    esp_now_send(NULL, (uint8_t *) &myMessage, sizeof(myMessage));
    while (digitalRead(pir));  
  }
  else{
    myMessage.detect=false;
  }
  Serial.println("Send a new message");
  esp_now_send(NULL, (uint8_t *) &myMessage, sizeof(myMessage));
  delay (80);
}
