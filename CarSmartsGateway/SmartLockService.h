#include "Types.h"

//static const uint8_t WINDOW_CHARACTERISTIC_UUID[] = {0x79, 0xEF, 0xB0, 0xFA, 0x48, 0x4F, 0x42, 0x02, 0x8C, 0x5F, 0xA9, 0x0E, 0xC8, 0xFD, 0x66, 0x05};
static const uint8_t COMMAND_UUID[]     = {0x61, 0x21, 0x29, 0x4f, 0x51, 0x71, 0x4f, 0x3d, 0xbd, 0x46, 0x43, 0xad, 0xaa, 0xdd, 0xa7, 0x5c};
static const uint8_t SERVICE_UUID[]     = {0xa1, 0xa4, 0xc2, 0x56, 0x33, 0x70, 0x4d, 0x9a, 0x99, 0xaa, 0x70, 0xbf, 0xa8, 0x1b, 0x90, 0x6b};
static const uint8_t SERVICE_UUID_REV[] = {0x6b, 0x90, 0x1b, 0xa8, 0xbf, 0x70, 0xaa, 0x99, 0x9a, 0x4d, 0x70, 0x33, 0x56, 0xc2, 0xa4, 0xa1};

static SmartLockCommand startingCommand = SLCommandCancel;

class SmartLockService {
  public:
    SmartLockService(BLE &_ble) :
      ble(_ble),
      command(COMMAND_UUID, &startingCommand, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
    {
      command.requireSecurity(SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM);
      
      GattCharacteristic *charTable[] = {&command};
      GattService         slService(SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
      ble.gattServer().addService(slService);
    }

    void writeCommand(SmartLockCommand newCommand) {
      ble.gattServer().write(getCommandHandle(), (uint8_t *)&newCommand, sizeof(SmartLockCommand));
    }

    SmartLockCommand getLastCommand() {
      uint8_t buffer[1];
      ble.gattServer().read(getCommandHandle(), buffer, NULL);
      return (SmartLockCommand)buffer[0];
    }

    GattAttribute::Handle_t getCommandHandle() const {
      return command.getValueHandle();
    }

  private:
    BLE                                          &ble;
    ReadWriteGattCharacteristic<SmartLockCommand> command;
};
