CFLAGS:=$(CFLAGS) --std=gnu99 -O0 -g3
CXXFLAGS:=$(CXXFLAGS) --std=gnu++0x -O0 -g3

OBJECTS:=main.o tokenizer.o parser.o utils.o

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
