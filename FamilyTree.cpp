#include <iostream>
#include <string>
#include "FamilyTree.hpp"
using namespace std;

namespace family{

	Tree::node* Tree::findPerson(string child, node* root){
		if(root == NULL)
			return NULL;
		else if(root->name == child)
			return root;

		node* person = findPerson(child, root->mother);
		if(person == NULL){
			person = findPerson(child, root->father);
		}
		return person;
	}

	Tree Tree::addFather(string child, string father){
		node* pFather = new node;
		pFather->name = father;
		pFather->mother = NULL;
		pFather->father = NULL;
		pFather->male=true;
		if(child == this->root->name) {
			if (this->root->father != NULL)
				throw runtime_error(child + "already has a father!");
			else
				this->root->father = pFather;

		}
		else{
			node* temp = new node;
			temp = findPerson(child, this->root);
			if(temp == NULL)
				throw runtime_error(child + "not found!");
			else if(temp->father != NULL)
				throw runtime_error(child + "already has a father!");
			temp->father=pFather;
		}
		return *this;
	}

	Tree Tree::addMother(string child, string mother){
		node* pMother = new node;
		pMother->name = mother;
		pMother->mother = NULL;
		pMother->father = NULL;
		pMother->male=false;
		if(child == this->root->name){
			if(this->root->mother != NULL)
				throw runtime_error(child + "already has a mother!");

			else
				this->root->mother = pMother;
		}
		else{
			node* temp = new node;
			temp = findPerson(child, this->root);
			if(temp == NULL)
				throw runtime_error(child + " not found!");
			else if(temp->mother != NULL)
				throw runtime_error(child + " already has a mother!");
			temp->mother=pMother;
		}
		return *this;
	}
	string Tree::relation(string person){
		int deaph=findDeepth(this->root,person,0);
		if(deaph==0 && person!=root->name)
			return "unrelated";
		struct node* p= findPerson(person,this->root);
		if(p->male==true){
			if(deaph==0)
				return "me";
			else if(deaph==1)
				return "father";
			else if (deaph==2)
				return "grandfather";
			else if(deaph==3)
				return "great-grandfather";
			else
			{
				string base="grandfather";
				string chain="great-";
				for(size_t i=0;i<deaph-3;i++)
					chain.append("great-");
				chain.append(base);
				return chain;
			}
		}
		else{
			if(deaph==0)
				return "me";
			else if(deaph==1)
				return "mother";
			else if (deaph==2)
				return "grandmother";
			else if(deaph==3)
				return "great-grandmother";
			else{
				string base="grandmother";
				string chain="great-";
				for(size_t i=0;i<deaph-3;i++)
					chain.append("great-");
				chain.append(base);
				return chain;
			}
		}
	}
	int Tree::findDeepth(node* root,string name,int level){
		if(root==NULL)
			return 0;
		if(root->name==name)
			return level;
		int deaph=findDeepth(root->father,name,level+1);
		if(deaph==0)
			deaph=findDeepth(root->mother,name,level+1);
		return deaph;
	}

	string Tree::find(string relation){
		int level=levelOfRelation(relation);
		if(level==0)
			return root->name;
		bool male=ifMale(relation);
		int h=height(this->root);
		if(h<=level || level<0)
			throw runtime_error("This relation does not exist in the ancestor tree!");
		node* person=findPersonByRelation(this->root,male,level);
		if(person==NULL)
			throw runtime_error("This relation does not exist in the ancestor tree!");
		return person->name;
	}

	Tree::node* Tree::findPersonByRelation(node* root,bool male,int level){
		if(root==NULL)
			return NULL;
		if(level==0 && root->male==male)
			return root;
		node* person=findPersonByRelation(root->father,male,level-1);
		if(person==NULL)
			person=findPersonByRelation(root->mother,male,level-1);
		return person;
	}

	int Tree::levelOfRelation(string relation){
		if(relation=="me")
			return 0;
		else if(relation=="father" || relation=="mother")
			return 1;
		else if(relation=="grandfather" || relation=="grandmother")
			return 2;
		else{
			string maleBase="grandfather";
			string femaleBase="grandmother";
			string chain="great-";
			for (size_t i = 3; i < 10; i++){
				maleBase=chain+maleBase;
				femaleBase=chain+femaleBase;
				if(maleBase==relation || femaleBase==relation)
					return i;
			}
			throw runtime_error("Unexpection error from levelOfRealtion function");
		}
	}

	bool Tree::ifMale(string relation){
		size_t found = relation.find("father");
		if (found != string::npos)
			return true;
		found = relation.find("mother");
		if(found != string::npos)
			return false;
		throw runtime_error("unexpected ifMale function error!");
	}

	int Tree::height(node* node){
		if(node==NULL)
			return 0;
		else{
			int hMother = Tree::height(node->mother);
			int hFather = Tree::height(node->father);
			if (hMother > hFather)
				return(hMother + 1);
			else
				return(hFather + 1);
		}
	}

	void Tree::printGivenLevel(node* root, int level){
		if (root ==	 NULL)
			return;
		if (level == 1)
			cout << root->name << " ";
		else if (level > 1){
			cout<<"(";
			printGivenLevel(root->mother, level-1);
			cout<<",";
			printGivenLevel(root->father, level-1);
			cout<<")";
		}
	}

	void Tree::printLevelOrder(node* root){
		int h = height(root);
		int i;
		for (i = 1; i <= h; i++){
			printGivenLevel(root, i);
			cout<<endl;
		}
	}

	void Tree::display(){
		printLevelOrder(this->root);
	}

	void Tree::remove(string name){
		if(this->root->name==name)
			throw runtime_error("You can't remove the root!");
		else{
			node* person=findPerson(name,this->root);
			if(person==NULL)
				throw runtime_error("This name is not exist in the tree");
			removeTree(person);
			node* child=findChild(name,this->root);
			if(child==NULL)
				throw runtime_error("Unexpected error from remove function");
			if(child->father!=NULL && child->father->name==name){
				child->father=NULL;
				delete child->father;
			}
			else if(child->mother!=NULL && child->mother->name==name){
				child->mother=NULL;
				delete child->mother;
			}
			else
				throw runtime_error("Unexpected error in remove function");
		}
	}

	Tree::node* Tree::findChild(string name,node* root){
		if (root==NULL)
			return NULL;
		if((root->father!=NULL && root->father->name==name) || (root->mother!=NULL && root->mother->name==name))
			return root;
		node* person=findChild(name,root->father);
		if(person==NULL)
			person=findChild(name,root->mother);
		return person;
	}

	void Tree::removeTree(node* root){
		if (root == NULL) return;
		removeTree(root->mother);
		removeTree(root->father);
		root->father=NULL;
		root->mother=NULL;
		delete root->father;
		delete root->mother;
	}
}