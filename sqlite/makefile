PROJECT=sqlite

all: build/Makefile
	@$(MAKE) --no-print-directory -C build

build/Makefile: CMakeLists.txt build
	@cd build && CC=clang-3.7 CXX=clang++-3.7 cmake \
		-DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=.. \
		..

build:
	@mkdir build

build/$(PROJECT).a: build/Makefile
	@$(MAKE) --no-print-directory -C build $(PROJECT)

install: build/Makefile
	@$(MAKE) --no-print-directory -C build install

clean:
	rm -rf build
