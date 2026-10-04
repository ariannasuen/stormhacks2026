#pragma once
#include <iostream>
#include <string>
using namespace std;

/**********************
 * This header file contains the following classes:
 *
 * parent: Character
 * children:
 *  - You
 *  - Love   (the romantic interests)
 *  - Side   (otters, Shady, the raccoon)
 ***********************/

class Character {
    public:
        Character() : name("SkibidiRizz"), major("engineering"), age(67) {}
        Character(string n, string m, int a) : name(n), major(m), age(a) {}
        virtual ~Character() = default;

        void   setName(string n)  { name = n; }
        string getName() const    { return name; }

        void   setMajor(string m) { major = m; }
        string getMajor() const   { return major; }

        void   setAge(int a)      { age = a; }
        int    getAge() const     { return age; }

        // Prints all fields (for debugging)
        void test() const;

    private:
        string name;
        string major;
        int age;
};

class You : public Character {
    public:
        You(string n, string m, int a) : Character(n, m, a) {}
};

class Love : public Character {
    public:
        Love(string n, string m, int a) : Character(n, m, a) {}
};

class Side : public Character {
    public:
        Side(string n, string m, int a) : Character(n, m, a) {}
};
