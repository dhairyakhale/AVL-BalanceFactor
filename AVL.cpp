#include<bits/stdc++.h>
#include<fstream>

using namespace std;

//Used to create a node for AVL (self-balancing) BST
class AVL_Node
{
	private:

		int key;
		int bf;		//Balance factor = height(left subtree) - height(right subtree)
					//However, we aren't allowed to calculate it

		AVL_Node *LChild, *RChild;

		//Initializing AVL node with key k
		AVL_Node(int k)
		{
		 	key = k;
		 	LChild = NULL;
		 	RChild = NULL;
		 	bf = 0;
		}

		//Initializing empty AVL node
		AVL_Node()
		{
		 	key = 0;
		 	LChild = NULL;
		 	RChild = NULL;
		 	bf = 0;
		}

		//Destructor to free the node
		~AVL_Node()
		{
			delete LChild;
			delete RChild;
		}

		//Associating AVL_Tree class to this class
	friend class AVL_Tree;
};

//Used to create tree using AVL nodes defined above
//Also contains functions and operations on tree
class AVL_Tree
{

	private:

		AVL_Node* head;		//Pointer to dummy node. head->RChild = root of the tree

	public:

		//Initializing an AVL tree
		AVL_Tree()
		{
			head = new AVL_Node();	//Dummy node created
			head->LChild = NULL;
			head->RChild = NULL;
		}
		
		//AVL_Tree(const AVL_Tree &T);
		
		//AVL_Tree &operator=(const AVL_Tree &T);

		//Resulting from L-L or R-R imbalance
		void singleRotation(int a, AVL_Node **p, AVL_Node **r, AVL_Node **s)
		{
			(*p) = (*r);	//p will be the root node of the subtree resulting from rotation

			//L-L imbalance, results Right rotation
			if(a == 1)
			{
				(*s)->LChild = (*r)->RChild;
				(*r)->RChild = (*s);
			}

			//R-R imbalance, results Left rotation
			else if (a == -1)
			{
				(*s)->RChild = (*r)->LChild;
				(*r)->LChild = (*s);
			}

			//Balance factors of left and right nodes after rotation is zero
			(*r)->bf = 0;
			(*s)->bf = 0;
		}

		//Resulting from R-L or L-R imbalance
		void doubleRotation(int a, AVL_Node **p, AVL_Node **r, AVL_Node **s)
		{
			//p will be the root node of the subtree resulting from rotation

			//L-R imbalance, results Left-Right rotation
			if(a == 1)
			{
				(*p) = (*r)->RChild;
				(*r)->RChild = (*p)->LChild;
				(*p)->LChild = (*r);
				(*s)->LChild = (*p)->RChild;
				(*p)->RChild = (*s);
			}

			//R-L imbalance, results Right-Left rotation
			else if (a == -1)
			{
				(*p) = (*r)->LChild;
				(*r)->LChild = (*p)->RChild;
				(*p)->RChild = (*r);
				(*s)->RChild = (*p)->LChild;
				(*p)->LChild = (*s);
			}

			//Adjustment of balance factors

			if((*p)->bf == a)	//If root has bf=a
			{
				(*s)->bf = -1*a;
				(*r)->bf = 0;
			}
			else if((*p)->bf == 0)	//If root has bf=0
			{
				//Both its children will also be balanced
				(*s)->bf = 0;
				(*r)->bf = 0;
			}
			else if((*p)->bf == (-1*a))		//If root has bf = -a
			{
				(*s)->bf = 0;
				(*r)->bf = a;
			}

			(*p)->bf = 0;
		}
		
