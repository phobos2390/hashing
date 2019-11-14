all:
	@echo "test, lib, clean"

test: lib 
	build/hasher_test
	
lib:
	@mkdir -p build
	@cd build; cmake ..
	@cd build; make

clean:
	@rm -rf build
