#ifndef CRYPTOGRAM_H
#define CRYPTOGRAM_H

#include <string>
using namespace std;



//-------------------------------------------------------------------
// Class: Cryptogram
//
// This class uses an array of linked lists to represent a cryptogram 
// puzzle. The array stores the 26 possible characters that can appear
// in the puzzle, although not all elements will be used. Each element
// of this array is of type CharMapNode, a structure that holds
// relevant character mappings and a pointer to a link list. The link
// list links all the positions within the puzzle where the character
// in question can be found. Each node is of type CharPositionNode, a
// structure that holds position information of a character's location
// within the puzzle.
//
// Two arrays will also be used to hold the puzzle. The first is
// a char array that holds the encrypted puzzle, and the other is a
// char array that holds the current state of the puzzle, reflecting
// all the user's guessed.
//-------------------------------------------------------------------
class Cryptogram
{
private:
	struct CharPositionNode;
	typedef CharPositionNode* CharPositionPointer;

	// These consts must be static (class level), since they must exist
	// before the arrays charMap and puzzle that use them are created,
	// which happens when an instance is created.
	static const int MAX_CHAR_ROWS = 26;
	static const int MAX_PUZZLE_ROWS = 20;
	static const int MAX_CHARS_PER_ROW = 80;

	
	//-------------------------------------------------------------------
	// Struct: CharMapNode
	//
	// This structure holds the solution character, encrypted character,
	// guess character and pointer to the linked list of positions.
	//-------------------------------------------------------------------
	struct CharMapNode
	{
		char charSolution;				// unencryted char
		char charEncrypted;				// encrypted char
		char charGuess;					// user's guess to unencrypted char
		CharPositionPointer head;

		// Constructor initializes the members to default values if none
		// are provided.
		CharMapNode(const char charSolution = '_',
			        const char charEncrypted = '_',
				    const char charGuess = '_',
					CharPositionPointer head = NULL)
			: charSolution(charSolution), charEncrypted(charEncrypted),
			  charGuess(charGuess), head(head)
		{}// end CharMapNode()

	}; // end CharMapNode



	//-------------------------------------------------------------------
	// Struct: CharPositionNode
	//
	// This structure holds the row and col number of the character in
	// the puzzle, as well as a link to the next node.
	//-------------------------------------------------------------------
	struct CharPositionNode
	{
		int charRow;
		int charCol;
		CharPositionPointer next;

		// Constructor initializes the members to default values if
		// none are provided.
		CharPositionNode(const int charRow = 0,
			             const int charCol = 0,
						 CharPositionPointer next = NULL)
			: charRow(charRow), charCol(charCol), next(next)
		{}// end CharPostionNode()

	}; // end CharPositionNode


	CharMapNode charMap[MAX_CHAR_ROWS];			// array of linked lists

	// This array holds the encrypted puzzle.
	string puzzleEncrypted[MAX_PUZZLE_ROWS];

	// This array holds the current state of the puzzle.
	string puzzleGuessed[MAX_PUZZLE_ROWS];

	int numberOfPuzzleRows;


	// Method creates a new CharPositionNode and populates it with the
	// given parameters.
	CharPositionPointer CreateNewCharPositionNode(const int charRow,
												  const int charCol,
												  CharPositionPointer next);

	// This utility method performs a deep copy of the copyObject. It is
	// used by both the copy constructor and the =() operator.
	void CopyArray(const Cryptogram& copyObject);


	// This utility method performs a total memory cleanup. It is used by
	// both the destructor and =() operator. It basically deletes all the
	// nodes and sets the head to NULL for each linked list released.
	void DeleteArray();


	// Method encrypts each character using the following algorithm.
	// First it creates a char array of 26 elements, 'A' through 'Z'. This
	// array is then shuffled 5 times, so the characters are no longer in
	// alphabetical order. Then it assigns each character in succession to
	// each encrypted character in the charMap array. That way each
	// solution character is essentially mapped to a random encrypted character.
	//
	// After the charMap has been updated, it initializes the puzzleEncrypted array
	// so that the encrypted puzzle can be displayed when needed.
	void EncryptPuzzle();


	// Method updates the puzzleGuessed array by traversing the charMap[]
	// array and updating each affected character. For each character 
	// position in the charMap array, the corresponding char in the 
	// puzzleGuessed array is updated with the charGuessed value. This method
	// gets called initially by the constructor and each time a character is 
	// replaced.
	void UpdatePuzzleAfterGuess();

public:
	// Constructor initializes the charMap array from the given message and
	// sets the numberOfPuzzleRows to the given parameter value. In addition,
	// both puzzle arrays are build to accommodate the message.
	Cryptogram(const string message[], const int numberOfRows);


	// Copy constructor makes a copy of the puzzle array. Calls utility
	// method for this.
	Cryptogram(const Cryptogram& copyObject);


	// Destructor must delete all the nodes that have been allocated.
	// No bread crumbs this time, all memory must be released. Calls
	// utility method for this.
	~Cryptogram();


	// Overloaded assignment makes a copy of the source obect. It first
	// checks for self assignment, i.e. obj1 = obj1;
	// It then deletes the existing array and copies the source. Calls
	// utility methods to accomplish this.
	Cryptogram operator = (const Cryptogram& sourceObject);


	// Displays the puzzle to the screen. This consists of the encrypted
	// characters along with the guess characters. To make it a bit more
	// readable for the user display the guess characters first for each
	// row, and below that the encrypted characters.
	void DisplayPuzzle() const;


	// Replaces each occurence of the encrypted character with the guess
	// character.
	void Replace(const char thisChar, const char withThisChar);

};

#endif