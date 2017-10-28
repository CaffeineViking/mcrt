config="release"

files="share/render.png"
scene="share/scene.json"
param="share/param.json"

all: FORCE
	premake5 gmake
	make -j8 -C build config=${config}
run: FORCE
	premake5 gmake
	make -j8 -C build mcrt config=${config}
	bin/mcrt ${args}
render: FORCE
	premake5 gmake
	make -j8 -C build mcrt config=${config}
	bin/mcrt ${files} ${scene} ${param}
view-render: FORCE
	premake5 gmake
	make -j8 -C build mcrt config=${config}
	sleep 2s && feh --force-aliasing -R2 -F share/render.png &
	bin/mcrt ${files} ${scene} ${param}

profile: FORCE
	premake5 gmake
	make -j8 -C build mcrt config=${config}
	perf record -F 512 -g -- bin/mcrt share/render.png share/scene.json share/param.json
	perf script | utils/stackcollapse-perf.pl > share/perf.out
	utils/flamegraph.pl share/perf.out > share/perf.svg
	rm -rf perf.data
view-profile: profile
	$(BROWSER) share/perf.svg

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
	rm -rf share/perf.out
	rm -rf share/perf.svg
	rm -rf share/photons.csv
	rm -rf share/statistics.csv
	rm -rf share/photon_map.csv
distclean: clean
	rm -rf bin
	rm -f docs/mcrt.pdf
	rm -f docs/slides/slides.pdf
FORCE:

.PHONY: all run render view-render profile view-profile docs slides tags clean distclean
