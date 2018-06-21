#include <Arduino.h>
#include <ble/BLE.h>
#include <ble/services/UARTService.h>

#include "Types.h"
#include "SmartLockService.h"

SmartLockService *smartLockServicePtr;
UARTService *uartServicePtr;

#define DEBUG_EN 0

#if DEBUG_EN
  #define PRINT(VAL) Serial.println(VAL)
#else
  #define PRINT(VAL)
#endif

#include "BLEInit.h"

void setup() {
  Serial.begin(115200);
  PRINT("Program Start");

  BLE &ble = BLE::Instance();
  ble.init(setupBLE);

  /* SpinWait for initialization to complete. This is necessary because the
     BLE object is used in the main loop below. */
  while (ble.hasInitialized()  == false) {
    /* spin loop */
  }
}

void loop() {
  BLE::Instance().waitForEvent();

  while (Serial.available()) {
    uint8_t receivedByte = Serial.read();
    uartServicePtr->write(&receivedByte, 1);
  }
}
