headers := -I headers
std := -std=c++2a
out := trs

get_name = $(basename $(notdir $(1)))
get_bin_prefix = $(subst /,.,$(subst ./,,$(dir $(subst _src/,,$(1)))))
src := $(addprefix _, $(wildcard src/*.cpp) $(wildcard src/*/*.cpp))

all: clean $(src) $(out)

clean:
	@echo Clean UP...
	@rm -f $(out)
	@rm -f bin/*

$(out):
	@echo Linking...
	@g++ bin/terminal.*.o -o $@.terminal $(std)

$(src):
	@echo Compiling $(patsubst _src/%,%, $@)
	@g++ $(patsubst _%,%, $@) -o bin/$(call get_bin_prefix, $@)$(call get_name, $@).o -c $(std) $(headers)