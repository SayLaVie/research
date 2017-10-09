 /**********************
Hex Research Project

author: Michael McCarver
advisor: Dr. Rob LeGrand
**********************/

#include "hexWorld.h"
#include <string>

vector<vector<vector<double> > > neuralNetFileParser(ifstream &fin);

int main(int argc, char *argv[])
{
	// If no arguments provided, simply printout hexAgent to file
	if (argc != 2)
	{
		ofstream fout;
		fout.open("test.out");

		// It's overkill to create a whole population, but it is less code for right now
		hexWorld population(NUM_PLAYERS);
		population.nextGeneration();
		population.getHexGamePlayer(0).printWeights(fout);

		fout.close();
	}
	else
	{
		ifstream fin;
		ofstream fout;
		vector<vector<vector<double> > > neuralNetWeights;

		fin.open(argv[1]);
		fout.open("compare.out");

		if (!fin.is_open())
		{
			cout << "File " << argv[1] << " could not be opened." << endl;
		}
		// Parse file and create set of neural net weights
		else
		{
			neuralNetWeights = neuralNetFileParser(fin);
		}

		hexGamePlayer test(neuralNetWeights);

		test.printWeights(fout);

		fin.close();
		fout.close();
	}
}

// Takes ifstream and parses nerualNetWeights 3-D vector from file.
vector<vector<vector<double> > > neuralNetFileParser(ifstream &fin)
{
	char symbol;
	double weight;
	string line, token, delim;
	size_t pos;
	vector<double> singleInputNodeVector;
	vector<vector<double> > vectorOfInputNodes;
	vector<vector<vector<double> > > vectorOfLayers;

	delim = ",";

	while (fin >> symbol)
	{
		if (symbol == '&' || symbol == '%')
		{
			if (vectorOfInputNodes.size() == 0)
			{
				// This is the beginning of the file, and there's nothing to push back
				vectorOfLayers.clear();
				vectorOfInputNodes.clear();
			}
			else
			{
				vectorOfLayers.push_back(vectorOfInputNodes);
				vectorOfInputNodes.clear();
			}
		}
		else if (symbol == '$')
		{
			// The rest of the line should be double values seperated by comas
			getline(fin, line);
			singleInputNodeVector.clear();

			while (line.length() > 0)
			{
				pos = line.find(delim);
				token = line.substr(0, pos);
				
				weight = atof(token.c_str());
				singleInputNodeVector.push_back(weight);

				line.erase(0, pos + 1);
			}

			vectorOfInputNodes.push_back(singleInputNodeVector);
		}
		else
		{
			cout << "Invalid symbol found in file" << endl;
			exit(0);
		}
	}

	return vectorOfLayers;
}
