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

clean:
	@rm -rf build
