/*
 * Copyright 2023 University of Michigan EECS183
 *
 * AI.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <#Names#>
 * Daniel Bao, Evan Stocker, Spencer Hu, Zihan Li
 * <#Uniqnames#>
 * danbao, zihanl, zianghu, stockere
 *
 * Final Project - Elevators
 */

#include "AI.h"
#include <cassert>

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core

string getAIMoveString(const BuildingState& buildingState) {
    int totalPeople = 0;
    for (int i = 0; i < NUM_FLOORS; i++) {
        totalPeople += buildingState.floors[i].numPeople;
    }

    if (totalPeople == 0) {
        return "";
    }

    int numServicing = 0;
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        if (buildingState.elevators[i].isServicing) {
            numServicing++;
        }
    }

    if (numServicing == NUM_ELEVATORS) {
        return "";
    }

    for (int i = 0; i < NUM_ELEVATORS; i++) {
        if (!buildingState.elevators[i].isServicing) {
            int currentFloor = buildingState.elevators[i].currentFloor;
            int peopleOnCurrentFloor = buildingState.floors[currentFloor].numPeople;

            if (peopleOnCurrentFloor > 0) {
                return "e" + to_string(i) + "p";
            }
            int maxPeople = 0;
            int targetFloor = 0;

            for (int j = 0; j < NUM_FLOORS; j++) {
                if (buildingState.floors[j].numPeople > maxPeople) {
                    maxPeople = buildingState.floors[j].numPeople;
                    targetFloor = j;
                }
            }

            return "e" + to_string(i) + "f" + to_string(targetFloor);
        }
    }
    return "";
}

string getAIPickupList(const Move& move, const BuildingState& buildingState,
                       const Floor& floorToPickup) {

    // Below Implementation assumes elevator auto goes to next highest floor when explosion
    // not sure if this will work but it's were using a simple counter and EV calculation need move to test viability
    // read which elevator this is and where it’s going
    int elevatorId = move.getElevatorId();
    int currentFloor = buildingState.elevators[elevatorId].currentFloor;
    int targetFloor = move.getTargetFloor();
    int travelDistance = abs(targetFloor - currentFloor);

    // prepare arrays to hold indices of riders wanting to go up or down
    int numPeople = floorToPickup.getNumPeople();
    int upIndices[MAX_PEOPLE_PER_FLOOR];
    int upCount = 0;
    int downIndices[MAX_PEOPLE_PER_FLOOR];
    int downCount = 0;

    // partition each person into up or down based on their targetFloor
    for (int i = 0; i < numPeople; ++i) {
        Person p = floorToPickup.getPersonByIndex(i);
        int personTarget = p.getTargetFloor();

        if (personTarget > currentFloor) {
            // person wants to go up
            upIndices[upCount] = i;
            upCount++;
        }
        else if (personTarget < currentFloor) {
            // person wants to go down
            downIndices[downCount] = i;
            downCount++;
        }
    }

    // if there are only up requests on this floor, load all of them
    if (upCount > 0 && downCount == 0) {
        string pickupList;
        for (int k = 0; k < upCount; ++k) {
            pickupList += to_string(upIndices[k]);
        }
        return pickupList;
    }

    // if there are only down requests on this floor, load all of them
    if (downCount > 0 && upCount == 0) {
        string pickupList;
        for (int k = 0; k < downCount; ++k) {
            pickupList += to_string(downIndices[k]);
        }
        return pickupList;
    }

    // compute the expected value (EV) of picking all up going riders
    // EV_up = sum of max(0, remaining patience) for each up rider
    int evUp = 0;
    for (int k = 0; k < upCount; ++k) {
        int idx = upIndices[k];
        Person p = floorToPickup.getPersonByIndex(idx);
        int anger = p.getAngerLevel();
        int remain = MAX_ANGER - (anger + travelDistance);
        if (remain > 0) {
            evUp += remain;
        }
    }

    // compute the expected value EV of picking all down‑going riders
    int evDown = 0;
    for (int k = 0; k < downCount; ++k) {
        int idx = downIndices[k];
        Person p = floorToPickup.getPersonByIndex(idx);
        int anger = p.getAngerLevel();
        int remain = MAX_ANGER - (anger + travelDistance);
        if (remain > 0) {
            evDown += remain;
        }
    }

    // compare EVs and choose the direction with the higher score
    string pickupList;
    if (evUp >= evDown) {
        // Up‑direction has greater or equal EV build list of upIndices
        for (int k = 0; k < upCount; ++k) {
            pickupList += to_string(upIndices[k]);
        }
    }
    else {
        // down direction has greater EV build list of downIndices
        for (int k = 0; k < downCount; ++k) {
            pickupList += to_string(downIndices[k]);
        }
    }

    return pickupList;
}


