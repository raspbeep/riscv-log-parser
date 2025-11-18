CXX := g++
RVCCC := riscv64-unknown-elf-gcc
CXXFLAGS := -O2 -Wall -Wextra -std=c++23 -I./src

SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TESTS_SRC_DIR := tests/src
TESTS_BUILD_DIR := build/tests/bin
TESTS_DISASM_DIR := build/tests/disasm
TESTS_TRACE_DIR := build/tests/trace
TESTS_PARSE_DIR := build/tests/parse
TESTS_SRCS := $(wildcard $(TESTS_SRC_DIR)/*.c)
TESTS_BINS := $(patsubst $(TESTS_SRC_DIR)/%.c,$(TESTS_BUILD_DIR)/%,$(TESTS_SRCS))
TESTS_TRACES := $(wildcard $(TESTS_TRACE_DIR)/*.trace)

RUNNER := spike
OBJDUMP := riscv64-unknown-elf-objdump
PK := /opt/riscv/riscv64-unknown-elf/bin/pk
ISA_OPTS := --log-commits --isa=rv64gcv_zicsr_zicntr

TARGET := parser

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

run:
	./$(TARGET)


generate-tests: $(TARGET) build-tests run-tests disasm-tests

build-tests: $(TESTS_BINS)

$(TESTS_BUILD_DIR)/%: $(TESTS_SRC_DIR)/%.c
	@mkdir -p $(TESTS_BUILD_DIR)
	$(RVCCC) -O2 -Wall -Wextra -march=rv64gcv_zicsr_zicntr -o $@ $<

run-tests: $(TESTS_BINS)
	@mkdir -p $(TESTS_TRACE_DIR)
	@for bin in $(TESTS_BINS); do \
	    bname=$$(basename $$bin); \
	    log="$(TESTS_TRACE_DIR)/$${bname}.trace"; \
	    echo "Running $$bin ..."; \
	    $(RUNNER) $(ISA_OPTS) $(PK) -p $$bin 2> $$log; \
	done

disasm-tests: $(TESTS_BINS)
	@mkdir -p $(TESTS_DISASM_DIR)
	@for bin in $(TESTS_BINS); do \
	    bname=$$(basename $$bin); \
	    disasm="$(TESTS_DISASM_DIR)/$${bname}.disasm"; \
	    echo "Disassembling $$bin ..."; \
	    $(OBJDUMP) -d $$bin > $$disasm; \
	done

parse-tests: $(TARGET)
	@mkdir -p $(TESTS_PARSE_DIR)
	@for trace in $(TESTS_TRACES); do \
	    bname=$$(basename $${trace} .trace); \
	    parsed="$(TESTS_PARSE_DIR)/$${bname}.parsed"; \
	    echo "Parsing $${trace} ..."; \
	    ./$(TARGET) $${trace} > $${parsed}; \
	done

.PHONY: all clean test build-tests run-tests disasm-tests generate-tests parse-tests
