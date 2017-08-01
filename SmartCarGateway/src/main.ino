#include <BLE_API.H>

#include "SmartLockService.h"
#include "HondaLockController.h"

#include "Advertising.h"
#include "Types.h"

#define LOCK     4 // P0_16 // Key lock
#define UNLOCK   5 // P0_17 // Key unlock
#define LOCKED   6 // P0_21 // locked sensor
#define UNLOCKED 7 //P0_23 // unlocked sensor

SmartLockService *smartLockServicePtr;

HondaLockController lockController(UNLOCKED, LOCKED, LOCK, UNLOCK);

// Called when data is written by connected pref
void writtenHandle(const GattWriteCallbackParams *params)
{
  if (params->handle == smartLockServicePtr->getLockHandle())
  {
    if (params->len == 1) {
        LockState newState = (LockState)params->data[0];

        lockController.performStateChange(newState);
    } else {
        // Some junk sent
        //   startDFU(); ???
    }
  }
}

Serial pc(USBTX, USBRX);

void onBleInitError(BLE &ble, ble_error_t error)
{
    /* Initialization error handling should go here */
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        /* In case of error, forward the error handling to onBleInitError */
        onBleInitError(ble, error);
        return;
    }

    /* Ensure that it is the default instance of BLE */
    if(ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    ble.gattServer().onDataWritten(writtenHandle);

    /* Setup primary service. */
    smartLockServicePtr = new SmartLockService(ble, LockState::unset /* initial value */);

    /* setup advertising */
    setupAdvertising(ble.gap(), SERVICE_UUID_REV);
}

void setup() {
    BLE &ble = BLE::Instance();
    ble.init(bleInitComplete);

    /* SpinWait for initialization to complete. This is necessary because the
     * BLE object is used in the main loop below. */
    while (ble.hasInitialized()  == false) { /* spin loop */ }
}

void loop() {
  ble.waitForEvent();
}
