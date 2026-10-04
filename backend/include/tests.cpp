// Test cases for the SFU Speed Dating game.
// Build & run:   make tests      (or: g++ -std=c++17 tests.cpp character.cpp text.cpp ending.cpp game.cpp -o tests && ./tests)
// No external framework needed. Exit code is 0 only if every check passes.
#include <functional>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "action.h"
#include "character.h"
#include "choice.h"
#include "ending.h"
#include "game.h"
#include "relationship.h"
#include "text.h"
using namespace std;

/* ------------------------------------------------------------------ tiny test framework */
struct TestCase { string name; function<void()> fn; };
static vector<TestCase>& registry() { static vector<TestCase> r; return r; }
struct Registrar { Registrar(const string& n, function<void()> f) { registry().push_back({n, f}); } };

static int g_checks = 0, g_failed = 0;
static string g_current;

#define TEST(name) static void name(); static Registrar reg_##name(#name, name); static void name()
#define CHECK(cond) do { g_checks++; if (!(cond)) { g_failed++; \
    cerr << "  FAIL [" << g_current << "] line " << __LINE__ << ": " #cond "\n"; } } while (0)
#define CHECK_EQ(a, b) do { g_checks++; if (!((a) == (b))) { g_failed++; \
    cerr << "  FAIL [" << g_current << "] line " << __LINE__ << ": " #a " == " #b \
         << "   (got '" << (a) << "' vs '" << (b) << "')\n"; } } while (0)

/* ------------------------------------------------------------------ helpers */
// Replaces cin/cout with strings for the lifetime of the object.
struct IoCapture {
    istringstream in; ostringstream out; streambuf *oldIn, *oldOut;
    explicit IoCapture(const string& input) : in(input) {
        cin.clear();
        oldIn = cin.rdbuf(in.rdbuf());
        oldOut = cout.rdbuf(out.rdbuf());
    }
    ~IoCapture() { cin.rdbuf(oldIn); cout.rdbuf(oldOut); cin.clear(); }
    string output() const { return out.str(); }
};

static bool contains(const string& hay, const string& needle) { return hay.find(needle) != string::npos; }

static int bestIdx(const vector<Choice>& c) {   // first reply with the most points
    int b = 0;
    for (size_t i = 1; i < c.size(); i++) if (c[i].getPoints() > c[b].getPoints()) b = (int)i;
    return b;
}
static int worstIdx(const vector<Choice>& c) {  // first reply with the fewest points
    int w = 0;
    for (size_t i = 1; i < c.size(); i++) if (c[i].getPoints() < c[w].getPoints()) w = (int)i;
    return w;
}

// Builds the keystrokes for a whole playthrough:
//  - for people in maxKeys the best reply is chosen every time, for everyone else the worst
//  - pickIndex (1-based) is the answer to the "whose ending?" menu when it appears
static string makeInput(const set<string>& maxKeys, int pickIndex = 1, const string& name = "Alex") {
    string in;
    for (const Scene& sc : Text::scenes()) {
        for (const Step& s : sc.steps) {
            switch (s.type) {
                case StepType::Say:
                case StepType::Narrate:   in += "\n"; break;
                case StepType::TimesUp:   break;
                case StepType::EnterName: in += name + "\n"; break;
                case StepType::Ask: {
                    int idx = 0;
                    if (sc.scored) idx = maxKeys.count(sc.characterKey) ? bestIdx(s.choices) : worstIdx(s.choices);
                    in += to_string(idx + 1) + "\n";
                    break;
                }
            }
        }
    }
    in += "\n";                                              // "press Enter" after the summary
    size_t unlocked = maxKeys.size() + (maxKeys.size() == scoredKeys().size() ? 1 : 0);
    if (unlocked > 1) in += to_string(pickIndex) + "\n";     // ending menu
    return in;
}

static int maxPointsInScene(const Scene& sc) {
    int total = 0;
    for (const Step& s : sc.steps)
        if (s.type == StepType::Ask) {
            int m = 0;
            for (const Choice& c : s.choices) m = max(m, c.getPoints());
            total += m;
        }
    return total;
}

/* ================================================================== Choice */
TEST(choice_stores_text_and_points) {
    Choice c("Hello", 2);
    CHECK_EQ(c.getText(), string("Hello"));
    CHECK_EQ(c.getPoints(), 2);
}

