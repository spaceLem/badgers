SHELL = /bin/bash
#CXX = g++
#CXXFLAGS = -ggdb -Wall -O2 -march=x86-64 #-pg
#LIBS = -lgsl -lgslcblas #-lstdc++
CXX = clang++
CXXFLAGS = --std=c++17 -Wall -O2 -march=corei7 -I/opt/local/include #-pg
#LIBS = -L/opt/local/lib -lgsl -lgslcblas #-lstdc++
LIBS = -lgsl -lgslcblas #-lstdc++

OBJ = bias.o birth.o check.o cull.o death.o details.o distance_kernel.o \
	  infect.o main.o movement.o nbhd.o pick.o pick_to.o records.o \
	  setup_params.o setup_popns.o simulate.o update_ring.o write.o

all: SIspatial

# $@ = target (SIspatial)
# $^ = all dependencies

SIspatial: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
#-cp SIspatial ..

clean:
	-rm -f *.o

# clang++ -MM *cpp >> Makefile
bias.o: bias.cpp bias.hpp defs.hpp
birth.o: birth.cpp birth.hpp defs.hpp
check.o: check.cpp check.hpp defs.hpp utils.hpp
cull.o: cull.cpp cull.hpp defs.hpp utils.hpp nbhd.hpp pick.hpp \
  update_ring.hpp
death.o: death.cpp death.hpp defs.hpp
details.o: details.cpp details.hpp defs.hpp utils.hpp
distance_kernel.o: distance_kernel.cpp distance_kernel.hpp defs.hpp
infect.o: infect.cpp infect.hpp defs.hpp utils.hpp
main.o: main.cpp defs.hpp records.hpp utils.hpp simulate.hpp birth.hpp \
  check.hpp cull.hpp nbhd.hpp pick.hpp update_ring.hpp death.hpp \
  details.hpp infect.hpp movement.hpp pick_to.hpp setup_params.hpp \
  bias.hpp distance_kernel.hpp setup_popns.hpp write.hpp
movement.o: movement.cpp movement.hpp defs.hpp nbhd.hpp utils.hpp
nbhd.o: nbhd.cpp nbhd.hpp defs.hpp
pick.o: pick.cpp pick.hpp defs.hpp utils.hpp
pick_to.o: pick_to.cpp pick_to.hpp defs.hpp nbhd.hpp pick.hpp utils.hpp
records.o: records.cpp records.hpp defs.hpp utils.hpp
setup_params.o: setup_params.cpp setup_params.hpp defs.hpp bias.hpp \
  distance_kernel.hpp nbhd.hpp utils.hpp
setup_popns.o: setup_popns.cpp setup_popns.hpp defs.hpp pick.hpp \
  utils.hpp
simulate.o: simulate.cpp simulate.hpp defs.hpp birth.hpp check.hpp \
  utils.hpp cull.hpp nbhd.hpp pick.hpp update_ring.hpp death.hpp \
  details.hpp infect.hpp movement.hpp pick_to.hpp records.hpp \
  setup_params.hpp bias.hpp distance_kernel.hpp setup_popns.hpp
update_ring.o: update_ring.cpp update_ring.hpp defs.hpp
write.o: write.cpp write.hpp defs.hpp records.hpp utils.hpp
