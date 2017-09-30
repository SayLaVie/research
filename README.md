# Research on The Game of Hex
Author: Michael McCarver<br />
Advisor: Dr. Rob LeGrand

/*research description goes here*/
***

### To Do for Next Meeting (parentheticals indicate completed tasks)
- Translate boards accross diagonal
- Change initial standard deviation
- Simplify shape of neural network
	- Single node
- Lower depth to 3
- Code more main functions
   - Main for genetic evolution
	- Human playable
	- Play two previously evolved agents against one another
- Write a Makefile
	- Multiple targets
- Save state of evolution at certain points
   - Every 10 iterations??
- New way to evaluate board state
	- Use two values; one is 0/1 depending on whether we own the tile, the other is 0/1 whether the opponent owns the tile
		- This allows the neural net to find different importance in whether we own a tile or opponent owns a tile
- Plan out how to print information out during execution

### Notes/Reminders
- maybe create new directory to contain different net topologies (specific maps could be chosen at compilation, or at beginning of execution via stdin prompt)
- devise hardness scale
- Save/Resume function
   - via command line arguments
