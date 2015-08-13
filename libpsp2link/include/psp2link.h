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

#ifndef _PSP2LINK_H_
#define _PSP2LINK_H_

#include <psp2/types.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>



int psp2LinkIoOpen(const char *file, int flags, SceMode mode);
int psp2LinkIoClose(SceUID fd);
int psp2LinkIoRead(SceUID fd, void *data, SceSize size);
int psp2LinkIoWrite(SceUID fd, const void *data, SceSize size);
int psp2LinkIoLseek(SceUID fd, int offset, int whence);
int psp2LinkIoRemove(const char *file);
int psp2LinkIoMkdir(const char *dirname, SceMode mode);
int psp2LinkIoRmdir(const char *dirname);
int psp2LinkIoDopen(const char *dirname);
int psp2LinkIoDread(SceUID fd, SceIoDirent *dir);
int psp2LinkIoDclose(SceUID fd);

int psp2LinkIsConnected();
int psp2LinkInit(char *serverIp, int requestPort,int debugPort, int commandPort, int level);
void psp2LinkFinish();



#endif
