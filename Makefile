CXX=g++
CXX_FLAGS=-std=c++11 -Isrc/include/
CPP=$(wildcard src/*.cpp)
SHARED_OBJECTS=src/build/hex.o src/build/hexGamePlayer.o
EVOLUTION_OBJECTS=src/build/hexEvolutionMain.o src/build/hexEvolutionHelpers.o src/build/hexWorld.o
HUMANBOT_OBJECTS=src/build/hexHumanVsBotMain.o src/build/hexHumanVsBotHelpers.o
BOTBOT_OBJECTS=src/build/hexBotVsBotMain.o src/build/hexBotVsBotHelpers.o
POPPOP_OBJECTS=src/build/hexPopulationVsPopulationMain.o src/build/hexPopulationVsPopulationHelpers.o src/build/hexWorld.o
PLAYERRATING_OBJECTS=src/build/hexPlayerRatingMain.o src/build/hexPlayerRatingHelpers.o
ALL_OBJECTS=$(wildcard src/build/*.o)
DEP=$(ALL_OBJECTS:%.o=%.d)

all : hexEvolution hexHumanVsBot hexBotVsBot hexPopulationVsPopulation hexPlayerRating

hexEvolution : $(SHARED_OBJECTS) $(EVOLUTION_OBJECTS)
	$(CXX) $(CXX_FLAGS) $^ -o hexEvolution

hexHumanVsBot : $(SHARED_OBJECTS) $(HUMANBOT_OBJECTS)
	$(CXX) $(CXX_FLAGS) $^ -o hexHumanVsBot

hexBotVsBot : $(SHARED_OBJECTS) $(BOTBOT_OBJECTS)
	$(CXX) $(CXX_FLAGS) $^ -o hexBotVsBot

hexPopulationVsPopulation : $(SHARED_OBJECTS) $(POPPOP_OBJECTS)
	$(CXX) $(CXX_FLAGS) $^ -o hexPopulationVsPopulation

hexPlayerRating : $(SHARED_OBJECTS) $(PLAYERRATING_OBJECTS)
	$(CXX) $(CXX_FLAGS) $^ -o hexPlayerRating

-include $(DEP)

src/build/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

clean:
	rm $(ALL_OBJECTS) $(DEP) hexEvolution hexHumanVsBot hexBotVsBot hexPopulationVsPopulation hexPlayerRating
