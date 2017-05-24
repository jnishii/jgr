CC = g++

INCLUDE = /usr/include/g++-include
CFLAGS = -Wno-deprecated
#CFLAGS = -Wall -I ${INCLUDE}
#Loadlibs = -lcurses -ltermcap -lg++ -lm -lX11
Loadlibs = -lm -L/usr/local/lib -ljgr -L/usr/X11R6/lib -lX11
TAGS= sample sin tiny

all : ${TAGS}

clean : 
	rm *.o *~ ${TAGS} *.eps

sample : sample.o
	${CC} -o $@ $@.o ${Loadlibs}

tiny : tiny.o
	${CC} -o $@ $@.o ${Loadlibs}

sin : sin.o
	${CC} -o $@ $@.o ${Loadlibs}

menu : menu.o
	${CC} -o $@ $@.o ${Loadlibs}

tinymenu : tinymenu.o
	${CC} -o $@ $@.o ${Loadlibs}

.cc.o:
	${CC} ${CFLAGS} -c -o $@ $<