/* ================================================================== Character */
TEST(character_default_values) {
    Character c;
    CHECK_EQ(c.getName(), string("SkibidiRizz"));
    CHECK_EQ(c.getMajor(), string("engineering"));
    CHECK_EQ(c.getAge(), 67);
}
TEST(character_setters_change_only_their_own_field) {   // regression: setMajor used to overwrite name
    Character c("Bob", "physics", 20);
    c.setMajor("math");
    CHECK_EQ(c.getName(), string("Bob"));
    CHECK_EQ(c.getMajor(), string("math"));
    c.setName("Rob");
    CHECK_EQ(c.getName(), string("Rob"));
    CHECK_EQ(c.getMajor(), string("math"));
    c.setAge(21);
    CHECK_EQ(c.getAge(), 21);
}
TEST(character_subclasses_keep_data) {
    You y("A", "b", 1); Love l("C", "d", 2); Side s("E", "f", 3);
    CHECK_EQ(y.getName(), string("A"));
    CHECK_EQ(l.getAge(), 2);
    CHECK_EQ(s.getMajor(), string("f"));
}
TEST(character_test_prints_all_fields) {
    IoCapture io("");
    Character c("Chad", "business", 24);
    c.test();
    string o = io.output();
    CHECK(contains(o, "Name: Chad"));
    CHECK(contains(o, "Major: business"));
    CHECK(contains(o, "Age: 24"));
}

/* ================================================================== Relationship */
TEST(relationship_starts_at_zero) {
    You u("u", "m", 1); Love l("l", "m", 2);
    Relationship r(u, l);
    CHECK_EQ(r.getPoints(), 0);
    CHECK(!r.isMaxed());
}
TEST(relationship_adds_points) {
    You u("u", "m", 1); Love l("l", "m", 2);
    Relationship r(u, l);
    CHECK_EQ(r.addPoints(2), 2);
    CHECK_EQ(r.addPoints(1), 3);
    CHECK_EQ(r.getPoints(), 3);
}
TEST(relationship_caps_at_six) {
    You u("u", "m", 1); Love l("l", "m", 2);
    Relationship r(u, l);
    r.addPoints(5);
    r.addPoints(5);
    CHECK_EQ(r.getPoints(), MAX_POINTS);
    CHECK(r.isMaxed());
}
TEST(relationship_never_goes_below_zero) {
    You u("u", "m", 1); Love l("l", "m", 2);
    Relationship r(u, l);
    r.addPoints(-3);
    CHECK_EQ(r.getPoints(), 0);
}
TEST(relationship_exposes_both_people) {
    You u("Me", "m", 1); Love l("Chad", "m", 24);
    Relationship r(u, l);
    CHECK_EQ(r.getPlayer().getName(), string("Me"));
    CHECK_EQ(r.getPartner().getName(), string("Chad"));
}
TEST(relationship_status_matches_points) {
    CHECK(statusFromPoints(0) == RelationshipStatus::Hates);
    CHECK(statusFromPoints(1) == RelationshipStatus::Dislikes);
    CHECK(statusFromPoints(2) == RelationshipStatus::Neutral);
    CHECK(statusFromPoints(3) == RelationshipStatus::BeFriends);
    CHECK(statusFromPoints(4) == RelationshipStatus::SecondDate);
    CHECK(statusFromPoints(5) == RelationshipStatus::Likes);
    CHECK(statusFromPoints(6) == RelationshipStatus::Infatuated);
    CHECK(statusFromPoints(99) == RelationshipStatus::Infatuated);   // clamps
    CHECK(statusFromPoints(-4) == RelationshipStatus::Hates);
    CHECK_EQ(statusText(RelationshipStatus::Hates), string("Hates you"));
}

