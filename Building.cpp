/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Building.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <#Names#>
 * Daniel Bao, Evan Stocker, Spencer Hu, Zihan Li
 * <#Uniqnames#>
 * danbao, zihanl, zianghu, stockere
 *
 * Final Project - Elevators
 */

#include "Building.h"

using namespace std;

void Building::spawnPerson(Person newPerson){
	//adds new person based on their current floor and their target floor
	floors[newPerson.getCurrentFloor()].addPerson(newPerson, newPerson.getTargetFloor());
}

void Building::update(Move move){
    //skip if its a pass move
    if (move.isPassMove()){
    	return;
    }

	//TODO: figure out how to add people to pickup to list
   
        int elevatorId = move.getElevatorId();
        Elevator& which_elevator = elevators[elevatorId];
    
        // Handle Pickup Moves
        if (move.isPickupMove()) {
            // Get current floor of the elevator (where pickup happens)
            int currentFloor = which_elevator.getCurrentFloor();
            Floor& floor = floors[currentFloor];
    
            // Copy list of people to pickup from the move
            int peopleIndices[MAX_PEOPLE_PER_FLOOR];
            move.copyListOfPeopleToPickup(peopleIndices);
            int numPeopleToPickup = move.getNumPeopleToPickup();
    
            // Remove those people from the floor
            floor.removePeople(peopleIndices, numPeopleToPickup);
        }

    }

    else if (move.isPickupMove()&& move.isServicing()){
 // For both Pickup Moves and Service Moves, service the target floor
        which_elevator.serviceRequest(move.getTargetFloor());
    }



int Building::tick(Move move){
    int exploded = 0;
    //iterates through all elevators
    for (int i = 0; i < NUM_ELEVATORS; i++){
        elevators[i].tick(move);
    }

    //iterates through all floors
    for (int i = 0; i < NUM_FLOORS; i++){
        exploded += floors[i].tick(move);
    }

    update(move);

    return exploded;
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Building::Building() {
    time = 0;
}

void Building::setElevator(string elevatorInfo, int elevatorId) {
    int i = 0;
    string currPosString = "";
    string destPosString = "";

    while(elevatorInfo[i] != 's' && elevatorInfo[i] != 'w'){
        currPosString += elevatorInfo[i];
        i++;
    }
    elevators[elevatorId] = Elevator();

    elevators[elevatorId].setCurrentFloor(stoi(currPosString));

    if (elevatorInfo[i] == 's') {
        i++;
        while (i < (int)elevatorInfo.size()) {
            destPosString += elevatorInfo[i];
            i++;
        }
        elevators[elevatorId].serviceRequest(stoi(destPosString));
    }
}

int Building::getTime() const {
    return time;
}

void Building::setTime(int timeIn) {
    time = timeIn;
}

void Building::prettyPrintBuilding(ostream& outs) const {
    for (int i = NUM_FLOORS - 1; i >= 0; --i) {
            outs << "   ";
            for (int j = 0; j < 2 * NUM_ELEVATORS + 1; ++j) {

                outs << '-';
            }

            outs << endl << "   ";

            for (int j = 0; j <  NUM_ELEVATORS; ++j) {
                char floorIndicatorIcon = ' ';

                if (elevators[j].getCurrentFloor() == i &&
                    elevators[j].isServicing()) {

                    char floorServiceIndicator = ('0' 
                                               + elevators[j].getTargetFloor());
                    floorIndicatorIcon = floorServiceIndicator;
                }

                outs << "|" << floorIndicatorIcon;
            }
            outs << "|";

            floors[i].prettyPrintFloorLine1(outs);
            outs << i << "  ";
            for (int j = 0; j <  NUM_ELEVATORS; ++j) {

                outs << "|" << (elevators[j].getCurrentFloor() == i ? 'E' : ' ');
            }
            outs << "|";

            floors[i].prettyPrintFloorLine2(outs);
        }
        outs << "   ";
        for (int j = 0; j < 2 * NUM_ELEVATORS + 1; ++j) {
            outs << '-';
        }
        outs << endl << "   ";
        for (int j = 0; j < NUM_ELEVATORS; ++j) {
            outs << " " << j;
        }
        outs << endl;
}

Elevator Building::getElevatorById(int elevatorId) const {
    return elevators[elevatorId];
}

Floor Building::getFloorByFloorNum(int floorNum) const {
    return floors[floorNum];
}

BuildingState Building::getBuildingState() const {
    BuildingState buildingState;
    buildingState.turn = time;

    for (int i = 0; i < NUM_FLOORS; ++i) {
        buildingState.floors[i].floorNum = i;
        buildingState.floors[i].hasDownRequest = floors[i].getHasDownRequest();
        buildingState.floors[i].hasUpRequest = floors[i].getHasUpRequest();

        int & np = buildingState.floors[i].numPeople;

        for (int j = 0; j < floors[i].getNumPeople(); ++j) {

            Person p = floors[i].getPersonByIndex(j);
            buildingState.floors[i].people[np++].angerLevel = p.getAngerLevel();
        }
    }

    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        Elevator e = elevators[i];

        buildingState.elevators[i].elevatorId = i;
        buildingState.elevators[i].targetFloor = e.getTargetFloor();
        buildingState.elevators[i].currentFloor = e.getCurrentFloor();
        buildingState.elevators[i].isServicing = e.isServicing();
    }

    return buildingState;
}
