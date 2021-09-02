#include<bits/stdc++.h>
#include<fstream>

using namespace std;

class AVL_Node
{
	private:
		int key;
		int bf;
		AVL_Node *LChild, *RChild;

		AVL_Node(int k)
		{
		 	key = k;
		 	LChild = NULL;
		 	RChild = NULL;
		 	bf = 0;
		}

		AVL_Node()
		{
		 	key = 0;
		 	LChild = NULL;
		 	RChild = NULL;
		 	bf = 0;
		}

		~AVL_Node()
		{
			delete LChild;
			delete RChild;
		}

	friend class AVL_Tree;
};

class AVL_Tree
{

	private:
		AVL_Node* head;

	public:
		AVL_Tree()
		{
			head = new AVL_Node();
			head->LChild = NULL;
			head->RChild = NULL;
		}
		
		//AVL_Tree(const AVL_Tree &T);
		
		//AVL_Tree &operator=(const AVL_Tree &T);

		void singleRotation(int a, AVL_Node **p, AVL_Node **r, AVL_Node **s)
		{
			(*p) = (*r);

			if(a == -1)
			{
				(*s)->LChild = (*r)->RChild;
				(*r)->RChild = (*s);
			}
			else if (a == 1)
			{
				(*s)->RChild = (*r)->LChild;
				(*r)->LChild = (*s);
			}

			(*r)->bf = 0;
			(*s)->bf = 0;
		}

		void doubleRotation(int a, AVL_Node **p, AVL_Node **r, AVL_Node **s)
		{
			if(a == -1)
			{
				(*p) = (*r)->RChild;
				(*r)->RChild = (*p)->LChild;
				(*p)->LChild = (*r);
				(*s)->LChild = (*p)->RChild;
				(*p)->RChild = (*s);
			}
			else if (a == 1)
			{
				(*p) = (*r)->LChild;
				(*r)->LChild = (*p)->RChild;
				(*p)->RChild = (*r);
				(*s)->RChild = (*p)->LChild;
				(*p)->LChild = (*s);
			}

			if((*p)->bf == a)
			{
				(*s)->bf = -1*a;
				(*r)->bf = 0;
			}
			else if((*p)->bf == 0)
			{
				(*s)->bf = 0;
				(*r)->bf = 0;
			}
			else if((*p)->bf == (-1*a))
			{
				(*s)->bf = 0;
				(*r)->bf = a;
			}

			(*p)->bf = 0;
		}
		
