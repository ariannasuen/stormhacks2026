#pragma once
#include <string>
using namespace std;

// One selectable reply and the love points it is worth (0, 1 or 2).
class Choice {
public:
    Choice(string t, int p) : text(t), points(p) {}

    string getText() const { return text; }
    int getPoints() const { return points; }

private:
    string text;
    int points;
};
