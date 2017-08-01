#define LOCK_OPEN_INTERVAL  .25 //250

class LockController {

    DigitalOut lockKey;
    DigitalOut unlockKey;

    DigitalOut lockPin;
    DigitalOut unlockPin;

    Timeout t;

    // this is set to the current command
    // NOTE: not this might not match the current state of pins,
    // if the controller is in a temporary break between setting signals
    SmartLockCommand performingCommand;

  public:
    LockController(PinName _lockKey, PinName _unlockKey, PinName _lockPin, PinName _unlockPin) :
      lockKey(_lockKey),
      unlockKey(_unlockKey),
      lockPin(_lockPin),
      unlockPin(_unlockPin),
      performingCommand(SLCommandCancel)
    {
      pinMode(13, OUTPUT);
    }

    /// Gets the current command if one is running, otherwise cancel
    SmartLockCommand currentCommand() {
      return performingCommand;
    }

    void performCommand(SmartLockCommand command) {
      Serial.print("Command Recived: "); Serial.println(commandDescription(command));

      if (command == SLCommandCancel) {
        reset();
        return;
      }

      performingCommand = command;
      setOutput(command);

      switch (command) {
        case SLCommandUnlock:
          // Special command, requires multi-step
          t.attach(this, &LockController::commandBreak, LOCK_OPEN_INTERVAL);
          break;
        default:
          // Eveything else: one-step commands, do them and reset
          t.attach(this, &LockController::reset, LOCK_OPEN_INTERVAL);
          break;
      }

    }

  protected:
    void reset() {
      t.detach();

      performingCommand = SLCommandCancel;

      setOutput(SLCommandCancel);
    }

    // provides a short break between commands, before going back to `performingCommand`
    void commandBreak() {
      setOutput(SLCommandCancel);

      t.attach(this, &LockController::returnFromBreak, LOCK_OPEN_INTERVAL / 3);
    }

    void returnFromBreak() {
      setOutput(performingCommand);

      t.attach(this, &LockController::reset, LOCK_OPEN_INTERVAL);
    }

    void setOutput(SmartLockCommand command) {
      // SLCommandCancel: all high
      // SLCommandLock:   lockPin down,   lockKey down
      // SLCommandUnlock/Driver: unlockPin down, unlockKey down
      // SLCommandWindowDOWN: unlockKey down
      // SLCommandWindowUP:  lockKey down

      bool lock   = command == SLCommandLock;
      bool unlock = command == SLCommandUnlock || command == SLCommandDriver;
      bool up     = command == SLCommandWindowUP;
      bool down   = command == SLCommandWindowDOWN;

      Serial.print(lock); Serial.print(unlock); Serial.print(up); Serial.println(down);

      lockPin   = lock;
      unlockPin = unlock;

      // Key switch changes when we're locking or moving windows
      lockKey   = lock   || up;
      unlockKey = unlock || down;
    }
};