		void AVL_Insert(int k)
		{
			AVL_Node* t = head;
			AVL_Node* s = head->RChild;
			AVL_Node* p = head->RChild;

			AVL_Node *q = NULL, *r = NULL;

			AVL_Node* temp = new AVL_Node(k);
			
			if(p==NULL)
			{
				head->RChild = temp;
				return;
			}

			else
			{
				while(1)
				{
					if(k < p->key)
					{
						q = p->LChild;

						if(!q)
						{
							q = temp;
							p->LChild = q;
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

					else
					{
						cout<<"Element already present."<<endl;
						return;
					}
				}

				int a;

				if(k < s->key)
					a = -1;
				else
					a = 1;

				if(a == -1)
					p = s->LChild;
				else
					p = s->RChild;

				r = p;

				while(p != q)
				{
					if(k < p->key)
					{
						p->bf = -1;
						p = p->LChild;
					}

					else if(k > p->key)
					{
						p->bf = 1;
						p = p->RChild;
					}
					else break;
				}


				if(s->bf == 0)
				{
					s->bf = a;
					return;
				}

				else if(s->bf == (-1*a))
				{
					s->bf = 0;
					return;
				}

				else if(s->bf == a)
				{
					if(r->bf == a)
					{
						singleRotation(a, &p, &r, &s);
					}

					else if(r->bf == (-1*a))
					{
						doubleRotation(a, &p, &r, &s);
					}
				}
				if (s == t->RChild)
					t->RChild = p;
				else
					t->LChild = p;
			}
		}

		void Del0Child(AVL_Node* q, AVL_Node* par)
		{
			if(q->key < par->key)
				par->LChild = NULL;

			else
				par->RChild = NULL;

			delete q;
		}

		void Del1Child(AVL_Node* q, AVL_Node* par)
		{
			par->bf = q->bf;

			if(!q->LChild)
			{
				if(q->key < par->key)
					par->LChild = q->RChild;
				else
					par->RChild = q->RChild;

				q->RChild = NULL;
			}

			else
			{
				if(q->key < par->key)
					par->LChild = q->LChild;
				else
					par->RChild = q->LChild;

				q->LChild = NULL;
			}

			delete q;
		}

		void AVL_Delete(int k)
		{
			AVL_Node* t = head;
			AVL_Node* s = NULL;
			AVL_Node* p = head->RChild;

			AVL_Node *q = NULL, *r = NULL;

			stack<AVL_Node*> parStk;
			parStk.push(head);

			while(p)
			{
				if(k < p->key)
				{
					parStk.push(p);
					p = p->LChild;
				}

				else if(k > p->key)
				{
					parStk.push(p);
					p = p->RChild;
				}

				else
					break;
			}

			if(!p)
			{
				cout<<"Element not found."<<endl;
				return;
			}

			s = parStk.top();

			if(!p->LChild && !p->RChild)
			{
				Del0Child(p, s);
			}

			else if(!p->LChild || !p->RChild)
			{
				Del1Child(p, s);
			}

			else
			{
				parStk.push(p);
				AVL_Node *inSucc = p->RChild;

				while(inSucc->LChild)
				{
					parStk.push(inSucc);
					inSucc = inSucc->LChild;
				}

				p->key = inSucc->key;

				k = inSucc->key;

				AVL_Node* inSuccPar = parStk.top();

				if(!inSucc->LChild && !inSucc->RChild)

					Del0Child(inSucc, inSuccPar);

				else
					Del1Child(inSucc, inSuccPar);
			}

			int a=0;

			while(parStk.top()!=head)
			{
				s = parStk.top();

				parStk.pop();
				t = parStk.top();

				if(k < s->key)
					a = -1;
				else
					a = 1;

				if(s->bf == -1)
					r = s->LChild;
				else
					r = s->RChild;

				if(s->bf == 0)
				{
					s->bf = (-1*a);
					return;
				}

				else if(s->bf == a)
				{
					s->bf = 0;
					continue;
				}

				else if(s->bf == (-1*a))
				{

					if(r->bf == (-1*a))
					{
						singleRotation((-1*a), &p, &r, &s);
					}

					//Special case, unlike Insert
					else if(r->bf == 0)
					{
						p = r;

						if(a == 1)
						{
							s->LChild = r->RChild;
							r->RChild = s;
						}
						else if(a == -1)
						{
							s->RChild = r->LChild;
							r->LChild = s;
						}

						r->bf = a;
					}

					else if(r->bf == a)
					{
						doubleRotation((-1*a), &p, &r, &s);
					}
					
					if(p->bf == 1 || p->bf == -1)
					{
						if(s == t->RChild)
							t->RChild = p;
						else
							t->LChild = p;
						return;
					}
				}

				if (s == t->RChild)
					t->RChild = p;
				else
					t->LChild = p;
			}
		}

		bool AVL_Search(int k)
		{
			AVL_Node* node = head->RChild;

			while(node!=NULL)
			{
				if(k < node->key)
					node = node->LChild;

				else if (k > node->key)
					node = node->RChild;

				else
					return true;
			}
			
			return false;
		}

		void inorderLabel(AVL_Node* node, fstream &myfile)
		{
			if(!node)
				return;

			inorderLabel(node->LChild, myfile);
			myfile<<"    n"<<node->key<<" [label="<<node->key<<", xlabel="<<node->bf<<"];\n";
			inorderLabel(node->RChild, myfile);
		}

		//Used to print null nodes on diagram
		//Called when node is NULL
		//Helper function of printTree()
		void printTree_null(int val, int nullc, fstream &myfile)
		{
		    myfile<<"    null"<<nullc<<" [shape=point];\n";
		    myfile<<"    n"<<val<<" -> null"<<nullc<<";\n";
		}
		
		//Used to print the nodes and their edges
		//Helper function of printTree()
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
			
			//WORKS ONLY ON WINDOWS
			int win=0;
			string s;

			cout<<"Are you using\n0: Windows\n1:Linux\nEnter: ";
			cin>>win;

			switch(win)
			{
				case 0:
					s = " & ";
					break;
				case 1:
					s = " && ./";
					break;
				default:
					cout<<"Assuming you're running Linux."<<endl;
					s = " && ./";
					break;
			}

			string command = "dot -Tsvg graph1.gv > "+string(filename)+s+string(filename)+".svg";
			system((const char*)command.c_str());	//Opens cmd, converts ,gv into .svg, opens it
		}

		~AVL_Tree()
		{
			head->RChild = NULL;
			head->LChild = NULL;
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
				cout<<"Goodbye!";
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
					cout<<k<<"not found."<<endl;

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

				t->AVL_Insert(20);
				t->AVL_Insert(10);
				t->AVL_Insert(30);
				t->AVL_Insert(9);
				t->AVL_Insert(15);
				t->AVL_Insert(25);
				t->AVL_Insert(35);
				t->AVL_Insert(2);
				t->AVL_Insert(3);

				//t->AVL_Delete(15);

				t->AVL_Print("myfile");

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