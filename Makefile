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
INCFLAGS += -I. -I.. -Itjs2 -Ivisual -Ivisual/gl -Ivisual/glgen
ALLSRCFLAGS += $(INCFLAGS) -DGIT_TAG=\"$(GIT_TAG)\"
ASMFLAGS += $(ALLSRCFLAGS) -fwin32 -DWIN32
OPTFLAGS := -Ofast -march=ivybridge
CFLAGS += -gstabs 
CFLAGS += $(ALLSRCFLAGS) -Wall -Wno-unused-value -Wno-format -DNDEBUG -DWIN32 -D_WIN32 -D_WINDOWS 
CFLAGS += -D_USRDLL -DMINGW_HAS_SECURE_API -DUNICODE -D_UNICODE -DNO_STRICT
CXXFLAGS += $(CFLAGS) -fpermissive
WINDRESFLAGS += $(ALLSRCFLAGS) --codepage=65001
LDFLAGS += -static-libstdc++ -static-libgcc -Wl,--kill-at
LDFLAGS_LIB += -shared
LDFLAGS_BIN += -municode
LDLIBS += 
LDLIBS_BIN += -lwinmm

%.o: %.c
	@printf '\t%s %s\n' CC $<
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o $@ $<

%.o: %.cpp
	@printf '\t%s %s\n' CXX $<
	$(CXX) -c $(CXXFLAGS) $(OPTFLAGS) -o $@ $<

%.o: %.nas
	@printf '\t%s %s\n' ASM $<
	$(ASM) $(ASMFLAGS) $< -o$@ 

%.o: %.rc
	@printf '\t%s %s\n' WINDRES $<
	$(WINDRES) $(WINDRESFLAGS) $< $@

SOURCES := main.cpp tvpgl.rc visual/glgen/tvpgl.c
ifeq (disabled,)
SOURCES += visual/IA32/detect_cpu.cpp
SOURCES_IA32 += visual/IA32/tvpgl_ia32_intf.c visual/IA32/addalphablend.nas visual/IA32/addblend.nas visual/IA32/adjust_color.nas visual/IA32/affine.nas visual/IA32/alphablend.nas visual/IA32/boxblur.nas visual/IA32/colorfill.nas visual/IA32/colormap.nas visual/IA32/darkenblend.nas visual/IA32/lightenblend.nas visual/IA32/make_alpha_from_key.nas visual/IA32/mulblend.nas visual/IA32/pixelformat.nas visual/IA32/screenblend.nas visual/IA32/stretch.nas visual/IA32/subblend.nas visual/IA32/tlg5.nas visual/IA32/tlg6_chroma.nas visual/IA32/tlg6_golomb.nas visual/IA32/tvpps_asm.nas visual/IA32/univtrans.nas
SOURCES_IA32_C += visual/IA32_c/tvpgl_ia32_intf_c.c visual/IA32_c/addalphablend.c visual/IA32_c/addblend.c visual/IA32_c/adjust_color.c visual/IA32_c/affine.c visual/IA32_c/alphablend.c visual/IA32_c/boxblur.c visual/IA32_c/colorfill.c visual/IA32_c/colormap.c visual/IA32_c/darkenblend.c visual/IA32_c/lightenblend.c visual/IA32_c/make_alpha_from_key.c visual/IA32_c/mulblend.c visual/IA32_c/pixelformat.c visual/IA32_c/screenblend.c visual/IA32_c/stretch.c visual/IA32_c/subblend.c visual/IA32_c/tlg5.c visual/IA32_c/tlg6_chroma.c visual/IA32_c/tlg6_golomb.c visual/IA32_c/tvpps_asm.c visual/IA32_c/univtrans.c
SOURCES_IA32_PFRACTION += visual/IA32_pfraction/tvpgl_ia32_intf_pfraction.c visual/IA32_pfraction/addalphablend.nas visual/IA32_pfraction/addblend.nas visual/IA32_pfraction/adjust_color.nas visual/IA32_pfraction/affine.nas visual/IA32_pfraction/alphablend.nas visual/IA32_pfraction/boxblur.nas visual/IA32_pfraction/colorfill.nas visual/IA32_pfraction/colormap.nas visual/IA32_pfraction/darkenblend.nas visual/IA32_pfraction/lightenblend.nas visual/IA32_pfraction/make_alpha_from_key.nas visual/IA32_pfraction/mulblend.nas visual/IA32_pfraction/pixelformat.nas visual/IA32_pfraction/screenblend.nas visual/IA32_pfraction/stretch.nas visual/IA32_pfraction/subblend.nas visual/IA32_pfraction/tlg5.nas visual/IA32_pfraction/tlg6_chroma.nas visual/IA32_pfraction/tlg6_golomb.nas visual/IA32_pfraction/tvpps_asm.nas visual/IA32_pfraction/univtrans.nas
SOURCES_IA32_PFRACTION_C += visual/IA32_pfraction_c/tvpgl_ia32_intf_pfraction_c.c visual/IA32_pfraction_c/addalphablend.c visual/IA32_pfraction_c/addblend.c visual/IA32_pfraction_c/adjust_color.c visual/IA32_pfraction_c/affine.c visual/IA32_pfraction_c/alphablend.c visual/IA32_pfraction_c/boxblur.c visual/IA32_pfraction_c/colorfill.c visual/IA32_pfraction_c/colormap.c visual/IA32_pfraction_c/darkenblend.c visual/IA32_pfraction_c/lightenblend.c visual/IA32_pfraction_c/make_alpha_from_key.c visual/IA32_pfraction_c/mulblend.c visual/IA32_pfraction_c/pixelformat.c visual/IA32_pfraction_c/screenblend.c visual/IA32_pfraction_c/stretch.c visual/IA32_pfraction_c/subblend.c visual/IA32_pfraction_c/tlg5.c visual/IA32_pfraction_c/tlg6_chroma.c visual/IA32_pfraction_c/tlg6_golomb.c visual/IA32_pfraction_c/tvpps_asm.c visual/IA32_pfraction_c/univtrans.c
SOURCES_TEMPLATE += visual/gl/adjust_color_sse2.cpp visual/gl/blend_function.cpp visual/gl/blend_function_avx2.cpp visual/gl/blend_function_sse2.cpp visual/gl/boxblur_sse2.cpp visual/gl/colorfill_sse2.cpp visual/gl/colormap_sse2.cpp visual/gl/pixelformat_sse2.cpp visual/gl/tlg_sse2.cpp visual/gl/univtrans_sse2.cpp visual/gl/x86simdutil.cpp visual/gl/x86simdutilAVX2.cpp
SOURCES += $(SOURCES_IA32) $(SOURCES_IA32_C) $(SOURCES_IA32_PFRACTION) $(SOURCES_TEMPLATE) $(SOURCES_IA32_PFRACTION_C) $(SOURCES_IA32_BEHAVIOR_C)
endif
ifneq (disabled,)
SOURCES_IA32_BEHAVIOR_C += visual/IA32_behavior_c/tvpgl_ia32_intf_behavior_c.c visual/IA32_behavior_c/addalphablend.c visual/IA32_behavior_c/affine.c visual/IA32_behavior_c/alphablend.c visual/IA32_behavior_c/boxblur.c visual/IA32_behavior_c/colorfill.c visual/IA32_behavior_c/colormap.c visual/IA32_behavior_c/darkenblend.c visual/IA32_behavior_c/mulblend.c visual/IA32_behavior_c/screenblend.c visual/IA32_behavior_c/stretch.c visual/IA32_behavior_c/tvpps_asm.c visual/IA32_behavior_c/univtrans.c
SOURCES += $(SOURCES_IA32_BEHAVIOR_C)
endif
OBJECTS := $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)
OBJECTS := $(OBJECTS:.nas=.o)
OBJECTS := $(OBJECTS:.rc=.o)

