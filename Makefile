curent_dir=$(shell pwd)
libevent_dir := $$(dirname $(wildcard ${curent_dir}/third_party/libevent*/configure))

.PHONY : all build-dir build-src build-libevent clean

all:
	$(MAKE) build-libevent
	$(MAKE) build-src

build-dir:
	@if [ ! -d "build" ]; \
	then \
		mkdir -p build; \
	fi
	@echo "ready to building..."

build-src:
	@cd build && cmake .. && make -j4

clean:
	@if [ -d "build" ]; \
	then \
		rm -rf build; \
	fi

	@echo "make clean ok"

build-libevent: build-dir
	@if [ -f "${libevent_dir}/Makefile" ]; \
	then \
		cd ${libevent_dir} && make -j4; \
	else \
		cd ${libevent_dir} && ./configure && make; \
	fi

	@install -d ${curent_dir}/build/lib
	@install -d ${curent_dir}/build/libevent/include
	@install ${libevent_dir}/.libs/*.a ${curent_dir}/build/lib/
	@install ${libevent_dir}/.libs/*.so* ${curent_dir}/build/lib/
	@cp -rf ${libevent_dir}/include/* ${curent_dir}/build/libevent/include

	@cd ${libevent_dir} && make clean