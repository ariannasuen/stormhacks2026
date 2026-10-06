#pragma once
// All of the game's script. The script is DATA: a list of scenes, each a list of steps.
// Placeholders in text:  {name} = player's name,  {age} = current date's age.
#include <string>
#include <vector>
#include "choice.h"
using namespace std;

enum class StepType {
    Say,        // who: "text"
    Narrate,    // scene description
    Ask,        // optional line from the date, then the player picks a Choice
    TimesUp,    // "Time's Up!" banner
    EnterName   // player types their name
};

struct Step {
    StepType type;
    string who;               // speaker (Say / Ask)
    string text;              // line (Say / Narrate / Ask)
    vector<Choice> choices;   // only for Ask
};

struct Scene {
    string id;                // "intro", "chad", ...
    string title;             // shown as a header ("" = none)
    string characterKey;      // whose relationship earns the points ("" = nobody)
    bool   scored;            // false for intro / outro / Shady
    vector<Step> steps;
};

class Text {
    public:
        // The whole game in order: intro, 9 dates (incl. Shady's unscored one), outroduction.
        static const vector<Scene>& scenes();
        static void test();
};
