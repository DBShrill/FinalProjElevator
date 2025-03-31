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
    //TODO: Implement non-default constructor

    // Initialize all boolean flags to false
    isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
    numPeopleToPickup = 0;
    totalSatisfaction = 0;

    // Convert command string to lowercase for case-insensitive comparison
    transform(commandString.begin(), commandString.end(), commandString.begin(), ::tolower);

    if (commandString.empty()) {
        isPass = true;
        return;
    }

    // Check for single-character commands first
    if (commandString.length() == 1) {
        char cmd = commandString[0];
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
                isPass = true; // Invalid floor, treat as pass
            }
        }
        return;
    }

    // Parse multi-character commands
    if (commandString.length() >= 3) {
        // Check for elevator commands (e1f4, e2p, etc.)
        if (commandString[0] == 'e') {
            // Get elevator ID (should be 0, 1, or 2)
            if (isdigit(commandString[1])) {
                elevatorId = commandString[1] - '0';
                if (elevatorId < 0 || elevatorId > 2) {
                    isPass = true; // Invalid elevator, treat as pass
                    return;
                }

                // Check the command type
                char cmdType = commandString[2];
                if (cmdType == 'f' && commandString.length() >= 4) {
                    // Move to floor command (e1f4)
                    if (isdigit(commandString[3])) {
                        targetFloor = commandString[3] - '0';
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
}

bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
    //TODO: Implement isValidMove
    
    //Returning false to prevent compilation error
    return false;
}

void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, 
                             const Floor& pickupFloor) {
    //TODO: Implement setPeopleToPickup
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
