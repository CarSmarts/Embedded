#ifndef __SC_TYPES_H__
#define __SC_TYPES_H__

enum SmartLockCommand {
  SLCommandCancel     = 0,
  SLCommandLock       = 1,
  SLCommandUnlock     = 2,
  SLCommandDriver     = 3,
  SLCommandWindowUP   = 6,
  SLCommandWindowDOWN = 7
};

const char *commandDescription(SmartLockCommand command) {
  switch (command) {
    case SLCommandCancel:     return "Cancel";
    case SLCommandLock:       return "Lock";
    case SLCommandUnlock:     return "Unlock";
    case SLCommandDriver:     return "Driver";
    case SLCommandWindowUP:   return "WindowUP";
    case SLCommandWindowDOWN: return "WindowDOWN";
  }
}

#endif /* #ifndef __SC_TYPES_H__ */
