#include <ble/SecurityManager.h>

SecurityManager::Passkey_t passkey = {'6', '6', '2', '5', '0', '0'};

void securitySetupCompletedCallback(Gap::Handle_t handle, SecurityManager::SecurityCompletionStatus_t status)
{
    if (status == SecurityManager::SEC_STATUS_SUCCESS) {
        PRINT("Security success");
    } else {
        PRINT("Security failed: "); PRINT(status);
    }
}

void securitySetupInititatedCallback(Gap::Handle_t, bool allowBonding, bool requireMITM, SecurityManager::SecurityIOCapabilities_t iocaps)
{
  //Serial.println("Security Init");
}

void passkeyDisplayCallback(Gap::Handle_t handle, const SecurityManager::Passkey_t passkey) {
  PRINT("Passkey: ");
  for (int i = 0; i<6; i++) {
    PRINT((char)passkey[i]);
  }
}

void linkSecuredCallback(Gap::Handle_t handle, SecurityManager::SecurityMode_t securityMode)
{
  PRINT("Link Secured: "); PRINT(securityMode);
}

void setupSecurity(BLE& ble)
{
  /* Initialize BLE security */
  bool enableBonding = true;
  bool requireMITM   = true;
  ble.securityManager().init(enableBonding, requireMITM, SecurityManager::IO_CAPS_DISPLAY_ONLY, passkey);
  ble.securityManager().onSecuritySetupInitiated(securitySetupInititatedCallback);
  ble.securityManager().onSecuritySetupCompleted(securitySetupCompletedCallback);
  ble.securityManager().onPasskeyDisplay(passkeyDisplayCallback);
  ble.securityManager().onLinkSecured(linkSecuredCallback);
}

