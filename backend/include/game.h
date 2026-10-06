#pragma once
#include <map>
#include <memory>
#include <string>
#include "character.h"
#include "relationship.h"
#include "ending.h"
#include "text.h"
using namespace std;

// Owns the cast, the relationships and the flow of one playthrough.
class Game {
    public:
        Game();
        Game(const Game&) = delete;             // relationships point at our characters
        Game& operator=(const Game&) = delete;

        // Plays everything (intro -> dates -> summary -> ending) and returns the ending reached.
        Ending run();

        void   playScene(const Scene& scene);
        void   showSummary() const;
        Ending decideEnding();                  // may prompt the player if several are unlocked
        void   showEnding(const Ending& e) const;

        // Queries (used by main and by the tests)
        int                 pointsFor(const string& key) const;   // 0 if no relationship
        map<string, int>    allPoints() const;
        bool                hasCharacter(const string& key) const;
        bool                hasRelationship(const string& key) const;
        string              playerName() const { return user.getName(); }

    private:
        string fmt(const string& text) const;  // fills in {name} / {age}

        You user;
        map<string, unique_ptr<Character>> cast;
        map<string, Relationship> rels;
        string currentKey;
};
