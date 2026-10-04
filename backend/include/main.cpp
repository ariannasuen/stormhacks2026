#include <iostream>
#include "action.h"
#include "game.h"
using namespace std;

int main() {
    Game game;
    game.run();

    cout << "\n(press Enter to exit)";
    waitForEnter();
    return 0;
}
