SecurityManager::Passkey_t passkey = {'6', '6', '2', '5', '0', '0'};

void securitySetupCompletedCallback(Gap::Handle_t handle, SecurityManager::SecurityCompletionStatus_t status)
{
    if (status == SecurityManager::SEC_STATUS_SUCCESS) {
        Serial.println("Security success");
    } else {
        Serial.print("Security failed: "); Serial.println(status);
    }
}

void securitySetupInititatedCallback(Gap::Handle_t, bool allowBonding, bool requireMITM, SecurityManager::SecurityIOCapabilities_t iocaps)
{
  //Serial.println("Security Init");
}

void passkeyDisplayCallback(Gap::Handle_t handle, const SecurityManager::Passkey_t passkey) {
  Serial.print("Passkey: ");
  for (int i = 0; i<6; i++) {
    Serial.print((char)passkey[i]);
  }
  Serial.println();
}

void linkSecuredCallback(Gap::Handle_t handle, SecurityManager::SecurityMode_t securityMode)
{
  Serial.print("Link Secured: "); Serial.println(securityMode);
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

