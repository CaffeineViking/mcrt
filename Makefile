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
slides: FORCE
	make -C docs/slides/
tags: FORCE
	ctags -R src/mcrt include/mcrt
	cscope -Rb -s src/mcrt -s include/mcrt

clean: FORCE
	rm -rf build
	rm -rf docs/build
	rm -rf docs/slides/build
distclean: clean
	rm -rf bin
	rm -f docs/mcrt.pdf
	rm -f docs/slides/slides.pdf
FORCE:

# Clarifies gmake that these aren't real dependencies
.PHONY: all test run docs slides tags clean distclean
