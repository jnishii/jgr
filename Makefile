NAME=jgr
SRCDIR=$(HOME)/rpm/SOURCES
#####################################
VERSION=3.4
RELEASE=0vl1
SOVERSION=3.4
#####################################
### 
CC = g++
### for Compaq Compiler
#CC = cxx
AR = ar
### for Intel Compiler
#CC = icpc -DICC -mp -cxxlib-icc
#AR = xiar
### for Portland Compiler
PCC = pgCC
#####################################
.SUFFIXES: .cc .o

#DEBUG = -DDEBUG -g
CFLAGS = -Wall -Wno-deprecated ${DEBUG}
LSRCS  = jgr.cc #jgrPS.cc
LHEAD  = ${LSRCS:.cc=.h} 
LOBJS  = ${LSRCS:.cc=.o}
#POBJS	= Pjgr.o Pjutil.o
LIBS = libjgr.a
SOLIBS = ${LIBS:.a=.so}
PCFLAGS = -b3 --bool -e20

PREFIX = ${DESTDIR}/usr
INCLUDE_DIR = ${PREFIX}/include/jkit/
LIBRARY_DIR = ${PREFIX}/lib/

all : ${LOBJS} ${LIBS} ${SOLIBS}

clean : 
	rm -f *.o *.a *~ *.so.* *.so core ||:
	make clean -C doc
	make clean -C samples

libjgr.a: ${LOBJS}
	${AR} cr $@ jgr.o
	ranlib $@

libjgr.so: jgr.o jgrPS.o
	${CC} ${DEBUG} -shared -Wl,-soname,libjgr.so.${SOVERSION} -o libjgr.so.${SOVERSION} jgr.o jgrPS.o

#libjgr.so: jgr.o
#	${CC} ${DEBUG} -shared -Wl,-soname,libjgr.so.${SOVERSION} -o libjgr.so.${SOVERSION} jgr.o

Pjgr.o: jgr.cc
	${PCC} ${PCFLAGS} ${DEFS} -DPGI -c -o $@ $<

.cc.o:
	${CC} ${CFLAGS} ${DEFS} -c -o $@ $<

install:  ${LOBJS} ${LIBS} ${SOLIBS}
	install -d ${INCLUDE_DIR}
	install -m 644 jgr.h ${INCLUDE_DIR} 
	install -m 644 jgrPS.h ${INCLUDE_DIR} 
	install -d ${LIBRARY_DIR}
	install -m 644 libjgr.a ${LIBRARY_DIR}
	install -m 755 libjgr.so.${SOVERSION} ${LIBRARY_DIR}

version:
	perl -p -i -e "s/^%define version .*/%define version $(VERSION)/" ${NAME}.spec
	perl -p -i -e "s/^%define soversion .*/%define soversion $(SOVERSION)/" ${NAME}.spec
	perl -p -i -e "s/^%define release .*/%define release $(RELEASE)/" ${NAME}.spec


targz: clean version
	cd .. && rm -rf ${NAME}-${VERSION}
	cd .. && cp -arf ${NAME} ${NAME}-${VERSION}
	cd .. && tar cf - ${NAME}-${VERSION} --exclude CVS | gzip -9 > ${NAME}-${VERSION}.tar.gz
	rm -rf ${NAME}-${VERSION}

rpm: targz
	rpm -ba ${NAME}.spec
##	[ -f ${NAME}-${VERSION}.tar.gz ] || mv ../${NAME}-${VERSION}.tar.gz ${SRCDIR}

