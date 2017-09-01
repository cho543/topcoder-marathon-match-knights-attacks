.PHONY: bild visualize test score

PROBLEM := KnightsAttacks
CXX := g++
CXXFLAGS := -std=c++11 -Wall -O2

RANDOM = $(shell bash -c 'echo $$RANDOM')
SEED ?= ${RANDOM}
SEED := ${SEED}

build: a.out
a.out: tester/main.cpp ${PROBLEM}.hpp
	${CXX} ${CXXFLAGS} -g -DLOCAL $<
a.out.visualize: tester/main.cpp ${PROBLEM}.hpp
	${CXX} ${CXXFLAGS} -g -DLOCAL $< -o $@ -DVISUALIZE
a.out.debug: tester/main.cpp ${PROBLEM}.hpp
	${CXX} -std=c++11 -Wall -g -DLOCAL -fsanitize=undefined -D_GLIBCXX_DEBUG $< -o $@ -DVISUALIZE
tester/KnightsAttacksVis.class: tester/KnightsAttacksVis.java
	cd tester ; javac KnightsAttacksVis.java
test: a.out.visualize tester/KnightsAttacksVis.class
	cd tester ; java KnightsAttacksVis -exec ../a.out.visualize -seed ${SEED} -vis -debug
score: a.out tester/KnightsAttacksVis.class
	cd tester ; for seed in `seq 10` ; do java KnightsAttacksVis -exec ../a.out -seed $$seed ; done
case/%.in:
	python3 scrypt/generate.py `basename $@ .in` --tester tester/tester.jar > $@
