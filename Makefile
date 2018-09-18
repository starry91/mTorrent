#Name: Praveen Balireddy
#Roll: 2018201052

PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

TRACKER_OBJS =  main.o trackerHandler.o seeder.o fileAttr.o client.o 
CLIENT_OBJS = client_main.o clientHandler.o mtorrent.o seeder.o chunk.o
CFLAGS = -I.
LDIR = -L.
LIBS =  -pthread -lpthread
CLIENT_LIBS = -lcrypto -lssl
CFLAGS += -g
#ifeq ($(BUILD_MODE),debug)
#	CFLAGS += -g
##	CFLAGS += -O2
#else
#	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
#endif

all: Client_run Tracker 

Tracker: $(TRACKER_OBJS)
	$(CXX) -o $@ $^ $(LIBS)

Client_run: $(CLIENT_OBJS)
	$(CXX) -o $@ $^ $(CLIENT_LIBS)

%.o:	$(PROJECT_ROOT)%.cpp $(PROJECT_ROOT)%.h
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS)  $(LDIR) -o $@ $< 

%.o:	$(PROJECT_ROOT)%.c $(PROJECT_ROOT)%.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm Tracker $(TRACKER_OBJS)
