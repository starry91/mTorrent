CLIENT_OBJS = main.o trackerHandler.o fileAttr.o
LDFLAGS +=  -lpthread -lcrypto -lssl
CFLAGS += -g -I$(PROJECT_ROOT)/include


COMMON_OBJS := $(wildcard $(PROJECT_ROOT)/common/*.o)

all: ttracker

ttracker: $(CLIENT_OBJS)
	$(CXX) -o $@ $^ $(COMMON_OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm ttracker $(CLIENT_OBJS)
