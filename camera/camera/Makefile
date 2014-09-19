LIBS	:= camera.a

AOBJS	:= $(patsubst %.s, %.o, $(wildcard *.s))
COBJS	:= $(patsubst %.c, %.o, $(wildcard *.c))
CPPOBJS	:= $(patsubst %.cpp, %.o, $(wildcard *.cpp))
OBJS	:= $(AOBJS) $(COBJS) $(CPPOBJS)

all: $(OBJS)
	$(AR) crs $(LIBS) $^  

clean:
	rm -f $(OBJS) $(LIBS) *.h.gch

