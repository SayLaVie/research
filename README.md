# Research on The Game of Hex
Author: Michael McCarver<br />
Advisor: Dr. Rob LeGrand

#### Research Description
*Hex* is a two-player adversarial board game in which there is always exactly one winner. We create game-playing agents that use feed-forward Artificial Neural Networks to make evaluations of the board-state at any given point in the game, and use a genetic algorithm to teach the agents to improve the accuracy of their evaluations.

#### HowTo
This code is meant to be run in a *nix environment. To compile, run `make all` from the terminal. This will produce five executables:
- hexEvolution
   - This program begins/resumes experimental iterations. After every *n* iterations, the program will output a file containing the ANN data for the entire current population, the ANN data for each individual player, and a player that represents a weighted average of all players in the population.
- hexBotVsBot
   - This program plays two hex-playing agents against one another. The input files should be the same format as the output files from hexEvolution
- hexHumanVsBot
