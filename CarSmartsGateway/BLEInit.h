
#include "BLESecurity.h"

void connectionCallback(const Gap::ConnectionCallbackParams_t *params)
{
    Serial.println("Connected!");
}

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params) {
  Serial.println("Disconnected..");
  
  //TODO: More interesting auto-lock
  lockController.performCommand(SLCommandLock);
  
  BLE::Instance().gap().startAdvertising();
};

void dataWrittenCallback(const GattWriteCallbackParams *params)
{
  if (params->handle == smartLockServicePtr->getCommandHandle()) {
    if (params->len == 1) {
      SmartLockCommand command = (SmartLockCommand)params->data[0];

      lockController.performCommand(command);
    }
  }
}

void onBleInitError(BLE &ble, ble_error_t error)
{
  /* Initialization error handling should go here */
  Serial.println("BLE Init Error");
}

void setupAdvertising(BLE &ble) {
  // Ad Payload

  Gap &gap = ble.gap();

  gap.clearAdvertisingPayload();
  gap.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);

  gap.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                   SERVICE_UUID_REV, sizeof(SERVICE_UUID_REV));

  // set adv_type
  gap.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);


  // set adv_interval, 100ms in multiples of 0.625ms.
  gap.setAdvertisingInterval(160);
  // set adv_timeout, in seconds
  gap.setAdvertisingTimeout(0);

  // set device name
  gap.setDeviceName((const uint8_t *)"Smart Car Gateway");
  // set tx power,valid values are -40, -20, -16, -12, -8, -4, 0, 4
  gap.setTxPower(4);

  // start advertising
  gap.startAdvertising();
}

void setupBLE(BLE::InitializationCompleteCallbackContext *params)
{
  BLE&        ble   = params->ble;
  ble_error_t error = params->error;

  if (error != BLE_ERROR_NONE) {
    onBleInitError(ble, error);
    return;
  }

  /* Ensure that it is the default instance of BLE */
  if (ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
    return;
  }

  setupSecurity(ble);

  /* Handle Events */
  ble.gap().onConnection(connectionCallback);
  ble.gap().onDisconnection(disconnectionCallback);
  ble.gattServer().onDataWritten(dataWrittenCallback);

  /* Setup primary service. */
  smartLockServicePtr = new SmartLockService(ble);

  /* setup advertising */
  setupAdvertising(ble);
}
