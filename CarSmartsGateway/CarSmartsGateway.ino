#include <mcp_can.h>
#include <mcp_can_dfs.h>

#include "Types.h"
#include "SmartLockService.h"
#include "LockController.h"

#define LOCK     P0_16 //4 // Key lock
#define UNLOCK   P0_17 //5 // Key unlock
#define LOCKED   P0_21 //6 // locked sensor
#define UNLOCKED P0_23 //7 // unlocked sensor

SmartLockService *smartLockServicePtr;

LockController lockController(UNLOCKED, LOCKED, LOCK, UNLOCK);

#include "BLEInit.h"
#include "CAN.h"

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

MCP_CAN CAN0(10);                               // Set CS to pin 10

void setup() {
  Serial.begin(9600);
  Serial.println("Program Start");

  BLE &ble = BLE::Instance();
  ble.init(setupBLE);

  setupCAN(CAN0);

  /* SpinWait for initialization to complete. This is necessary because the
     BLE object is used in the main loop below. */
  while (ble.hasInitialized()  == false) {
    /* spin loop */
  }
}

void loop() {
  BLE::Instance().waitForEvent();
}
