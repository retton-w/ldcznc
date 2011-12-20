
CSRCS   := 
CXXSRCS := preference.cpp \
	   preference_items.cpp \
	   xmlParser.cpp

COBJS   := $(CSRCS:.c=.o)
CXXOBJS := $(CXXSRCS:.cpp=.o)
OBJS    := $(COJBS) $(CXXOBJS)
DEPS    := $(OBJS:.o=.d)

TARGET := $(TARGET).a
all : $(TARGET)

%.o : %.c
	$(CC)  $(CFLAGS) $(INC_FLAG) -c $< -o $@
%.d : %.c
	@set -e;$(CC)  $(CFLAGS) $(INC_FLAG) $(CPPFLAGS) -MM $< | sed -e 's/$(basename $@).o/$(basename $@).o $(basename $@).d/' > $@
%.o : %.cpp
	$(CXX) $(CFLAGS) $(INC_FLAG) -c $< -o $@
%.d : %.cpp
	@set -e;$(CXX) $(CFLAGS) $(INC_FLAG) $(CPPFLAGS) -MM $< | sed -e 's/$(basename $@).o/$(basename $@).o $(basename $@).d/' > $@

$(TARGET) : $(OBJS)
	$(AR) r $@ $^

clean :
	$(RM) $(OBJS) $(DEPS) $(TARGET)

sinclude $(DEPS)
