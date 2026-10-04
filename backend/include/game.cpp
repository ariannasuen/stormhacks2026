#include "game.h"
#include <algorithm>
#include "action.h"
using namespace std;

namespace {
string replaceAll(string s, const string& from, const string& to) {
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != string::npos) {
        s.replace(pos, from.size(), to);
        pos += to.size();
    }
    return s;
}
}

Game::Game() : user("", "undeclared", 20) {
    // Otters and other side characters
    cast["spendy"]  = make_unique<Side>("Spendy", "host", 18);
    cast["trendy"]  = make_unique<Side>("Trendy", "host", 18);
    cast["stormy"]  = make_unique<Side>("Stormy", "host", 18);
    cast["sparky"]  = make_unique<Side>("Sparky", "host", 18);
    cast["shady"]   = make_unique<Side>("Shady", "host", 18);
    cast["raccoon"] = make_unique<Side>("Raccoon", "Special", 1000);

    // Love interests
    cast["chad"]      = make_unique<Love>("Chad", "business", 24);
    cast["ricky"]     = make_unique<Love>("Ricky", "cs", 30);
    cast["vlada"]     = make_unique<Love>("Vlada", "engineering", 19);   // no scripted date yet
    cast["barth"]     = make_unique<Love>("Barth", "physics", 20);
    cast["gerd"]      = make_unique<Love>("Gerd", "polisci", 18);
    cast["charlie"]   = make_unique<Love>("Charlie", "engineering", 23);
    cast["anastasia"] = make_unique<Love>("Anastasia", "creative writing", 22);  // age assumed
    cast["mackenzie"] = make_unique<Love>("Mackenzie", "communications", 21);
    cast["eighdyn"]   = make_unique<Love>("Eighdyn", "healthsci/premed", 27);

    // One relationship per scored character
    for (const string& k : scoredKeys())
        rels.emplace(k, Relationship(user, *cast.at(k)));
}

string Game::fmt(const string& text) const {
    string age;
    auto it = cast.find(currentKey);
    if (it != cast.end()) age = to_string(it->second->getAge());
    return replaceAll(replaceAll(text, "{name}", user.getName()), "{age}", age);
}

void Game::playScene(const Scene& scene) {
    currentKey = scene.characterKey;
    if (!scene.title.empty()) header(scene.title);

    for (const Step& s : scene.steps) {
        switch (s.type) {
            case StepType::Say:
                say(s.who, fmt(s.text));
                break;
            case StepType::Narrate:
                narrate(fmt(s.text));
                break;
            case StepType::Ask: {
                int idx = askIndex(s.who, fmt(s.text), s.choices);
                if (scene.scored) {
                    auto it = rels.find(scene.characterKey);
                    if (it != rels.end()) it->second.addPoints(s.choices[idx].getPoints());
                }
                break;
            }
            case StepType::TimesUp:
                cout << "\nTime's Up!\n";
                break;
            case StepType::EnterName: {
                cout << "\nYour name > ";
                string n;
                getline(cin, n);
                if (n.empty()) n = "Player";
                user.setName(n);
                cout << "\n";
                break;
            }
        }
    }
}

void Game::showSummary() const {
    cout << "\n=========== Event Summary: Loves & Hates ===========\n";
    vector<string> order = scoredKeys();
    stable_sort(order.begin(), order.end(), [&](const string& a, const string& b) {
        return pointsFor(a) > pointsFor(b);
    });
    for (const string& k : order) {
        int p = pointsFor(k);
        string bar(p, '#');
        bar += string(MAX_POINTS - p, '.');
        string name = cast.at(k)->getName();
        name.resize(10, ' ');
        cout << "  " << name << " [" << bar << "] "
             << statusText(rels.at(k).getStatus()) << "\n";
    }
    cout << "\n(press Enter)";
}

Ending Game::decideEnding() {
    vector<Ending> options = unlockedEndings(allPoints());
    if (options.empty()) return aloneEnding();          // Ending A
    if (options.size() == 1) return options[0];

    cout << "\nSeveral people want you! Whose ending do you want?\n";
    for (size_t i = 0; i < options.size(); i++)
        cout << "  " << (i + 1) << ") Ending " << options[i].letter << " - " << options[i].name << "\n";
    return options[readChoice(options.size())];
}

void Game::showEnding(const Ending& e) const {
    cout << "\n=============== ENDING " << e.letter << " - " << e.name << " ===============\n";
    cout << e.text << "\n\nThe End.\n";
}

Ending Game::run() {
    for (const Scene& sc : Text::scenes()) playScene(sc);
    showSummary();
    waitForEnter();
    Ending e = decideEnding();
    showEnding(e);
    return e;
}

int Game::pointsFor(const string& key) const {
    auto it = rels.find(key);
    return it == rels.end() ? 0 : it->second.getPoints();
}

map<string, int> Game::allPoints() const {
    map<string, int> m;
    for (const auto& kv : rels) m[kv.first] = kv.second.getPoints();
    return m;
}

bool Game::hasCharacter(const string& key) const    { return cast.count(key) > 0; }
bool Game::hasRelationship(const string& key) const { return rels.count(key) > 0; }
