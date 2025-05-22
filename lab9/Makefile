.PHONY: all dev debug release clean

all: debug

dev:
	@if [ ! -d "build-debug" ]; then mkdir build-debug; fi
	@cd build-debug && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	@mv build-debug/compile_commands.json .

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
	@rm -rf build

test: release
	@echo "Running tests..."
	@echo "|    |     128     |     256     |     512     |    1024     |    2048     |" > ./build-release/result_global.md
	@echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build-release/result_global.md
	@for tile_size in 1 2 4 8 16 32; do \
		printf "| %2d | $$( \
			for n in 128 256 512 1024 2048; do \
				./build-release/task2 $$n $$tile_size G; \
			done | tr '\n' '|' | sed 's/|/ | /g' \
		)\n" $$tile_size >> ./build-release/result_global.md; \
	done
	@echo "|    |     128     |     256     |     512     |    1024     |    2048     |" > ./build-release/result_shared.md
	@echo "|----|-------------|-------------|-------------|-------------|-------------|" >> ./build-release/result_shared.md
	@for tile_size in 1 2 4 8 16 32; do \
		printf "| %2d | $$( \
			for n in 128 256 512 1024 2048; do \
				./build-release/task2 $$n $$tile_size S; \
			done | tr '\n' '|' | sed 's/|/ | /g' \
		)\n" $$tile_size >> ./build-release/result_shared.md; \
	done
	@clear
	@printf 'Global Memory Access:\n'
	@cat ./build-release/result_global.md
	@printf 'Shared Memory Access:\n'
	@cat ./build-release/result_shared.md
