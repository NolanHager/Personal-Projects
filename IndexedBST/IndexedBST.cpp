//NAME:Nolan Hager 
//Date: 12/2/2010
//Program: THis is a Program that shows how a
//			Indexed Binary Search Tree is used and 
//			Implemented.


#include "IndexedBST.h"
#include <string>
#include <iostream>
using namespace std;

namespace bstNS {
	//---------------------------------------------------------------------------------------------------
	// Class: KeyedField
	// This class stores the search field name to be used by the tree operations.
	//---------------------------------------------------------------------------------------------------

	//THis sets the Static variable of teh KeyedField class to a 
	//default variable so that it can be used
	// ------------------------DONE---------------------------------	
	string KeyedField::name = "nothing";

	//---------------------------------------------------------------------------------------------------
	// Class: MailRoutingInfo
	// This class stores mail routing information.
	//---------------------------------------------------------------------------------------------------

		// Sets the fields to the supplied parameters.
		// ------------------------DONE---------------------------------
		MailRoutingInfo::MailRoutingInfo(const string& firstName, const string& lastName, const string& street,
			const string& city, const string& state, const string& zipcode):firstName(firstName),lastName(lastName),
			street(street),city(city),state(state),zipcode(zipcode)
		{}
		
	 	// Returns the value in the current key. The name field of the KeyedItem class identifies the key.
		// ------------------------DONE---------------------------------
		string MailRoutingInfo::GetKeyValue() const
		{
			string success;

			if(KeyedField::name == "firstName")
				success = firstName;
			if(KeyedField::name == "lastName")
				success = lastName;
			if(KeyedField::name == "street")
				success = street;
			if(KeyedField::name == "city")
				success = city;
			if(KeyedField::name == "state")
				success =state;
			if(KeyedField::name == "zipcode")
				success = zipcode;

			return success;
		}
		
		// Returns true if the current object is less than the parameter object, based on the key value.
		// Returns false otherwise.
		// ------------------------DONE---------------------------------
		bool MailRoutingInfo::operator <(const MailRoutingInfo& rhs) const{

			return GetKeyValue() < rhs.GetKeyValue();
		}
		
		// Returns true if the current object is greater than or equal to the paremeter object, based on the 
		// key value. Returns false otherwise.
		// ------------------------DONE---------------------------------
		bool MailRoutingInfo::operator >=(const MailRoutingInfo& rhs) const
		{
			return(GetKeyValue() >= rhs.GetKeyValue());
		}
		
		// Displays to the screen the record, comma delimited on one line.
		// ------------------------DONE---------------------------------
		ostream& operator <<(ostream& out, const MailRoutingInfo& rhs)
		{
			out << rhs.firstName << " "<< rhs.lastName<<", "<<rhs.street<<", "<<rhs.city
				<<", "<<rhs.state <<", "<<rhs.zipcode;
			return(out);
		}
	//---------------------------------------------------------------------------------------------------
	// Class: TreeNode
	// This class represents a tree node.
	//---------------------------------------------------------------------------------------------------
		// ------------------------DONE---------------------------------
		TreeNode::TreeNode(const TreeItemType& item, TreeNode* left, TreeNode* right):
						item(item),left(left),right(right)
		{}
	//---------------------------------------------------------------------------------------------------
	// Class: IndexedBST
	// This class represents a binary search.
	//--------------------------------------------------------------------------------------------------

						 
		// Inserts the item into the tree.
		//------------------Done--------------------------------------------------------------
		void IndexedBST::InsertNode(TreeNode*& pTree, const TreeItemType& item){

			//checking to see if this is the first item to be inserted into
			//the tree
			if(pTree == NULL)
				pTree = new TreeNode(item, NULL,NULL);
			else if(item < pTree->item)
				InsertNode(pTree->left,item);
			else 
				InsertNode(pTree->right,item);
		}
			
		// Finds the node containing the keyFieldValue or gives error message if value not found.
		// Once found calls RemoveNode to remove it.
		//----------------DONE---------------------------------------------------------------
		void IndexedBST::Remove(TreeNode*& pTree, const string& keyFieldValue){

			//Checks to see if Item is Found or not. If not then gives and error
			// message.
			if(pTree == NULL)
				cerr << "Item is Not Found";
			else if(keyFieldValue == pTree->item.GetKeyValue())
				RemoveNode(pTree);

			else if(keyFieldValue<pTree->item.GetKeyValue())
				Remove(pTree->left,keyFieldValue);

			else if(keyFieldValue >= pTree->item.GetKeyValue())
				Remove(pTree->right,keyFieldValue);
		}
		
