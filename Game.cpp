﻿/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Game.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * Daniel Bao, Evan Stocker, Spencer Hu, Zihan Li
 * danbao, zihanl, zianghu, stockere
 *
 * Final Project - Elevators
 */
#include <random>
#include <sstream>
#include "Game.h"
#include "AI.h"
#include "Utility.h"
using namespace std;
// Stub for playGame for Core, which plays random games
// You *must* revise this function according to the RME and spec
// Code that will not appear in your solution is noted in the comments
void Game::playGame(bool isAIModeIn, ifstream& gameFile) {
    // Step 1: Exit if game file isn't open
    if (!gameFile.is_open()) {
        exit(1);
    }

    // Step 2: Set AI mode
    isAIMode = isAIModeIn;

    // Step 3: Print game start prompt
    printGameStartPrompt();

    // Step 4: Initialize game using the file
    initGame(gameFile);

    // Step 5: Prepare for reading person events
    string nextLine;
    Person nextPerson;
    bool hasBufferedPerson = false;

    // Main game loop
    while (true) {
        int currentTime = building.getTime();

        // Step 6: Read and spawn all people scheduled for this tick
        while (true) {
            if (!hasBufferedPerson) {
                if (!getline(gameFile, nextLine)) break;  // no more people
                if (nextLine.empty()) continue;           // skip empty lines

                nextPerson = Person(nextLine);
                hasBufferedPerson = true;
            }

            if (nextPerson.getTurn() > currentTime) {
                break;  // wait for future tick
            }

            // Spawn person for this tick
            building.spawnPerson(nextPerson);
            checkForGameEnd();  // catch explosions from spawn
            hasBufferedPerson = false;
        }

        // Step 7: Show state, check end, get move
        building.prettyPrintBuilding(cout);
        satisfactionIndex.printSatisfaction(cout, false);
        checkForGameEnd();

        Move nextMove = getMove();
        update(nextMove);
    }
}

// Stub for isValidPickupList for Core
// You *must* revise this function according to the RME and spec
bool Game::isValidPickupList(const string& pickupList,
                             const int pickupFloorNum) const {
               // 1. Check for duplicate indices
            for (int i = 0; i < pickupList.size(); i++) {
                    for (int j = i+1; j < pickupList.size(); j++) {
                        if (pickupList[i] == pickupList[j]) {
                return false;
            }
        }
    }
            // 2. Check each character is a digit between '0'-'9'
            for (int k =0; k < pickupList.size(); k++){
                if (!isdigit(pickupList.at(k))){
                    return false;
                }
            }
            ////3. The length of the pickupList is less than or equal to the capacity of an elevator
            if (pickupList.size() > ELEVATOR_CAPACITY ){
                return false;
            }
            // Get the floor and its people count
            Floor pickupFloor = building.getFloorByFloorNum(pickupFloorNum);
            int numPeople = pickupFloor.getNumPeople();
            // 4. Check all indices are valid (less than number of people on floor)
            for (int i = 0; i < pickupList.size(); i++) {
                if (numPeople <= (pickupList[i] - '0')){
                    return false;
                  }
    }
        // 5. Check all selected people are going in same direction
        string direction = "up";
        Person p1 = pickupFloor.getPersonByIndex(pickupList[0] - '0');
        if (p1.getTargetFloor() - p1.getCurrentFloor() < 0) {
          direction = "down";
        }
        if (direction == "up") {
          for (int i = 0; i < pickupList.length(); i++) {
            Person p2 = pickupFloor.getPersonByIndex(pickupList[i] - '0');
            if (p2.getTargetFloor() - p2.getCurrentFloor() < 0) {
              return false;
            }
          }
        }
        if (direction == "down") {
          for (int i = 0; i < pickupList.length(); i++) {
            Person p = pickupFloor.getPersonByIndex(pickupList[i] - '0');
            if (p.getTargetFloor() - p.getCurrentFloor() > 0) {
              return false;
            }
          }
        }
    return true;
}


//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

bool Game::performMove(Move& move) const
{
    Elevator elevators[NUM_ELEVATORS];

    for (int i = 0; i < NUM_ELEVATORS; ++i)
    {
        elevators[i] = building.getElevatorById(i);
    }

    if (move.isValidMove(elevators) && move.isPickupMove())
    {

        Elevator taggedElevator = building.getElevatorById(move.getElevatorId());
        Floor taggedFloor =
                  building.getFloorByFloorNum(taggedElevator.getCurrentFloor());

        if (taggedFloor.getNumPeople() > 0)
        {
            getPeopleToPickup(move);
            return true;
        }

        return false;
    }
    else if (move.isSaveMove())
    {
        performSave();
    }
    else if (move.isQuitMove())
    {
        performQuit();
    }
    else if (!move.isValidMove(elevators))
    {
        return false;
    }

    return true;
}

