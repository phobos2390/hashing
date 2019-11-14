all:
	@echo "test, lib, clean"

test: lib
	build/hash_test
	
lib:
	@mkdir build
	@cd build; @cmake ..
	@cd build; @make

clean:
	@rm -rf build
