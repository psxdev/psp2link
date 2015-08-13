/*
 * psp2link library for PSP2 to communicate and use host file system with psp2client host tool 
 * Copyright (C) 2003,2015 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/psp2link
 * based on ps2vfs, ps2client, ps2link, ps2http tools. 
 * Credits goes for all people involved in ps2dev project https://github.com/ps2dev
 * This file is subject to the terms and conditions of the PSP2Link License.
 * See the file LICENSE in the main directory of this distribution for more
 * details.
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/net/net.h>
#include <psp2/kernel/threadmgr.h>

#include <psp2/types.h>
#include <debugnet.h>

int psp2link_requests_thread(SceSize args, void *argp);
int psp2link_commands_thread(SceSize args, void *argp);

int  psp2link_initialized=0;
int  psp2link_requests_port=0;
int  psp2link_commands_port=0;
int  psp2link_fileio_active=1; 
SceUID server_request_thid;
SceUID server_command_thid;
int psp2link_loglevel=1;



/**
 * Init psp2link library 
 *
 * @par Example:
 * @code
 * int ret;
 * ret = psp2LinkInit("172.26.0.2",0x4711,0x4712,0x4712, DEBUG);
 * @endcode
 * 
 * @param serverIp - server ip for udp debug
 * @param requestPort - psp2  port server for requests
 * @param debugPort - udp port for debug
 * @param commandPort - psp2  port server for commands
 * @param level - DEBUG,ERROR,INFO or NONE 
 */
int psp2LinkInit(char *serverIp, int requestPort,int debugPort, int commandPort, int level)
{
	int ret;
	psp2link_loglevel=level;
	psp2link_requests_port=requestPort;
	psp2link_commands_port=commandPort;
	
	if(psp2link_initialized)
	{
		return psp2link_initialized;
	}
	
	if(debugNetInit(serverIp,debugPort,level))
	{
		
		server_request_thid = sceKernelCreateThread("psp2link_request_server_thread", psp2link_requests_thread, 64, 0x80000, 0, 0, NULL);
		
		debugNetPrintf(DEBUG,"Server request thread UID: 0x%08X\n", server_request_thid);

		if(server_request_thid)
		{
			/* Start the server request thread */
			ret=sceKernelStartThread(server_request_thid, 0, NULL);
			if(ret<0)
			{
				debugNetPrintf(ERROR,"Server command thread could not start error: 0x%08X\n", ret);
				sceKernelDeleteThread(server_request_thid);
				return 0;
				
			}
		}
		else
		{
			debugNetPrintf(ERROR,"Server request thread could not create error: 0x%08X\n", server_request_thid);
			
			return 0;
		}
	
		server_command_thid = sceKernelCreateThread("psp2link_command_server_thread", psp2link_commands_thread, 0x10000100, 0x10000, 0, 0, NULL);
		
		debugNetPrintf(DEBUG,"Server command thread UID: 0x%08X\n", server_command_thid);

		if(server_command_thid)
		{
			/* Start the server command thread */
			ret=sceKernelStartThread(server_command_thid, 0, NULL);
			if(ret<0)
			{
				debugNetPrintf(ERROR,"Server command thread could not start error: 0x%08X\n", ret);
				sceKernelDeleteThread(server_command_thid);
				return 0;
			}
		}
		else
		{
			debugNetPrintf(ERROR,"Server command thread could not create error: 0x%08X\n", server_command_thid);
			
			return 0;
		}
	
		
	
		/*library psp2link initialized*/	
	    	psp2link_initialized = 1;
		
	}
	else
	{
		psp2link_initialized = 0;
	}

    return psp2link_initialized;
}
/**
 * Finish debugnet library 
 *
 * @par Example:
 * @code
 * psp2LinkFinish();
 * @endcode
 */
void psp2LinkFinish()
{
    if (psp2link_initialized) {
	psp2link_fileio_active=0;
        psp2link_initialized = 0;
	debugNetFinish();
    }
}
