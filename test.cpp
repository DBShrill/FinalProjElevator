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
void testPerson();
void testElevator();
void testMove();
void testFloor();
void testGame();


//temporary main function
int main() {
    start_tests();
}

// declare your test functions here

void start_tests() {
    testFloorAddPerson();
    testFloorRemovePerson();
    testPerson();
    testElevator();
    testMove();
    testFloor();
    // testGame();
}

void testFloorAddPerson() {
    Floor floor;
    Person person("0f1t9a1");

    for (int i = 0; i < MAX_PEOPLE_PER_FLOOR - 1; i++) {
        floor.addPerson(person, person.getTargetFloor() - person.getCurrentFloor());
    }
    cout << "// Testing Floor Add Person\n";
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
    cout << "// Testing Floor Remove Person\n";
    cout << "ExpectingL 3f0t9a1, Actual: " << floor.getPersonByIndex(0) << endl;
}

void testPerson() {
    cout << "// Testing Person class\n";

    // Test constructor and getters
    Person p("7f4t8a3");
    cout << "Expecting turn: 7, Actual: " << p.getTurn() << endl;
    cout << "Expecting currentFloor: 4, Actual: " << p.getCurrentFloor() << endl;
    cout << "Expecting targetFloor: 8, Actual: " << p.getTargetFloor() << endl;
    cout << "Expecting angerLevel: 3, Actual: " << p.getAngerLevel() << endl;

    // Test tick - not increasing anger if not on tick interval
    cout << "// Testing tick at time 1 (no anger increase expected)" << endl;
    p.tick(1);
    cout << "Expecting angerLevel: 3, Actual: " << p.getAngerLevel() << endl;

    // Test tick - increase anger at TICKS_PER_ANGER_INCREASE
    cout << "// Testing tick at time 5 (anger should increase by 1)" << endl;
    bool result = p.tick(5);
    cout << "Expecting angerLevel: 4, Actual: " << p.getAngerLevel() << endl;
    cout << "Expecting result: false, Actual: " << result << endl;

    // Keep ticking until MAX_ANGER - 1 is reached
    while (p.getAngerLevel() < MAX_ANGER - 1) {
        result = p.tick(p.getAngerLevel() * TICKS_PER_ANGER_INCREASE);
    }
    cout << "// Testing tick when reaching MAX_ANGER - 1" << endl;
    cout << "Expecting angerLevel: " << MAX_ANGER - 1 << ", Actual: " << p.getAngerLevel() << endl;
    cout << "Expecting result: true, Actual: " << result << endl;

    // Test print
    cout << "// Testing print\n";
    cout << "Expecting: f4t8a" << p.getAngerLevel() << ", Actual: ";
    p.print(cout);
    cout << endl;

    // Test operator<< overload
    cout << "// Testing << operator overload\n";
    cout << "Expecting: f4t8a" << p.getAngerLevel() << ", Actual: " << p << endl;
}

void testElevator() {
    cout << "// Testing Elevator class\n";

    Elevator e;

    // Test initial state
    cout << "Expecting currentFloor: 0, Actual: " << e.getCurrentFloor() << endl;
    cout << "Expecting isServicing: 0, Actual: " << e.isServicing() << endl;

    // Test serviceRequest
    e.serviceRequest(5);
    cout << "After serviceRequest(5)\n";
    cout << "Expecting targetFloor: 5, Actual: " << e.getTargetFloor() << endl;
    cout << "Expecting isServicing: 1, Actual: " << e.isServicing() << endl;

    // Test tick - simulate multiple ticks until reaching target
    int currentTime = 0;
    while (e.isServicing()) {
        e.tick(currentTime);
        cout << "Tick time " << currentTime << ", Floor: " << e.getCurrentFloor() << endl;
        currentTime++;
    }

    // Test going downward
    e.serviceRequest(2);
    for (int time = 0; e.isServicing(); ++time) {
        e.tick(time * TICKS_PER_ELEVATOR_MOVE);
        cout << "Tick time " << time * TICKS_PER_ELEVATOR_MOVE << ", Floor: " << e.getCurrentFloor() << endl;
    }
    cout << "Expecting final floor: 2, Actual: " << e.getCurrentFloor() << endl;
    cout << "Expecting isServicing: 0, Actual: " << e.isServicing() << endl;

    // Test print
    cout << "// Testing print()\n";
    cout << "Expecting: 2w, Actual: ";
    e.print(cout);
    cout << endl;
}

