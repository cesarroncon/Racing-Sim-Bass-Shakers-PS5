#include <WiFi.h> // ESP32 WiFi include
//#include <ESP8266WiFi.h> // ESP8266 WiFi include
#include <GT7UDPParser.h>

const char *SSID = "TP-Link_342C";
const char *Password = "02198286";
const IPAddress ip(192, 168, 1, 64); // Insert your PS4/5 IP address here
const int dacPinL = 25;//L
const int dacPinR = 26;//R

void startWiFi();

unsigned long previousT = 0; 
const long interval = 500; 

GT7_UDP_Parser gt7Telem;
Packet packetContent;

  
void setup()
{
  Serial.begin(115200);
  startWiFi();
  gt7Telem.begin(ip);
  gt7Telem.sendHeartbeat();
}

void AlmostBackTyerGripLoss(Packet packetContent){
    float TyreSlipRatioRL = gt7Telem.getTyreSlipRatio(2);
    float TyreSlipRatioRR = gt7Telem.getTyreSlipRatio(3);
  
    if (TyreSlipRatioRL > 1.10 or TyreSlipRatioRR > 1.10) { // threshold in m/s — adjust as you test
    Serial.println("Close Limit RW");
    int value = random(50, 150);  // Avoid too low values 
    dacWrite(dacPinL, value);
    }
    //dacWrite(dacPinL, 0);
} 

void BackTyerGripLoss(Packet packetContent){
    float TyreSlipRatioRL = gt7Telem.getTyreSlipRatio(2);
    float TyreSlipRatioRR = gt7Telem.getTyreSlipRatio(3);
  
    if (TyreSlipRatioRL > 1.20 or TyreSlipRatioRR > 1.20) { // threshold in m/s — adjust as you test
      Serial.println("Limit RW");
      int value = random(0, 256);  // Avoid too low values 
      dacWrite(dacPinL, value);
    }else{
      //AlmostBackTyerGripLoss(packetContent);
    }
    //dacWrite(dacPinL, 0);
}

void isABSActive(Packet packetContent) {
  if (packetContent.packetContent.brake > 200 && packetContent.packetContent.speed > 5.0f) {
    float avgWheelSpeed = 0.0f;
    for (int i = 0; i < 4; i++) {
      avgWheelSpeed += gt7Telem.getTyreSpeed(i);
    }
    avgWheelSpeed /= 4.0f;

    if (avgWheelSpeed < (packetContent.packetContent.speed * 3.6 * 0.9f)) {
      Serial.println("ABS");
      int value = random(0, 256);  // Avoid too low values 
      dacWrite(dacPinR, value);
    }
  }
  //dacWrite(dacPinR, 0);
}

void Slip_Angle(Packet packetContent){
  float threshold = 0.2; //0.10 – 0.20  radians (≈ 5.7° to 11.5°)
  float yawThreshold = 0.5; //0.3 – 0.7  rad/s
  
  float carYaw = packetContent.packetContent.orientationRelativeToNorth * 2.0f * 3.14159265f; // If orientationRelativeToNorth is 0.0 (South) -> 1.0 (North)

  float worldVelX = packetContent.packetContent.worldVelocity[0];
  float worldVelZ = packetContent.packetContent.worldVelocity[2];
  
  // Rotate world velocity vector into car local space
  float forwardVelocity  =  cos(carYaw) * worldVelZ + sin(carYaw) * worldVelX;
  float lateralVelocity  = -sin(carYaw) * worldVelZ + cos(carYaw) * worldVelX;

  //float lateralVelocity = packetContent.packetContent.worldVelocity[0]; // X-axis velocity: left-right across the car
  //float forwardVelocity = packetContent.packetContent.worldVelocity[2]; // Z-axis velocity: forward-back across the car
  //float yawRate = packetContent.packetContent.angularVelocity[1]; // Rotation speed around the vertical axis (Y)
  Serial.print("lateralVelocity: ");
  Serial.println(lateralVelocity);
  Serial.print("forwardVelocity: ");
  Serial.println(forwardVelocity);
  Serial.println();
  //float velocityAngle = atan2(lateralVelocity, forwardVelocity);
   

  /*float carOrientation = packetContent.packetContent.rotation[1] * PI; // convert normalized -1.0 to 1.0 to radians

  float slipAngle = velocityAngle - carOrientation;*/

  //if (abs(slipAngle) > threshold && abs(yawRate) > yawThreshold) {
    // Potential spin detected
    /*Serial.print("Slip Angle: ");
    Serial.println(slipAngle);*/
    /*Serial.print("velocityAngle: ");
    Serial.println(velocityAngle);*/
    /*Serial.print("carOrientation: ");
    Serial.println(carOrientation);*/
   //Serial.println(""); 
    /*int value = random(0, 256);  // Avoid too low values 
    dacWrite(dacPinL, value);*/
//}

}

void loop()
{
  delay(15);
  unsigned long currentT = millis();
  packetContent= gt7Telem.readData();
  uint8_t CarOnTrack = gt7Telem.getFlag(1);
  
  if (CarOnTrack){   
    BackTyerGripLoss(packetContent);
    AlmostBackTyerGripLoss(packetContent);
    isABSActive(packetContent);
    //Slip_Angle(packetContent);
  }else{
    /*dacWrite(dacPinL, 0);
    dacWrite(dacPinR, 0);*/
  }
  
  if (currentT - previousT >= interval)
  { // Send heartbeat every 500ms
    previousT = currentT;
    gt7Telem.sendHeartbeat();
    //Serial.println("Pckt sent");
  }
}

void startWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, Password);
  Serial.print("Attempting to connect to ");
  Serial.println(SSID);

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(250);

    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }

  Serial.print(F("Connection Successful | IP Address: "));
  Serial.println(WiFi.localIP());
}
