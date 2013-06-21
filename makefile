# makefile
#
#	This is the Linux/x86 makefile for the TextInputCanvas plugin.
#
# (c) 2013 Xojo, Inc. -- All Rights Reserved

CC = g++
BASEDIR = ./
PLUGINSDKDIR = $(BASEDIR)/sdk
SDKGLUECODEDIR = $(PLUGINSDKDIR)/Glue\ Code
SDKINCLUDEDIR = $(PLUGINSDKDIR)/Includes
SOURCEDIR = $(BASEDIR)/src

INCLUDE =	-iquote$(SDKINCLUDEDIR) \
		-iquote$(SOURCEDIR) \
		-iquote./
		

GTKINCLUDES = `pkg-config --cflags gtk+-2.0`
GTKLIBS = `pkg-config --libs gtk+-2.0`

LIBS = $(GTKLIBS)

CFLAGS = -s -O2 -D__INTEL__ -D__GCC__ -DWIDGET_GTK=1 $(INCLUDE) $(GTKINCLUDES)
PREFIXHEADER = $(SDKINCLUDEDIR)/LinuxHeader.h

PRODUCT = libTextInputCanvas.so
BUILDOUTPUTDIR = BuildObjects

OBJS =	$(BUILDOUTPUTDIR)/main.o \
	$(BUILDOUTPUTDIR)/PluginMain.o \
	$(BUILDOUTPUTDIR)/TextInputCanvas.o \
	$(BUILDOUTPUTDIR)/TextInputGTK.o \
	$(BUILDOUTPUTDIR)/TextRange.o \
	$(BUILDOUTPUTDIR)/UnicodeHelper.o

all: createbuilddir $(OBJS)
	$(CC) $(CFLAGS) -shared -o $(PRODUCT) $(OBJS) $(LIBS)

createbuilddir:
	mkdir -p $(BUILDOUTPUTDIR)

$(BUILDOUTPUTDIR)/main.o: $(SOURCEDIR)/main.cpp
	$(CC) $(CFLAGS) -include $(PREFIXHEADER) "$<" -c -o $@

$(BUILDOUTPUTDIR)/PluginMain.o: $(SDKGLUECODEDIR)/PluginMain.cpp
	$(CC) $(CFLAGS) -include $(PREFIXHEADER) "$<" -c -o $@

$(BUILDOUTPUTDIR)/TextInputCanvas.o: $(SOURCEDIR)/TextInputCanvas.cpp
	$(CC) $(CFLAGS) -include $(PREFIXHEADER) "$<" -c -o $@

$(BUILDOUTPUTDIR)/TextInputGTK.o: $(SOURCEDIR)/TextInputGTK.cpp
	$(CC) $(CFLAGS) -include $(PREFIXHEADER) "$<" -c -o $@

$(BUILDOUTPUTDIR)/TextRange.o: $(SOURCEDIR)/TextRange.cpp
	$(CC) $(CFLAGS) -include $(PREFIXHEADER) "$<" -c -o $@

$(BUILDOUTPUTDIR)/UnicodeHelper.o: $(SOURCEDIR)/UnicodeHelper.cpp
	$(CC) $(CFLAGS) -include $(PREFIXHEADER) "$<" -c -o $@


clean:
	rm -rf $(BUILDOUTPUTDIR)
	rm -f $(PRODUCT)