void testMove() {
    cout << "// Testing Move class\n";

    Elevator elevators[NUM_ELEVATORS];
    elevators[0].setCurrentFloor(0);
    elevators[1].setCurrentFloor(2);
    elevators[2].setCurrentFloor(5);

    // Test pass command
    Move move1("p");
    cout << "Expecting isPass: 1, Actual: " << move1.isPassMove() << endl;
    cout << "Expecting isValid: 1, Actual: " << move1.isValidMove(elevators) << endl;

    // Test quit command
    Move move2("q");
    cout << "Expecting isQuit: 1, Actual: " << move2.isQuitMove() << endl;
    cout << "Expecting isValid: 1, Actual: " << move2.isValidMove(elevators) << endl;

    // Test save command
    Move move3("s");
    cout << "Expecting isSave: 1, Actual: " << move3.isSaveMove() << endl;
    cout << "Expecting isValid: 1, Actual: " << move3.isValidMove(elevators) << endl;

    // Test move to floor command (valid)
    Move move4("e1f4");
    cout << "Expecting elevatorId: 1, Actual: " << move4.getElevatorId() << endl;
    cout << "Expecting targetFloor: 4, Actual: " << move4.getTargetFloor() << endl;
    cout << "Expecting isValid: 1, Actual: " << move4.isValidMove(elevators) << endl;

    // Test move to floor command (invalid - same floor)
    Move move5("e1f2");
    cout << "Expecting isValid: 0, Actual: " << move5.isValidMove(elevators) << endl;

    // Test pickup command
    Move move6("e0p");
    cout << "Expecting isPickup: 1, Actual: " << move6.isPickupMove() << endl;
    cout << "Expecting isValid: 1, Actual: " << move6.isValidMove(elevators) << endl;

    // Test invalid elevator
    Move move7("e9f2");
    cout << "Expecting isValid: 0, Actual: " << move7.isValidMove(elevators) << endl;

    // Test single digit move command (elevator 0 goes to floor 3)
    Move move8("3");
    cout << "Expecting elevatorId: 0, Actual: " << move8.getElevatorId() << endl;
    cout << "Expecting targetFloor: 3, Actual: " << move8.getTargetFloor() << endl;
    cout << "Expecting isValid: 1, Actual: " << move8.isValidMove(elevators) << endl;
}

