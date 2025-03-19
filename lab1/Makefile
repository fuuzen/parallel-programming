dev:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	mv build/compile_commands.json .

build:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake .. && make -j8

NP ?= 16
# eg: make test1 NP=4

test1: build
	mpirun -np $(NP) ./build/test1

test2: build
	mpirun -np $(NP) ./build/test2

clean:
	if [ -d "build" ]; then rm -rf build; fi

.PHONY: all clean