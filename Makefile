VERSION=0.1.0
WHEEL=dist/python_mesa3d-$(VERSION)-cp313-cp313-linux_x86_64.whl
PACKAGE=python-mesa3d-$(VERSION)-1-x86_64.pkg.tar.zst

all: install run
run:
	LD_PRELOAD=/usr/lib/libasan.so.8 ASAN_OPTIONS=detect_leaks=0 python test.py
install: build
	makepkg -si --noconfirm
build: $(PACKAGE)
$(PACKAGE): $(WHEEL)
	cp $(WHEEL) .
	updpkgsums
	makepkg -f
$(WHEEL): bindings.cpp
	#python -m build --wheel
	python -m build --wheel -Csetup-args="-Dbuildtype=debug" -Csetup-args="-Db_sanitize=address"
clean:
	rm -rf dist pkg src *.zst *.whl
	yay -R python-mesa3d
gdb:
	LD_PRELOAD=/usr/lib/libasan.so.8 ASAN_OPTIONS=detect_leaks=0 gdb -ex=r --directory=./subprojects --args python test.py

# Setup build dir for syntax checking
# meson setup build -Dbuildtype=debug -Db_sanitize=address
check:
	cd build; ninja
.PHONY: all build check gdb install clean
