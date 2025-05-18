.PHONY: all dev debug release clean

all: debug

dev:
	@if [ ! -d "build-debug" ]; then mkdir build-debug; fi
	@cd build-debug && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	@mv build-debug/compile_commands.json .

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

task2: release
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
