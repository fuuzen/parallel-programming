dev:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	mv build/compile_commands.json .

build:
	if [ ! -d "build" ]; then mkdir build; fi
	cd build && cmake .. && make -j8

NP ?= 16
# eg: make test NP=4

test: build
	echo "|    |     128     |     256     |     512     |     1024    |     2048    |" > ./build/result.md
	echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "|  1 | `mpirun -np 1 ./build/test 128 128 128` | `mpirun -np 1 ./build/test 256 256 256` | `mpirun -np 1 ./build/test 512 512 512` | `mpirun -np 1 ./build/test 1024 1024 1024` | `mpirun -np 1 ./build/test 2048 2048 2048` |" >> ./build/result.md
	echo "|  2 | `mpirun -np 2 ./build/test 128 128 128` | `mpirun -np 2 ./build/test 256 256 256` | `mpirun -np 2 ./build/test 512 512 512` | `mpirun -np 2 ./build/test 1024 1024 1024` | `mpirun -np 2 ./build/test 2048 2048 2048` |" >> ./build/result.md
	echo "|  4 | `mpirun -np 4 ./build/test 128 128 128` | `mpirun -np 4 ./build/test 256 256 256` | `mpirun -np 4 ./build/test 512 512 512` | `mpirun -np 4 ./build/test 1024 1024 1024` | `mpirun -np 4 ./build/test 2048 2048 2048` |" >> ./build/result.md
	echo "|  8 | `mpirun -np 8 ./build/test 128 128 128` | `mpirun -np 8 ./build/test 256 256 256` | `mpirun -np 8 ./build/test 512 512 512` | `mpirun -np 8 ./build/test 1024 1024 1024` | `mpirun -np 8 ./build/test 2048 2048 2048` |" >> ./build/result.md
	echo "| 16 | `mpirun -np 16 ./build/test 128 128 128` | `mpirun -np 16 ./build/test 256 256 256` | `mpirun -np 16 ./build/test 512 512 512` | `mpirun -np 16 ./build/test 1024 1024 1024` | `mpirun -np 16 ./build/test 2048 2048 2048` |" >> ./build/result.md
	clear
	cat ./build/result.md

test-mkl: build
	echo "|    |     128     |     256     |     512     |     1024    |     2048    |" > ./build/result.md
	echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "|  1 | `mpirun -np 1 ./build/test-mkl 128 128 128` | `mpirun -np 1 ./build/test-mkl 256 256 256` | `mpirun -np 1 ./build/test-mkl 512 512 512` | `mpirun -np 1 ./build/test-mkl 1024 1024 1024` | `mpirun -np 1 ./build/test-mkl 2048 2048 2048` |" >> ./build/result.md
	echo "|  2 | `mpirun -np 2 ./build/test-mkl 128 128 128` | `mpirun -np 2 ./build/test-mkl 256 256 256` | `mpirun -np 2 ./build/test-mkl 512 512 512` | `mpirun -np 2 ./build/test-mkl 1024 1024 1024` | `mpirun -np 2 ./build/test-mkl 2048 2048 2048` |" >> ./build/result.md
	echo "|  4 | `mpirun -np 4 ./build/test-mkl 128 128 128` | `mpirun -np 4 ./build/test-mkl 256 256 256` | `mpirun -np 4 ./build/test-mkl 512 512 512` | `mpirun -np 4 ./build/test-mkl 1024 1024 1024` | `mpirun -np 4 ./build/test-mkl 2048 2048 2048` |" >> ./build/result.md
	echo "|  8 | `mpirun -np 8 ./build/test-mkl 128 128 128` | `mpirun -np 8 ./build/test-mkl 256 256 256` | `mpirun -np 8 ./build/test-mkl 512 512 512` | `mpirun -np 8 ./build/test-mkl 1024 1024 1024` | `mpirun -np 8 ./build/test-mkl 2048 2048 2048` |" >> ./build/result.md
	echo "| 16 | `mpirun -np 16 ./build/test-mkl 128 128 128` | `mpirun -np 16 ./build/test-mkl 256 256 256` | `mpirun -np 16 ./build/test-mkl 512 512 512` | `mpirun -np 16 ./build/test-mkl 1024 1024 1024` | `mpirun -np 16 ./build/test-mkl 2048 2048 2048` |" >> ./build/result.md
	clear
	cat ./build/result.md

cannon: build
	echo "|    |     128     |     256     |     512     |     1024    |     2048    |" > ./build/result.md
	echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build/result.md
	echo "|  1 | `mpirun -np 1 ./build/cannon 128 128 128` | `mpirun -np 1 ./build/cannon 256 256 256` | `mpirun -np 1 ./build/cannon 512 512 512` | `mpirun -np 1 ./build/cannon 1024 1024 1024` | `mpirun -np 1 ./build/cannon 2048 2048 2048` |" >> ./build/result.md
	echo "|  4 | `mpirun -np 4 ./build/cannon 128 128 128` | `mpirun -np 4 ./build/cannon 256 256 256` | `mpirun -np 4 ./build/cannon 512 512 512` | `mpirun -np 4 ./build/cannon 1024 1024 1024` | `mpirun -np 4 ./build/cannon 2048 2048 2048` |" >> ./build/result.md
	echo "| 16 | `mpirun -np 16 ./build/cannon 128 128 128` | `mpirun -np 16 ./build/cannon 256 256 256` | `mpirun -np 16 ./build/cannon 512 512 512` | `mpirun -np 16 ./build/cannon 1024 1024 1024` | `mpirun -np 16 ./build/cannon 2048 2048 2048` |" >> ./build/result.md
	clear
	cat ./build/result.md

# test-interactive: build
# 	mpirun -np $(NP) ./build/test-interactive

# test-mkl-interactive: build
# 	mpirun -np $(NP) ./build/test-mkl-interactive

# cannon-interactive: build
# 	mpirun -np $(NP) ./build/cannon-interactive

clean:
	if [ -d "build" ]; then rm -rf build; fi

.PHONY: all clean