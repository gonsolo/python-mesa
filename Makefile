VERSION=0.1.0
WHEEL=dist/python_mesa3d-$(VERSION)-cp313-cp313-linux_x86_64.whl
PACKAGE=python-mesa3d-$(VERSION)-1-x86_64.pkg.tar.zst

all: install run
run:
	python test.py
install: build
	makepkg -si --noconfirm
build: $(PACKAGE)
$(PACKAGE): $(WHEEL)
	cp $(WHEEL) .
	updpkgsums
	makepkg -f
$(WHEEL): bindings.cpp
	#python -m build --wheel
	python -m build --wheel -Csetup-args="-Dbuildtype=debug" # For debuginfo
clean:
	rm -rf dist pkg src *.zst *.whl
	yay -R python-mesa3d
gdb:
	gdb -ex=r --args python test.py

.PHONY: all build gdb install clean