		//Inserting a new node into AVL tree with key of node as k
		void AVL_Insert(int k)
		{
			AVL_Node* t = head;				//Parent of s
			AVL_Node* s = head->RChild;		//Point of imbalance
			AVL_Node* p = head->RChild;		//Node which will travel through tree, also parent of q

			AVL_Node *q = NULL, *r = NULL;	//q will be location of insertion, r will be relevant child of point of imbalance

			AVL_Node* temp = new AVL_Node(k);	//temp = new AVL node which is to be inserted
			
			//Step 1: Insertion of node

			if(p==NULL)						//If tree is empty
			{
				head->RChild = temp;		//New node becomes root
				return;
			}

			else
			{
				while(1)
				{
					//Traversal into left subtree
					if(k < p->key)
					{
						q = p->LChild;

						if(!q)	//If location to insert is reached
						{
							q = temp;
							p->LChild = q;
							break;
						}

						else
						{
							if(q->bf)		//If balance factor of q is non-zero, it becomes possible point of imbalance
							{
								t = p;		
								s = q;
							}
							p = q;
						}
					}

					//Traversal into right subtree
					else if(k > p->key)
					{
						q = p->RChild;

						if(!q)
						{
							q = temp;
							p->RChild = q;
							break;
						}

						else
						{
							if(q->bf)
							{
								t = p;
								s = q;
							}
							p = q;
						}
					}

					//Element already present
					else
					{
						delete temp;
						cout<<k<<" already present."<<endl;
						return;
					}
				}

				//Step 2: Adjustment of balance factors

				int a;

				if(k < s->key)		//If new element inserted in LST
					a = 1;
				else
					a = -1;			//If new element inserted in RST


					//Changing bf from below the point of imbalance to inserted node
				if(a == 1)
					p = s->LChild;
				else
					p = s->RChild;

				r = p;	//r will be the child of s in subtree of inserted node


					//For each p, if new element is in its LST, bf = 1. If RST, bf = -1
				while(p != q)
				{
					if(k < p->key)
					{
						p->bf = 1;
						p = p->LChild;
					}

					else if(k > p->key)
					{
						p->bf = -1;
						p = p->RChild;
					}
					else break;
				}

				//Step 3: Rotation

					//If bf of point of imbalance was 0, it will become a
					//This is due to new insertion into either its LST or RST
				if(s->bf == 0)
				{
					s->bf = a;
					return;
				}

					//If bf of point of imbalance was -a, it will become 0
					//This is due to new insertion now balancing the tree
				else if(s->bf == (-1*a))
				{
					s->bf = 0;
					return;
				}

				else if(s->bf == a)
				{
					//L-L or R-R imbalance
					if(r->bf == a)
					{
						singleRotation(a, &p, &r, &s);
					}

					//L-R or R-L imbalance
					else if(r->bf == (-1*a))
					{
						doubleRotation(a, &p, &r, &s);
					}
				}

					//Since s is removed from original location because of rotation
					//p is in old location of s
				if (s == t->RChild)
					t->RChild = p;
				else
					t->LChild = p;
			}
		}

		//Deleting node with no children (leaf)
		//Helper function of AVL_Delete()
		void Del0Child(AVL_Node* q, AVL_Node* par)
		{
			if(q->key < par->key)		//If q is left child of parent
				par->LChild = NULL;

			else						//If q is right child of parent
				par->RChild = NULL;

			delete q;
		}

		//Deleting node with 1 child
		//Helper function of AVL_Delete()
		void Del1Child(AVL_Node* q, AVL_Node* par)
		{

			if(!q->LChild)				//If q has only right child
			{
				//Connecting child of q to parent

				par->bf = q->RChild->bf;	//Copying imbalance of new child to parent

				if(q->key < par->key)			//If q is left child of parent
					par->LChild = q->RChild;
				else							//If q is right child of parent
					par->RChild = q->RChild;

				q->RChild = NULL;	//Detach q
			}

			else	//If q has only left child
			{
				//Connecting child of q to parent
				//Same as above

				par->bf = q->LChild->bf;

				if(q->key < par->key)
					par->LChild = q->LChild;
				else
					par->RChild = q->LChild;

				q->LChild = NULL;
			}

			delete q;
		}

