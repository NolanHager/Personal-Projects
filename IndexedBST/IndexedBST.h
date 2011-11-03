#ifndef INDEXED_BST_H
#define INDEXED_BST_H

#include <string>
#include <iostream>
using namespace std;

namespace bstNS {	
	//---------------------------------------------------------------------------------------------------
	// Class: KeyedField
	// This class stores the search field name to be used by the tree operations.
	//---------------------------------------------------------------------------------------------------
	class KeyedField
	{
	public:
		static string name;
	};

	//---------------------------------------------------------------------------------------------------
	// Class: MailRoutingInfo
	// This class stores mail routing information.
	//---------------------------------------------------------------------------------------------------
	class MailRoutingInfo
	{
	private:
		string firstName;
		string lastName;
		string street;
		string city;
		string state;
		string zipcode;

	public:
		// Sets the fields to the supplied parameters.
		MailRoutingInfo(const string& firstName = "", const string& lastName = "", const string& street = "",
			            const string& city = "", const string& state = "", const string& zipcode = "");
		
		// Returns the value in the current key. The name field of the KeyedItem class identifies the key.
		string GetKeyValue() const;
		
		// Returns true if the current object is less than the parameter object, based on the key value.
		// Returns false otherwise.
		bool operator <(const MailRoutingInfo& rhs) const;
		
		// Returns true if the current object is greater than or equal to the paremeter object, based on the 
		// key value. Returns false otherwise.
		bool operator >=(const MailRoutingInfo& rhs) const;
		
		// Displays to the screen the record, comma delimited on one line.
		friend ostream& operator <<(ostream& out, const MailRoutingInfo& rhs);
	};

	typedef MailRoutingInfo TreeItemType;
	typedef void (*FunctionType)(const TreeItemType& item);

	//---------------------------------------------------------------------------------------------------
	// Class: TreeNode
	// This class represents a tree node.
	//---------------------------------------------------------------------------------------------------
	class TreeNode
	{
	private:
		friend class IndexedBST;

		TreeItemType item;
		TreeNode* left;
		TreeNode* right;

	public:
		// Sets the fields to the supplied parameters.
		TreeNode(const TreeItemType& item, TreeNode* left, TreeNode* right);
	};

	//---------------------------------------------------------------------------------------------------
	// Class: IndexedBST
	// This class represents a binary search.
	//---------------------------------------------------------------------------------------------------
	class IndexedBST
	{
	private:
		TreeNode* root;

		// Inserts the item into the tree.
		void InsertNode(TreeNode*& pTree, const TreeItemType& item);
			
		// Finds the node containing the keyFieldValue or gives error message if value not found.
		// Once found calls RemoveNode to remove it.
		void Remove(TreeNode*& pTree, const string& keyFieldValue);
		
		// Removes the actual node from the tree.
		void RemoveNode(TreeNode*& pTree);
		
		// Finds the left most node on the right subtree to replace the node to be removed.
		void ProcessLeftMost(TreeNode*& pTree, TreeItemType& replacementItem);
		
		// Performs an inorder traversal and executes the function visit upon root visitation.
		void Inorder(TreeNode* pTree, FunctionType visit);
		
		// Destroys the bst tree.
		void DestroyTree(TreeNode*& pTree);
		
		// Performs a preorder copy of the current tree to create a new one.
		void CopyTree(TreeNode* pFromTree, TreeNode*& pToTree);
		
		// Performs a preorder creation of a new tree based on the current key field.
		void IndexBy(TreeNode* pOldTree, TreeNode*& pNewTree);

	public:
		// Sets the root to NULL and the key to keyFieldName;
		IndexedBST(const string& keyFieldName);
		
		// Calls CopyTree() to make a copy of the current tree.
		IndexedBST(const IndexedBST& tree);
		
		// Calls DestroyTree() to destroy the current tree.
		~IndexedBST();
		
		// Calls private IndexBy() to create a new tree based on the new key. Deletes the old tree
		// and reassigns the root pointer to the new tree.
		void IndexBy(const string& keyFieldName);
		
		// Calls InsertNode() to insert the item into the tree.
		void Insert(const TreeItemType& item);
		
		// Calls the private Remove() to remove the node containing the keyFieldValue.
		void Remove(const string& keyFieldValue);
		
		// Calls Inorder() to perform an inorder traversal of the tree.
		void InorderTraversal(FunctionType visit);
	};// end IndexedBST
}// end namespace bstNS
#endif