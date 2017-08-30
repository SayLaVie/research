// Pseudo code layout of classes that will be used
#include <vector>

using namespace std;

class Player {
private:
	// These heuristic weights are determined via genetic algorithm before
	// the instantiation of the player class.
	vector<int> heuristicWeights; 

public:
	Player(vector<int> weights) {heuristicWeights = weights;}
	int play(const Board &board, player whichPlayer); // this function will start the minimax off.
	int minMax(Board board, player whichPlayer, int depth, int alpha, int beta); // same for everyone
	int heuristic(Board board); // This function will use the weights passed in (neural net)
}





/*************************************************************************************************/

main() {
	while () {
		geneticAlgorithm() // Determine weights for new players
		Player a, b, c.....n // instantiate player classes with new weights

		// Every player plays against one another
		for each Player {
			playHex(Player A, Player B);
		}

		playerRankings() // Some sort of ranking system to feed into genetic algorithm
	}
}
