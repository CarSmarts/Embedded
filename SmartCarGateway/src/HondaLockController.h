#include "mbed.h"
#include "Types.h"

#define LOCK_OPEN_INTERVAL .25 //250

class HondaLockController {

  DigitalOut lockSensor;
  DigitalOut unlockSensor;

  DigitalOut lockPin;
  DigitalOut unlockPin;

  LockState state;

  Ticker t;

public:
  HondaLockController(PinName _lockSensor, PinName _unlockSensor, PinName _lockPin, PinName _unlockPin) :
  lockSensor(_lockSensor), unlockSensor(_unlockSensor),
  lockPin(_lockPin), unlockPin(_unlockPin), state(LockState::unset)
  {

  }

  void performStateChange(LockState toState) {

    if (toState == LockState::unset) {
      reset();

    } else if (toState == LockState::lock) {
      setState(LockState::lock);

      t.attach(callback(this, &HondaLockController::reset), LOCK_OPEN_INTERVAL);

    } else if (toState == LockState::unlock) {
      setState(LockState::unlock);

      t.attach([&]() -> void {
        setState(LockState::unset); // temporary break in unlock

        t.attach(callback(this, &HondaLockController::reset), LOCK_OPEN_INTERVAL/3);
      }, LOCK_OPEN_INTERVAL);

    }
  }
  // TODO: window state written

  //else if (Handler->handle == window_state.getValueHandle())
  //  {
  //    uint8_t buf[1];
  //    ble.gattServer().read(window_state.getValueHandle(), buf, NULL);
  //
  //    if (buf[0] == window_state_open) {
  //      windowController.setState(first);
  //
  //      windowTicker.attach(&resetWindow, WINDOW_OPEN_INTERVAL);
  //
  //      pc.printf("UP");
  //    } else if (buf[0] == window_state_close) {
  //      windowController.setState(second);
  //
  //      windowTicker.attach(&resetWindow, WINDOW_OPEN_INTERVAL);
  //
  //      pc.printf("DOWN");
  //    } else {
  //      windowController.setState(off);
  //
  //      pc.printf("Window Off");
  //
  //      windowTicker.detach();
  //    }
  //  }

  //void resetWindow() {
  //  windowController.setState(off);
  //
  //  window_state.setValue(window_state_stopped);
  //
  //  windowTicker.detach();
  //}


protected:
  void reset() {
    t.detach();

    setState(LockState::unset);
  }

  void setState(LockState state) {
    bool lock   = state == LockState::lock;
    bool unlock = state == LockState::unlock;

    lockPin   = lockSensor   = lock ? 0 : 1; // Ground lock when state is lock
    unlockPin = unlockSensor = unlock ? 0 : 1; // Ground unlock when state is unlock
  }
};
