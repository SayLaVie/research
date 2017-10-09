CXX=g++
CXX_FLAGS=-std=c++11
CPP=$(wildcard src/*.cpp)
SHARED_OBJECTS=src/build/hex.o src/build/hexGamePlayer.o src/build/hexWorld.o src/build/hexHelperFunctions.o
ALL_OBJECTS=$(wildcard src/build/*.o)
DEP=$(ALL_OBJECTS:%.o=%.d)

hexEvolution : $(SHARED_OBJECTS) src/build/hexEvolutionMain.o
	$(CXX) $(CXX_FLAGS) $^ -o hexEvolution

-include $(DEP)

src/build/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

clean:
	rm $(ALL_OBJECTS) $(DEP) hexEvolution 
