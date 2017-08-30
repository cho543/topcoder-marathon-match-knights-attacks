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
test: a.out
	cd tester ; java -jar tester.jar -exec ../a.out -seed ${SEED} -vis
score: a.out
	for seed in `seq 10` ; do java -jar tester/tester.jar -exec ./a.out -seed $$seed ; done
case/%.in:
	python3 scrypt/generate.py `basename $@ .in` --tester tester/tester.jar > $@