void testFloor() {
    cout << "// Testing Floor class\n";
    cout << boolalpha;

    // --- Test: Add person going up ---
    Floor floorUp;
    Person p1("0f0t3a0");  // currentFloor 0 -> targetFloor 3 (up)
    floorUp.addPerson(p1, p1.getTargetFloor() - p1.getCurrentFloor());

    cout << "Expecting numPeople: 1, Actual: " << floorUp.getNumPeople() << endl;
    cout << "Expecting hasUpRequest: true, Actual: " << floorUp.getHasUpRequest() << endl;
    cout << "Expecting hasDownRequest: false, Actual: " << floorUp.getHasDownRequest() << endl;

    // --- Test: Add person going down ---
    Floor floorDown;
    Person p2("1f5t2a0");  // currentFloor 5 -> targetFloor 2 (down)
    floorDown.addPerson(p2, p2.getTargetFloor() - p2.getCurrentFloor());

    cout << "Expecting numPeople: 1, Actual: " << floorDown.getNumPeople() << endl;
    cout << "Expecting hasUpRequest: false, Actual: " << floorDown.getHasUpRequest() << endl;
    cout << "Expecting hasDownRequest: true, Actual: " << floorDown.getHasDownRequest() << endl;

    // --- Test: Add person close to explosion and tick ---
    Floor floorExplode;
    Person p3("2f2t8a" + to_string(MAX_ANGER - 1)); // anger = MAX_ANGER - 1
    floorExplode.addPerson(p3, p3.getTargetFloor() - p3.getCurrentFloor());

    cout << "Before tick, numPeople: " << floorExplode.getNumPeople() << endl;
    int exploded = floorExplode.tick(MAX_ANGER * TICKS_PER_ANGER_INCREASE);
    cout << "Expecting exploded: 1, Actual: " << exploded << endl;
    cout << "Expecting numPeople after tick: 0, Actual: " << floorExplode.getNumPeople() << endl;

    // --- Test: Remove multiple people ---
    Floor floorRemove;
    Person p4("0f1t9a1");
    Person p5("1f0t8a0");
    Person p6("2f0t9a0");
    Person p7("3f0t9a1");

    floorRemove.addPerson(p4, p4.getTargetFloor() - p4.getCurrentFloor());
    floorRemove.addPerson(p5, p5.getTargetFloor() - p5.getCurrentFloor());
    floorRemove.addPerson(p6, p6.getTargetFloor() - p6.getCurrentFloor());
    floorRemove.addPerson(p7, p7.getTargetFloor() - p7.getCurrentFloor());

    int toRemove[3] = { 0, 1, 2 };
    floorRemove.removePeople(toRemove, 3);

    cout << "Expecting numPeople after removal: 1, Actual: " << floorRemove.getNumPeople() << endl;
    cout << "Expecting remaining person: ";
    floorRemove.getPersonByIndex(0).print(cout);
    cout << endl;

    // --- Visual confirmation of pickup menu ---
    cout << "// Testing printFloorPickupMenu()\n";
    floorRemove.printFloorPickupMenu(cout);

    
}

// void testGame() {
// // --- Test: Game::isValidPickUpList---
// Game game;
//
// // Test Case 1: Duplicate Indices
// cout << "Test 1 - Duplicate indices (Expecting: false): "
//      << game.isValidPickupList("011", 1) << endl;
// cout << "Test 1 - Non-consecutive duplicates (Expecting: false): "
//      << game.isValidPickupList("101", 1) << endl;
// cout << "Test 1 - Valid no duplicates (Expecting: true): "
//      << game.isValidPickupList("012", 1) << endl << endl;
//
//
// // Test Case 2: Non-Digit or Out-of-Range Characters
// cout << "Test 2 - Contains letter (Expecting: false): "
//      << game.isValidPickupList("a12", 1) << endl;
// cout << "Test 2 - Contains symbol (Expecting: false): "
//      << game.isValidPickupList("12:", 1) << endl;
// cout << "Test 2 - Valid digits only (Expecting: true): "
//      << game.isValidPickupList("123", 1) << endl << endl;
//
//
// // Test Case 3: Exceeds Elevator Capacity
// cout << "Test 3 - Exceeds capacity (Expecting: false): "
//      << game.isValidPickupList("01234567890", 1) << endl;
// cout << "Test 3 - At capacity limit (Expecting: true): "
//      << game.isValidPickupList("0123456789", 1) << endl << endl;
//
//
// // Test Case 4: Invalid Floor Indices
// cout << "Test 4 - Index too high (Expecting: false): "
//      << game.isValidPickupList("05", 1) << endl;  // Assuming 5 people on floor
// cout << "Test 4 - All indices valid (Expecting: true): "
//      << game.isValidPickupList("012", 1) << endl << endl;
//
//
// // Test Case 5: Mixed Directions
// cout << "Test 5 - Mixed directions (Expecting: false): "
//      << game.isValidPickupList("01", 1) << endl;  // Assuming 0=up, 1=down
// cout << "Test 5 - Same direction (Expecting: true): "
//      << game.isValidPickupList("123", 5) << endl << endl;
//
//
// // Bonus Edge Case
// cout << "Test 6 - Target equals current floor (Expecting: false): "
//      << game.isValidPickupList("0", 1) << endl;  // Person not moving
// }
