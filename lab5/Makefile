dev:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	mv build/compile_commands.json .

build:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake .. && make -j8

test1: build
	echo "|    |     128     |     256     |     512     |     1024    |     2048    |" > ./build/result.md
	echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "|  1 | `./build/test1 128 128 128 1` | `./build/test1 256 256 256 1` | `./build/test1 512 512 512 1` | `./build/test1 1024 1024 1024 1` | `./build/test1 2048 2048 2048 1` |" >> ./build/result.md
	echo "|  2 | `./build/test1 128 128 128 2` | `./build/test1 256 256 256 2` | `./build/test1 512 512 512 2` | `./build/test1 1024 1024 1024 2` | `./build/test1 2048 2048 2048 2` |" >> ./build/result.md
	echo "|  4 | `./build/test1 128 128 128 4` | `./build/test1 256 256 256 4` | `./build/test1 512 512 512 4` | `./build/test1 1024 1024 1024 4` | `./build/test1 2048 2048 2048 4` |" >> ./build/result.md
	echo "|  8 | `./build/test1 128 128 128 8` | `./build/test1 256 256 256 8` | `./build/test1 512 512 512 8` | `./build/test1 1024 1024 1024 8` | `./build/test1 2048 2048 2048 8` |" >> ./build/result.md
	echo "| 16 | `./build/test1 128 128 128 16` | `./build/test1 256 256 256 16` | `./build/test1 512 512 512 16` | `./build/test1 1024 1024 1024 16` | `./build/test1 2048 2048 2048 16` |" >> ./build/result.md
	clear
	cat ./build/result.md

test2: build
	echo "|    |     128     |     256     |     512     |     1024    |     2048    |" > ./build/result.md
	echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "|  1 | `./build/test2 128 128 128 1` | `./build/test2 256 256 256 1` | `./build/test2 512 512 512 1` | `./build/test2 1024 1024 1024 1` | `./build/test2 2048 2048 2048 1` |" >> ./build/result.md
	echo "|  2 | `./build/test2 128 128 128 2` | `./build/test2 256 256 256 2` | `./build/test2 512 512 512 2` | `./build/test2 1024 1024 1024 2` | `./build/test2 2048 2048 2048 2` |" >> ./build/result.md
	echo "|  4 | `./build/test2 128 128 128 4` | `./build/test2 256 256 256 4` | `./build/test2 512 512 512 4` | `./build/test2 1024 1024 1024 4` | `./build/test2 2048 2048 2048 4` |" >> ./build/result.md
	echo "|  8 | `./build/test2 128 128 128 8` | `./build/test2 256 256 256 8` | `./build/test2 512 512 512 8` | `./build/test2 1024 1024 1024 8` | `./build/test2 2048 2048 2048 8` |" >> ./build/result.md
	echo "| 16 | `./build/test2 128 128 128 16` | `./build/test2 256 256 256 16` | `./build/test2 512 512 512 16` | `./build/test2 1024 1024 1024 16` | `./build/test2 2048 2048 2048 16` |" >> ./build/result.md
	clear
	cat ./build/result.md


clean:
	if [ -d "build" ]; then rm -rf build; fi

.PHONY: all clean