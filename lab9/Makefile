.PHONY: all dev debug release clean

all: debug

build:
	@echo "Building Release version..."
	@if [ ! -d "build" ]; then mkdir build; fi
	@nvcc -o build/task1 task1/main.cu
	@nvcc -o build/task2 task2/main.cu

clean:
	@echo "Cleaning build directories..."
	@rm -rf build

test: release
	@echo "Running tests..."
	@echo "|    |     930     |     525     |" > ./build-release/result.md
	@echo "|----|-------------|-------------|" >> ./build-release/result.md
	@for threads in 1 2 4 8 16; do \
		printf "| %2d | \
$$(printf '0 0' | ./build-release/task1 $$threads ./data/updated_flower.csv | head -n 1 ) | \
$$(printf '0 0' | ./build-release/task1 $$threads ./data/updated_mouse.csv | head -n 1 ) |\n" \
		$$threads \
		>> ./build-release/result.md; \
	done
	@clear
	@cat ./build-release/result.md
