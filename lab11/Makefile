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
	@echo "|      | SlidingWindow |  im2col+gemm  |     CuDNN     |" > ./build-release/result.md
	@echo "|      | SlidingWindow |  im2col+gemm  |     CuDNN     |"
	@echo "|------|---------------|---------------|---------------|" >> ./build-release/result.md
	@echo "|------|---------------|---------------|---------------|"
	@for n in 128 256 512 1024 2048; do \
		s=$$(printf "| %4d | \
$$(./build-release/test --input=images/input_$$n.png --method=0 | grep Performance | awk '{printf "%13.3f |", $$2}')\
$$(./build-release/test --input=images/input_$$n.png --method=1 | grep Performance | awk '{printf "%13.3f |", $$2}')\
$$(./build-release/test --input=images/input_$$n.png --method=2 | grep Performance | awk '{printf "%13.3f |", $$2}')\
\n" $$n); \
		echo "$$s" >> ./build-release/result.md; \
		echo "$$s"; \
	done

test-stride2: release
	@echo "Running tests..."
	@echo "|      | SlidingWindow |  im2col+gemm  |     CuDNN     |" > ./build-release/result.md
	@echo "|      | SlidingWindow |  im2col+gemm  |     CuDNN     |"
	@echo "|------|---------------|---------------|---------------|" >> ./build-release/result.md
	@echo "|------|---------------|---------------|---------------|"
	@for n in 128 256 512 1024 2048; do \
		s=$$(printf "| %4d | \
$$(./build-release/test --input=images/input_$$n.png --method=0 --stride=2 | grep Performance | awk '{printf "%13.3f |", $$2}')\
$$(./build-release/test --input=images/input_$$n.png --method=1 --stride=2 | grep Performance | awk '{printf "%13.3f |", $$2}')\
$$(./build-release/test --input=images/input_$$n.png --method=2 --stride=2 | grep Performance | awk '{printf "%13.3f |", $$2}')\
\n" $$n); \
		echo "$$s" >> ./build-release/result.md; \
		echo "$$s"; \
	done

test-stride3: release
	@echo "Running tests..."
	@echo "|      | SlidingWindow |  im2col+gemm  |     CuDNN     |" > ./build-release/result.md
	@echo "|      | SlidingWindow |  im2col+gemm  |     CuDNN     |"
	@echo "|------|---------------|---------------|---------------|" >> ./build-release/result.md
	@echo "|------|---------------|---------------|---------------|"
	@for n in 128 256 512 1024 2048; do \
		s=$$(printf "| %4d | \
$$(./build-release/test --input=images/input_$$n.png --method=0 --stride=3 | grep Performance | awk '{printf "%13.3f |", $$2}')\
$$(./build-release/test --input=images/input_$$n.png --method=1 --stride=3 | grep Performance | awk '{printf "%13.3f |", $$2}')\
$$(./build-release/test --input=images/input_$$n.png --method=2 --stride=3 | grep Performance | awk '{printf "%13.3f |", $$2}')\
\n" $$n); \
		echo "$$s" >> ./build-release/result.md; \
		echo "$$s"; \
	done
