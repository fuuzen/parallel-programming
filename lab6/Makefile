.PHONY: all dev debug release clean

all: debug

dev:
	@if [ ! -d "build" ]; then mkdir build; fi
	@cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	@mv build/compile_commands.json .

update:
	@if [ -d "build-release" ]; then cd build-release && make -j8 && cd ..; fi
	@if [ -d "build-debug" ]; then cd build-debug && make -j8 && cd ..; fi

debug:
	@echo "Building Debug version..."
	@if [ ! -d "build-debug" ]; then mkdir build-debug; fi
	@cd build-debug && cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j8

release:
	@echo "Building Release version..."
	@if [ ! -d "build-release" ]; then mkdir build-release; fi
	@cd build-release && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8

clean:
	@echo "Cleaning build directories..."
	@rm -rf build-debug build-release

test: release
	@echo "Running tests..."
	@echo "|    |      64     |     128     |     256     |     512     |     1024    |" > ./build-release/result.md
	@echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build-release/result.md
	@for threads in 1 2 4 8 16; do \
		printf "| %2d | $$( \
			for grid in 64 128 256 512 1024; do \
				./build-release/heated_plate $$grid $$grid $$threads | grep "Wallclock time" | awk '{print $$4}'; \
			done | tr '\n' '|' | sed 's/|/ | /g' \
		)\n" $$threads >> ./build-release/result.md; \
	done
	@clear
	@cat ./build-release/result.md

test-omp: release
	@echo "Running OpenMP Version tests..."
	@echo "|    |      64     |     128     |     256     |     512     |     1024    |" > ./build-release/result-omp.md
	@echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build-release/result-omp.md
	@for threads in 1 2 4 8 16; do \
		printf "| %2d | $$( \
			for grid in 64 128 256 512 1024; do \
			  echo "# define M $$grid\n# define N $$grid\n# define NUM_THREADS $$threads" > omp_ver/heated_plate_openmp.h; \
				make update > /dev/null; \
				./build-release/omp_ver | grep "Wallclock time" | awk '{print $$4}'; \
			done | tr '\n' '|' | sed 's/|/ | /g' \
		)\n" $$threads >> ./build-release/result-omp.md; \
	done
	@clear
	@cat ./build-release/result-omp.md

