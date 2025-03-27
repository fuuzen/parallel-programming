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

test1-all: build
	echo "|    |     128     |     256     |     512     |     1024    |     2048    |" > ./build/result.md
	echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "128" > ./build/input128
	echo "128" >> ./build/input128
	echo "128" >> ./build/input128
	echo "256" > ./build/input256
	echo "256" >> ./build/input256
	echo "256" >> ./build/input256
	echo "512" > ./build/input512
	echo "512" >> ./build/input512
	echo "512" >> ./build/input512
	echo "1024" > ./build/input1024
	echo "1024" >> ./build/input1024
	echo "1024" >> ./build/input1024
	echo "2048" > ./build/input2048
	echo "2048" >> ./build/input2048
	echo "2048" >> ./build/input2048
	echo "|  1 | `mpirun -np 1 ./build/test1-all < ./build/input128` | `mpirun -np 1 ./build/test1-all < ./build/input256` | `mpirun -np 1 ./build/test1-all < ./build/input512` | `mpirun -np 1 ./build/test1-all < ./build/input1024` | `mpirun -np 1 ./build/test1-all < ./build/input2048` |" >> ./build/result.md
	echo "|  2 | `mpirun -np 2 ./build/test1-all < ./build/input128` | `mpirun -np 2 ./build/test1-all < ./build/input256` | `mpirun -np 2 ./build/test1-all < ./build/input512` | `mpirun -np 2 ./build/test1-all < ./build/input1024` | `mpirun -np 2 ./build/test1-all < ./build/input2048` |" >> ./build/result.md
	echo "|  4 | `mpirun -np 4 ./build/test1-all < ./build/input128` | `mpirun -np 4 ./build/test1-all < ./build/input256` | `mpirun -np 4 ./build/test1-all < ./build/input512` | `mpirun -np 4 ./build/test1-all < ./build/input1024` | `mpirun -np 4 ./build/test1-all < ./build/input2048` |" >> ./build/result.md
	echo "|  8 | `mpirun -np 8 ./build/test1-all < ./build/input128` | `mpirun -np 8 ./build/test1-all < ./build/input256` | `mpirun -np 8 ./build/test1-all < ./build/input512` | `mpirun -np 8 ./build/test1-all < ./build/input1024` | `mpirun -np 8 ./build/test1-all < ./build/input2048` |" >> ./build/result.md
	echo "| 16 | `mpirun -np 16 ./build/test1-all < ./build/input128` | `mpirun -np 16 ./build/test1-all < ./build/input256` | `mpirun -np 16 ./build/test1-all < ./build/input512` | `mpirun -np 16 ./build/test1-all < ./build/input1024` | `mpirun -np 16 ./build/test1-all < ./build/input2048` |" >> ./build/result.md
	clear
	cat ./build/result.md

test2-all: build
	echo "|    |     128     |     256     |     512     |     1024    |     2048    |" > ./build/result.md
	echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "128" > ./build/input128
	echo "128" >> ./build/input128
	echo "128" >> ./build/input128
	echo "256" > ./build/input256
	echo "256" >> ./build/input256
	echo "256" >> ./build/input256
	echo "512" > ./build/input512
	echo "512" >> ./build/input512
	echo "512" >> ./build/input512
	echo "1024" > ./build/input1024
	echo "1024" >> ./build/input1024
	echo "1024" >> ./build/input1024
	echo "2048" > ./build/input2048
	echo "2048" >> ./build/input2048
	echo "2048" >> ./build/input2048
	echo "|  1 | `mpirun -np 1 ./build/test2-all < ./build/input128` | `mpirun -np 1 ./build/test2-all < ./build/input256` | `mpirun -np 1 ./build/test2-all < ./build/input512` | `mpirun -np 1 ./build/test2-all < ./build/input1024` | `mpirun -np 1 ./build/test2-all < ./build/input2048` |" >> ./build/result.md
	echo "|  2 | `mpirun -np 2 ./build/test2-all < ./build/input128` | `mpirun -np 2 ./build/test2-all < ./build/input256` | `mpirun -np 2 ./build/test2-all < ./build/input512` | `mpirun -np 2 ./build/test2-all < ./build/input1024` | `mpirun -np 2 ./build/test2-all < ./build/input2048` |" >> ./build/result.md
	echo "|  4 | `mpirun -np 4 ./build/test2-all < ./build/input128` | `mpirun -np 4 ./build/test2-all < ./build/input256` | `mpirun -np 4 ./build/test2-all < ./build/input512` | `mpirun -np 4 ./build/test2-all < ./build/input1024` | `mpirun -np 4 ./build/test2-all < ./build/input2048` |" >> ./build/result.md
	echo "|  8 | `mpirun -np 8 ./build/test2-all < ./build/input128` | `mpirun -np 8 ./build/test2-all < ./build/input256` | `mpirun -np 8 ./build/test2-all < ./build/input512` | `mpirun -np 8 ./build/test2-all < ./build/input1024` | `mpirun -np 8 ./build/test2-all < ./build/input2048` |" >> ./build/result.md
	echo "| 16 | `mpirun -np 16 ./build/test2-all < ./build/input128` | `mpirun -np 16 ./build/test2-all < ./build/input256` | `mpirun -np 16 ./build/test2-all < ./build/input512` | `mpirun -np 16 ./build/test2-all < ./build/input1024` | `mpirun -np 16 ./build/test2-all < ./build/input2048` |" >> ./build/result.md
	clear
	cat ./build/result.md

cannon-all: build
	echo "|    |     128     |     256     |     512     |     1024    |     2048    |" > ./build/result.md
	echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "128" > ./build/input128
	echo "128" >> ./build/input128
	echo "128" >> ./build/input128
	echo "256" > ./build/input256
	echo "256" >> ./build/input256
	echo "256" >> ./build/input256
	echo "512" > ./build/input512
	echo "512" >> ./build/input512
	echo "512" >> ./build/input512
	echo "1024" > ./build/input1024
	echo "1024" >> ./build/input1024
	echo "1024" >> ./build/input1024
	echo "2048" > ./build/input2048
	echo "2048" >> ./build/input2048
	echo "2048" >> ./build/input2048
	echo "|  1 | `mpirun -np 1 ./build/cannon-all < ./build/input128` | `mpirun -np 1 ./build/cannon-all < ./build/input256` | `mpirun -np 1 ./build/cannon-all < ./build/input512` | `mpirun -np 1 ./build/cannon-all < ./build/input1024` | `mpirun -np 1 ./build/cannon-all < ./build/input2048` |" >> ./build/result.md
	echo "|  4 | `mpirun -np 4 ./build/cannon-all < ./build/input128` | `mpirun -np 4 ./build/cannon-all < ./build/input256` | `mpirun -np 4 ./build/cannon-all < ./build/input512` | `mpirun -np 4 ./build/cannon-all < ./build/input1024` | `mpirun -np 4 ./build/cannon-all < ./build/input2048` |" >> ./build/result.md
	echo "| 16 | `mpirun -np 16 ./build/cannon-all < ./build/input128` | `mpirun -np 16 ./build/cannon-all < ./build/input256` | `mpirun -np 16 ./build/cannon-all < ./build/input512` | `mpirun -np 16 ./build/cannon-all < ./build/input1024` | `mpirun -np 16 ./build/cannon-all < ./build/input2048` |" >> ./build/result.md
	clear
	cat ./build/result.md

clean:
	if [ -d "build" ]; then rm -rf build; fi

.PHONY: all clean