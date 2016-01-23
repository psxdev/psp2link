#
# Copyright (c) 2015 Antonio Jose Ramos Marquez aka bigboss (@psxdev)
#

TARGET	:= psp2linksample

OBJS= main.o font.o font_data.o file_chooser.o
LIBS=-lvita2d -lSceKernel_stub -lSceDisplay_stub -lSceGxm_stub \
	-lSceSysmodule_stub -ldebugnet -lpsp2link -lSceNet_stub -lSceNetCtl_stub  -lSceCtrl_stub -lScePgf_stub \
	-lSceCommonDialog_stub -lpng -ljpeg -lfreetype -lz -lm -lc

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -O3
ASFLAGS = $(CFLAGS)

all: $(TARGET).velf

%.velf: %.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf $(TARGET).velf $(TARGET).elf $(OBJS)
	
	
copy:
	cp $(TARGET).velf /usr/local/vitadev/rejuvenate/homebrew.elf
