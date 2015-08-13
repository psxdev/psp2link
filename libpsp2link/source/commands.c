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

#include <stdlib.h>
#include <string.h>
#include <psp2/net/net.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/types.h>
#include <psp2/io/dirent.h>
#include <psp2/io/stat.h>
#include <debugnet.h>
#include "psp2link_internal.h"

//TODO implement command server 
int server_commands_sockfd = -1;
int psp2link_commands_thread(SceSize args, void *argp)
{
	
	sceKernelExitDeleteThread(0);
	return 0;
}
