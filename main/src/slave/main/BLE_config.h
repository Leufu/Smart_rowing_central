//#include <esp32-hal-bt.c>
#include "Timer_header.h"
#include <NimBLEDevice.h>
#define	size_of_struct 12000
#define  chunk_size 128


const char* matrixUUID= "180C";
static bool restart=0;

// UUIDs personalizables para var que se recivira. 
#define SERVICE_RESET_UUID "12345678-1234-1234-1234-1234567890ab" 
#define CHARACTERISTIC_RESET_UUID "87654321-4321-4321-4321-ba0987654321"



//BLEService sensorService(matrixUUID);
//BLEStringCharacteristic sensorCharacteristic(matrixUUID,BLERead | BLENotify,128);

// Creation of the server, service and characteristic for sending data
static NimBLEServer* pServer = nullptr;
static NimBLEService* pSensorService = nullptr;
static NimBLECharacteristic* pSensorCharacteristic = nullptr;

//Create service and char for reset
//static NimBLEServer* pServer = nullptr;
static NimBLEService* pService = nullptr;
static NimBLECharacteristic* pCharacteristic = nullptr;


class TimeWriteCallback : public NimBLECharacteristicCallbacks{
	void ServiceOnWrite(NimBLECharacteristic *pCharacteristic) override
	{
		std::string value= pCharacteristic-> getValue();
		if(value.length >0)
		{
			time_cero=get_time_us();
			pCharacteristic->setValue("0"); // reinicio del valor inicial para evitar tiempo actualizado constantemente
			Serial.println("Tiempo actualizado");
	
		}
	}
};
void Init_BLE()
{
	

  NimBLEDevice::init("Timer-Test");

  NimBLEDevice::setMTU(256); //setMTU after init the BLEDEVICE XDD else crash 
	if (!NimBLEDevice::setDeviceName("Timer-Test")) 
	{Serial.print("Name not changed");}
  pServer = NimBLEDevice::createServer();

  // Crear servicio
  pSensorService = pServer->createService(matrixUUID);

  // reset service 
  	pService= pServer->createService(SERVICE_RESET_UUID);


  // Crear característica con notificación
  pSensorCharacteristic = pSensorService->createCharacteristic(
    matrixUUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
  );

	// characteristic for reset
	
  pCharacteristic= pService -> createCharacteristic(
		  CHARACTERISTIC_RESET_UUID,
		  NIMBLE_PROPERTY::WRITE
		  );

	pCharacteristic -> setCallbacks(new TimeWriteCallback());
  // Valor inicial
  pSensorCharacteristic->setValue("0");

  // Iniciar el servicio
  pSensorService->start();

  // Configurar publicidad
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_RESET_UUID);

  pAdvertising->addServiceUUID(pSensorService->getUUID());
  ////pAdvertising->setScanResponse(true);
  pAdvertising->enableScanResponse(true);
  pAdvertising->start();

  Serial.println("IMU Peripheral (Sending Data) - NimBLE iniciado");
  Serial.printf("MTU = %i",NimBLEDevice::getMTU() );

}



