#
# Copyright (c) 2015 Antonio Jose Ramos Marquez aka bigboss (@psxdev)
#

TARGET	:= psp2linksample

OBJS= main.o font.o font_data.o file_chooser.o
LIBS=-lvita2d -lpng -lz -lSceCtrl_stub -lSceDisplay_stub -lSceGxm_stub -lSceNet_stub -lSceNetCtl_stub -ldebugnet -lpsp2link

PREFIX  = arm-none-eabi
AS	= $(PREFIX)-as
CC      = $(PREFIX)-gcc
CXX	=$(PREFIX)-g++
READELF = $(PREFIX)-readelf
OBJDUMP = $(PREFIX)-objdump
CFLAGS  = -O2 -Wall -specs=psp2.specs $(DEFINES) $(INCDIR) -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -ffunction-sections
CXXFLAGS = $(CFLAGS) -fno-unwind-tables -fno-rtti -fno-exceptions -Wno-deprecated -Wno-comment -Wno-sequence-point
ASFLAGS = $(CFLAGS) --gc-sections

all: $(TARGET).velf

$(TARGET).velf: $(TARGET).elf
	psp2-fixup -q -S $< $@
	

$(TARGET).elf: $(OBJS)
	$(CC)  $^  -o $@ $(CFLAGS) $(LIBS)

clean:
	@rm -rf $(TARGET).elf $(TARGET).velf $(OBJS) $(DATA)/*.h
	
copy:
	cp $(TARGET).velf /usr/local/psp2unity/rejuvenate/homebrew.elf
