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
#include <psp2/types.h>
#include <psp2/io/dirent.h>
#include <psp2/io/stat.h>
#include <debugnet.h>
#include "psp2link_internal.h"
#include "psp2link.h"

#define HOST0 "host0:"
#define UX0   "ux0:"
#define MAX_OPEN_DESCRIPTORS 10
char *localPath;
char *remotePath;

int psp2link_open_descriptors[MAX_OPEN_DESCRIPTORS];

int emuIoSetLocalDir(char *path)
{
	if(path)
	{
		if (strncmp(path, HOST0, sizeof(HOST0) - 1) != 0) 
		{
			strcpy(localPath,path);
			return 0;
		}
		else
		{
			debugNetPrintf(DEBUG,"[PSP2LINK] %s is not a valid local directory\n",path);
		}
	}
	return 0x80010016;
}
int emuIoSetRemoteDir(char *path)
{
	if(path)
	{
		if (strncmp(path, HOST0, sizeof(HOST0) - 1) == 0) 
		{
			strcpy(remotePath,path);
			return 0;
		}
		else
		{
			debugNetPrintf(DEBUG,"[PSP2LINK] %s is not a valid remote directory\n",path);
		}
		
	}
	return 0x80010016;
	
}
char * emuIoGetLocalDir()
{
	if(!localPath)
	{
		return NULL;
	}
	return localPath;
}
char * emuIoGetRemoteDir()
{
	if(!remotePath)
	{
		return NULL;
	}
	return remotePath;
}
char * emuIoCreateFullPath(char *path,int pathmode)
{
	if(!path || pathmode<0)
	{
		return NULL;
	}
	char *new_path=NULL;
	char *base_path=NULL;
	if(pathmode)
	{
		base_path=emuIoGetRemoteDir();
		if(!base_path)
		{
			return NULL;
		}
		new_path=malloc(strlen(base_path)+strlen(path)+2);
		snprintf(new_path, PSP2LINK_MAX_PATH, "%s/%s", base_path,path);
	}
	else
	{
		base_path=emuIoGetLocalDir();
		if(!base_path)
		{
			return NULL;
		}
		new_path=malloc(strlen(base_path)+strlen(path)+2);
		snprintf(new_path, PSP2LINK_MAX_PATH, "%s/%s", base_path,path);
	}
	return new_path;
		
}
int emuIoCheckByFd(SceUID fd)
{
	if(fd<0)
	{
		return -1;
	}
	int i = 0;
	int slot = -1;

	// search slot
	for (i=0; i<MAX_OPEN_DESCRIPTORS; i++) 
	{ 
		if (psp2link_open_descriptors[i]==fd) 
		{ 
				slot = i; 
				break; 	
		}
	}
	return slot;
}
int emuIoCheckFreeFd()
{
	int i=0;
	int slot=-1;
	// search slot
	for (i=0; i<MAX_OPEN_DESCRIPTORS; i++) 
	{ 
		if (psp2link_open_descriptors[i]==-1) 
		{ 
				slot = i; 
				break; 	
		}
	}
	return slot;
}
int emuIoInit(char *local,char *remote)
{
	int i;
	int ret;
	if(!local || !remote)
	{
		debugNetPrintf(DEBUG,"[PSP2LINK] arguments are not valid\n");
	}
	for(i=0;i<MAX_OPEN_DESCRIPTORS;i++)
	{
		psp2link_open_descriptors[i]=-1;
	}
	localPath=malloc(PSP2LINK_MAX_PATH);
	remotePath=malloc(PSP2LINK_MAX_PATH);
	
	ret=emuIoSetLocalDir(local);
	if(!ret)
	{
		return -1;
	}
	ret=emuIoSetRemoteDir(remote);
	if(!ret)
	{
		return -1;
	}
	return 1;
}
int emuIoFinish()
{
	int i;
	for(i=0;i<MAX_OPEN_DESCRIPTORS;i++)
	{
		if(psp2link_open_descriptors[i]>=0)
		{
			psp2LinkIoClose(psp2link_open_descriptors[i]);
		}
		psp2link_open_descriptors[i]=-1;
	}
	free(localPath);
	free(remotePath);
	return 0;
}
int emuIoGetstat(const char *file, SceIoStat *stat)
{
	if(!file || !stat)
	{
		return 0x80010016;
	}
	if (strncmp(file, HOST0, sizeof(HOST0) - 1) != 0) 
	{
		return sceIoGetstat(file,stat);
	}
	return psp2LinkIoGetstat(file,stat);
}
int emuIoChstat(const char *file, SceIoStat *stat,int bit)
{
	if(!file || !stat)
	{
		return 0x80010016;
	}
	if (strncmp(file, HOST0, sizeof(HOST0) - 1) != 0) 
	{
		return sceIoChstat(file,stat,bit);
	}
	return psp2LinkIoChstat(file,stat,bit);
	
}
int emuIoGetstatByFd(SceUID fd, SceIoStat *stat)
{
	if(!stat || fd<0)
	{
		return 0x80010016;
	}
	int slot = -1;
	// search slot
	slot=emuIoCheckByFd(fd);
	
	if (slot == -1)
	{
		return sceIoGetstatByFd(fd,stat);
	}
	
	return psp2LinkIoGetstatByFd(fd,stat);
}
int emuIoRename(const char *oldname, const char *newname)
{
	if(!oldname || !newname)
	{
		return 0x80010016;
	}
	if (strncmp(oldname, HOST0, sizeof(HOST0) - 1) != 0 &&  strncmp(newname, HOST0, sizeof(HOST0) - 1) != 0) 
	{
		return sceIoRename(oldname,newname);
	}
	if (strncmp(oldname, HOST0, sizeof(HOST0) - 1) == 0 &&  strncmp(newname, HOST0, sizeof(HOST0) - 1) == 0) 
	{
		return psp2LinkIoRename(oldname,newname);
	}
	//names have not a valid host0 device
	return 0x80010013;
;
}
int emuIoGetCwd(char *dirname)
{
	if(!dirname)
	{
		return 0x80010016;
	}
	if (strncmp(dirname, HOST0, sizeof(HOST0) - 1) != 0) 
	{
		//return sceIoGetcwd(dirname);
		return -1;
	}
	return psp2LinkIoGetCwd(dirname);
}
int emuIoSetCwd(const char *dirname)
{
	if(!dirname)
	{
		return 0x80010016;
	}
	if (strncmp(dirname, HOST0, sizeof(HOST0) - 1) != 0) 
	{
		//return sceIoSetCwd(dirname);
		return -1;
	}
	return psp2LinkIoSetCwd(dirname);	
}
int emuIoOpen(const char *file, int flags, SceMode mode)
{
	int ret;
	int result=-1;
	if(!file)
	{
		return 0x80010016;
	}
	if (strncmp(file, HOST0, sizeof(HOST0) - 1) == 0) 
	{		
		result=emuIoCheckFreeFd();
		if (result != -1)
		{
			ret=psp2LinkIoOpen(file, flags, mode);
			if(ret>=0)
			{
				psp2link_open_descriptors[result]=ret;
			}
			return ret;					
		}
		else
		{
			debugNetPrintf(DEBUG,"[PSP2LINK] too much open descritptors\n");
			return 0x80010018;
		}
	}
	else
	{
		return sceIoOpen(file, flags, mode);
	}
}
int emuIoClose(SceUID fd)
{
	int slot = -1;
	// search slot
	if(fd<0)
	{
		return 0x80010016;
	}
	slot=emuIoCheckByFd(fd);
	
	if (slot == -1)
	{
		return sceIoClose(fd);
	}
	psp2link_open_descriptors[slot]= -1;
	return psp2LinkIoClose(fd);
}
int emuIoRead(SceUID fd, void *data, SceSize size)
{
	if(fd<0 || !data)
	{
		return 0x80010016;
	}
	int slot = -1;
	// search slot
	slot=emuIoCheckByFd(fd);
	
	if (slot == -1)
	{
		return sceIoRead(fd,data,size);
	}
	return psp2LinkIoRead(fd,data,size);
}
int emuIoWrite(SceUID fd, const void *data, SceSize size)
{
	if(fd<0 || !data)
	{
		return 0x80010016;
	}
	int slot = -1;
	// search slot
	slot=emuIoCheckByFd(fd);
	
	if (slot == -1)
	{
		return sceIoWrite(fd,data,size);
	}
	return psp2LinkIoWrite(fd,data,size);
}
int emuIoLseek(SceUID fd, int offset, int whence)
{
	if(fd<0)
	{
		return 0x80010016;
	}
	int slot = -1;
	// search slot
	slot=emuIoCheckByFd(fd);
	
	if (slot == -1)
	{
		return sceIoLseek(fd,offset,whence);
	}
	return psp2LinkIoLseek(fd,offset,whence);
}
int emuIoRemove(const char *file)
{
	if(!file)
	{
		return 0x80010016;
	}
	if (strncmp(file, HOST0, sizeof(HOST0) - 1) != 0) 
	{
		return sceIoRemove(file);
	}
	return psp2LinkIoRemove(file);
	
}
int emuIoMkdir(const char *dirname, SceMode mode)
{
	if(!dirname)
	{
		return 0x80010016;
	}
	if (strncmp(dirname, HOST0, sizeof(HOST0) - 1) != 0) 
	{
		return sceIoMkdir(dirname,mode);
	}
	return psp2LinkIoMkdir(dirname,mode);
}
int emuIoRmdir(const char *dirname)
{
	if(!dirname)
	{
		return 0x80010016;
	}
	if (strncmp(dirname, HOST0, sizeof(HOST0) - 1) != 0) 
	{
		return sceIoRmdir(dirname);
	}
	return psp2LinkIoRmdir(dirname);
}
int emuIoDopen(const char *dirname)
{
	int ret;
	if(!dirname)
	{
		return 0x80010016;
	}
	int result=-1;
	if (strncmp(dirname, HOST0, sizeof(HOST0) - 1) == 0) 
	{		
		result=emuIoCheckFreeFd();
		if (result != -1)
		{
			ret=psp2LinkIoDopen(dirname);
			if(ret>=0)
			{
				psp2link_open_descriptors[result]=ret;
			}
			return ret;						
		}
		else
		{
			debugNetPrintf(DEBUG,"[PSP2LINK] too much open descritptors\n");
			return 0x80010018;
		}	
	}
	else
	{
		return sceIoDopen(dirname);
	}
}
int emuIoDread(SceUID fd, SceIoDirent *dir)
{
	if(!dir || fd<0)
	{
		return 0x80010016;
	}
	int slot = -1;
	// search slot
	slot=emuIoCheckByFd(fd);
	
	if (slot == -1)
	{
		return sceIoDread(fd,dir);
	}
	return psp2LinkIoDread(fd,dir);	
}
int emuIoDclose(SceUID fd)
{
	if(fd<0)
	{
		return 0x80010016;
	}
	int slot = -1;
	// search slot
	slot=emuIoCheckByFd(fd);
	
	if (slot == -1)
	{
		return sceIoDclose(fd);
	}
	psp2link_open_descriptors[slot]= -1;
	return psp2LinkIoDclose(fd);		
}
