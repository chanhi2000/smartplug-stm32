/*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    cmd_func.h
 *      Purpose: UART Console Definitions
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __CMD_FUNC_H
#define __CMD_FUNC_H

#include <stdint.h>
#include "cmdline.h"


#define CMD_BUF_SIZE            64

/* Command function prototypes */
extern 				tCmdLineEntry g_psCmdTable[];
extern char 	g_pcCmdBuf[CMD_BUF_SIZE];

#endif  /* CMD_FUNC_H */





