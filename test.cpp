/*
 * Copyright 2023 University of Michigan EECS183
 *
 * test.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * Final Project - Elevators
 */
 //* <#Names#>
 //* Daniel Bao, Evan Stocker, Spencer Hu, Zihan Li
 //* <#Uniqnames#>
 //* danbao, zihanl, zianghu, stockere
////////////////////////////////////////////////
// You will submit test.cpp to the autograder //
// FOR LAB 8, but NOT the final project Core  //
////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include "AI.h"
#include "Building.h"
#include "BuildingState.h"
#include "Elevator.h"
#include "Floor.h"
#include "Game.h"
#include "Move.h"
#include "Person.h"
#include "SatisfactionIndex.h"
#include "Utility.h"

using namespace std;

void testFloorAddPerson();
void start_tests();
void testFloorRemovePerson();

//temporary main function
int main() {
    start_tests();
}

// declare your test functions here

void start_tests() {
    testFloorAddPerson();
    testFloorRemovePerson();
}

void testFloorAddPerson() {
    Floor floor;
    Person person("0f1t9a1");

    for (int i = 0; i < MAX_PEOPLE_PER_FLOOR - 1; i++) {
        floor.addPerson(person, person.getTargetFloor() - person.getCurrentFloor());
    }
    cout << "Testing Floor Add Person\n";
    cout << "Expecting f1t9a1, Actual: " << floor.getPersonByIndex(0) << endl;
    cout << "Expecting: Up (1), Actual: " << floor.getHasUpRequest() << endl;
}

void testFloorRemovePerson() {
    Floor floor;
    Person person0("0f1t9a1");
    Person person1("1f0t8a0");
    Person person2("2f0t9a0");
    Person person3("3f0t9a1");
    floor.addPerson(person0, person0.getTargetFloor() - person0.getCurrentFloor());
    floor.addPerson(person1, person1.getTargetFloor() - person1.getCurrentFloor());
    floor.addPerson(person2, person2.getTargetFloor() - person2.getCurrentFloor());
    floor.addPerson(person3, person3.getTargetFloor() - person3.getCurrentFloor());

    int remove[3] = {0,1,2};

    floor.removePeople(remove, 3);
    cout << "Testing Floor Remove Person\n";
    cout << "ExpectingL 3f0t9a1, Actual: " << floor.getPersonByIndex(0) << endl;
}