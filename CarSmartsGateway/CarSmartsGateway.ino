#include "Types.h"
#include "SmartLockService.h"
#include "LockController.h"

#define LOCK     P0_16 //4 // Key lock
#define UNLOCK   P0_17 //5 // Key unlock
#define LOCKED   P0_21 //6 // locked sensor
#define UNLOCKED P0_23 //7 // unlocked sensor

SmartLockService *smartLockServicePtr;
UAR *smartLockServicePtr;

LockController lockController(UNLOCKED, LOCKED, LOCK, UNLOCK);

#include "BLEInit.h"
#include "CAN.h"

//long unsigned int rxId;
//unsigned char len = 0;
//unsigned char rxBuf[8];
//char msgString[128];                        // Array to store serial string

void setup() {
  Serial.begin(115200);
  Serial.println("Program Start");

  setupCAN();

  BLE &ble = BLE::Instance();
  ble.init(setupBLE);

  /* SpinWait for initialization to complete. This is necessary because the
     BLE object is used in the main loop below. */
  while (ble.hasInitialized()  == false) {
    /* spin loop */
  }
}

void loop() {
  proccessMessages();
  BLE::Instance().waitForEvent();
}
