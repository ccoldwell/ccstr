#
# simple makefile
# Chris Coldwell
#
# compile terms
DEBUGFLAGS = -g -Wall
#CFLAGS = $(DEBUGFLAGS)
CFLAGS = -O3
LIB = /usr/local/lib
INC = /usr/local/include

GCC = gcc
GPP = g++
#
# what exactly are we putting together
#

.PHONY: clean

#
# the string lib
#
libccstr.a: ccstr.c ccstr.h
	@echo + compiling string library
	@$(GCC) $(CFLAGS) -c ccstr.c -o ccstr.o
	@ar rcs libccstr.a ccstr.o

install: libccstr.a
	@echo + installing string library
	@cp libccstr.a $(LIB)/
	@chmod a+r $(LIB)/libccstr.a
	@cp ccstr.h $(INC)/
	@chmod a+r $(INC)/ccstr.h
	@ar rcs libccstr.a ccstr.o


clean:
	@echo + cleaning libraries
	@rm *.o
	@rm *.a
