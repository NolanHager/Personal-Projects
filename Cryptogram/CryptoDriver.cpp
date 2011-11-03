#include "cryptogram.h"
#include <iostream>
using namespace std;

int main()
{
	string puzzle[] = {"THIS IS JUST TOO MUCH FUN!",
					    "DON'T YOU THINK SO?"};


	char replaceThisChar;
	char withThisChar;

	
	Cryptogram cryptoPuzzle(puzzle, 2);
	
	cout << "Enter a $ to quit."
		 << endl
		 << endl;
	do
	{
		cout << endl << endl;
		cryptoPuzzle.DisplayPuzzle();

		cout << "Replace char: ";
		cin >> replaceThisChar;

		if (replaceThisChar != '$')
		{
			cout << "With: ";
			cin >> withThisChar;

			cryptoPuzzle.Replace(replaceThisChar, withThisChar);
		}// end if
	}while(replaceThisChar != '$');

	return (0);
}// end main()