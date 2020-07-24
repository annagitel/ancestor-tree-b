#pragma once
#include <iostream>
#include <string>
#include <stdbool.h>
using namespace std;
namespace family{
	class Tree{
		struct node {
			bool male; //true- male false-female
			string name;
			node* father;
			node* mother;
		};
	public:
		node* root;
		//Constructors
		Tree(string name,bool male){
			root= new node;
			root->name=name;
			root->father=NULL;
			root->mother=NULL;
			root->male=male;
		}

		Tree(string name){
			root= new node;
			root->name=name;
			root->father=NULL;
			root->mother=NULL;
			root->male=false;
		}

		Tree addFather(string child,string father);
		Tree addMother(string child,string mother);

		struct node* findPerson(string child,node* root);
		string relation(string name);
		int findDeepth(node* root,string name,int level);
		string find(string relation);
		int levelOfRelation(string relation);
		bool ifMale(string relation);
		struct node* findPersonByRelation(node* root,bool male,int level);
		void display();
		int height(node* node);
		void printGivenLevel(node* root, int level);
		void printLevelOrder(node* root);
		void remove(string name);
		void removeTree(node* root);
		struct node* findChild(string name,node* root);

	};
};