SOURCES_BIN := tests/test.cpp
SOURCES_BIN += $(SOURCES)
OBJECTS_BIN := $(SOURCES_BIN:.c=.o)
OBJECTS_BIN := $(OBJECTS_BIN:.cpp=.o)
OBJECTS_BIN := $(OBJECTS_BIN:.nas=.o)
OBJECTS_BIN := $(OBJECTS_BIN:.rc=.o)

BINARY ?= tvpgl.dll
BINARY_BIN ?= tvpgl.exe
ARCHIVE ?= tvpgl.$(GIT_TAG).7z

all: $(BINARY)

archive: $(ARCHIVE)

clean:
	rm -f $(OBJECTS) $(OBJECTS_BIN) $(BINARY) $(BINARY_BIN) $(ARCHIVE) visual/glgen/tvpgl.c visual/glgen/tvpgl.h visual/glgen/tvpgl_info.h

$(ARCHIVE): $(BINARY) LICENSE
	rm -f $(ARCHIVE)
	7z a $@ $^

$(BINARY): $(OBJECTS) 
	@printf '\t%s %s\n' LNK $@
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LDFLAGS_LIB) -o $@ $^ $(LDLIBS)

$(BINARY_BIN): $(OBJECTS_BIN) 
	@printf '\t%s %s\n' LNK $@
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LDFLAGS_BIN) -o $@ $^ $(LDLIBS_BIN)

visual/glgen/tvpgl.c visual/glgen/tvpgl.h: visual/glgen/gengl.pl visual/glgen/maketab.c visual/glgen/tvpps.c 
	cd visual/glgen && perl gengl.pl

visual/glgen/tvpgl_info.h: visual/glgen/maketvpglinfo.pl visual/glgen/tvpgl.h
	cd visual/glgen && perl maketvpglinfo.pl

main.o: OPTFLAGS := -O2

$(SOURCES_IA32:.nas=.o): INCFLAGS += -Ivisual/IA32

$(SOURCES_IA32_PFRACTION:.nas=.o): INCFLAGS += -Ivisual/IA32_pfraction

tests/test.o main.o $(SOURCES_TEMPLATE:.cpp=.o): INCFLAGS += -Ivisual/IA32 -Ivisual/IA32_c -Ivisual/IA32_pfraction -Ivisual/IA32_pfraction_c -Ivisual/IA32_behavior_c

$(SOURCES_IA32:.c=.o): INCFLAGS += -Ivisual/IA32

$(SOURCES_IA32_C:.c=.o): INCFLAGS += -Ivisual/IA32 -Ivisual/IA32_c

$(SOURCES_IA32_PFRACTION:.c=.o): INCFLAGS += -Ivisual/IA32_pfraction

$(SOURCES_IA32_PFRACTION_C:.c=.o): INCFLAGS += -Ivisual/IA32 -Ivisual/IA32_pfraction_c

$(SOURCES_IA32_BEHAVIOR_C:.c=.o): INCFLAGS += -Ivisual/IA32 -Ivisual/IA32_behavior_c

tests/test.cpp: visual/glgen/tvpgl.h visual/glgen/tvpgl_info.h

main.cpp: visual/glgen/tvpgl.h visual/glgen/tvpgl_info.h
