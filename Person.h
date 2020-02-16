#pragma once
#include <string>
#include <vector>

class Person;
typedef std::vector<Person*> People;

class Person
{
public:

	enum Sex { MALE, FEMALE };

	Person(Sex, const std::string& name = "");
	~Person();

	/**
		Set the father of this person.  This method automatically removes
			this person from the list of children of the previous father
			and adds it to the new father's children. It can be used to
			unset the parent of a person.

		@param the father object, NULL is allowed
		@return true if successful
	*/
	// need to work on switching from one father to another while changing the children vector
	bool setFather(Person*);

	/**
		Set the mother of this person.  This method automatically removes
			this person from the list of children of the previous mother
			and adds it to the new mother's children. It can be used to
			unset the parent of a person.

		@param the mother object, NULL is allowed
		@return true if successful
	*/
	// remove and insertion
	bool setMother(Person*);

	// checks the mother and father of node
	Person* getFather() const { return father; }
	Person* getMother() const { return mother; }

	/**
		Checks if this person has a given child.

		@param the child to check
		@return true if it does
	*/
	bool hasChild(const Person*) const;

	/**
		Adds a child to the person's child list.  It automatically makes this
		object the child's parent and removes it from the old parent's children
		list.

		@param child to be added
		@return true if successful
	*/
	bool addChild(Person*);

	/**
		Remove a child from this person's children list.  It automatically sets
		the corresponding parent of the child to NULL.

		@param child to be removed
		@return true if successful
	*/
	bool removeChild(Person*);

	People::size_type getNumChildren() const { return children.size(); }
	Person* getChild(People::size_type index) const { return children.at(index); }

	/**
		Remove all children of this person.  It automatically sets the children's
		corresponding parents to NULL.
	*/
	void removeAllChildren();

	/**
		Get all ancestors of this person.
	*/
	void getAncestors(People& results) const;

	/**
		Get all descendants of this person.
	*/
	void getDescendants(People& results) const;

	/**
		Get all siblings of this person.
	*/
	void getSiblings(People& results) const;

	/**
		Get all cousins of this person in the same generation.
	*/
	void getCousins(People& results) const;

	const Sex sex;
	const std::string name;

	int levelPerson(const Person* person, int level) const;


private:

	People::iterator findChild(const Person*);
	People::const_iterator findChild(const Person*) const;

	Person *mother, *father;
	People children;
};
