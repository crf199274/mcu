#ifndef   __OPBIT_H
#define  __OPBIT_H

#include "stm32f10x.h"

#define  Op_BitAddr(addr,bit_p)    (((addr&0xF0000000)+0x02000000)+(((addr&0xFFFFF)<<5)+(bit_p<<2)))
#define  C_addr(addr)                     *((volatile unsigned int*)(addr))
#define  send_pa(addr,bit_p)          (C_addr(Op_BitAddr(addr,bit_p)))

#define  GPIOA_Out_Addr             ((GPIOA_BASE)+12)
#define  GPIOB_Out_Addr             ((GPIOB_BASE)+12)
#define  GPIOC_Out_Addr             ((GPIOC_BASE)+12)
#define  GPIOD_Out_Addr             ((GPIOD_BASE)+12)
#define  GPIOE_Out_Addr             ((GPIOE_BASE)+12)
#define  GPIOF_Out_Addr             ((GPIOF_BASE)+12)
#define  GPIOG_Out_Addr             ((GPIOG_BASE)+12)


#define  GPIOA_In_Addr             ((GPIOA_BASE)+8)
#define  GPIOB_In_Addr             ((GPIOB_BASE)+8)
#define  GPIOC_In_Addr             ((GPIOC_BASE)+8)
#define  GPIOD_In_Addr             ((GPIOD_BASE)+8)
#define  GPIOE_In_Addr             ((GPIOE_BASE)+8)
#define  GPIOF_In_Addr             ((GPIOF_BASE)+8)
#define  GPIOG_In_Addr             ((GPIOG_BASE)+8)


#define  PAout(n)                              (send_pa(GPIOA_Out_Addr,n))
#define  PAin(n)                                (send_pa(GPIOA_In_Addr,n))

#define  PBout(n)                              (send_pa(GPIOB_Out_Addr,n))
#define  PBin(n)                                (send_pa(GPIOB_In_Addr,n))

#define  PCout(n)                              (send_pa(GPIOC_Out_Addr,n))
#define  PCin(n)                                (send_pa(GPIOC_In_Addr,n))

#define  PDout(n)                              (send_pa(GPIOD_Out_Addr,n))
#define  PDin(n)                                (send_pa(GPIOD_In_Addr,n))

#define  PEout(n)                              (send_pa(GPIOE_Out_Addr,n))
#define  PEin(n)                                (send_pa(GPIOE_In_Addr,n))

#define  PFout(n)                              (send_pa(GPIOF_Out_Addr,n))
#define  PFin(n)                                (send_pa(GPIOF_In_Addr,n))

#define  PGout(n)                              (send_pa(GPIOG_Out_Addr,n))
#define  PGin(n)                                (send_pa(GPIOG_In_Addr,n))

#endif
