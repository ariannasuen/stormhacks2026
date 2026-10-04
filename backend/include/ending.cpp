#include "ending.h"

const vector<string>& scoredKeys() {
    static const vector<string> k = {
        "chad", "ricky", "charlie", "barth", "gerd",
        "anastasia", "mackenzie", "eighdyn", "raccoon"
    };
    return k;
}

const vector<Ending>& characterEndings() {
    static const vector<Ending> e = {
        {'B', "chad",      "Chad",      "You and Chad date for a while until he decides after a week that you're holding him back from his potential."},
        {'C', "ricky",     "Ricky",     "You date Ricky for a little bit, but ultimately a disagreement over the superior coding language ends your love story. He codes in assembly, and you prefer to appreciate the simplicity of python."},
        {'D', "charlie",   "Charlie",   "You two go on a few dates, but eventually she realizes she doesn't have time for you. It's busy out there for an Engineering student, and those 5 labs are catching up to her."},
        {'E', "barth",     "Barth",     "Unfortunately Barth's missing thallium-204 was actually in his pocket, irradiating the two of you gradually over time. You both get radiation poisoning."},
        {'F', "gerd",      "Gerd",      "Although it seems like a good match at first, in the end, Gerd ends up nerding you out. The relationship does not last."},
        {'G', "anastasia", "Anastasia", "Anastasia reinvented her aesthetic after two months and you didn't quite fit her new muse concept. Safe to say the concept of the relationship didn't last either.."},
        {'H', "mackenzie", "Mackenzie", "She bled both your wallet and your heart dry and soon enough, you had nothing left to give. Ghosted after 6 weeks."},
        {'I', "eighdyn",   "Eighdyn",   "Eight months in you catch him talking to five other people on his phone. So much for feminism."},
        {'J', "raccoon",   "Raccoon",   "You domesticated the raccoon and made a lifelong friend.. What's the purpose of a speed dating event again?"},
    };
    return e;
}

const Ending& aloneEnding() {
    static const Ending e = {'A', "alone", "Alone",
        "You finish your degree with no complications. No love complications, that is."};
    return e;
}

const Ending& specialEnding() {
    static const Ending e = {'K', "special", "Special",
        "You're quite the people pleaser. Everyone's head over heels for you!"};
    return e;
}

vector<Ending> unlockedEndings(const map<string, int>& points) {
    auto get = [&](const string& k) {
        auto it = points.find(k);
        return it == points.end() ? 0 : it->second;
    };

    vector<Ending> out;
    for (const Ending& e : characterEndings())
        if (get(e.key) >= MAX_POINTS) out.push_back(e);

    bool everyone = true;
    for (const string& k : scoredKeys())
        if (get(k) < MAX_POINTS) everyone = false;
    if (everyone) out.push_back(specialEnding());

    return out;
}