/* ================================================================== Input helpers */
TEST(readChoice_accepts_valid_number) {
    IoCapture io("3\n");
    CHECK_EQ(readChoice(4), 2);
}
TEST(readChoice_reprompts_on_bad_input) {
    IoCapture io("abc\n0\n9\n2abc\n\n-1\n4\n");
    CHECK_EQ(readChoice(4), 3);
    CHECK(contains(io.output(), "Please enter a number from 1 to 4."));
}
TEST(readChoice_trims_whitespace) {
    IoCapture io("  2  \n");
    CHECK_EQ(readChoice(4), 1);
}
TEST(readChoice_returns_zero_on_eof_instead_of_looping) {
    IoCapture io("");
    CHECK_EQ(readChoice(4), 0);
}
TEST(askIndex_shows_line_numbered_options_and_hides_points) {
    IoCapture io("2\n");
    Opts o = { Choice("first", 0), Choice("second", 2) };
    int idx = askIndex("Chad", "Do you invest?", o);
    string out = io.output();
    CHECK_EQ(idx, 1);
    CHECK(contains(out, "Chad: \"Do you invest?\""));
    CHECK(contains(out, "1) first"));
    CHECK(contains(out, "2) second"));
    CHECK(!contains(out, "points"));
}
TEST(ask_returns_points_of_chosen_reply) {
    IoCapture io("2\n");
    Opts o = { Choice("a", 0), Choice("b", 2) };
    CHECK_EQ(ask("X", "Y", o), 2);
}
TEST(say_and_narrate_wait_for_enter) {
    IoCapture io("\n\n");
    say("Spendy", "Hello!");
    narrate("Scene text");
    string o = io.output();
    CHECK(contains(o, "Spendy: \"Hello!\""));
    CHECK(contains(o, "Scene text"));
}

/* ================================================================== Script integrity */
TEST(script_has_expected_scenes_in_order) {
    const vector<string> expected = { "intro","chad","ricky","charlie","barth","gerd",
        "shady","anastasia","mackenzie","raccoon","eighdyn","outro" };
    const auto& sc = Text::scenes();
    CHECK_EQ(sc.size(), expected.size());
    for (size_t i = 0; i < expected.size() && i < sc.size(); i++) CHECK_EQ(sc[i].id, expected[i]);
}
TEST(every_scored_date_is_worth_exactly_six_points) {
    for (const Scene& sc : Text::scenes()) {
        if (!sc.scored) continue;
        g_current = "max points of " + sc.id;
        CHECK_EQ(maxPointsInScene(sc), MAX_POINTS);
    }
    g_current = "every_scored_date_is_worth_exactly_six_points";
}
TEST(every_scored_date_can_also_be_scored_zero) {
    for (const Scene& sc : Text::scenes()) {
        if (!sc.scored) continue;
        int total = 0;
        for (const Step& s : sc.steps) if (s.type == StepType::Ask) total += s.choices[worstIdx(s.choices)].getPoints();
        CHECK_EQ(total, 0);
    }
}
TEST(shady_and_intro_and_outro_give_no_points) {
    for (const Scene& sc : Text::scenes()) {
        if (sc.scored) continue;
        CHECK_EQ(maxPointsInScene(sc), 0);
    }
}
TEST(no_ask_is_empty_and_no_choice_is_out_of_range) {
    for (const Scene& sc : Text::scenes())
        for (const Step& s : sc.steps)
            if (s.type == StepType::Ask) {
                CHECK(!s.choices.empty());
                for (const Choice& c : s.choices) { CHECK(c.getPoints() >= 0 && c.getPoints() <= 2); CHECK(!c.getText().empty()); }
            }
}
TEST(only_name_and_age_placeholders_are_used) {
    for (const Scene& sc : Text::scenes())
        for (const Step& s : sc.steps) {
            string t = s.text;
            size_t p = 0;
            while ((p = t.find('{', p)) != string::npos) {
                bool ok = t.compare(p, 6, "{name}") == 0 || t.compare(p, 5, "{age}") == 0;
                CHECK(ok);
                p++;
            }
        }
}
TEST(every_scored_scene_matches_a_character_and_an_ending) {
    Game g;
    set<string> endingKeys;
    for (const Ending& e : characterEndings()) endingKeys.insert(e.key);
    for (const Scene& sc : Text::scenes()) {
        if (!sc.scored) continue;
        CHECK(g.hasCharacter(sc.characterKey));
        CHECK(g.hasRelationship(sc.characterKey));
        CHECK(endingKeys.count(sc.characterKey) == 1);
    }
}
TEST(exactly_one_name_entry_step_in_the_whole_game) {
    int n = 0;
    for (const Scene& sc : Text::scenes()) for (const Step& s : sc.steps) if (s.type == StepType::EnterName) n++;
    CHECK_EQ(n, 1);
}
TEST(script_has_no_unfinished_placeholders) {     // fails until Eighdyn's 3rd question is written
    int found = 0;
    for (const Scene& sc : Text::scenes())
        for (const Step& s : sc.steps) {
            if (contains(s.text, "PLACEHOLDER")) found++;
            for (const Choice& c : s.choices) if (contains(c.getText(), "[Reply worth")) found++;
        }
    if (found) cerr << "  NOTE: " << found << " placeholder line(s) still need real script text (Eighdyn Q3)\n";
    CHECK_EQ(found, 0);
}

