#include "text.h"
#include <iostream>
using namespace std;

// Wraps a spoken reply in quote marks:  Q("Hi!")  ->  "Hi!"
#define Q(s) "\"" s "\""

namespace {
Step S(string who, string text)  { return Step{StepType::Say,     who, text, {}}; }
Step N(string text)              { return Step{StepType::Narrate, "",  text, {}}; }
Step A(string who, string text, vector<Choice> c) { return Step{StepType::Ask, who, text, c}; }
Step TU()                        { return Step{StepType::TimesUp, "",  "",   {}}; }
Step NAME()                      { return Step{StepType::EnterName, "", "",  {}}; }

vector<Scene> build() {
    vector<Scene> v;

    // ------------------------------------------------------------ INTRO
    v.push_back({"intro", "", "", false, {
        N("After a grueling day at the SFU Burnaby campus, you want nothing more than a solo drink at the local Biercraft before you bus home. Heading into the pub, you're instantly bombarded by four colourful, furry critters."),
        S("Spendy", "Hello!"),
        S("Sparky", "You must be here for the SFU Speed Dating Event, right?"),
        A("", "", { Choice(Q("Uhh..."), 0) }),
        S("Stormy", "...you don't know what that is?"),
        S("Trendy", "It's only the most popular event ever held on campus!"),
        S("Spendy", "And you're just in luck, because we have an empty spot with your name on it!"),
        S("Sparky", "What's your name?"),
        NAME(),
        S("Spendy", "Nice to meet you {name}!"),
        S("Stormy", "Alright, hot single, here's the gameplan:"),
        S("Sparky", "Your date will ask a few questions, and you'll answer them."),
        S("Spendy", "And based on your replies, they might reach out at the end of the night to get your number."),
        S("Trendy", "Oooh, I can feel the romance in the air already!"),
        S("Stormy", "Pro Tip: play it cool and don't overthink it. If it's meant to be, then it's meant to be."),
        S("Trendy", "I think you're all set. Come right this way - you're in for the night of your life!"),
    }});

    // ------------------------------------------------------------- CHAD
    v.push_back({"chad", "Chad", "chad", true, {
        N("You're brought to a standing table where your newfound date is waiting. The blinding sheen of his oiled muscles nearly overpowers the ambient lighting."),
        S("Sparky", "This here is Chad, {age}, an attendee of the Beedie School of Business."),
        S("Spendy", "Have fun!"),
        S("Stormy", "But not too much fun."),
        S("Trendy", "See ya!"),
        N("The otters give you both some privacy."),
        A("Chad", "What's cookin', good looking?!", { Choice("Hi!", 0), Choice("Uh... Hello?", 0), Choice("*say nothing*", 0) }),
        A("Chad", "So, sweetcheeks, how often are you hitting the gym?", {
            Choice(Q("I LIVE at the gym!"), 2),
            Choice(Q("I don't leave the house all that much, honestly."), 0),
            Choice(Q("A couple times a month."), 1),
            Choice(Q("I have a pass, but I never use it."), 1) }),
        A("Chad", "Do you invest in crypto?", {
            Choice(Q("I've heard of Bitcoin."), 1),
            Choice(Q("I maxed out my credit card on a meme coin once."), 1),
            Choice(Q("Yes, I even have a whole investment portfolio in multiple stocks."), 2),
            Choice(Q("People who do that sorta stuff are silly."), 0) }),
        A("Chad", "How do you feel about clubbing on the first date?", {
            Choice(Q("I only go clubbing with my friends, really."), 1),
            Choice(Q("I'm a party animal!"), 2),
            Choice(Q("I don't participate in dangerous activities."), 0),
            Choice(Q("I mean, if you really wanna..."), 1) }),
        TU(),
        S("Stormy", "How was that, {name}?"),
        S("Spendy", "Did you have fun?"),
        A("", "", { Choice(Q("Yes!"), 0), Choice(Q("Not really-"), 0) }),
        S("Sparky", "That's great to hear!"),
        S("Trendy", "Keep those love juices flowing, because we've got another hot single right around the corner!"),
    }});

    // ------------------------------------------------------------ RICKY
    v.push_back({"ricky", "Ricky", "ricky", true, {
        N("You smell him before you see him: the stench almost knocks you off your feet. He slicks his greasy hair back with a cheeto-dusted hand and smiles shakily at you."),
        S("Spendy", "*coughs* This is Ricky, an 8th year Computer Science major."),
        S("Stormy", "He's hoping that this won't just be the year he graduates, but also the year he finds love."),
        S("Sparky", "Have at it!"),
        A("Ricky", "Um... hi there.", { Choice(Q("Hiya!"), 0), Choice(Q("Hey."), 0), Choice("*say nothing*", 0) }),
        A("Ricky", "Do you like vibe coding?", {
            Choice(Q("I don't know what that is."), 1),
            Choice(Q("I do it all the time!"), 2),
            Choice(Q("I don't use AI."), 0),
            Choice(Q("Don't really like coding, honestly."), 1) }),
        A("Ricky", "What's your rank on Valorant?", {
            Choice("*laugh in his face*", 1),
            Choice(Q("I like Animal Crossing."), 0),
            Choice(Q("Bronze."), 2),
            Choice(Q("Gold."), 1) }),
        A("Ricky", "I don't believe in showers... are you okay with that?", {
            Choice(Q("What?! No!"), 0),
            Choice(Q("We can work on it."), 1),
            Choice(Q("I have a sensitive nose."), 1),
            Choice(Q("I like a little stink."), 2) }),
        TU(),
        S("Stormy", "Still with us, {name}?"),
        S("Sparky", "Let's keep going!"),
    }});

    // ---------------------------------------------------------- CHARLIE
    v.push_back({"charlie", "Charlie", "charlie", true, {
        N("A pale woman crawls into the light. She looks like she hasn't seen the sun in weeks."),
        S("Spendy", "This is Charlie, {age}, who took a brief break from her five Engineering labs to be here."),
        S("Sparky", "Good luck!"),
        A("Charlie", "Sorry... I didn't really get a chance to clean up before getting here...", {
            Choice(Q("I don't mind!"), 0), Choice(Q("All good."), 0), Choice("*say nothing*", 0) }),
        A("Charlie", "Have you applied for co-ops yet?", {
            Choice(Q("I don't know what that is."), 0),
            Choice(Q("Yeah, I've already got a few offers."), 1),
            Choice(Q("Nah, those aren't required for my program."), 1),
            Choice(Q("Yeah, but I haven't gotten any interviews..."), 2) }),
        A("Charlie", "Are you more of a homebody?", {
            Choice(Q("Yeah, I'm lowkey a vampire at this point."), 2),
            Choice(Q("Depends on the day."), 1),
            Choice(Q("You can't keep me locked up!"), 0),
            Choice(Q("I like going out sometimes."), 1) }),
        A("Charlie", "What's your CGPA?", {
            Choice("4.2", 1), Choice("3.4", 2), Choice("2.1", 1),
            Choice(Q("I don't wanna talk about it..."), 0) }),
        TU(),
        S("Sparky", "Ready for the next one?"),
        S("Trendy", "Yes! Yes! Yes!"),
    }});

    // ------------------------------------------------------------ BARTH
    v.push_back({"barth", "Barth", "barth", true, {
        N("The ominous clicking of a geiger counter unsettles you. A scruffy man glances this way and that before sitting down."),
        S("Spendy", "This is Bartholomew-"),
        S("Barth", "I go by Barth, dude!"),
        S("Spendy", "...this is Barth, {age}, and he's a Physics major."),
        S("Sparky", "Get chatting!"),
        A("Barth", "You seen my thallium-204 anywhere?", {
            Choice(Q("Your thallium what now?"), 0),
            Choice(Q("I don't know what that is."), 1),
            Choice(Q("Unfortunately no, sorry."), 2),
            Choice(Q("Nah, good luck with the search."), 1) }),
        A("Barth", "Are you sure? It's like a little green circle.", {
            Choice(Q("It's green?"), 1),
            Choice(Q("Yes, I'm sorry, man."), 2),
            Choice(Q("Yup. Hope you find it."), 0),
            Choice(Q("I don't even know what it is."), 1) }),
        A("Barth", "I just feel really weird, like, so nauseous right now...", {
            Choice(Q("Should I call someone?"), 1),
            Choice(Q("You don't look too good..."), 2),
            Choice(Q("Are you irradiated right now, dude?"), 1),
            Choice(Q("Maybe get some air?"), 0) }),
        TU(),
        S("Spendy", "I'm sure he'll be fine."),
        S("Sparky", "Onto the next one!"),
    }});

    // ------------------------------------------------------------- GERD
    v.push_back({"gerd", "Gerd", "gerd", true, {
        N("A very disproportionate man takes the seat across from you. He combs back his flowing hair and squints at you, assessing your every move."),
        S("Sparky", "This is Gerd, {age}. He's in political science and always has a little tidbit to share-!"),
        S("Gerd", "Erm, actually, they're not tidbits, they're morsels of information that are my duty to share!"),
        S("Stormy", "That's wonderful, Gerd..."),
        S("Spendy", "Have fun, {name}!"),
        A("Gerd", "Are you a fan of debates?", {
            Choice(Q("I often amuse myself with philosophical ponderings."), 2),
            Choice(Q("Watching 'em is cool."), 1),
            Choice(Q("Not a fan of conflict."), 0),
            Choice(Q("I don't mind them."), 1) }),
        A("Gerd", "How many books do you read in a year?", {
            Choice(Q("I don't read much."), 0),
            Choice(Q("Lots of comic books, mostly."), 1),
            Choice(Q("I aim for two a week, but I squeeze in a third if I'm feeling nasty."), 2),
            Choice(Q("Couple audiobooks a month."), 1) }),
        A("Gerd", "You're a very attractive person. I think our genes would go nicely together. Shall we procreate?", {
            Choice(Q("I'll have to think about it."), 1),
            Choice(Q("What?! No!"), 0),
            Choice(Q("Maybe somewhere down the line."), 2),
            Choice(Q("I don't really want kids."), 1) }),
        TU(),
        S("Stormy", "You gotta see it through my guy."),
        S("Spendy", "Yeah, keep going!"),
    }});

    // ------------------------------------------------------------ SHADY
    // Spooky interlude: every reply is worth 0 and there is no ending.
    const vector<Choice> who = {
        Choice("Who are you?", 0), Choice("What do you want?", 0),
        Choice("How did you find me?", 0), Choice("Leave me alone!", 0) };
    v.push_back({"shady", "???", "shady", false, {
        N("The room's temperature seems to plummet ten degrees. You feel strangely observed, scanning the bar, not noticing the shadowed figure that silently meets you from across the table."),
        N("The figure says nothing."),
        A("", "", who),
        S("?????", "Hello... {name}..."),
        A("", "", who),
        S("?ha??", "you know... what you did..."),
        A("", "", who),
        S("Shady", "Don't forget about me, {name}."),
        A("", "", who),
        N("The presence is gone quickly, as if you imagined it."),
        S("Sparky", "You okay? You zoned out for a bit over there."),
        S("Trendy", "We were worried about you."),
        S("Spendy", "You're ready for the next date, right?"),
        S("Stormy", "Because you're over halfway there!"),
    }});

    // -------------------------------------------------------- ANASTASIA
    v.push_back({"anastasia", "Anastasia", "anastasia", true, {
        N("Soft-footed and mysterious, your next date slinks into her seat like a rolling wave of tar, adorned with silver jewelry and crude makeup. Her expression is set in a frigid sort of numbness."),
        S("Trendy", "This is Anastasia, {age}, a Creative Writing major!"),
        S("Sparky", "Have fun!"),
        A("Anastasia", "My soul greets yours.", {
            Choice(Q("Uh... thanks?"), 0), Choice(Q("And mine in turn."), 0), Choice("*say nothing*", 0) }),
        A("Anastasia", "Do you also view the concept of speed-dating as an unnecessary display of vulnerability?", {
            Choice(Q("Yes, I wish we had a different way to meet each other."), 2),
            Choice(Q("...not really?"), 0),
            Choice(Q("Sort of."), 1),
            Choice(Q("I've never thought of it like that."), 1) }),
        A("Anastasia", "Are you familiar with the occult?", {
            Choice(Q("I'm open to learning what it is."), 1),
            Choice(Q("I practice it in my own time."), 2),
            Choice(Q("I know a couple of witches, actually."), 1),
            Choice(Q("No clue what that is."), 0) }),
        A("Anastasia", "I'm taken by you. Would you be my muse?", {
            Choice(Q("As long as it's nothing weird."), 1),
            Choice(Q("I mean, sure, I guess."), 1),
            Choice(Q("That's kinda creepy."), 0),
            Choice(Q("I would be honoured."), 2) }),
        TU(),
        S("Spendy", "We're just flying through them, aren't we?"),
        S("Trendy", "Are you feeling a surge of love in the air yet?"),
    }});

    // -------------------------------------------------------- MACKENZIE
    v.push_back({"mackenzie", "Mackenzie", "mackenzie", true, {
        N("A new woman takes Anastasia's place, popping her bubblegum as her nails click away noisily on her phone. She takes half glances up from her screen before finally putting her phone away to give you her undivided attention."),
        S("Stormy", "This is Mackenzie, {age}, a Communications Major."),
        S("Sparky", "She's a tough nut to crack, but you got this!"),
        A("Mackenzie", "So... what do you do for fun?", {
            Choice(Q("I like experimenting with my look."), 1),
            Choice(Q("I mostly study, honestly."), 0),
            Choice(Q("I go to the gym!"), 2),
            Choice(Q("I read."), 1) }),
        A("Mackenzie", "Where do you see yourself in five years?", {
            Choice(Q("Unemployed, and in my mom's basement."), 0),
            Choice(Q("Definitely a finance bro in NYC."), 2),
            Choice(Q("Prolly working at McDonalds."), 1),
            Choice(Q("Ideally with you, wink wink."), 1) }),
        A("Mackenzie", "Would you do 50/50 in a relationship?", {
            Choice(Q("No, I'd pay for everything."), 2),
            Choice(Q("No, I want my partner to pay for everything."), 0),
            Choice(Q("Uh, I guess."), 1),
            Choice(Q("Ever heard of this thing called dine and dash?"), 1) }),
        TU(),
        S("Sparky", "One more left!"),
        S("Trendy", "Make it count!"),
    }});

    // ---------------------------------------------------------- RACCOON
    v.push_back({"raccoon", "Raccoon", "raccoon", true, {
        N("Instead of a human like you were expecting, a raccoon clambers into the seat. It stares up at you with big, black eyes, making soft raccoon noises."),
        N("The raccoon says nothing because it can't speak."),
        A("", "", { Choice("pet it", 0), Choice("call it closer", 2), Choice("feed it garbage", 0), Choice("ignore it", 0) }),
        N("The raccoon continues to say nothing because it can't speak."),
        A("", "", { Choice("pet it", 2), Choice("call it closer", 0), Choice("feed it garbage", 0), Choice("ignore it", 0) }),
        N("The raccoon still says nothing because it still can't speak."),
        A("", "", { Choice("pet it", 0), Choice("call it closer", 0), Choice("feed it garbage", 2), Choice("ignore it", 0) }),
        N("The raccoon scampers off quietly, because it can't speak. You almost miss it."),
        S("Stormy", "That was weird..."),
        S("Spendy", "Yeah, but anyways, back to our regularly scheduled programming!"),
    }});

    // ---------------------------------------------------------- EIGHDYN
    v.push_back({"eighdyn", "Eighdyn", "eighdyn", true, {
        N("The man across from you swirls his matcha cup, a labubu hanging neatly from his carabiner, unopened novel sticking out of his backpocket. He flicks the curled up end of his mullet, lounged back in his chair like speed dating is his comfort zone."),
        S("Sparky", "This is Eighdyn, {age}, a Health Sciences Major!"),
        S("Spendy", "Get chatting!"),
        A("Eighdyn", "Sup. Did it hurt when you fell from heaven?", {
            Choice(Q("Hey."), 0), Choice(Q("Not really."), 0), Choice("*say nothing*", 0) }),
        A("Eighdyn", "Are you into Matcha, maybe we could go sometime? By the way, I'm an avid reader of feminist literature.", {
            Choice(Q("I love Matcha, I also appreciate Labubus."), 2),
            Choice(Q("I like reading too, I guess."), 1),
            Choice(Q("I hope you are actively deconstructing misogyny. On the daily."), 1),
            Choice(Q("That green stuff is nasty. Hard pass!"), 0) }),
        A("Eighdyn", "What kind of music do you listen to? I cry to Phoebe Bridgers in the shower sometimes.", {
            Choice(Q("That's kind of lame, dude."), 0),
            Choice(Q("No way, same!"), 2),
            Choice(Q("I kind of listen to everything, not going to lie..."), 1),
            Choice(Q("Overshare much..."), 1) }),
        // TODO: the script has an empty "Eighdyn:" line here. Replace this placeholder with his
        // 3rd question. Keep the replies worth 2 / 1 / 1 / 0 so he can still reach 6 points.
        A("Eighdyn", "[PLACEHOLDER - Eighdyn's 3rd question goes here]", {
            Choice("[Reply worth 2 points]", 2), Choice("[Reply worth 1 point]", 1),
            Choice("[Reply worth 1 point]", 1),  Choice("[Reply worth 0 points]", 0) }),
        TU(),
        S("Sparky", "Looks like that was the last guy!"),
    }});

    // ----------------------------------------------------- OUTRODUCTION
    v.push_back({"outro", "", "", false, {
        S("Spendy", "Thank you for coming to our SFU Speed Dating Event!"),
        S("Sparky", "We know it was impromptu, so we really appreciate it!"),
        S("Trendy", "And we hope you think it was the best thing ever!"),
        S("Stormy", "Let's see how you did, champ!"),
    }});

    return v;
}
} // namespace

const vector<Scene>& Text::scenes() {
    static const vector<Scene> all = build();
    return all;
}

void Text::test() {
    cout << "Test to see if the script loads: " << scenes().size() << " scenes\n";
}
