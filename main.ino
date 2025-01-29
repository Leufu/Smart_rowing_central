#include <TinyGPSPlus.h>
#include <SD.h>
#include <SPI.h>

TinyGPSPlus gps;
File gpxFile;

const int ledPin = 2; // Define el pin del LED
const int chipSelect = 10;      // Pin CS de la tarjeta SD
int fileIndex = 1;              // Contador para identificar los archivos GPX
String fileNameGlobal;          // Variable global para el nombre del archivo

void setup() 
{
  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
  digitalWrite(ledPin, LOW); // Asegúrate de que el LED esté apagado inicialmente

  Serial.begin(115200);
  Serial1.begin(9600);
  delay(3000);

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

  // Generar un nombre único para el archivo
  String fileName = generateUniqueFileName();

  // Crear y abrir el archivo GPX
  gpxFile = SD.open(fileName.c_str(), FILE_WRITE);
  if (gpxFile) {
    gpxFile.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    gpxFile.println("<gpx version=\"1.1\" creator=\"Arduino GPS\">");
    gpxFile.close();
    Serial.print("Archivo creado: ");
    Serial.println(fileName);
    fileNameGlobal = fileName; // Guarda el nombre del archivo globalmente
  } else {
    Serial.println("Error al crear el archivo GPX.");
    while (true); // Detén el programa si falla al crear el archivo
  }

  Serial.println("Iniciando GPS y buscando satélites...");
}

void loop() 
{
  // Lee los datos entrantes del GPS
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }

  if (gps.location.isValid()) 
  {
    // Si hay datos válidos, enciende el LED y guarda la información
    digitalWrite(ledPin, HIGH); // LED encendido para indicar conexión

    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitud: ");
    Serial.println(gps.altitude.meters());

    // Escribe los datos en el archivo GPX
    gpxFile = SD.open(fileNameGlobal.c_str(), FILE_WRITE);
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

    delay(1000); // Guarda datos cada segundo
  }
  else 
  {
    // Si no hay datos válidos, parpadea el LED
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
    Serial.println("Esperando datos GPS...");
  }
}

String generateUniqueFileName() {
  String fileName;
  do {
    fileName = "datos_" + String(fileIndex++) + ".gpx";
  } while (SD.exists(fileName.c_str()));
  return fileName;
}
