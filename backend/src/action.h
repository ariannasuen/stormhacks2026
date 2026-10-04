#pragma once
// Console helpers for showing dialogue and reading the player's input.
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include "choice.h"
using namespace std;

using Opts = vector<Choice>;

// Wait for the player to press Enter
inline void waitForEnter() {
    string s;
    getline(cin, s);
}

// Speaker: "line"   (then wait for Enter)
inline void say(const string& speaker, const string& line) {
    cout << speaker << ": \"" << line << "\"";
    waitForEnter();
}

// Narration / scene description (then wait for Enter)
inline void narrate(const string& text) {
    cout << "\n" << text;
    waitForEnter();
}

inline void header(const string& name) {
    cout << "\n=============== " << name << " ===============\n";
}

// Reads a number 1..n from the player (re-prompts on bad input).
// Returns a 0-based index. If input is closed (EOF) returns 0 so we never loop forever.
inline int readChoice(size_t n) {
    while (true) {
        cout << "> ";
        string input;
        if (!getline(cin, input)) return 0;

        size_t a = input.find_first_not_of(" \t\r");
        size_t b = input.find_last_not_of(" \t\r");
        if (a != string::npos) {
            string t = input.substr(a, b - a + 1);
            bool digits = t.size() <= 3;
            for (char c : t) if (!isdigit(static_cast<unsigned char>(c))) digits = false;
            if (digits) {
                int v = stoi(t);
                if (v >= 1 && v <= (int)n) return v - 1;
            }
        }
        cout << "Please enter a number from 1 to " << n << ".\n";
    }
}

// Shows an optional line from the date, then the numbered replies.
// Points stay hidden. Returns the 0-based index of the chosen reply.
inline int askIndex(const string& speaker, const string& line, const Opts& choices) {
    if (!line.empty())
        cout << "\n" << speaker << ": \"" << line << "\"\n";

    cout << "\nYou:\n";
    for (size_t i = 0; i < choices.size(); i++)
        cout << "  " << (i + 1) << ") " << choices[i].getText() << "\n";

    int pick = readChoice(choices.size());
    cout << "\nYou: " << choices[pick].getText() << "\n";
    return pick;
}

// Convenience: same as askIndex but returns the points of the chosen reply.
inline int ask(const string& speaker, const string& line, const Opts& choices) {
    return choices[askIndex(speaker, line, choices)].getPoints();
}
