# DSPTk
DSPTK = libdsptk.a
LIBS = -lm -pthread
INCS = Modem.h \
       MPSK.h \
       QAM.h \
       Random.h \
       Signal.h \
       SignalSpace.h \
       Simulator.h \
       SimpleSimulator.h \
       MultiThreadedSimulator.h \
       Source.h \
       RandomBitsSource.h \
       modem.h \
       signal.h \
       source.h \
       tool.h \
       utils.h

CXXFLAGS = -Wall -g -O3

# signal
SIGNAL_OBJS = Signal.o \
	      SignalSpace.o

# source
SOURCE_OBJS = Random.o

# modem
MODEM_OBJS = MPSK.o QAM.o

# tool
TOOL_OBJS = SimpleSimulator.o MultiThreadedSimulator.o

# utils
UTILS_OBJS = utils.o

ALL_OBJS = $(SIGNAL_OBJS) $(SOURCE_OBJS) $(MODEM_OBJS) $(UTILS_OBJS) $(TOOL_OBJS)


$(DSPTK): $(ALL_OBJS) $(INCS)
	ar rcs $(DSPTK) $(ALL_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) $(DEFINES) -c -o $@ $<

clean:
	rm -f $(ALL_OBJS) $(DSPTK)
