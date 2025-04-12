dev:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	mv build/compile_commands.json .

build:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake .. && make -j8

test2: build
	echo "|        | monte carlo |" > ./build/result.md
	echo "|--------|-------------|" >> ./build/result.md
	echo "| 0x1p04 | `./build/test2 0x1p04 16` |" >> ./build/result.md
	echo "| 0x1p05 | `./build/test2 0x1p05 16` |" >> ./build/result.md
	echo "| 0x1p06 | `./build/test2 0x1p06 16` |" >> ./build/result.md
	echo "| 0x1p07 | `./build/test2 0x1p07 16` |" >> ./build/result.md
	echo "| 0x1p08 | `./build/test2 0x1p08 16` |" >> ./build/result.md
	echo "| 0x1p09 | `./build/test2 0x1p09 16` |" >> ./build/result.md
	echo "| 0x1p11 | `./build/test2 0x1p11 16` |" >> ./build/result.md
	echo "| 0x1p12 | `./build/test2 0x1p12 16` |" >> ./build/result.md
	echo "| 0x1p13 | `./build/test2 0x1p13 16` |" >> ./build/result.md
	echo "| 0x1p14 | `./build/test2 0x1p14 16` |" >> ./build/result.md
	echo "| 0x1p15 | `./build/test2 0x1p15 16` |" >> ./build/result.md
	echo "| 0x1p16 | `./build/test2 0x1p16 16` |" >> ./build/result.md
	echo "| 0x1p17 | `./build/test2 0x1p17 16` |" >> ./build/result.md
	echo "| 0x1p18 | `./build/test2 0x1p18 16` |" >> ./build/result.md
	echo "| 0x1p19 | `./build/test2 0x1p19 16` |" >> ./build/result.md
	echo "| 0x1p20 | `./build/test2 0x1p20 16` |" >> ./build/result.md
	clear
	cat ./build/result.md

clean:
	if [ -d "build" ]; then rm -rf build; fi

.PHONY: all clean