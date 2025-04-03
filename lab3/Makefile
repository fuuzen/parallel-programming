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
	echo "|        |      1      |      2      |      4      |      8      |      16     |" > ./build/result.md
	echo "|--------|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "| 0x1p20 | `./build/test2 0x1p20 1` | `./build/test2 0x1p20 2` | `./build/test2 0x1p20 4` | `./build/test2 0x1p20 8` | `./build/test2 0x1p20 16` |" >> ./build/result.md
	echo "| 0x1p21 | `./build/test2 0x1p21 1` | `./build/test2 0x1p21 2` | `./build/test2 0x1p21 4` | `./build/test2 0x1p21 8` | `./build/test2 0x1p21 16` |" >> ./build/result.md
	echo "| 0x1p22 | `./build/test2 0x1p22 1` | `./build/test2 0x1p22 2` | `./build/test2 0x1p22 4` | `./build/test2 0x1p22 8` | `./build/test2 0x1p22 16` |" >> ./build/result.md
	echo "| 0x1p23 | `./build/test2 0x1p23 1` | `./build/test2 0x1p23 2` | `./build/test2 0x1p23 4` | `./build/test2 0x1p23 8` | `./build/test2 0x1p23 16` |" >> ./build/result.md
	echo "| 0x1p24 | `./build/test2 0x1p24 1` | `./build/test2 0x1p24 2` | `./build/test2 0x1p24 4` | `./build/test2 0x1p24 8` | `./build/test2 0x1p24 16` |" >> ./build/result.md
	echo "| 0x1p25 | `./build/test2 0x1p25 1` | `./build/test2 0x1p25 2` | `./build/test2 0x1p25 4` | `./build/test2 0x1p25 8` | `./build/test2 0x1p25 16` |" >> ./build/result.md
	echo "| 0x1p26 | `./build/test2 0x1p26 1` | `./build/test2 0x1p26 2` | `./build/test2 0x1p26 4` | `./build/test2 0x1p26 8` | `./build/test2 0x1p26 16` |" >> ./build/result.md
	echo "| 0x1p27 | `./build/test2 0x1p27 1` | `./build/test2 0x1p27 2` | `./build/test2 0x1p27 4` | `./build/test2 0x1p27 8` | `./build/test2 0x1p27 16` |" >> ./build/result.md
	clear
	cat ./build/result.md

test3: build
	echo "|    |     128     |     256     |     512     |     1024    |     2048    |" > ./build/result.md
	echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "|  1 | `./build/test3 128 128 128 1` | `./build/test3 256 256 256 1` | `./build/test3 512 512 512 1` | `./build/test3 1024 1024 1024 1` | `./build/test3 2048 2048 2048 1` |" >> ./build/result.md
	echo "|  2 | `./build/test3 128 128 128 2` | `./build/test3 256 256 256 2` | `./build/test3 512 512 512 2` | `./build/test3 1024 1024 1024 2` | `./build/test3 2048 2048 2048 2` |" >> ./build/result.md
	echo "|  4 | `./build/test3 128 128 128 4` | `./build/test3 256 256 256 4` | `./build/test3 512 512 512 4` | `./build/test3 1024 1024 1024 4` | `./build/test3 2048 2048 2048 4` |" >> ./build/result.md
	echo "|  8 | `./build/test3 128 128 128 8` | `./build/test3 256 256 256 8` | `./build/test3 512 512 512 8` | `./build/test3 1024 1024 1024 8` | `./build/test3 2048 2048 2048 8` |" >> ./build/result.md
	echo "| 16 | `./build/test3 128 128 128 16` | `./build/test3 256 256 256 16` | `./build/test3 512 512 512 16` | `./build/test3 1024 1024 1024 16` | `./build/test3 2048 2048 2048 16` |" >> ./build/result.md
	clear
	cat ./build/result.md

test4: build
	echo "|        |      1      |      2      |      4      |      8      |      16     |" > ./build/result.md
	echo "|--------|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "| 0x1p20 | `./build/test4 0x1p20 1` | `./build/test4 0x1p20 2` | `./build/test4 0x1p20 4` | `./build/test4 0x1p20 8` | `./build/test4 0x1p20 16` |" >> ./build/result.md
	echo "| 0x1p21 | `./build/test4 0x1p21 1` | `./build/test4 0x1p21 2` | `./build/test4 0x1p21 4` | `./build/test4 0x1p21 8` | `./build/test4 0x1p21 16` |" >> ./build/result.md
	echo "| 0x1p22 | `./build/test4 0x1p22 1` | `./build/test4 0x1p22 2` | `./build/test4 0x1p22 4` | `./build/test4 0x1p22 8` | `./build/test4 0x1p22 16` |" >> ./build/result.md
	echo "| 0x1p23 | `./build/test4 0x1p23 1` | `./build/test4 0x1p23 2` | `./build/test4 0x1p23 4` | `./build/test4 0x1p23 8` | `./build/test4 0x1p23 16` |" >> ./build/result.md
	echo "| 0x1p24 | `./build/test4 0x1p24 1` | `./build/test4 0x1p24 2` | `./build/test4 0x1p24 4` | `./build/test4 0x1p24 8` | `./build/test4 0x1p24 16` |" >> ./build/result.md
	echo "| 0x1p25 | `./build/test4 0x1p25 1` | `./build/test4 0x1p25 2` | `./build/test4 0x1p25 4` | `./build/test4 0x1p25 8` | `./build/test4 0x1p25 16` |" >> ./build/result.md
	echo "| 0x1p26 | `./build/test4 0x1p26 1` | `./build/test4 0x1p26 2` | `./build/test4 0x1p26 4` | `./build/test4 0x1p26 8` | `./build/test4 0x1p26 16` |" >> ./build/result.md
	echo "| 0x1p27 | `./build/test4 0x1p27 1` | `./build/test4 0x1p27 2` | `./build/test4 0x1p27 4` | `./build/test4 0x1p27 8` | `./build/test4 0x1p27 16` |" >> ./build/result.md
	clear
	cat ./build/result.md

clean:
	if [ -d "build" ]; then rm -rf build; fi

.PHONY: all clean