/* ================================================================== Endings */
static map<string, int> zeros() { map<string, int> m; for (auto& k : scoredKeys()) m[k] = 0; return m; }

TEST(endings_cover_letters_B_to_K) {
    string letters;
    for (const Ending& e : characterEndings()) letters += e.letter;
    CHECK_EQ(letters, string("BCDEFGHIJ"));
    CHECK_EQ(aloneEnding().letter, 'A');
    CHECK_EQ(specialEnding().letter, 'K');
}
TEST(no_points_means_no_unlocked_endings) {
    CHECK(unlockedEndings(zeros()).empty());
}
TEST(five_points_is_not_enough) {
    auto p = zeros(); p["chad"] = 5;
    CHECK(unlockedEndings(p).empty());
}
TEST(six_points_unlocks_that_characters_ending) {
    auto p = zeros(); p["ricky"] = 6;
    auto u = unlockedEndings(p);
    CHECK_EQ(u.size(), (size_t)1);
    CHECK_EQ(u[0].letter, 'C');
}
TEST(multiple_maxed_characters_unlock_multiple_endings) {
    auto p = zeros(); p["chad"] = 6; p["barth"] = 6; p["raccoon"] = 6;
    auto u = unlockedEndings(p);
    CHECK_EQ(u.size(), (size_t)3);
    CHECK_EQ(u[0].letter, 'B');
    CHECK_EQ(u[1].letter, 'E');
    CHECK_EQ(u[2].letter, 'J');
}
TEST(special_needs_everyone_maxed) {
    auto p = zeros();
    for (auto& k : scoredKeys()) p[k] = 6;
    auto u = unlockedEndings(p);
    CHECK_EQ(u.size(), scoredKeys().size() + 1);
    CHECK_EQ(u.back().letter, 'K');

    p["gerd"] = 5;                       // one person short -> no Special
    for (auto& e : unlockedEndings(p)) CHECK(e.letter != 'K');
}
TEST(missing_keys_count_as_zero) {
    map<string, int> p;
    CHECK(unlockedEndings(p).empty());
}

