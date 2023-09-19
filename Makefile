#PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = emise.o spotreba.o zdroje.o objekty.o baterie.o
FLAGS = -g3
CFLAGS += -std=c99
CPPFLAGS = -std=c++11 -fpermissive #-Wno-write-strings

all: emise

emise:   $(OBJS)
	$(CXX) -o $@ $^

%.o:    $(PROJECT_ROOT)%.cpp                                                        
	$(CXX) -c $(FLAGS) $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

%.o:    $(PROJECT_ROOT)%.c                                                          
	$(CC) -c $(FLAGS) $(CFLAGS) -o $@ $<

run: 
	@./emise ${ARGS}

clean:                                                                              
	rm -fr emise $(OBJS)

zip:
	tar -cvf xosker03.tar *.h *.c *.hpp *.cpp Makefile