void Game::printWelcomeMenu() const
{
    cout << "########################################" << endl;
    cout << "# Welcome to EECS 183 Elevator Escape! #" << endl;
    cout << "########################################" << endl
        << endl;
}

void Game::printIntroMenu() const
{
    cout << "[0] Run Tests" << endl;
    cout << "[1] Load saved game" << endl;
    cout << "[2] Start new game" << endl;
    cout << "Choice: ";
}

void Game::printAIMenu() const {
    cout << "[1] Play as human" << endl;
    cout << "[2] Watch AI play" << endl;
    cout << "Choice: ";
}

void Game::printOptions() const
{
    cout << "########################################" << endl;
    cout << "#         Enter service move:          #" << endl;
    cout << "#     'e' elevatorId 'f' floorNum      #" << endl;
    cout << "#            Or pickup move:           #" << endl;
    cout << "#          'e' elevatorId 'p'          #" << endl;
    cout << "########################################" << endl;
}

void Game::printGameStartPrompt() const
{
    cout << endl;
    cout << "########################################" << endl;
    cout << "#                 Enter:               #" << endl;
    cout << "#               Q to Quit              #" << endl;
    cout << "#               S to Save              #" << endl;
    cout << "#      At any time after this prompt   #" << endl;
    cout << "########################################" << endl;
    cout << endl;
    cout << "Press enter to continue....";

    string trash;
    getline(cin, trash);
    cout << endl;
}

IntroChoice Game::getIntroChoice() const {

    if (IS_AI_OVERRIDE) {
        return IntroChoice::New;
    }

    int choice = -1;

    while ((choice < 0) || (choice > 2))
    {
        printIntroMenu();
        string s_choice;
        getline(cin, s_choice);
        choice = stoi(s_choice);

        if ((choice < 0) || (choice > 2))
        {
            cout << endl
                << "Invalid menu choice!" << endl
                << endl;
        }
    }

    cout << endl;

    return static_cast<IntroChoice>(choice);
}

AIChoice Game::getAIChoice() const {
    if (IS_AI_OVERRIDE) {
        return AIChoice::AI;
    }

    int choice = 0;
    printAIMenu();
    string s_choice;
    getline(cin, s_choice);
    choice = stoi(s_choice);
    while (choice != 1 && choice != 2) {
        cout << endl << "Invalid menu choice!" << endl << endl;
        getline(cin, s_choice);
        choice = stoi(s_choice);
    }

    cout << endl;

    return static_cast<AIChoice>(choice);
}

void Game::printSatisfactionIndex() const
{
    cout << endl
        << endl;
    cout << "Your hotel's current satisfaction index is: " << satisfactionIndex;
    cout << endl
        << endl;
}

void Game::getPeopleToPickup(Move& move) const
{
    int currentFloorNum =
               building.getElevatorById(move.getElevatorId()).getCurrentFloor();
    Floor currentFloor = building.getFloorByFloorNum(currentFloorNum);

    if (currentFloor.getNumPeople() == 1)
    {
        move.setPeopleToPickup("0", currentFloorNum, currentFloor);
        return;
    }

    string peopleIndices = "";
    bool isValidPickup = false;

    while (!isValidPickup)
    {

        currentFloor.printFloorPickupMenu(cout);
        cout << endl;
        cout << "Choices: ";
        getline(cin, peopleIndices);
        cout << "\n";

        if (isAIMode) {
            Elevator e = building.getElevatorById(move.getElevatorId());
            Floor f = building.getFloorByFloorNum(e.getCurrentFloor());
            peopleIndices = getAIPickupList(move, building.getBuildingState(), f);
            cout << "AI OVERRIDE: " << peopleIndices << endl;
        }

        isValidPickup = isValidPickupList(peopleIndices, currentFloorNum);

        if (!isValidPickup)
        {
            cout << "Invalid list of people chosen!" << endl;
            checkForGameEnd();
        }
    }

    move.setPeopleToPickup(peopleIndices, currentFloorNum, currentFloor);
}

