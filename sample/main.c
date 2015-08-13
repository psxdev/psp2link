/*
 * psp2link sample for PSP2 to communicate and use host file system with psp2client host tool 
 * Copyright (C) 2003,2015 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/psp2link
 * based on ps2vfs, ps2client, ps2link, ps2http tools. 
 * Credits goes for all people involved in ps2dev project https://github.com/ps2dev
 * File chooser, font and vita2d from @xerpi and @SMOKE587
 */

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>
#include <debugnet.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/dirent.h>
#include <vita2d.h>


#include <psp2link.h>
#include "logo.h"

//#define imgpath "cache0:/VitaDefilerClient/Documents/psp2link.png"

PSP2_MODULE_INFO(0, 0, "psp2linkSample");
static const char *supported_ext[] = {
	"elf", NULL
};
//enable debug level 3
#define LOGLEVEL 3
int main()
{
	int ret;
	char rom_path[256];
	//ip mac/linux, port udp, port request, port command, log level
	ret=psp2LinkInit("192.168.1.3",0x4711,0x4712,0x4712,LOGLEVEL);
	debugNetPrintf(DEBUG,"Test debug level %d\n",ret);
	vita2d_init();
	
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
    vita2d_texture *splash = vita2d_create_empty_texture(960, 544);
	//splash = vita2d_load_PNG_file(imgpath);
	splash = vita2d_load_PNG_buffer(psp2link_png);
	
 
           
	
	while(!psp2LinkIsConnected())
	{
		vita2d_start_drawing();
		vita2d_clear_screen();
        vita2d_draw_texture(splash, 0, 0);
        vita2d_end_drawing();
        vita2d_swap_buffers();
		
	}
	while(1)
	{
		file_choose(
					"host0:/usr/local",
					rom_path,
					"Choose a host file:",
					supported_ext
					);
	}
	vita2d_free_texture(splash);
	vita2d_fini();
	psp2LinkFinish();
	sceKernelExitProcess(0);
	return 0;
	
}
