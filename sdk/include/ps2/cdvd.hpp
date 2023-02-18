#pragma once

// CDVD Command
#define NCMD_COMMAND      0x1f402004
#define NCMD_STATUS       0x1f402005
#define NCMD_SEND         0x1f402005
#define SCMD_COMMAND      0x1f402016
#define SCMD_STATUS       0x1f402017
#define SCMD_SEND         0x1f402017
#define SCMD_RECV         0x1f402018
#define CMD_STATUS_EMPTY  0x40 /* Data is unavailable */
#define CMD_STATUS_BUSY   0x80 /* Command is processing */

#define N_STATUS_BUFFER 0x0897810
#define S_STATUS_BUFFER 0x0897820
#define N_STATUS_INDEX  0x0897890
#define S_STATUS_INDEX  0x08978A0