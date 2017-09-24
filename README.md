# Research on The Game of Hex
Author: Michael McCarver<br />
Advisor: Dr. Rob LeGrand

/*research description goes here*/
***

### To Do for Next Meeting (parentheticals indicate completed tasks)
- Write multiple mains:
	- Main for testing
	- Main for genetic evolution
	- Main for playing two evolved agents against one another
	- Main for playing human vs agent
- Write Makefile
- Change the way we are handling evolution/mutation
	- Mutation always occurs, but according to a normal distribution centered on the original gene-weight
- Consider changing the way we evaluate board state
	- Use two values; one is 0/1 depending on whether we own the tile, the other is 0/1 whether the opponent owns the tile
		- This allows the neural net to find different importance in whether we own a tile or opponent owns a tile
- Go through code and ensure consistent use of keywords
	- e.g. 'layer' vs 'column'
- (Change the geometric structure of the hexWorld)
	- (Change getNeighbor's method of wrapping around)
- Plan out how to print information out during execution
- Save progress of genetic evolution at certain intervals

### Notes/Reminders
- maybe create new directory to contain different net topologies (specific maps could be chosen at compilation, or at beginning 	of execution via stdin prompt)
- devise hardness scale
- Save/Resume function
   - via command line arguments

