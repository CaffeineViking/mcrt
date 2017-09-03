config="debug"
all: FORCE
	premake5 gmake
	make -j8 -C build config=${config}
test: FORCE
	premake5 gmake
	make -j8 -C build mcrt-tests config=${config}
	bin/mcrt-tests ${args}
run: FORCE
	premake5 gmake
	make -j8 -C build mcrt config=${config}
	bin/mcrt ${args}

docs: FORCE
	make -C docs/
view: FORCE
	make -C docs/ view
tags: FORCE
	ctags -R src/mcrt include/mcrt
	cscope -Rb -s src/mcrt -s include/mcrt

clean: FORCE
	rm -rf build
	rm -rf docs/build
distclean: clean
	rm -rf bin
	rm -f docs/mcrt.pdf
FORCE:

# Clarifies gmake that these aren't real deps.
.PHONY: all test run docs tags clean distclean
