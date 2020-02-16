// Johnston Yang
//January 20 2020


#include "pch.h"
#include "Person.h"
#include <iostream>
#include <vector>

using namespace std;

// Constructor
Person::Person(Sex gender, const string & name) :sex(gender), name(name) 
{}

// Deconstructor
Person::~Person()
{	
}

bool Person::setFather(Person * fParent)
{
	// assumed that all fathers must be male
	if (fParent->sex == MALE)
	{
		// if there was no father before
		if (Person::father == NULL)
		{
			Person::father = fParent;
			Person::father->children.push_back(this);
		}
		// if the father is different
		else if (Person::father != fParent)
		{
			Person* prev = Person::father;
			vector<Person*>::iterator it = find(prev->children.begin(), prev->children.end(), this);
			if (it != prev->children.end())
			{
				prev->children.erase(it);
			}
			Person::father = fParent;
		}
		// if the father is the same
		else
		{
			vector<Person*>::iterator it = find(fParent->children.begin(), fParent->children.end(), this);
			if (it != fParent->children.end())
			{
				fParent->children.erase(it);
			}
			Person::father = NULL;
		}
		return true;
	}
	else
	{
		cout << "Father must be male" << endl;
		return false;
	}
}

bool Person::setMother(Person * mParent)
{
	//assumed that all mothers are female
	if (mParent->sex == FEMALE)
	{
		// if there was no mother before
		if (Person::mother == NULL)
		{
			Person::mother = mParent;
			Person::mother->children.push_back(this);
		}
		// if the current mother is different
		else if (Person::mother != mParent)
		{
			Person* prev = Person::mother;
			vector<Person*>::iterator it = find(prev->children.begin(), prev->children.end(), this);
			if (it != prev->children.end())
			{
				prev->children.erase(it);
			}
			Person::mother = mParent;
		}
		// if the mother is the same
		else
		{
			vector<Person*>::iterator it = find(mParent->children.begin(), mParent->children.end(), this);
			if (it != mParent->children.end())
			{
				mParent->children.erase(it);
			}
			Person::mother = NULL;
		}
		return true;
	}
	else
	{
		cout << "Mother must be female" << endl;
		return false;
	}
}


bool Person::hasChild(const Person * child) const
{
	// checks if the child exists in the children vector
	People::const_iterator it = find(this->children.begin(), this->children.end(), child);
	if (it != this->children.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Person::addChild(Person * child)
{
	this->children.push_back(child);
	// if the parent is female, then modifies the child's mother
	if (this->sex == FEMALE)
	{
		child->mother = this;
	}
	// if the parent is male, then modifies the child's father
	else
	{
		child->father = this;
	}
	return true;
}

bool Person::removeChild(Person * child)
{
	//checks if the child exists in the children vector
	People::iterator it = find(this->children.begin(), this->children.end(), child);
	if (it != this->children.end())
	{
		this->children.erase(it);
		// after removing the child from the children vector, checks if parent is male or female
		if (this->sex == FEMALE)
		{
			child->mother = NULL;
		}
		else
		{
			child->father = NULL;
		}
		return true;
	}
	return false;
}

void Person::removeAllChildren()
{
	//iterates through the children vector and sets the child's parent to NULL
	People::iterator it;
	for (it = this->children.begin(); it != this->children.end(); it++)
	{
		if (this->sex == FEMALE)
		{
			(*it)->mother = NULL;
		}
		else
		{
			(*it)->father = NULL;
		}
	}
	// clears the vector for the parent
	this->children.clear();
	return;
}


//binary tree traversal
void Person::getAncestors(People & results) const
{
	if (this != NULL)
	{
		this->mother->getAncestors(results);
		if (this->mother != NULL)
		{
			vector<Person*>::iterator it = find(results.begin(), results.end(), this->mother);
			if (it == results.end())
			{
				results.push_back(this->mother);
			}
		}
		if (this->father != NULL)
		{
			vector<Person*>::iterator iter = find(results.begin(), results.end(), this->father);
			if (iter == results.end())
			{
				results.push_back(this->father);
			}
		}
		this->father->getAncestors(results);
	}
	return;
}

//n-ary tree traversal
void Person::getDescendants(People & results) const
{
	if (this->children.empty())
	{
		return;
	}
	int numChild = this->getNumChildren();
	for (int i = 0; i < numChild - 1; i++)
	{
		this->children[i]->getDescendants(results);
		results.push_back(this->children[i]);
	}
	results.push_back(this->children[numChild-1]);
}

//Assumed sibilings must have the same mother and father
void Person::getSiblings(People & results) const
{
	vector<Person*>::iterator it;
	if (this->mother != NULL)
	{
		int numChild = this->mother->getNumChildren();
		for (int i = 0; i < numChild; i++)
		{
			if (this->mother->children[i] != this)
			{
				results.push_back(this->mother->children[i]);
			}
		}
		//this->mother->children
	}
	if (this->father != NULL)
	{
		int numChild = this->father->getNumChildren();
		for (int i = 0; i < numChild; i++)
		{
			if (this->father->children[i] != this)
			{
				it = find(this->father->children.begin(), this->father->children.end(), this->father->children[i]);
				if (it == this->father->children.end())
				{
					results.push_back(this->father->children[i]);
				}
			}
		}
		//this->father->children
	}
}

// Assumed cousins will be when children have both different mother and father
void Person::getCousins(People & results) const
{
	int level = levelPerson(this, 0);
	if (level <= 1 || this == NULL)
	{
		return;
	}
	else
	{
		People ancestors;
		People descendents; // grabs intial descendents
		People fDescendents; // removes all repeat descendents

		this->getAncestors(ancestors);

		vector<Person*>::iterator findDescendents;
		vector<Person*>::iterator placeDescendents;
		vector<Person*>::iterator findDepth;
		
		for (findDescendents = ancestors.begin(); findDescendents != ancestors.end(); findDescendents++)
		{
			(*findDescendents)->getDescendants(descendents); // grabs initial descendents
			for (placeDescendents = descendents.begin(); placeDescendents != descendents.end(); placeDescendents++)
			{
				vector<Person*>::iterator vecDesc = find(fDescendents.begin(), fDescendents.end(), *placeDescendents);
				if (fDescendents.empty())
				{
					fDescendents.push_back(*placeDescendents);
				}
				else if (vecDesc == fDescendents.end()) // removes repeat descendents
				{
					fDescendents.push_back(*placeDescendents);
				}
			}
		}
		for (findDepth = fDescendents.begin(); findDepth != fDescendents.end(); findDepth++)
		{
			int depth = levelPerson(*findDepth, 0);
			if ((depth == level) && ((*findDepth)->father != this->father) && ((*findDepth)->mother != this->mother))
			{
				results.push_back(*findDepth);
			}
		}
		return;
	}
	return;
}

// Helper function that finds the depth of the child
int Person::levelPerson(const Person* person, int level) const
{
	if (person == NULL)
	{
		return 0;
	}
	else if (person->getMother() == NULL)
	{
		return level;
	}
	else if (person->getFather() == NULL)
	{
		return level;
	}
	else //if (person != NULL)
	{
		int iter = levelPerson(person->getFather(), level + 1);
		if (iter != 0)
		{
			return iter;
		}
		iter = levelPerson(person->getMother(), level + 1);
		return iter;
	}
}

People::iterator Person::findChild(const Person *)
{
	return People::iterator();
}

People::const_iterator Person::findChild(const Person *) const
{
	return People::const_iterator();
}