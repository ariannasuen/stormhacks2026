#pragma once
#include <map>
#include <string>
#include <vector>
#include "relationship.h"
using namespace std;

struct Ending {
    char   letter;   // 'A'..'K'
    string key;      // character key ("chad"), "special" or "alone"
    string name;     // display name
    string text;
};

// Everyone who has a score (and therefore an ending), in script order.
const vector<string>& scoredKeys();

// Endings B..J, one per scored character, in letter order.
const vector<Ending>& characterEndings();

const Ending& aloneEnding();     // Ending A - nobody reached 6 points
const Ending& specialEnding();   // Ending K - everybody reached 6 points

// Every ending the player has unlocked from these points (character key -> points):
// each character with MAX_POINTS, plus Special (K) if ALL scored characters have MAX_POINTS.
// Empty means "no one" - the caller should use aloneEnding().
vector<Ending> unlockedEndings(const map<string, int>& points);
