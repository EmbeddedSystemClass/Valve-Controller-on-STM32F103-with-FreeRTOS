#ifndef __VALVE_H__
#define __VALVE_H__
//#include "hw_config.h"
//
typedef enum
{
  VALVE_START=0,
  VALVE_ZW_CONNECT,
  VALVE_ON,
  VALVE_OFF,
  VALVE_FACTORYRESET,
  VALVE_IDE,
}val_mode_t;
typedef enum
{
  CMD_CLASS_BIN_OFF = 0x00,
  CMD_CLASS_BIN_ON  = 0xFF
}
CMD_CLASS_BIN_SW_VAL;
#endif
