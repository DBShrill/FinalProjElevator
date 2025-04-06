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

#include <algorithm>
#include "Floor.h"

using namespace std;

int Floor::tick(int currentTime) {
	int exploded = 0;
	int indicesToRemove[MAX_PEOPLE_PER_FLOOR];
	int numPeopleToRemove = 0;
    for (int i = 0; i < numPeople; i++) {
	    if (people[i].tick(currentTime)) {
			indicesToRemove[numPeopleToRemove] = i;
			numPeopleToRemove++;
	    	exploded++;
			//TODO Reimplement given new remove call
	    	/*for (int j = numPeople; j > i; j--) {
	    		people[j] = people[j - 1];
	    	}
			// up until here ^
			*/
	    }
    }
	if (numPeopleToRemove > 0) {
		removePeople(indicesToRemove, numPeopleToRemove);
	}
	return exploded;
}

void Floor::addPerson(Person newPerson, int request) {
    if (numPeople < MAX_PEOPLE_PER_FLOOR) {
	    people[numPeople] = newPerson;
    	if (request > 0) {
    		hasUpRequest = true;
    	}
    	else if (request < 0) {
    		hasDownRequest = true;
    	}
		numPeople++;
    }

}

void Floor::removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR],
                         int numPeopleToRemove) {
	int targetsToRemove[MAX_PEOPLE_PER_FLOOR] = {};

	//transferring indicesToRemove to a new array
	for (int i = 0; i < numPeopleToRemove; i++) {
		targetsToRemove[i] = indicesToRemove[i];
	}

	sort(targetsToRemove, targetsToRemove + numPeopleToRemove);

	//starts from end of people list and iterates backwards
	for (int i = numPeopleToRemove - 1; i >= 0; i--) {
		int removeIndex = targetsToRemove[i];

		//iterates from target to end
		for (int j = removeIndex; j < numPeople - 1; j++ ) {
			//"shifts" index+1 to index
			people[j] = people[j+1];
		}
		numPeople--;
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
