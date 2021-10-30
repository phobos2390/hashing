SEED:=$(shell date)

all:
	@echo "test, lib, clean"

test: lib 
	build/hasher_test
	
lib:
	@mkdir -p build
	@cd build; cmake ..
	@cd build; make

single_bit_hash_default_list: lib
	build/single_bit_hash_list
	
password: lib
	build/password_generator -d res/english_words.txt -s "$(SEED)"

clean:
	@rm -rf build
