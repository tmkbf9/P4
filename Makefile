.PHONY: clean compile test make_headers

GCC := g++
CFLAGS := -g
CPPFLAGS := --std=c++98
RM := rm
AR := ar

.cpp.o:
	$(GCC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

all: test compile

compile: P4.o compile.o libcompilersteps.a
	$(GCC) -o $@ $^

libcompilersteps.a: available_tokens.o parser.o scanner.o static_semantics.o token.o
	$(AR) rcs $@ $^

test: compile_test
	./compile_test

compile_test: compile_test.o compile.o libcompilersteps.a
	$(GCC) -o $@ $^

clean:
	$(RM) -f compile compile_test
	$(RM) -f libcompilersteps.a
	$(RM) -f *.o
	$(RM) -f *~

make_headers:
	$(GCC) -M P4.cpp compile.cpp compile_test.cpp
