TARGET = redis

SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)

MKDIR = mkdir
MKDIRFLAGS = -p
RM = rm
RMFLAGS = -rf

CPP = g++
CC = gcc

ver = 64

ifeq ($(ver),64)
DIR_LIBS = ../muduo_x64/lib 
INCLUDE_DIRS = -I../muduo_x64/include
else
DIR_LIBS = ../muduo_x86/lib 
INCLUDE_DIRS = -I../muduo_x86/include
endif

LINK_LIBS = -lmuduo_net -lmuduo_base -lpthread

CFLAGS = -Wall -gdwarf-2 -g3 -O0 $(INCLUDE_DIRS) -D__STDC_FORMAT_MACROS

RMS = $(OBJS) $(TARGET)

all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CPP) -L$(DIR_LIBS) -o $@ $^ $(LINK_LIBS)

%.o: %.cc
	$(CPP) $(CFLAGS) -o $@ -c $^

clean:
	$(RM) $(RMFLAGS) $(RMS)
