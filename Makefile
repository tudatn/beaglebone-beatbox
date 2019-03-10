# Makefile for building embedded application.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
TARGET= beatbox

SOURCES= beatbox.c


PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
DEPLOY_PATH = $(PUBDIR)/beatbox-server-copy/
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror


# Asound process:
# get alibsound2 lib on target:
# 	# apt-get install libasound2
# Copy target's /usr/lib/arm-linux-gnueabihf/libasound.so.2.0.0 
#      to host  ~/public/asound_lib_BBB/libasound.so
# Copy to just base library:

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB


# -pg for supporting gprof profiling.
#CFLAGS += -pg



all: wav node
	$(CC_C) $(CFLAGS) $(SOURCES) audioMixer_template.c joystick.c beat.c network.c accelerometer.c support.c -o $(OUTDIR)/$(TARGET)  $(LFLAGS) -lpthread -lasound

# Copy wave files to the shared folder
wav:
	mkdir -p $(PUBDIR)/beatbox-wave-files/
	cp -R beatbox-wave-files/* $(PUBDIR)/beatbox-wave-files/ 
	
node:
	mkdir -p $(DEPLOY_PATH)
	chmod a+rwx $(DEPLOY_PATH)
	cp -R as3-server/* $(DEPLOY_PATH)
	cd $(DEPLOY_PATH) && npm install
clean:
	rm -f $(OUTDIR)/$(TARGET)
