#include <TinyGPSPlus.h>

// The TinyGPSPlus object
TinyGPSPlus gps;

// include the SD library:
#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 10;
File dataFile;

//IMU 
#include <Arduino_LSM9DS1.h>

int accelX=1;
int accelY=1;
float x, y, z;


void setup() 
{
  Serial.begin(115200);
  Serial1.begin(9600);
  IMU.begin();

  while (!Serial) {
    ; // wait for serial port to connect
  }

  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }
  
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data

  }
}

void loop() 
{
  Serial.print("GPS lat,lng:");
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  Serial.println("");
  Serial.print("GPS date,time:");
  printDateTime(gps.date, gps.time);
  Serial.println("");
  Serial.print("GPS velocidad:");
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  Serial.println("");
  
  ///////////////
  read_Accel();

  Serial.print("aceleracion del IMU:");
  Serial.println("");
  Serial.print(accelX);
  Serial.print(" - ");
  Serial.println(accelY);
  Serial.println("");
  Serial.println("");
  //////////////////////
/*
  if(gps.location.isValid())
  {
    String dataString = "";
    dataString += String(gps.location.lat());
    dataString += ",";
    dataString += String(gps.location.lng());
    dataString += ","; 
    dataString += String(gps.date);
    dataString += ","; 
    dataString += String(gps.time);
    dataString += ","; 
    dataString += String(gps.speed.kmph());
    dataString += ","; 
    dataString += String(accelX);
    dataString += ","; 
    dataString += String(accelY);

    
    dataFile.println(dataString);
  
    // print to the serial port too:
    Serial.println(dataString);
    
    dataFile.flush();
    
  }
  */
  
        

}

/////////////funcion SD
void datalogger()
{
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ","; 
    }
  }

  dataFile.println(dataString);

  // print to the serial port too:
  Serial.println(dataString);
  
}
//////////////////

///////fuciones de gps

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}

////////////////////////

////////IMU////////////
void read_Accel()
{
  if (IMU.accelerationAvailable()) 
  {
    IMU.readAcceleration(x, y, z);
    accelX = (1+x)*100;
    accelY = (1+y)*100;
  }
}
///////////////////
