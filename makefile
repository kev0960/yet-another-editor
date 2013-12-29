# this is a makefile for a Parser project

# Defines the complier to use
CC = g++

# The additional flags (-g for gdb enable and -Wall to
# show every warnings)
CFLAGS = -g -Wall -std=c++11

# Library flags
LIBS = -lboost_filesystem -lboost_system

parser: parser.o character_input.o
	$(CC) $(CFLAGS) parser.o character_input.o $(LIBS) -o parser

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

character_input.o: character_input.cpp parser.h
	$(CC) $(CFLAGS) -c character_input.cpp

# To clean up, remove all the object files and backup files
clean:
	$(RM) parser *.o *~

