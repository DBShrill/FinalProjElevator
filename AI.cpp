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

    int servicingCount = 0;
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        if (buildingState.elevators[i].isServicing) {
            servicingCount++;
        }
    }

    if (servicingCount == NUM_ELEVATORS) {
        return "";
    }

    for (int i = 0; i < NUM_ELEVATORS; i++) {
        if (!buildingState.elevators[i].isServicing) {
            int currentFloor = buildingState.elevators[i].currentFloor;
            int peopleOnCurrentFloor = buildingState.floors[currentFloor].numPeople;

            if (peopleOnCurrentFloor > 0) {
                return "e" + to_string(i) + "p";
            }

            else {
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
    }
}

string getAIPickupList(const Move& move, const BuildingState& buildingState,
                       const Floor& floorToPickup) {
    return "";
}