/* ================================================================== Game (full playthroughs) */
TEST(game_starts_with_zero_points_for_everyone) {
    Game g;
    for (auto& k : scoredKeys()) CHECK_EQ(g.pointsFor(k), 0);
    CHECK_EQ(g.allPoints().size(), scoredKeys().size());
    CHECK(g.hasCharacter("vlada"));
    CHECK(!g.hasRelationship("vlada"));      // Vlada has no scripted date yet
}
TEST(playthrough_all_worst_answers_gives_ending_A) {
    IoCapture io(makeInput({}));
    Game g;
    Ending e = g.run();
    CHECK_EQ(e.letter, 'A');
    for (auto& k : scoredKeys()) CHECK_EQ(g.pointsFor(k), 0);
    CHECK(contains(io.output(), "No love complications, that is."));
}
TEST(playthrough_one_person_maxed_goes_straight_to_their_ending) {
    IoCapture io(makeInput({"barth"}));
    Game g;
    Ending e = g.run();
    CHECK_EQ(e.letter, 'E');
    CHECK_EQ(g.pointsFor("barth"), 6);
    CHECK_EQ(g.pointsFor("chad"), 0);
    CHECK(!contains(io.output(), "Whose ending do you want?"));
    CHECK(contains(io.output(), "radiation poisoning"));
}
TEST(playthrough_two_maxed_shows_menu_and_respects_choice) {
    {   IoCapture io(makeInput({"chad", "ricky"}, 2));
        Game g;
        Ending e = g.run();
        CHECK_EQ(e.letter, 'C');
        CHECK(contains(io.output(), "Whose ending do you want?"));
        CHECK(contains(io.output(), "Ending B - Chad"));
        CHECK(contains(io.output(), "Ending C - Ricky"));
    }
    {   IoCapture io(makeInput({"chad", "ricky"}, 1));
        Game g;
        CHECK_EQ(g.run().letter, 'B');
    }
}
TEST(playthrough_raccoon_only_gives_ending_J) {
    IoCapture io(makeInput({"raccoon"}));
    Game g;
    CHECK_EQ(g.run().letter, 'J');
    CHECK_EQ(g.pointsFor("raccoon"), 6);
}
TEST(playthrough_everyone_maxed_offers_special_K) {
    set<string> all(scoredKeys().begin(), scoredKeys().end());
    IoCapture io(makeInput(all, 10));
    Game g;
    Ending e = g.run();
    CHECK_EQ(e.letter, 'K');
    for (auto& k : scoredKeys()) CHECK_EQ(g.pointsFor(k), 6);
    CHECK(contains(io.output(), "Ending K - Special"));
}
TEST(playthrough_fills_in_name_and_ages) {
    IoCapture io(makeInput({}, 1, "Jordan"));
    Game g;
    g.run();
    string o = io.output();
    CHECK_EQ(g.playerName(), string("Jordan"));
    CHECK(contains(o, "Nice to meet you Jordan!"));
    CHECK(contains(o, "This here is Chad, 24,"));
    CHECK(contains(o, "This is Gerd, 18."));
    CHECK(!contains(o, "{name}"));
    CHECK(!contains(o, "{age}"));
}
TEST(empty_name_falls_back_to_Player) {
    IoCapture io(makeInput({}, 1, ""));
    Game g;
    g.run();
    CHECK_EQ(g.playerName(), string("Player"));
}
TEST(shady_scene_never_changes_any_score) {
    // Choose the "best" (first-listed) reply during Shady's scene; scores must stay 0.
    IoCapture io(makeInput({}));
    Game g;
    g.run();
    int sum = 0;
    for (auto& kv : g.allPoints()) sum += kv.second;
    CHECK_EQ(sum, 0);
}
TEST(summary_lists_everyone_with_a_status) {
    IoCapture io(makeInput({"gerd"}));
    Game g;
    g.run();
    string o = io.output();
    CHECK(contains(o, "Event Summary: Loves & Hates"));
    CHECK(contains(o, "Gerd"));
    CHECK(contains(o, "[######] Is infatuated with you!"));
    CHECK(contains(o, "[......] Hates you"));
}
TEST(invalid_input_during_a_date_does_not_break_scoring) {
    // Type junk before the first real answer of Chad's gym question.
    string in;
    for (const Scene& sc : Text::scenes()) {
        for (const Step& s : sc.steps) {
            if (s.type == StepType::Say || s.type == StepType::Narrate) in += "\n";
            else if (s.type == StepType::EnterName) in += "Alex\n";
            else if (s.type == StepType::Ask) {
                if (sc.id == "chad" && s.text.find("gym") != string::npos) in += "banana\n9\n";   // junk first
                in += "1\n";
            }
        }
    }
    in += "\n";
    IoCapture io(in);
    Game g;
    g.run();
    // gym reply #1 is worth 2, everything else in Chad's date is reply #1: greet 0, crypto 1, club 1
    CHECK_EQ(g.pointsFor("chad"), 2 + 1 + 1);
    CHECK(contains(io.output(), "Please enter a number from 1 to 4."));
}

/* ------------------------------------------------------------------ runner */
int main(int argc, char** argv) {
    string only = argc > 1 ? argv[1] : "";     // optional: ./tests <substring> runs matching tests
    int ran = 0, failedTests = 0;
    for (auto& t : registry()) {
        if (!only.empty() && t.name.find(only) == string::npos) continue;
        g_current = t.name;
        int before = g_failed;
        t.fn();
        ran++;
        bool ok = g_failed == before;
        if (!ok) failedTests++;
        cout << (ok ? "[ OK ] " : "[FAIL] ") << t.name << "\n";
    }
    cout << "\n" << ran << " tests, " << g_checks << " checks, " << failedTests << " failing test(s)\n";
    return g_failed ? 1 : 0;
}
