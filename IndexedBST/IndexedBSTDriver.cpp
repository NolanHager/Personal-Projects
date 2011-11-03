//NAME:Nolan Hager 
//Program: THis is a Program that shows how a
//			Indexed Binary Search Tree is used and 
//			Implemented.


#include "IndexedBST.h"
#include <iostream>

using namespace std;
using namespace bstNS;


void Display(const TreeItemType& item);

int main() {
	IndexedBST bst("zipcode");

	bst.Insert( TreeItemType ("Mary", "Poppins", "12 Rodeo Drive", "Hollywood","CA", "45230") );
	bst.Insert( TreeItemType ("Peter", "Pan", "One Island ave.", "Neverland", "HI", "34902") );
	bst.Insert( TreeItemType ("Donald", "Duck", "22 Quak Rd.", "Duckey", "MN", "22234") );
	bst.Insert( TreeItemType ("Mickey", "Mouse", "10 Walt Lane", "Orlando", "FL", "94500") );
	bst.Insert( TreeItemType ("Captain", "Hook", "30 Floating Route", "Oceanview", "HI", "34903") );

	// List records by zipcode.
	cout << "Listing of records by: ZIPCODE"
		 << endl
		 << "------------------------------"
		 << endl;
	bst.InorderTraversal(Display);
	cout << endl;

	// List records by state.
	cout << "Listing of records by: STATE"
		 << endl
		 << "------------------------------"
		 << endl;
	bst.IndexBy("state");
	bst.InorderTraversal(Display);
	cout << endl;

	// List records by last name.
	cout << "Listing of records by: LASTNAME"
		 << endl
		 << "------------------------------"
		 << endl;
	bst.IndexBy("lastName");
	bst.InorderTraversal(Display);
	cout << endl;

	// Delete a record by state. Should give error.
	cout << "Deleting a record by: STATE"
		 << endl
		 << "------------------------------"
		 << endl;
	bst.Remove("HI");
	cout << endl;

	cout << "Deleting a record by: LASTNAME"
		 << endl
		 << "------------------------------"
		 << endl;
	bst.Remove("Duck");
	cout << endl;

	// List records by last name.
	cout << "Listing of records by: LASTNAME"
		 << endl
		 << "------------------------------"
		 << endl;
	bst.InorderTraversal(Display);
	cout << endl;

	IndexedBST bst2(bst);
	cout << endl;
	cout << "Listing of copied records by: LASTNAME"
		 << endl
		 << "--------------------------------------"
		 << endl;
	bst2.InorderTraversal(Display);

	cout << endl << endl;

	return (0);
}// end main()


void Display(const TreeItemType& item) {
	cout << item << endl;
}// end Display()