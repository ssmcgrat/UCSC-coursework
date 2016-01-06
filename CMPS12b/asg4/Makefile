# $Id: Makefile,v 1.7 2014-01-24 18:33:47-08 - - $

MKFILE    = Makefile
DEPSFILE  = ${MKFILE}.deps
NOINCLUDE = ci clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
GMAKE     = gmake --no-print-directory

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99
MKDEPS    = gcc -MM

CSOURCE   = debug.c stack.c bigint.c token.c main.c
CHEADER   = debug.h stack.h bigint.h token.h
OBJECTS   = ${CSOURCE:.c=.o}
EXECBIN   = mydc
SOURCES   = ${CHEADER} ${CSOURCE} ${MKFILE}
LISTING   = Listing.ps
PROJECT   = cmps012b-wm.w14 asg4

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

%.o : %.c
	${GCC} -c $<

ci : ${SOURCES}
	cid + ${SOURCES}
	checksource ${SOURCES}

lis : ${SOURCES} ${DEPSFILE}
	mkpspdf ${LISTING} ${SOURCES} ${DEPSFILE}

clean :
	- rm ${OBJECTS} ${DEPSFILE} core ${EXECBIN}.errs

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

deps : ${CSOURCE} ${CHEADER}
	@ echo "# ${DEPSFILE} created `date`" >${DEPSFILE}
	${MKDEPS} ${CSOURCE} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${GMAKE} deps

again :
	${GMAKE} spotless deps ci all lis

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

