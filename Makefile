.PHONY: bild visualize test score

PROBLEM := KnightsAttacks
CXX := g++
CXXFLAGS := -std=c++11 -Wall -O2

RANDOM = $(shell bash -c 'echo $$RANDOM')
SEED ?= ${RANDOM}
SEED := ${SEED}

build: a.out
a.out: ${PROBLEM}.cpp ${PROBLEM}.hpp
	${CXX} ${CXXFLAGS} -g -DLOCAL $<
a.out.visualize: ${PROBLEM}.cpp ${PROBLEM}.hpp
	${CXX} ${CXXFLAGS} -g -DLOCAL $< -o $@ -DVISUALIZE
test: a.out
	cd tester ; java -jar tester.jar -exec ../a.out -seed ${SEED} -vis
