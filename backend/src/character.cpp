#include "character.h"
using namespace std;

void Character::test() const {
    cout << "Making sure that all character information is correct..." << endl;
    cout << "Name: " << getName() << endl;
    cout << "Major: " << getMajor() << endl;
    cout << "Age: " << getAge() << endl;
}
