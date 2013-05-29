# BUILD SETTINGS ###########################################

ifndef PLATFORM
    ifdef SystemRoot
        PLATFORM := WIN32
        RMDIR := rmdir /s /q
        RMFILE := del
        MKDIR := mkdir
    else
        PLATFORM := UNIX
        RMDIR := rm -rf
        RMFILE := rm -f
        MKDIR := mkdir -p
    endif
endif

TARGET := opentyrian
ifndef WITH_NETWORK
    WITH_NETWORK := 1
endif

############################################################

STRIP := strip
SDL_CONFIG := sdl-config

include crosscompile.mk

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=obj/%.o)

# FLAGS ####################################################

ifneq ($(MAKECMDGOALS), release)
    EXTRA_CFLAGS += -g3 -O0 -Werror
else
    EXTRA_CFLAGS += -g0 -O2 -DNDEBUG
endif
EXTRA_CFLAGS += -MMD -pedantic -Wall -Wextra -Wno-missing-field-initializers
ifeq ($(WITH_NETWORK),1)
    EXTRA_CFLAGS += -DWITH_NETWORK
endif

#HG_REV := $(shell hg id -ib && touch src/hg_revision.h)
#ifneq ($(HG_REV), )
#    EXTRA_CFLAGS += '-DHG_REV="$(HG_REV)"'
#endif

EXTRA_LDLIBS += -lm

SDL_CFLAGS := $(shell $(SDL_CONFIG) --cflags)
SDL_LDLIBS := $(shell $(SDL_CONFIG) --libs)

ifeq ($(PLATFORM),WIN32)
    SDL_LDLIBS := -lmingw32 -lSDLmain -lSDL
endif

ifeq ($(WITH_NETWORK),1)
    SDL_LDLIBS += -lSDL_net
endif

ALL_CFLAGS += -std=c11 -I./src -DTARGET_$(PLATFORM) $(EXTRA_CFLAGS) $(SDL_CFLAGS) $(CFLAGS)
ALL_LDFLAGS += $(LDFLAGS)
LDLIBS += $(EXTRA_LDLIBS) $(SDL_LDLIBS)

# RULES ####################################################

.PHONY : all release clean

all : dirtest $(TARGET) 

release : all
	$(STRIP) $(TARGET)

clean :
	$(RMDIR) obj
	$(RMFILE) $(TARGET)

ifneq ($(MAKECMDGOALS), clean)
    -include $(OBJS:.o=.d)
endif

dirtest :
	test -d obj || $(MKDIR) obj

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(ALL_LDFLAGS) $^ $(LDLIBS)

obj/%.o : src/%.c
	$(CC) -c -o $@ $(ALL_CFLAGS) $<

