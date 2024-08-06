TARGET ?= a.out
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
CC = gcc

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean test1 test2

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

test1:
	./routine.sh -d TestRoutine -s 10 -f 0.1 -n 3

test2:
	make
	./a.out TestCRoutine 8 0.1 0

-include $(DEPS)
