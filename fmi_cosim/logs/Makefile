#export SYSTEMC_HOME=/softwares/systemc/systemc-2.3.0
#export SYSTEMC_AMS=/softwares/systemc/systemc-ams

EXTRACFLAGS = -DDEBUG_SYSTEMC
EXTRA_LIBS =
MODULE = run
SRCS = $(wildcard *.c)

OBJS = $(SRCS:.cpp=.o)

## Variable that points to SystemC,Systemc-ams and TUV_AMS  installation path
#SYSTEMC = $(SYSTEMC_HOME)
#SYSTEMCAMS=$(SYSTEMC_AMS)

## edit this to gccsparcOS5 for solaris
TARGET_ARCH = macosx64

CC     = clang  
OPT    = -O0
DEBUG  = -g


          
INCDIR = -I. -I.. 
LIBDIR = -L. -L.. 

##using -m32 option for 32 bit compilation,i use this for linking with 32bit systemc ovp processor models

## Build with maximum gcc warning level
CFLAGS = -Wall -Wno-deprecated -Wno-return-type -Wno-char-subscripts $(DEBUG) $(OPT) $(EXTRACFLAGS)
LIBS   =  -lstdc++ -lpthread -lm $(EXTRA_LIBS) 

EXE    = $(MODULE).exe

.PHONY: clean 

all: $(EXE)

$(EXE): $(OBJS) 
	$(CC) $(CFLAGS) $(INCDIR) $(LIBDIR) -o $@ $(OBJS) $(LIBS) 2>&1 | c++filt

## based on http://www.paulandlesley.org/gmake/autodep.html
%.o : %.c
	$(CC) $(CFLAGS) $(INCDIR) -c -MMD -o $@ $<
	cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d


clean:
	-rm -f $(OBJS) *~ $(EXE) *.vcd *.wif *.isdb *.dmp *.P *.log *.dat

-include $(SRCS:.cpp=.P)
