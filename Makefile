curent_dir=$(shell pwd)
libevent_dir := $$(dirname $(wildcard ${curent_dir}/third_party/libevent*/configure))
mockcpp_dir := ${curent_dir}/tests/mockcpp

.PHONY : all build-dir build-src build-libevent clean clean-libevent install-libevent

all:
	$(MAKE) build-libevent
	$(MAKE) build-src
	$(MAKE) build-mockcpp

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
	@if [ ! -f "${curent_dir}/build/lib/libevent.so" ] && [ -f "${libevent_dir}/Makefile" ]; \
	then \
		cd ${libevent_dir} && make -j4; \
	elif [ ! -f "${curent_dir}/build/lib/libevent.so" ]; \
	then \
		cd ${libevent_dir} && ./configure && make -j4; \
	else \
		echo "libevent is installed"; \
	fi

	@if [ ! -f "${curent_dir}/build/lib/libevent.so" ]; \
	then \
		${MAKE} install-libevent; \
	fi

install-libevent:
	@install -d ${curent_dir}/build/lib
	@install -d ${curent_dir}/build/third_party/libevent/include
	@install ${libevent_dir}/.libs/*.a ${curent_dir}/build/lib/
	@install ${libevent_dir}/.libs/*.so* ${curent_dir}/build/lib/
	@cp -rf ${libevent_dir}/include/* ${curent_dir}/build/third_party/libevent/include
	@echo "libevent install successfully"

clean-libevent:
	@cd ${libevent_dir} && make clean

.PHONY : build-mockcpp install-mockcpp clean-mockcpp

build-mockcpp:
	@echo "mockcpp building"
	@if [ ! -d "${mockcpp_dir}/build" ]; then \
		mkdir -p ${mockcpp_dir}/build; \
	fi
	@cd ${mockcpp_dir}/build && cmake .. && make -j4

install-mockcpp:
	@install -d ${curent_dir}/build/lib
	@install -d ${curent_dir}/build/third_party/mockcpp/include
	@install ${mockcpp_dir}/build/src/*.a ${curent_dir}/build/lib/
	@cp -rf ${mockcpp_dir}/include/* ${curent_dir}/build/third_party/mockcpp/include/

clean-mockcpp:
	@rm -rf ${mockcpp_dir}/build