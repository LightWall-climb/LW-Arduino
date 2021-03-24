#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t wallVersion = 1;


#define SERVICE_UUID                     "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID

#define LIGHTWALL_ADV_SERVICE_ID         "57414C4C"
#define UART_ADV_SERVICE_ID              "6E400001"

#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++) {
          int intValue = rxValue[i];
          Serial.print(intValue);
          Serial.print(", ");
        }

        Serial.println();
        char command = rxValue[0];

        switch(command) {
          case 0: {
            uint8_t data[3] = {0, (uint8_t)WIDTH, (uint8_t)HEIGHT};
            write(data, 3);
            break;
          }
          case 1: {
            uint8_t x = rxValue[1];
            uint8_t y = rxValue[2]; 
            
            if (rxValue.length() == 6) {
              uint8_t r = rxValue[3];
              uint8_t g = rxValue[4];
              uint8_t b = rxValue[5];
              
              setPixel(x, y, CRGB(r, g, b));
            } else if (rxValue.length() == 3) {
              setPixel(x, y, activeColor);
            } else {
              
            }
            break;
          }
          case 2:   //set brightness
            break;
          case 3:   //clean display
            break;
          case 4:   //set color
            break;
          default:
            break;
        }

        FastLED.delay(50);
      }
    }
};

void write(uint8_t* data, size_t size) {
  if (deviceConnected) {
    pTxCharacteristic->setValue(data, size);
    pTxCharacteristic->notify();
    delay(30);
  }
}

void BLESetup() {
  // Create the BLE Device
  BLEDevice::init("LightWall");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_TX,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE
                                          );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(LIGHTWALL_ADV_SERVICE_ID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
//  pAdvertising->setMinPreferred(0x12);
  
  pAdvertising->start();
  Serial.println("Waiting a client connection to notify...");
}

void BLERoutine() {
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("Start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    Serial.println("Incoming connection...");
    clearDisplay();
    oldDeviceConnected = deviceConnected;
  }

//  Not connected
  if (!deviceConnected && !oldDeviceConnected) {
    paletteRoutine();
  }
}