		//Function to delete node with value k
		void AVL_Delete(int k)
		{
			AVL_Node* t = head;
			AVL_Node* s = NULL;
			AVL_Node* p = head->RChild;

			AVL_Node *q = NULL, *r = NULL;

			stack<AVL_Node*> parStk;		//Stack of parents of the element to be deleted
			parStk.push(head);

			//Locating element to be deleted
			while(p)
			{
				if(k < p->key)			//Traverse into left subtree
				{
					parStk.push(p);
					p = p->LChild;
				}

				else if(k > p->key)		//Traverse into right subtree
				{
					parStk.push(p);
					p = p->RChild;
				}

				else					//Element found
					break;
			}

			if(!p)						//If element not found or tree is empty
			{
				cout<<k<<" not found for deletion."<<endl;
				return;
			}

			s = parStk.top();			//s = parent of element to be deleted

			//Deletion of the element

			if(!p->LChild && !p->RChild)	//If element to be deleted is a leaf	
			{
				Del0Child(p, s);
			}

			else if(!p->LChild || !p->RChild)	//If element to be deleted has only 1 child
			{
				Del1Child(p, s);
				if(s->bf == 0)			//If parent is balanced post deletion, not disturbing it
					parStk.pop();
			}

			else	//If element to be deleted has 2 children
			{
				parStk.push(p);					//Pushing current element
				AVL_Node *inSucc = p->RChild;

				while(inSucc->LChild)			//Locating inorder successor of element
				{
					parStk.push(inSucc);		//Here we will push parents of inorder successor, and not the 2-degree node
					inSucc = inSucc->LChild;
				}

				p->key = inSucc->key;			//Copying inorder successor's key to the 2-degree node

				k = inSucc->key;				//Saving it in k

				AVL_Node* inSuccPar = parStk.top();		//Inorder successor's parent

				if(!inSucc->LChild && !inSucc->RChild)	//If inorder successor is leaf

					Del0Child(inSucc, inSuccPar);

				else									//If inorder successor has 1 child
				{
					Del1Child(inSucc, inSuccPar);
					if(s->bf == 0)						//Same as above
						parStk.pop();
				}
			}

			int a=0;	//For assignment of bf (Like in AVL_Insert)

			while(parStk.top()!=head)	//While all parents from stack are not removed
			{
				s = parStk.top();

				parStk.pop();
				t = parStk.top();	//t = parent of s

				//Same as AVL_Insert()
				if(k < s->key)
					a = 1;
				else
					a = -1;

				if(s->bf == 1)
					r = s->LChild;
				else
					r = s->RChild;

				if(s->bf == 0)
				{
					s->bf = (-1*a);		//Element was deleted, hence imbalance introduced
					return;
				}

				else if(s->bf == a)		//key is deleted from the direction of imbalance
				{
					s->bf = 0;			//Tree became more balanced
					continue;			//Rotation still required
				}

				else if(s->bf == (-1*a))	//key is deleted from opposite direction of imbalance
				{							//That is why -a is passed

					//L-L or R-R imbalance
					if(r->bf == (-1*a))
					{
						singleRotation((-1*a), &p, &r, &s);
					}

					//Special case, unlike AVL_Insert()
					//Still has a single rotation
					else if(r->bf == 0)
					{
						p = r;

						if(a == -1)
						{
							s->LChild = r->RChild;
							r->RChild = s;
						}
						else if(a == 1)
						{
							s->RChild = r->LChild;
							r->LChild = s;
						}

						r->bf = a;
					}

					//R-L or L-R imbalance
					else if(r->bf == a)
					{
						doubleRotation((-1*a), &p, &r, &s);
					}
					
					//If resulting tree has imbalance, parent won't be affected. Hence return.
					if(p->bf == 1 || p->bf == -1)
					{
						if(s == t->RChild)
							t->RChild = p;
						else
							t->LChild = p;
						return;
					}
				}

				//Same as AVL_Insert()
				if (s == t->RChild)
					t->RChild = p;
				else
					t->LChild = p;
			}
		}

		//Function to search element with value k
		bool AVL_Search(int k)
		{
			AVL_Node* node = head->RChild;	//node = root

			while(node!=NULL)
			{
				if(k < node->key)			//Traverse to LST
					node = node->LChild;

				else if (k > node->key)		//Traverse to RST
					node = node->RChild;

				else						//Element found
					return true;
			}
			
			return false;	//If element not found after entire traversal
		}

		//Set labels in .gv file for all nodes uniquely
		//Helper function of AVL_Print()
		void inorderLabel(AVL_Node* node, fstream &myfile)
		{
			//Exit condition
			if(!node)
				return;

			inorderLabel(node->LChild, myfile);	//Recursively go to left subtree
			myfile<<"    n"<<node->key<<" [label="<<node->key<<", xlabel="<<node->bf<<"];\n";	//Assign label for node
			inorderLabel(node->RChild, myfile);	//Recursively go to right subtree
		}

