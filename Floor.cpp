/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Floor.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * Daniel Bao, Evan Stocker, Spencer Hu, Zihan Li
 * danbao, zihanl, zianghu, stockere
 *
 * Final Project - Elevators
 */


#include "Floor.h"

using namespace std;

int Floor::tick(int currentTime) {
	int exploded;
    for (int i = 0; i < numPeople; i++) {
    	//
	    if (people[i].tick(currentTime)) {
	    	exploded++;
			//TODO keep track of index where explode
			//TODO remove said people
	    	for (int j = numPeople; j > i; j--) {
	    		people[j] = people[j - 1];
	    	}
	    }
    }
	return exploded;
}

void Floor::addPerson(Person newPerson, int request) {
    if (numPeople < MAX_PEOPLE_PER_FLOOR) {
	    people[numPeople] = newPerson;
    	if (request > 0) {
    		hasUpRequest = true;
    		hasDownRequest = false;
    	}
    	else if (request < 0) {
    		hasUpRequest = false;
    		hasDownRequest = true;
    	}
		numPeople++;
    }

}

void Floor::removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR],
                         int numPeopleToRemove) {
	numPeople -= numPeopleToRemove;
    for (int i = 0; i < numPeopleToRemove; i++) {
    	//TODO: check if there is a person default constructor
    	//resets the person at the indicated index
	    people[indicesToRemove[i]] = Person();

    	for (int j = indicesToRemove[i]; j < MAX_PEOPLE_PER_FLOOR - 1; j++) {
    		people[j] = people[j + 1];
    		people[MAX_PEOPLE_PER_FLOOR - 1] = Person();
    	}
    }
	resetRequests();

}

void Floor::resetRequests() {
	hasDownRequest = false;
	hasUpRequest = false;
	//iterates through people
    for (int i = 0; i < numPeople; i++) {
    	//checks if going up
	    if (people[i].getCurrentFloor() < people[i].getTargetFloor()) {
		    hasUpRequest = true;
	    }

    	//checks if going down
    	else if (people[i].getCurrentFloor() > people[i].getTargetFloor()) {
    		hasDownRequest = true;
    	}
    }
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Floor::Floor() {
    hasDownRequest = false;
    hasUpRequest = false;
    numPeople = 0;
}

void Floor::prettyPrintFloorLine1(ostream& outs) const {
	string up = "U";
	outs << (hasUpRequest ? up : " ") << " ";
	for (int i = 0; i < numPeople; ++i){
		outs << people[i].getAngerLevel();
		outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
	}
	outs << endl;
}

void Floor::prettyPrintFloorLine2(ostream& outs) const {
	string down = "D";
	outs << (hasDownRequest ? down : " ") << " ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "o   ";
	}
	outs << endl;
}

void Floor::printFloorPickupMenu(ostream& outs) const {
	cout << endl;
	outs << "Select People to Load by Index" << endl;
	outs << "All people must be going in same direction!";
	/*  O   O
	// -|- -|-
	//  |   |
	// / \ / \  */
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " O   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "-|-  ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " |   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "/ \\  ";
	}
	outs << endl << "INDEX:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << i << "   ";
	}
	outs << endl << "ANGER:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getAngerLevel() << "   ";
	}
	outs << endl << "TARGET FLOOR: ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getTargetFloor() << "   ";
	}
}

void Floor::setHasUpRequest(bool hasRequest) {
    hasUpRequest = hasRequest;
}

bool Floor::getHasUpRequest() const {
	return hasUpRequest;
}

void Floor::setHasDownRequest(bool hasRequest) {
    hasDownRequest = hasRequest;
}

bool Floor::getHasDownRequest() const {
	return hasDownRequest;
}

int Floor::getNumPeople() const {
    return numPeople;
}

Person Floor::getPersonByIndex(int index) const {
    return people[index];
}
