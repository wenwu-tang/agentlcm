SHELL       += -x
DIR = $(shell pwd)

#Compile  Model
DIR1 = ../model
TARGETS1 = simulationmodel landowner
OBJECTS1 = $(TARGETS1:=.o)
OBJECTS1_F = $(OBJECTS1:%=$(DIR1)/%)
SRCS1 = $(TARGETS1:=.cpp)
DIR2 = ../src
TARGETS2 = geoagent geodocs geocell geoobject
OBJECTS2 = $(TARGETS2:=.o)
OBJECTS2_F = $(OBJECTS2:%=$(DIR2)/%)
SRCS2 = $(TARGETS2:=.cpp)

TARGETS = $(TARGETS1) $(TARGETS2)

simulation:  $(OBJECTS1) $(OBJECTS2) 
	@g++ -g  -Wall  $(OBJECTS1_F) $(OBJECTS2_F) -o simulation  simulation.cpp -lm
$(OBJECTS1): %.o: %.cpp
	(cd $(DIR1); g++ -g -Wall  -I. -o $@ -c $<; cd $(DIR))
$(OBJECTS2): %.o: %.cpp
	(cd $(DIR2); g++ -g -Wall  -I. -o $@ -c $<; cd $(DIR))
$(SRCS1):
$(SRCS2):
clean:  
	@rm simulation $(OBJECTS1_F) $(OBJECTS2_F)
run:
	./simulation