		// Removes the actual node from the tree.
		//------------------Done---------------------------------
		void IndexedBST::RemoveNode(TreeNode*& pTree){
			TreeItemType ReplacementItem;
			TreeNode* cur;
			//removing a node in the tree
			if(pTree->left == NULL && pTree->right == NULL)
			{
				pTree->left = pTree->right = NULL;
				delete pTree;
			}
			else if(pTree->left == NULL){
				cur = pTree;
				pTree = pTree->right;
				cur->right = NULL;
				delete cur;
			}
			else if(pTree->right == NULL)
			{
				cur = pTree;
				pTree = pTree->left;
				cur->left = NULL;
				delete cur;
			}
			else
			{
				ProcessLeftMost(pTree->right,ReplacementItem);
				pTree->item = ReplacementItem;
			}
			
		}
		
		// Finds the left most node on the right subtree to replace the node to be removed.
		void IndexedBST::ProcessLeftMost(TreeNode*& pTree, TreeItemType& replacementItem){ 
			if(pTree->left == NULL)
			{
				replacementItem = pTree->item;
				TreeNode* cur = pTree;
				pTree = pTree->right;
				cur->right = NULL;
				delete cur;
			}
			else
			{
				ProcessLeftMost(pTree->left,replacementItem);
			}
		}
		
		// Performs an inorder traversal and executes the function visit upon root visitation.
		//------------------Done--------------------------------------------------------------
		void IndexedBST::Inorder(TreeNode* pTree, FunctionType visit){

			if(pTree->left != NULL)
				Inorder(pTree->left,visit);

			visit(pTree->item);

			if(pTree->right !=NULL)
				Inorder(pTree->right,visit);
		}
		
		// Destroys the bst tree.
		//------------------Done--------------------------------------------------------------
		void IndexedBST::DestroyTree(TreeNode*& pTree){


			if(pTree !=NULL){
				DestroyTree(pTree->left);
				DestroyTree(pTree->right);
				pTree->left = pTree->right = NULL;
				delete pTree;
			}
		}
		
		// Performs a preorder copy of the current tree to create a new one.
		//------------------DONE----------------------------------------------
		void IndexedBST::CopyTree(TreeNode* pFromTree, TreeNode*& pToTree){
			if(pFromTree != NULL){
				pToTree = new TreeNode(pFromTree->item,NULL,NULL);
				CopyTree(pFromTree->left,pToTree->left);
				CopyTree(pFromTree->right,pToTree->right);
			}
			else
			{
				pToTree = NULL;
			}
		}
		
		// Performs a preorder creation of a new tree based on the current key field.
		//-----------------------DONE-----------------------------------------------------
		void IndexedBST::IndexBy(TreeNode* pOldTree, TreeNode*& pNewTree){
			if(pOldTree != NULL)
				InsertNode(pNewTree,pOldTree->item);
			if(pOldTree ->left != NULL)
				IndexBy(pOldTree->left,pNewTree);
			if(pOldTree->right!=NULL)
				IndexBy(pOldTree->right,pNewTree);
		}

		// Sets the root to NULL and the key to keyFieldName;
		//-------------------------done---------------------------------
		IndexedBST::IndexedBST(const string& keyFieldName){
			KeyedField::name = keyFieldName;
			root = NULL;
		}
		
		// Calls CopyTree() to make a copy of the current tree.
		// this will call the CopyTree() method which copies 
		// by preorder.
		//--------------------done---------------------------------
		IndexedBST::IndexedBST(const IndexedBST& tree){
			CopyTree(tree.root, root);
		}
		
		// Calls DestroyTree() to destroy the current tree.
		// The tree is destroyed by post order.
		//------------------done-----------------------------
		IndexedBST::~IndexedBST(){
			DestroyTree(root);
		}
		
		// Calls private IndexBy() to create a new tree based on the new key. Deletes the old tree
		// and reassigns the root pointer to the new tree.
		// ------------------------DONE---------------------------------
		void IndexedBST::IndexBy(const string& keyFieldName){
			KeyedField::name = keyFieldName; 
			TreeNode* pNewTree = NULL;
			IndexBy(root,pNewTree);
			DestroyTree(root);
			root = pNewTree;
		}
		
		// Calls InsertNode() to insert the item into the tree.
		// ------------------------DONE---------------------------------
		void IndexedBST::Insert(const TreeItemType& item){
			InsertNode(root,item);
		}
		
		// Calls the private Remove() to remove the node containing the keyFieldValue.
		// ------------------------DONE---------------------------------
		void IndexedBST::Remove(const string& keyFieldValue){
			Remove(root,keyFieldValue);
		}
		
		// Calls Inorder() to perform an inorder traversal of the tree.
		// ------------------------DONE---------------------------------
		void IndexedBST::InorderTraversal(FunctionType visit){
			Inorder(root, visit);   
		}
}// end namespace
