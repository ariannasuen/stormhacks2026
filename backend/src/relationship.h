#pragma once
#include <string>
#include "character.h"
#include "choice.h"

// Every date is worth 0..6 points (three questions, max 2 each).
// Reaching MAX_POINTS unlocks that person's ending.
constexpr int MAX_POINTS = 6;

// One value per point total 0..6, read as "<status> with you".
enum class RelationshipStatus {
    Hates,       // 0
    Dislikes,    // 1
    Neutral,     // 2
    BeFriends,   // 3
    SecondDate,  // 4
    Likes,       // 5
    Infatuated   // 6
};

inline RelationshipStatus statusFromPoints(int p) {
    if (p < 0) p = 0;
    if (p > MAX_POINTS) p = MAX_POINTS;
    return static_cast<RelationshipStatus>(p);
}

inline string statusText(RelationshipStatus s) {
    switch (s) {
        case RelationshipStatus::Hates:      return "Hates you";
        case RelationshipStatus::Dislikes:   return "Dislikes you";
        case RelationshipStatus::Neutral:    return "Feels neutral about you";
        case RelationshipStatus::BeFriends:  return "Wants to be friends";
        case RelationshipStatus::SecondDate: return "Wants a second date";
        case RelationshipStatus::Likes:      return "Likes you";
        case RelationshipStatus::Infatuated: return "Is infatuated with you!";
    }
    return "";
}

class Relationship {
    public:
        Relationship(Character& p1, Character& p2) : person1(&p1), person2(&p2) {}

        // Point System (always clamped to 0..MAX_POINTS)
        int  getPoints() const { return points; }
        int  addPoints(int value) {
            points += value;
            if (points > MAX_POINTS) points = MAX_POINTS;
            if (points < 0) points = 0;
            return points;
        }
        bool isMaxed() const { return points >= MAX_POINTS; }
        RelationshipStatus getStatus() const { return statusFromPoints(points); }

        const Character& getPlayer()  const { return *person1; }
        const Character& getPartner() const { return *person2; }

    private:
        int points{0};
        Character* person1;
        Character* person2;
};

class Romance      : public Relationship { public: using Relationship::Relationship; };
class Friendship   : public Relationship { public: using Relationship::Relationship; };
class Professional : public Relationship { public: using Relationship::Relationship; };
