CFLAGS:=$(CFLAGS) --std=gnu99 -O0 -ggdb3
CXXFLAGS:=$(CXXFLAGS) --std=gnu++0x -O0 -ggdb3

OBJECTS:=main.o tokenizer.o parser.o utils.o error.o

.PHONY: all clean

all: boltc

clean:
	$(RM) boltc
	$(RM) $(OBJECTS)

boltc: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
