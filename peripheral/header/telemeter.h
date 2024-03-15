#ifndef __TELEMETER_H__
#define __TELEMETER_H__
#include "config.h"

// 需要注意的是 DL1B 最高支持 400KHz 的 IIC 通信速率

#define DL1B_XS_PIN                 ( P23 )

#define DL1B_TIMEOUT_COUNT          ( 1000 )                                    // DL1B 超时计数

//================================================定义 DL1B 内部地址================================================

#define DL1B_DEV_ADDR                                           ( 0x52 >> 1 )   // 0b0101001，地址为低7位

#define DL1B_I2C_SLAVE__DEVICE_ADDRESS                          ( 0x0001 )
#define DL1B_GPIO__TIO_HV_STATUS                                ( 0x0031 )
#define DL1B_SYSTEM__INTERRUPT_CLEAR                            ( 0x0086 )
#define DL1B_RESULT__RANGE_STATUS                               ( 0x0089 )
#define DL1B_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0     ( 0x0096 )
#define DL1B_FIRMWARE__SYSTEM_STATUS                            ( 0x00E5 )

//================================================定义 DL1B 内部地址================================================

extern uint8 dl1b_finsh_flag;
extern const unsigned char code dl1b_default_configuration [135];

uint16   dl1b_get_distance (void);

void   dl1b_int_handler  (void);
uint8  dl1b_init         (void);
#endif
