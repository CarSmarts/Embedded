

#ifndef __SC_TYPES_H__
#define __SC_TYPES_H__

enum class LockState {
  unset  = 0,
  lock   = 1,
  unlock = 2
};

enum class WindowState {
  stopped = 0,
  open    = 1,
  close   = 2
};

#endif /* #ifndef __SC_TYPES_H__ */