		//Used to print null nodes on diagram
		//Called when node is NULL
		//Helper function of AVL_Print()
		void printTree_null(int val, int nullc, fstream &myfile)
		{
		    myfile<<"    null"<<nullc<<" [shape=point];\n";
		    myfile<<"    n"<<val<<" -> null"<<nullc<<";\n";
		}
		
		//Used to print the nodes and their edges
		//Helper function of AVL_Print()
		void printTree_main(AVL_Node* node, fstream &myfile)
		{
		    static int nullc = 0;	//Used to name null nodes uniquely (null1, null2)
		
			//Printing all the left edges
		    if (node->LChild!=NULL)
		    {
		    	myfile<<"    n"<<node->key<<" -> n"<<node->LChild->key<<";\n";
		        printTree_main(node->LChild, myfile);		//Recursively traverse to left subtree
		    }

		    else	//Printing edge to NULL node
		        printTree_null(node->key, nullc++, myfile);
		
			//Printing all the right edges
		    if (node->RChild)
		    {
		    	myfile<<"    n"<<node->key<<" -> n"<<node->RChild->key<<";\n";
		        printTree_main(node->RChild, myfile);		//Recursively traverse to right subtree
		    }

		    else	//Printing edge to NULL node
		        printTree_null(node->key, nullc++, myfile);
		}
		
		//Driver function for printTree_main()
		void AVL_Print(const char *filename)
		{
			fstream myfile;
			myfile.open("graph1.gv",ios::out|ios::trunc);	//Creating file for graph in write mode, which clears after it is opened again
			
			AVL_Node* node = head->RChild;
			
			//Initial printing in .gv file
			myfile<<"digraph BST {\n";
			myfile<<"    node [fontname=\"Arial\"];\n";
			myfile<<"    graph [ordering=\"out\"];\n";
			myfile<<"    forcelabels=true;\n";
			inorderLabel(head->RChild, myfile);
			
			if (node==NULL)		//If tree is empty, graph file is empty
	        	myfile<<endl;

		    else if (node->RChild == NULL && node->LChild == NULL)		//If tree has only one node, print that
		        myfile<<"    n"<<node->key<<";\n";

		    else
	        	printTree_main(node, myfile);		//Else call printTree_main
			
			myfile<<"}\n";		//Final printing in .gv file
			
			myfile.close();		//Closing the file

			string command = "dot -Tsvg graph1.gv > "+string(filename)+".svg";
			system((const char*)command.c_str());	//Opens cmd, converts .gv into .svg, opens it

			cout<<"Find file in same directory, and open."<<endl;
		}

		//Destructor to delete entire tree
		~AVL_Tree()
		{
			delete head;	//Delete head, thus the entire tree under it is deleted.
		}
};

int main()
{
	AVL_Tree *t = new AVL_Tree();

	int choice=0, k=0;

	cout<<"1. insert\n2. delete\n3. search\n4. print tree\n5. sample tree\n0. exit\n"<<endl;

	while(1)
	{
		cout<<"Enter your option: ";
		cin>>choice;

		switch(choice)
		{
			case 0:
			{
				delete t;
				cout<<"Goodbye!"<<endl;
				return 0;
			}
			case 1:
			{
				cout<<"Enter element you want to insert: ";
				cin>>k;
				t->AVL_Insert(k);
				break;
			}
			case 2:
			{
				cout<<"Enter element you want to delete: ";
				cin>>k;
				t->AVL_Delete(k);
				break;
			}
			case 3:
			{
				cout<<"Enter element you want to search: ";
				cin>>k;
				if(t->AVL_Search(k))
					cout<<k<<" found!"<<endl;
				else
					cout<<k<<" not found."<<endl;

				break;
			}
			case 4:
			{
				string filename;
				cout<<"Enter name of file: ";
				cin>>filename;
				t->AVL_Print((const char*)filename.c_str());
				break;
			}
			case 5:
			{
				t->AVL_Insert(10);
				t->AVL_Insert(20);
				t->AVL_Insert(30);
				t->AVL_Insert(40);
				t->AVL_Insert(25);
				t->AVL_Insert(9);
				t->AVL_Insert(15);
				t->AVL_Insert(35);
				t->AVL_Insert(2);
				t->AVL_Insert(3);

				t->AVL_Delete(15);

				t->AVL_Print("sample");

				break;
			}
			default:
			{
				cout<<"Invalid input."<<endl;
				break;
			}
		}
	}

	return 0;
}