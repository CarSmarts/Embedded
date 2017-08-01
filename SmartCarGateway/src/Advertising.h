#include "mbed.h"
#include "ble/BLE.h"

void disconnectionHandle(const Gap::DisconnectionCallbackParams_t *params) {
  BLE::Instance().gap().startAdvertising();
};

void setupAdvertising(Gap& gap, const uint8_t *car_service_uuid_rev) {
  // Ad Payload

  gap.clearAdvertisingPayload();
  gap.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);
  //    GAP.accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
  //                                     (const int *)"CSmarts", sizeof("CSmarts") - 1);
  gap.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                   car_service_uuid_rev, sizeof(car_service_uuid_rev));

  // Scan Response
  //    GAP.accumulateScanResponse(GapAdvertisingData::SHORTENED_LOCAL_NAME,
  //                              (const int *)"CSmarts2", sizeof("CSmarts2") - 1);
  //    GAP.accumulateScanResponse(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
  //                             (const int *)&car_service_uuid, sizeof(car_service_uuid));

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

  gap.onDisconnection(disconnectionHandle);

  // start advertising
  gap.startAdvertising();
}
