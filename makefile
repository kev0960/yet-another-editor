# this is a makefile for a Parser project

# Defines the complier to use
CC = g++

# The additional flags (-g for gdb enable and -Wall to
# show every warnings)
CFLAGS = -g -Wall -std=c++11

# Library flags
LIBS = -lboost_filesystem -lboost_system

parser: parser.o character_input.o tokenizer.o
	$(CC) $(CFLAGS) parser.o parser_util.o character_input.o tokenizer.o $(LIBS) -o parser

parser.o: parser.cpp parser_util.cpp parser.h
	$(CC) $(CFLAGS) -c parser_util.cpp parser.cpp

character_input.o: character_input.cpp parser.h
	$(CC) $(CFLAGS) -c character_input.cpp

tokenizer.o: tokenizer.cpp tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.cpp

# To clean up, remove all the object files and backup files
clean:
	$(RM) parser *.o *~

