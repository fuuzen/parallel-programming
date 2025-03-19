build:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake .. && make -j8

test: build
	./build/test

clean:
	if [ -d "build" ]; then rm -rf build; fi

.PHONY: all clean