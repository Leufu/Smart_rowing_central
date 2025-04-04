#include "globals.h"
#include "gps.h"
#include "sd_header.h"
////#include "rtos_task.h" 



const int ledPin = 2; // Define el pin del LED
const int chipSelect=10;
int fileIndex=1;
String fileNameGlobal;
TinyGPSPlus gps;

void setup() 
{
  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
  digitalWrite(ledPin, LOW); // Asegúrate de que el LED esté apagado inicialmente

  Serial.begin(115200); // Serial from MCU to PC
  Serial1.begin(9600); // Serial drom MCU to GPS module
  delay(3000);
  
  if(init_sd())
  { fileNameGlobal=generateUniqueFileName();
    init_gpx_file();
  }
  

}

void loop() 
{ 
  processGPSData();
}


