//#include <esp32-hal-bt.c>
#pragma once
//#include "Timer_header.h"
#include <NimBLEDevice.h>
#define	size_of_struct 12000
#define  chunk_size 128

static uint64_t time_cero;
static uint64_t time_actual;
const char* matrixUUID= "180C";
static bool restart=0;
// UUIDs personalizables para var que se recivira. 
#define SERVICE_RESET_UUID "12345678-1234-1234-1234-1234567890ab" 
#define CHARACTERISTIC_RESET_UUID "87654321-4321-4321-4321-ba0987654321"

//MAC = 84:FC:E6:FB:3F:3A

//BLEService sensorService(matrixUUID);
//BLEStringCharacteristic sensorCharacteristic(matrixUUID,BLERead | BLENotify,128);

// Creation of the server, service and characteristic for sending data
static NimBLEServer* pServer = nullptr;
static NimBLEService* pSensorService = nullptr;
static NimBLECharacteristic* pSensorCharacteristic = nullptr;

//Create service and char for reset
//static NimBLEServer* pServer = nullptr;
static NimBLEService* pResetService = nullptr;
static NimBLECharacteristic* pResetCharacteristic = nullptr;

class TimeWriteCallback : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override {
    	std::string value = pCharacteristic->getValue();
	 	Serial.print("Escribió: ");
		Serial.println(pCharacteristic->getValue().c_str());
	
	  	Serial.print("Desde: ");
	  	Serial.println(connInfo.getAddress().toString().c_str());
	
	  	Serial.printf("MTU: %d, Intervalo: %d ms\n",
	                connInfo.getMTU(),
	                connInfo.getConnInterval());
    if (value.length() > 0) {
      // Actualizar el tiempo base al tiempo actual
		time_cero=get_time_us();
		pCharacteristic->setValue("0");
      Serial.println("Tiempo base actualizado desde BLE");
    }

  }
};


/*
class TimeWriteCallback : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      // Actualizar el tiempo base al tiempo actual
		time_cero=get_time_us();
		pCharacteristic->setValue("0");
      time_cero = esp_timer_get_time();
      Serial.println("Tiempo base actualizado desde BLE");
    }
  }
};
*/
void Init_BLE()
{
	//
	

 	// Initialize BLE ////
  	NimBLEDevice::init("Timer-Test");
  	NimBLEDevice::setMTU(256); //setMTU after init the BLEDEVICE XDD else crash 
	if (!NimBLEDevice::setDeviceName("Timer-Test")) 
	{Serial.print("Name not changed");}
  	pServer = NimBLEDevice::createServer();	
  	//////////////////////////////////////////////////////

  
	// Created services////////////
  	pSensorService = pServer->createService(matrixUUID); // sensor service

  	pResetService= pServer->createService(SERVICE_RESET_UUID);// reset service 

	//////////////////////
	
  	// Crear característica con notificación//////////
  	pSensorCharacteristic = pSensorService->createCharacteristic(
   	 matrixUUID,
    	NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
  	);

	// characteristic for reset
	
  	pResetCharacteristic= pResetService -> createCharacteristic(
		  CHARACTERISTIC_RESET_UUID,
		  NIMBLE_PROPERTY::WRITE
	);
	pResetCharacteristic -> setCallbacks(new TimeWriteCallback());//added callback
	//////////////////////////////////////////////////////////
	

  	// Valor inicial
  	pSensorCharacteristic->setValue("0");
	pResetCharacteristic ->setValue("0");

  	// Iniciar el servicio
  	pSensorService->start();
	pResetService->start();

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



