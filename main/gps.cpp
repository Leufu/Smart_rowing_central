#include "gps.h"


void processGPSData() {
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }

  if (gps.location.isValid()) {
    digitalWrite(ledPin, HIGH);

    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitud: ");
    Serial.println(gps.altitude.meters());

    File gpxFile = SD.open(fileNameGlobal.c_str(), FILE_WRITE);
    if (gpxFile) {
      gpxFile.print("<wpt lat=\"");
      gpxFile.print(gps.location.lat(), 6);
      gpxFile.print("\" lon=\"");
      gpxFile.print(gps.location.lng(), 6);
      gpxFile.println("\">");
      gpxFile.print("  <ele>");
      gpxFile.print(gps.altitude.meters());
      gpxFile.println("</ele>");
      gpxFile.println("</wpt>");
      gpxFile.close();
    } else {
      Serial.println("Error al escribir en el archivo GPX.");
    }

    delay(1000);
  } else {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
    Serial.println("Esperando datos GPS...");
  }
}

