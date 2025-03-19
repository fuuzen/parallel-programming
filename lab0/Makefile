dev:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	mv build/compile_commands.json .

build:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake .. && make -j8

test1: build
	./build/test1

test2: build
	./build/test2

test3: build
	./build/test3

test4: build
	./build/test4

test5: build
	./build/test5

clean:
	if [ -d "build" ]; then rm -rf build; fi

.PHONY: all clean