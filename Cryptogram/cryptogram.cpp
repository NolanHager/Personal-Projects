//Name: Nolan Hager
//Description: This simulates a crypotgram which is a game where
//				one trys to find out what the cypher for a message is


#include "cryptogram.h"
#include <iostream>
#include <iomanip>


using namespace std;

	// Method creates a new CharPositionNode and populates it with the
	// given parameters.
	//-------------------------DONE---------------------------------------
	Cryptogram::CharPositionPointer Cryptogram::CreateNewCharPositionNode(const int charRow,
												  const int charCol,
												  CharPositionPointer next){
		CharPositionPointer Temp =new CharPositionNode(charRow, charCol,next);	
		return(Temp);
	}

	// This utility method performs a deep copy of the copyObject. It is
	// used by both the copy constructor and the =() operator.
	//-------------------------DONE---------------------------------------

	void Cryptogram::CopyArray(const Cryptogram& copyObject)
	{
		CharPositionPointer copyPointer = NULL;
		CharPositionPointer copyTOO = NULL;

		for(int i = 0; i < MAX_CHAR_ROWS; i++)
		{
			charMap[i].charEncrypted = copyObject.charMap[i].charEncrypted;
			charMap[i].charGuess = copyObject.charMap[i].charGuess;
			charMap[i].charSolution = copyObject.charMap[i].charSolution;
			if(copyObject.charMap[i].head != NULL)
			{
				copyPointer = copyObject.charMap[i].head;
				charMap[i].head = new CharPositionNode(copyPointer->charRow,copyPointer->charCol,NULL);

				copyPointer = copyPointer->next;
				copyTOO = charMap[i].head;

				while(copyPointer != NULL)
				{
					copyTOO = copyTOO->next = new CharPositionNode(copyPointer->charRow,copyPointer->charCol,NULL);
					copyPointer = copyPointer ->next;
				}

			}
		}
	}

	// This utility method performs a total memory cleanup. It is used by
	// both the destructor and =() operator. It basically deletes all the
	// nodes and sets the head to NULL for each linked list released.
	//-------------------------DONE---------------------------------------
	void Cryptogram::DeleteArray()
	{
		CharPositionPointer delPointer = NULL;

		for(int i = 0; i < MAX_CHAR_ROWS ; i++)
		{
			while(charMap[i].head != NULL)
			{
				delPointer = charMap[i].head;
				charMap[i].head = delPointer->next;
				delPointer->next = NULL;
				delete delPointer;
				delPointer = NULL;
			}
		}
	}

	// Method encrypts each character using the following algorithm.
	// First it creates a char array of 26 elements, 'A' through 'Z'. This
	// array is then shuffled 5 times, so the characters are no longer in
	// alphabetical order. Then it assigns each character in succession to
	// each encrypted character in the charMap array. That way each
	// solution character is essentially mapped to a random encrypted character.
	//
	// After the charMap has been updated, it initializes the puzzleEncrypted array
	// so that the encrypted puzzle can be displayed when needed.
	//-------------------------DONE---------------------------------------

	void Cryptogram::EncryptPuzzle()
	{
		char Encryptkey[MAX_CHAR_ROWS];

		//loads the array with A through Z
		for(int i = 0; i < MAX_CHAR_ROWS; i++ )
		{
			Encryptkey[i] = i + 65;
		}

		//Shuffle Array
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < MAX_CHAR_ROWS; j++)
			{
				char temp;
				int pos = rand() % 26;
				 temp = Encryptkey[i];
				 Encryptkey[i] = Encryptkey[pos];
				 Encryptkey[pos] = temp;
			}
		}

		// adding the encrypt
		for(int i = 0; i < MAX_CHAR_ROWS; i++)
		{
			charMap[i].charEncrypted = Encryptkey[i];
		}
	}

	// Method updates the puzzleGuessed array by traversing the charMap[]
	// array and updating each affected character. For each character 
	// position in the charMap array, the corresponding char in the 
	// puzzleGuessed array is updated with the charGuessed value. This method
	// gets called initially by the constructor and each time a character is 
	// replaced.
	//-------------------------DONE---------------------------------------
	void Cryptogram::UpdatePuzzleAfterGuess()
	{
		CharPositionPointer Rep =  NULL;

		for(int i = 0; i < MAX_CHAR_ROWS; i++)
		{
			if(charMap[i].head != NULL)
			{
				Rep = charMap[i].head;
				puzzleGuessed[Rep->charRow][Rep->charCol] = charMap[i].charGuess;
				Rep = Rep ->next;
				while(Rep != NULL)
				{
					puzzleGuessed[Rep->charRow][Rep->charCol] = charMap[i].charGuess;
					Rep = Rep ->next;
				}//end while
			}//end if
		}//end for
	}

	// Constructor initializes the charMap array from the given message and
	// sets the numberOfPuzzleRows to the given parameter value. In addition,
	// both puzzle arrays are build to accommodate the message.
	// 
	// The charMap array declaration above, will have already initialized each
	// CharMapNode element to its default value, so this method
	// creates the linked lists for the characters used in the puzzle and 
	// sets the charSolution for each list.
	// 
	// Each character in the message is visited one at a time, and its
	// position added to one of the lists. If the character has been
	// previously added, then the character's position is added to the
	// front of its list and the charSolution for that element is initialized. 
	// If the character has not been added before, then its position is added 
	// to a newly created link list. If the character is a punctuation or 
	// special, i.e. non uppercase alpha, then it is simply written to the 
	// puzzleEncrypted and puzzleGuessed arrays as is.
	// 
	// After the character is accounted for in the charMap[] array, the
	// two puzzle arrays are build up by concatenating the string "-" to each
	// of the strings. The puzzle arrays have effectively empty strings to
	// begin with, so by concatenating the "_" you will have a string that has
	// the same number of characters as the message. Thus, later on in this 
	// method, the appropriate char can be updated with a special character or
	// a puncuation character.
	//
	// At this stage, the charEncrypted and the charGuess are left as is 
	// for now. The encrypted char will be filled by the EncryptPuzzle()
	// method, while the guess char will change once the user starts solving 
	// the puzzle and UpdatePuzzleAfterGuess() is called.
	//
	// To find the proper array element faster, a hash algorithm is used,
	// instead of a linear search. The hash simply takes a character
	// from the message and subtracts the character 'A' from it, giving
	// the index into the array. For example, if the character is a 'Z',
	// then 'Z' - 'A' = 90 - 65 = 25, which is the last array element,
	// the one representing the character Z. For this to work properly,
	// all characters must be uppercase, and that is an assumption we can
	// make.
	//
	// Once the charMap array has been initialized, encrypt the message by 
	// calling the method EncryptPuzzle().
	//-------------------------DONE--------------------------------------
	Cryptogram::Cryptogram(const string message[], const int numberOfRows)
	{
		// set number of puzzle rows
		numberOfPuzzleRows = numberOfRows;

		//Populate the Solution Letters in charMap[]
		for(int i = 0; i< MAX_CHAR_ROWS; i++)
		{
			charMap[i].charSolution = i + 65;
		}
		 
		//Populate the encrypted letters in charMap[]
		EncryptPuzzle();

		//Populate the puzzleEncrypted array
		for(int i =0; i < numberOfPuzzleRows; i++)
		{
			for(int j = 0; j < message[i].length(); j++)
			{
				if( isalpha(message[i][j]) )
				{ 
					puzzleEncrypted[i] += charMap[message[i][j] - 65].charEncrypted;
				}
				else
				{
					puzzleEncrypted[i] +=message[i][j];
				}
			}
		}
		// end puzzleEncrypted

		//Populate the puzzleGuessed
		for(int i =0; i < numberOfPuzzleRows; i++)
		{
			for(int j = 0; j < message[i].length(); j++)
			{
				if(  isalpha(message[i][j]) )
				{ 
					puzzleGuessed[i] += "_";
				}
				else
				{
					puzzleGuessed[i] += message[i][j];
				}
			}
		}
		// end the puzzleGuessed 

		//Record the Positions of the Letters 
		//in the Message[]
		for(int i = 0; i < numberOfPuzzleRows; i++)
		{
			for(int j = 0; j < puzzleEncrypted[i].length(); j++)
			{
				if(isalpha(puzzleEncrypted[i][j]))
				{
				charMap[puzzleEncrypted[i][j] - 65].head = new CharPositionNode(i,j,charMap[puzzleEncrypted[i][j] - 65].head);
				}
			}
		}
		//end record

	}

	//-------------------------DONE--------------------------------------
	Cryptogram::Cryptogram(const Cryptogram& copyObject)
	{
		DeleteArray();
		CopyArray(copyObject);
	}

	//-------------------------DONE---------------------------------------
	Cryptogram::~Cryptogram()
	{
		DeleteArray();
	}

	//-------------------------DONE---------------------------------------
	Cryptogram Cryptogram::operator = (const Cryptogram& sourceObject)
	{
		if(this != &sourceObject){
			DeleteArray();
			CopyArray(sourceObject);
			return(*this);
		}
		else
		{
			return(*this);
		}
	}
	// Displays the puzzle to the screen. This consists of the encrypted
	// characters along with the guess characters. To make it a bit more
	// readable for the user display the guess characters first for each
	// row, and below that the encrypted characters.
	//-------------------------DONE---------------------------------------
	void Cryptogram::DisplayPuzzle() const
	{
		for(int i = 0; i < numberOfPuzzleRows; i++)
		{
			for(int j = 0; j <= puzzleGuessed[i].length() - 1; j++)
			{
					cout << puzzleGuessed[i].at(j)<<" ";
			}
			cout << endl<<endl;

			for(int t = 0; t <= puzzleEncrypted[i].length()-1; t++)
			{
				cout << puzzleEncrypted[i].at(t) <<" ";
			}

			cout <<endl<<endl<<endl;
		}
	}

	// Replaces each occurence of the encrypted character with the guess
	// character. Now, what actually happens programmatically is that
	// the guess character for each encrypted character matched is updated.
	// For instance, if currently charEncrypted = 'W', charGuess = 'P' and
	// request is made for 'W' to be replaced with 'X', then just the 'P'
	// will change. The encrypted characters are never modified. And that
	// is a good thing.
	// Don't forget to then update the puzzleGuessed, by simply calling
	// the UpdatePuzzleAfterGuess() method.
	//-------------------------DONE---------------------------------------
	void Cryptogram::Replace(const char thisChar, const char withThisChar)
	{
		char UpperthisChar = toupper(thisChar);
		char UpperwithThisChar = toupper(withThisChar);

		charMap[UpperthisChar - 65].charGuess = UpperwithThisChar;
		UpdatePuzzleAfterGuess();
	}
