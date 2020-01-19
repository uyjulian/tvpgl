#############################################
##                                         ##
##    Copyright (C) 2019-2019 Julian Uy    ##
##  https://sites.google.com/site/awertyb  ##
##                                         ##
## See details of license at "license.txt" ##
##                                         ##
#############################################

CC = i686-w64-mingw32-gcc
CXX = i686-w64-mingw32-g++
ASM := nasm
WINDRES := i686-w64-mingw32-windres
GIT_TAG := $(shell git describe --abbrev=0 --tags)
INCFLAGS += -I. -I.. -Itjs2 -Ivisual -Ivisual/gl -Ivisual/glgen -Ivisual/IA32
ALLSRCFLAGS += $(INCFLAGS) -DGIT_TAG=\"$(GIT_TAG)\"
ASMFLAGS += $(ALLSRCFLAGS) -fwin32 -DWIN32
CFLAGS += -Ofast -march=ivybridge
CFLAGS += -gstabs 
CFLAGS += $(ALLSRCFLAGS) -Wall -Wno-unused-value -Wno-format -DNDEBUG -DWIN32 -D_WIN32 -D_WINDOWS 
CFLAGS += -D_USRDLL -DMINGW_HAS_SECURE_API -DUNICODE -D_UNICODE -DNO_STRICT
CXXFLAGS += $(CFLAGS) -fpermissive
WINDRESFLAGS += $(ALLSRCFLAGS) --codepage=65001
LDFLAGS += -static -static-libstdc++ -static-libgcc -shared -Wl,--kill-at
LDLIBS += 

%.o: %.c
	@printf '\t%s %s\n' CC $<
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	@printf '\t%s %s\n' CXX $<
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%.o: %.nas
	@printf '\t%s %s\n' ASM $<
	$(ASM) $(ASMFLAGS) $< -o$@ 

%.o: %.rc
	@printf '\t%s %s\n' WINDRES $<
	$(WINDRES) $(WINDRESFLAGS) $< $@

SOURCES := main.cpp tvpgl.rc visual/glgen/tvpgl.c
ifeq (disabled,)
SOURCES += visual/IA32/detect_cpu.cpp
SOURCES += visual/IA32/tvpgl_ia32_intf.c visual/IA32/addalphablend.nas visual/IA32/addblend.nas visual/IA32/adjust_color.nas visual/IA32/affine.nas visual/IA32/alphablend.nas visual/IA32/boxblur.nas visual/IA32/colorfill.nas visual/IA32/colormap.nas visual/IA32/darkenblend.nas visual/IA32/lightenblend.nas visual/IA32/make_alpha_from_key.nas visual/IA32/mulblend.nas visual/IA32/pixelformat.nas visual/IA32/screenblend.nas visual/IA32/stretch.nas visual/IA32/subblend.nas visual/IA32/tlg5.nas visual/IA32/tlg6_chroma.nas visual/IA32/tlg6_golomb.nas visual/IA32/tvpps_asm.nas visual/IA32/univtrans.nas
SOURCES += visual/gl/adjust_color_sse2.cpp visual/gl/blend_function.cpp visual/gl/blend_function_avx2.cpp visual/gl/blend_function_sse2.cpp visual/gl/boxblur_sse2.cpp visual/gl/colorfill_sse2.cpp visual/gl/colormap_sse2.cpp visual/gl/pixelformat_sse2.cpp visual/gl/tlg_sse2.cpp visual/gl/univtrans_sse2.cpp visual/gl/x86simdutil.cpp visual/gl/x86simdutilAVX2.cpp
endif
OBJECTS := $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)
OBJECTS := $(OBJECTS:.nas=.o)
OBJECTS := $(OBJECTS:.rc=.o)

BINARY ?= tvpgl.dll
ARCHIVE ?= tvpgl.$(GIT_TAG).7z

all: $(BINARY)

archive: $(ARCHIVE)

clean:
	rm -f $(OBJECTS) $(BINARY) $(ARCHIVE) visual/glgen/tvpgl.c visual/glgen/tvpgl.h visual/glgen/tvpgl_info.h

$(ARCHIVE): $(BINARY) LICENSE
	rm -f $(ARCHIVE)
	7z a $@ $^

$(BINARY): $(OBJECTS) 
	@printf '\t%s %s\n' LNK $@
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

visual/glgen/tvpgl.c visual/glgen/tvpgl.h: visual/glgen/gengl.pl visual/glgen/maketab.c visual/glgen/tvpps.c 
	cd visual/glgen && perl gengl.pl

visual/glgen/tvpgl_info.h: visual/glgen/maketvpglinfo.pl visual/glgen/tvpgl.h
	cd visual/glgen && perl maketvpglinfo.pl


main.cpp: visual/glgen/tvpgl.h visual/glgen/tvpgl_info.h
