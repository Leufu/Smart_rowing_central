#include "sd_header.h"


bool init_sd()
{
 	// Inicialización de la tarjeta SD
  	if (!SD.begin(chipSelect)) {
    	Serial.println("Error al inicializar la tarjeta SD.");
    	// Parpadeo del LED en caso de error
    	while (true) {
      	digitalWrite(ledPin, HIGH);
      	delay(500);
      	digitalWrite(ledPin, LOW);
      	delay(500);
    	}
  	}
	return true;


}

String generateUniqueFileName() 
{
  String fileName;
  do {
    fileName = "datos_" + String(fileIndex++) + ".gpx";
  } while (SD.exists(fileName.c_str()));
  return fileName;
}

void init_gpx_file()
{

 	 // Crear y abrir el archivo GPX
 	File gpxFile = SD.open(fileNameGlobal.c_str(), FILE_WRITE);
 	 if (gpxFile) {
 	   gpxFile.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
 	   gpxFile.println("<gpx version=\"1.1\" creator=\"Arduino GPS\">");
 	   gpxFile.close();
 	   Serial.print("Archivo creado: ");
 	   Serial.println(fileNameGlobal);
 	 } else {
 	   Serial.println("Error al crear el archivo GPX.");
 	   while (true); // Detén el programa si falla al crear el archivo
 	 }

 	 Serial.println("Iniciando GPS y buscando satélites...");


}



