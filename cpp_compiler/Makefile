CXXFLAGS:=$(CXXFLAGS) --std=gnu++0x -O0 -g3

OBJECTS:=main.o tokenizer.o

.PHONY: all

all: boltc

boltc: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
