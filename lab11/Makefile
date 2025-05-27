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
	@echo "|      | CUBLAS GFLOPS |  CUBLAS msec  | MyImpl GFLOPS |  MyImpl msec  |" > ./build-release/result.md
	@echo "|      | CUBLAS GFLOPS |  CUBLAS msec  | MyImpl GFLOPS |  MyImpl msec  |"
	@echo "|------|---------------|---------------|---------------|---------------|" >> ./build-release/result.md
	@echo "|------|---------------|---------------|---------------|---------------|"
	@for n in 1 2 4 8 16 32 64 128; do \
		s=$$(printf "| %4d | \
$$(./build-release/test --sizemult=$$n --method=0 | grep Performance | awk '{printf "%13.3f | %13.3f |", $$2, $$5}')\
$$(./build-release/test --sizemult=$$n --method=1 | grep Performance | awk '{printf "%13.3f | %13.3f |", $$2, $$5}')\
\n" $$n); \
		echo "$$s" >> ./build-release/result.md; \
		echo "$$s"; \
	done
