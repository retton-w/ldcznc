
APPNAME := device
APP     := $(APPNAME)
VERSION := 1.0.0
BUILD   := $(shell date '+%Y-%m-%d %H:%M:%S')

all:  $(APP) install 

CROSS_COMPILER := $(MVL5TOOL)/bin/arm_v5t_le-
CC             := $(CROSS_COMPILER)gcc
CXX            := $(CROSS_COMPILER)g++
AR             := $(CROSS_COMPILER)ar
LD             := $(CROSS_COMPILER)ld

CFLAGS   += -Wall -W -Wextra
CFLAGS  += -g
CFLAGS  += -O2

LD_FLAG  += -lpthread

APP_ROOT := $(shell pwd)

#### incs
INC_FLAG += -I$(APP_ROOT)
INC_FLAG += -I$(APP_ROOT)/inc.ker
INC_FLAG += -I$(APP_ROOT)/inc.lib.cecodec
INC_FLAG += -I$(APP_ROOT)/pref
CFLAGS   += -DVERSION="$(VERSION)" -DBUILD="$(BUILD)"

export CC CXX AR LD CFLAGS INC_FLAG LD_FLAGS CROSS_COMPILER

#### external library
LIBS     :=
LIB_PATH := libs/
LIBFILES := $(addprefix $(LIB_PATH)lib, $(addsuffix .a, $(LIBS)))

#### applib
APP_LIB := pref
#APP_LIB := $(addprefix lib, $(APP_LIB))

CSRCS   :=

CXXSRCS := camera.cpp \
	   camera_500w.cpp \
	   camera_factory.cpp \
           gpio.cpp \
	   sensor.cpp \
	   SHA1.cpp \
	   ldspmil.cpp \
	   resize.cpp \
	   image_process.cpp \
	   socket.cpp \
	   thread.cpp \
	   image_buffer.cpp \
	   net_packet.cpp \
	   tcp_client.cpp \
	   tcp_server.cpp \
	   udp_server.cpp \
	   mutex.cpp \
	   peripherral_manage.cpp \
	   arbiter.cpp \
	   test.cpp

CXXOBJS := $(CXXSRCS:.cpp=.o)
COBJS   := $(CSRCS:.c=.o)

OBJS    := $(CXXOBJS) $(COBJS)
DEPS    := $(OBJS:.o=.d)

MODDEP  := moddependesxxx.d

$(MODDEP) :
	@printf "PHONY : $(APP_LIB) \n\n" > $@
	@for subdir in $(APP_LIB); do \
		printf "%s :\n\t$(MAKE) -C %s TARGET=%s.a\n\n" $$subdir $$subdir $$subdir >> $@; \
	done

%.o : %.c
	$(CC)  $(CFLAGS) $(INC_FLAG) -c $< -o $@
%.d : %.c
	@set -e;$(CC)  $(CFLAGS) $(INC_FLAG) $(CPPFLAGS) -MM $< | sed -e 's/$(basename $@).o/$(basename $@).o $(basename $@).d/' > $@
%.o : %.cpp
	$(CXX) $(CFLAGS) $(INC_FLAG) -c $< -o $@
%.d : %.cpp
	@set -e;$(CXX) $(CFLAGS) $(INC_FLAG) $(CPPFLAGS) -MM $< | sed -e 's/$(basename $@).o/$(basename $@).o $(basename $@).d/' > $@


.PHONY : $(APP) $(APP_LIB)

$(APP) : $(LIBFILES) $(APP_LIB) $(OBJS)
	$(CXX) $(LD_FLAG) $(OBJS) $(foreach n, $(APP_LIB), $(n)/$(n).a) $(LIBFILES) -L libs -l cecodec -l Overlay -l freetype -o $@

clean :
	$(RM) -f $(OBJS) $(DEPS) $(APP) $(MODDEP)

distclean : clean
	@for subdir in $(APP_LIB); do \
		$(MAKE) -C $$subdir CROSS_COMPILER=$(CROSS_COMPILER) TARGET=$$subdir.a clean || exit 1; \
	done

install :
#cp $(APP) /home/smbuser/share

sinclude $(DEPS)
sinclude $(MODDEP)
