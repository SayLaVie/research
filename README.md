# Research on The Game of Hex
Author: Michael McCarver<br />
Advisor: Dr. Rob LeGrand

#### Research Description
*Hex* is a two-player adversarial board game in which there is always exactly one winner. We create game-playing agents that use feed-forward Artificial Neural Networks to make evaluations of the board-state at any given point in the game, and use a genetic algorithm to teach the agents to improve the accuracy of their evaluations.

#### HowTo
This code is meant to be run in a *nix environment. To compile, run `make all` from the terminal. This will produce five executables:
- hexEvolution
   - This program begins/resumes experimental iterations. After every *n* iterations, the program will output files containing the ANN data for the entire current population, the ANN data for each individual player, and data for a player that represents a weighted average of all players in the population.
- hexBotVsBot
   - This program plays two hex-playing agents against one another. The input files should be the same format as the individual-player output files from hexEvolution
- hexHumanVsBot
   - This program allows a human player to play a game of hex with an agent. Input files should be the same format as the individual-player output files from hexEvolution
- hexPopulationVsPopulation
   - This program plays two experimental populations against one another and keeps track of:
      - Total games won for each population
      - Number of games won as both PlayerA and PlayerB
      - The number of games won by each player sorted from greatest to least
   The input files should be the same format as the population data files from hexEvolution
- hexPopulationRating
   - This program plays an experimental population against 2000 random agents and keeps track of each players number of games won as well as each player's winningness as PlayerA.
More detailed explanations can be viewed by running `./<program name> --help` or `./<program name> -h`. Subdirectories of research/data/ hold output files from experiments run during fall 2017.