Move Game::getMove() const
{
    Move moveObj;
    string moveString = "";
    bool isValid = false;
    GameChoice choiceCapture = GameChoice::Invalid;

    do
    {
        cout << "Enter move: ";

        getline(cin, moveString);
        cout << "\n";

        if (isAIMode) {
            moveString = getAIMoveString(building.getBuildingState());
            cout << "AI OVERRIDE: " << moveString << endl;
        }
        moveObj = Move(moveString);
        choiceCapture = static_cast<GameChoice>(moveString[0]);

        switch (choiceCapture)
        {
        case GameChoice::Quit:
            performQuit();
            break;
        case GameChoice::Save:
            isValid = performSave();
            break;
        case GameChoice::Pass:
        case GameChoice::Move:
            isValid = performMove(moveObj);
            break;
        default:
            isValid = false;
            break;
        }

        if (!isValid)
        {
            cout << endl
                << "Invalid move!" << endl
                << endl;
            checkForGameEnd();
        }

    } while (!isValid);

    return moveObj;
}

void Game::performQuit() const
{
    QuitChoice quitChoice = static_cast<QuitChoice>(getExitChoice());
    switch (quitChoice)
    {
    case QuitChoice::Quit:
        endGame();
        break;
    case QuitChoice::Stay:
        return;
        break;
    }
}

bool Game::performSave() const
{
    ofstream saveFile(LOAD_FILENAME);
    if (!saveFile.is_open())
    {
        cout << "Could not open " << LOAD_FILENAME << endl;
        return false;
    }
    return saveGame(saveFile);
}

void Game::initGame(ifstream& loadFile)
{
    cout << endl
        << "Loading game..." << endl;

    loadFile >> satisfactionIndex;

    int tempTime = 0;
    loadFile >> tempTime;

    building.setTime(tempTime);

    string elvState;
    int i = 0;
    while (i < NUM_ELEVATORS && loadFile >> elvState) {
        building.setElevator(elvState, i);
        i++;
    }

    if (!loadFile.fail()) {
        cout << "Loaded!" << endl << endl;
    }
    else {
        cout << endl << "Error when loading the file! "
            << "Verify that you have correctly created your project."
            << endl << endl;
        cout << "You will need to fix this before implementing Game::playGame "
            << "or the Reach AI." << endl << endl;
        cout << "Press enter to continue....";
        string junk;
        getline(cin, junk);
        cout << endl;
    }
    return;
}

void Game::printExitMenu() const
{
    cout << endl;
    cout << "Are you sure you want to quit?" << endl;
    cout << "[1] Yes, I'm sure!" << endl;
    cout << "[2] No, keep playing!" << endl;
    cout << "Choice: ";
}

int Game::getExitChoice() const
{
    int exitChoice = 0;

    while ((exitChoice < 1) || (exitChoice > 2))
    {
        printExitMenu();

        string choice = "";
        getline(cin, choice);
        exitChoice = stoi(choice);

        if ((exitChoice < 1) || (exitChoice > 2))
        {
            cout << endl
                << "Invalid menu choice!" << endl
                << endl;
        }
    }
    cout << endl;

    return exitChoice;
}

void Game::endGame() const
{
    cout << endl
        << "Goodbye!" << endl;
    satisfactionIndex.save();
    exit(0);
}

void Game::printSuccessEnding() const
{
    cout << endl
        << endl;
    cout << "Time is up! Your final score is: " << endl;
    satisfactionIndex.printSatisfaction(cout);
}

void Game::printFailureEnding() const
{
    cout << endl
        << endl;
    cout << "Uh oh! Your hotel has gone out of business!" << endl;
    cout << "Better luck next time!" << endl
        << endl;
    cout << "Your final achievement:" << endl
        << endl;
    satisfactionIndex.printSatisfaction(cout);
}

void Game::checkForGameEnd() const
{
    if (building.getTime() >= MAX_TURNS)
    {
        printSuccessEnding();
        endGame();
    }
    else if (satisfactionIndex.getSatisfaction() < 0)
    {
        printFailureEnding();
        endGame();
    }
}

void Game::update(const Move& m)
{
    if (m.isPickupMove())
    {
        satisfactionIndex.updateSumDirectionRequest(m, building);
    }
    satisfactionIndex.updateSumExploded(building.tick(m));
    satisfactionIndex.updateTimeReached(building);
    if (building.getTime() % 2)
    {
        satisfactionIndex.updateSumIdle(building);
    }
}

// Stub for saveGame for project
// You will *not* implement this function for the project
bool Game::saveGame(ofstream& saveFile) const {
    return true;
}