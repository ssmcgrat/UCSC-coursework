# Sean McGrath, ssmcgrat@ucsc.edu $ 
# $Id: Makefile,v 1.1 2014-01-14 11:05:50-08 - - $

JAVASRC    = jfmt.java
SOURCES    = ${JAVASRC} Makefile README
MAINCLASS  = jfmt
CLASSES    = ${JAVASRC:.java=.class}
JARCLASSES = ${CLASSES}
JARFILE    = jfmt
SUBMITDIR  = cmps012b-wm.w14 asg1
LISTING    = Listing.ps

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm -vf Manifest
	chmod +x ${JARFILE}

%.class : %.java
	javac $<

clean :
	- rm -vf ${JARCLASSES} test.output

spotless : clean
	- rm -vf ${JARFILE}

ci : ${SOURCES}
	cid + ${SOURCES} test.input
	checksource ${SOURCES}

lis : all
	${JARFILE} test.input >test.output
	mkpspdf ${LISTING} ${SOURCES} test.input test.output

submit : ${SOURCES} ci
	submit ${SUBMITDIR} ${SOURCES}

again : ${SOURCES}
	gmake --no-print-directory spotless lis
	
