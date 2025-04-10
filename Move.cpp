/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Move.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <Zihan Li, Spencer Hu, Daniel Bao, Evan Stocker>
 * <zihanl, danbao, zianghu, stockere>
 *
 * Final Project - Elevators
 */
 
#include <cmath>
#include <sstream>
#include <stdio.h>      
#include <stdlib.h>
#include "Move.h"

using namespace std;

Move::Move(string commandString) : Move() {
    // Initialize all boolean flags to false
    isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
   targetFloor = -1;
   elevatorId = -1;
   
string lower_commandString = "";
// initialized a new empty string for recode lowerchar string.

     for (int i=0 ; i < commandString.length(); i++){
        if (isalpha(commandString.at(i))){
            lower_commandString += tolower(commandString.at(i));
        }
        else {
            lower_commandString += commandString.at(i);
        }

}

    if (lower_commandString =="") {
        isPass = true;
        return;
    }

    // Check for single-character commands first
    if (lower_commandString.length() == 1) {
        char cmd = lower_commandString[0];
        if (cmd == 'p') {
            isPass = true;
        }
        else if (cmd == 'q') {
            isQuit = true;
        }
        else if (cmd == 's') {
            isSave = true;
        }
        else if (isdigit(cmd)) {
            // Single digit means elevator 0 and target floor
            elevatorId = 0;
            targetFloor = cmd - '0';
            if (targetFloor < 0 || targetFloor > 9) {
                isPass = true; 
                // Invalid floor, treat as pass
            }
        }
        return;
    }

    // Parse multi-character commands
    if (lower_commandString.length() >= 3) {
        // Check for elevator commands (e1f4, e2p, etc.)
        if (lower_commandString[0] == 'e') {
            // Get elevator ID (should be 0, 1, or 2)
            if (isdigit(lower_commandString[1])) {
                elevatorId = lower_commandString[1] - '0';
                if (elevatorId < 0 || elevatorId > 2) {
                    isPass = true; // Invalid elevator, treat as pass
                    return;
                }

                // Check the command type
                char cmdType = lower_commandString[2];
                if (cmdType == 'f' && lower_commandString.length() >= 4) {
                    // Move to floor command (e1f4)
                    if (isdigit(lower_commandString[3])) {
                        targetFloor = lower_commandString[3] - '0';
                        if (targetFloor < 0 || targetFloor > 9) {
                            isPass = true; // Invalid floor, treat as pass
                        }
                    } else {
                        isPass = true; // Invalid format, treat as pass
                    }
                }
                else if (cmdType == 'p') {
                    // Pickup command (e1p)
                    isPickup = true;
                }
                else {
                    isPass = true; // Unknown command, treat as pass
                }
            } else {
                isPass = true; // Invalid elevator ID, treat as pass
            }
        }
        else {
            isPass = true; // Not an elevator command, treat as pass
        }
    }
    else {
        isPass = true; // Command too short, treat as pass
    }
}


bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
        // Pass, Quit, and Save moves are always valid
        if (isPass || isQuit || isSave) {
            return true;
        }
    
        // For Pickup and Servicing moves, check elevatorId is valid
        if (elevatorId < 0 || elevatorId >= NUM_ELEVATORS) {
            return false;
        }
    
        // Check if the elevator is currently servicing a request
        if (elevators[elevatorId].isServicing()) {
            return false;
        }
    
        // Additional checks for Servicing moves (implied by having a targetFloor)
        if (!isPickup) {  // If it's not a pickup, it's a servicing move
            // Check targetFloor is valid
            if (targetFloor < 0 || targetFloor >= NUM_FLOORS) {
                return false;
            }
            
            // Check targetFloor is different from current floor 
            if (targetFloor == elevators[elevatorId].getCurrentFloor()) {
                return false;
            }

        }
    
        // All checks passed - move is valid
        return true;
    }
   

void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, 
                             const Floor& pickupFloor) {
    numPeopleToPickup = 0;
    totalSatisfaction = 0;

    for (int i = 0; i < pickupList.length(); i++){
        //converts index in peopleToPickup to an integer
        peopleToPickup[i] = static_cast<int>(pickupList.at(i) - '0');
        //adds satisfaction through (MAX_ANGER - current anger level)
        totalSatisfaction += MAX_ANGER - pickupFloor.getPersonByIndex(peopleToPickup[i]).getAngerLevel();
        numPeopleToPickup++;
    }

    int maxFloorDif = -1;
    for (int i = 0; i < numPeopleToPickup; i++) {
        //calculates the absolute difference between the target and current floor
        int tempMaxFloorDif = abs(pickupFloor.getPersonByIndex(peopleToPickup[i]).getTargetFloor() -
                                currentFloor);
        //if there is a greater floor difference, set maxFloorDif to new value
        if (tempMaxFloorDif > maxFloorDif) {
            maxFloorDif = tempMaxFloorDif;
            //set the new target floor to current person
            targetFloor = pickupFloor.getPersonByIndex(peopleToPickup[i]).getTargetFloor();
        }
    }
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Move::Move() {
    elevatorId = -1;
    targetFloor = -1;
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
	isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
}

bool Move::isPickupMove() const {
    return isPickup;
}

bool Move::isPassMove() const {
	return isPass;
}

bool Move::isSaveMove() const {
	return isSave;
}

bool Move::isQuitMove() const {
	return isQuit;
}

int Move::getElevatorId() const {
    return elevatorId;
}

int Move::getTargetFloor() const {
    return targetFloor;
}

int Move::getNumPeopleToPickup() const {
    return numPeopleToPickup;
}

int Move::getTotalSatisfaction() const {
    return totalSatisfaction;
}

void Move::setTargetFloor(int inTargetFloor) {
    targetFloor = inTargetFloor;
}

void Move::copyListOfPeopleToPickup(int newList[MAX_PEOPLE_PER_FLOOR]) const {
    for (int i = 0; i < numPeopleToPickup; ++i) {
        newList[i] = peopleToPickup[i];
    }
}
