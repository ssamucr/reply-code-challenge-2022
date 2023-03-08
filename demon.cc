#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class demon {
    public:
        int Sc;  // stamina required to face this demon
        int Tr;  // turns to regain Sr
        int Sr;  // stamina regained after Tr
        int Na;  // turns in wich pandora will earn fragments for defeating this demon
        bool status = true;  // true = alive , false = already defeated
        int fragments_per_turn[100000];  // fragments that can be earned per turn
        void BuilderDemon(int stamina_cost, int turns_to_recover_stamina, int stamina_recover, int turns_fragments) {
            Sc = stamina_cost;
            Tr = turns_to_recover_stamina;
            Sr = stamina_recover;
            Na = turns_fragments;
        }

        // returns the number of fragments pandora will earn at the end of the game if this demon is faced in the current turn
        int CalculateFragments(int current_turn, int T) {
            int fragments = 0;
            for (int i = 0; i < T - current_turn; i++) {
                fragments += fragments_per_turn[i];
            }
            return fragments;
        }

        // changes the status of the demon when is defeated
        void Fight() {
            status = false;
        }
};