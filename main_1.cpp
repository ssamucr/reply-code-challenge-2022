#include <iostream>
#include <string>
#include <fstream>
#include "demon.cc"

using namespace std;

string file_name = "00-example";        // input file location
string row;                             // temporarily stores the content of each row
int Si;                                 // initial stamina
int Smax;                               // max stamina
int T;                                  // turns
int D;                                  // amount of demons
int cont = 0;                           // counter
int pos = 0;                            // position using find()

int main() {
    // read input file
    fstream file("./inputs/" + file_name + ".txt");

    if (getline(file, row)) {  // read first row
        int temp_array[4];
        int cont = 0;

        while (cont < 4) {
            pos = row.find(" ");
            temp_array[cont] = stoi(row.substr(0, pos));
            row.erase(0, pos + 1);
            cont++;
        }

        Si = temp_array[0];
        Smax = temp_array[1];
        T = temp_array[2];
        D = temp_array[3];
    }
    
    string rows[D];

    cont = 0;
    while (getline(file, row)) {  // read each row with demon info
        rows[cont] = row;
        cont++;
    }
    
    file.close();

    demon demons[D];  // stores each demon info

    for (int i = 0; i < D; i++) {  // stores each demon info
        int temp_array[4];
        for (int j = 0; j < 4; j++) {  // takes the first 4 values ​​of each daemon
            pos = rows[i].find(" ");
            temp_array[j] = stoi(rows[i].substr(0, pos));
            rows[i].erase(0, pos + 1);
        }

        demons[i].BuilderDemon(temp_array[0], temp_array[1], temp_array[2], temp_array[3]);  // creates a demon object with the first 4 values
        int y = 0;
        while (rows[i].find(" ")!=string::npos) {  // fill an array inside each demon object with the fragments earned per turn
            pos = rows[i].find(" ");
            demons[i].fragments_per_turn[y] = stoi(rows[i].substr(0, pos));
            rows[i].erase(0, pos + 1);
            y++;
        }
        if (rows[i].find(" ")==string::npos) {
            pos = rows[i].find(" ");
            demons[i].fragments_per_turn[y] = stoi(rows[i].substr(0, pos));
            rows[i].erase(0, pos + 1);
            y++;
        }
    }

    int stamina_buffer[T];
    int fragments_buffer[T];
    for (int i = 0; i < T; i++) {
        stamina_buffer[i] = 0;
        fragments_buffer[i] = 0;
    }

    int best_demon_list[D];
    int demon_id = 0;
    int final_score = 0;
    int current_stamina = Si;
    for (int current_turn = 0; current_turn < T; current_turn++) {
        // regain stamina
        current_stamina += stamina_buffer[current_turn];
        if (current_stamina > Smax) {
            current_stamina = Smax;
        }

        // selects the demon to fight
        int best_demon_id = -1;
        int max_fragments = 0;
        for (int i = 0; i < D; i++) {
            if (demons[i].status == true && demons[i].Sc <= current_stamina) {
                int fragments = demons[i].CalculateFragments(current_turn, T);
                if (max_fragments < fragments) {
                    max_fragments = fragments;
                    best_demon_id = i;
                }
            }
        }

        if (best_demon_id != -1) {
            best_demon_list[demon_id] = best_demon_id;
            current_stamina -= demons[best_demon_id].Sc;

            stamina_buffer[current_turn + demons[best_demon_id].Tr] = demons[best_demon_id].Sr;
            
            for (int a = current_turn, b = 0; (a < T) && (b < demons[best_demon_id].Na); a++, b++) {
                fragments_buffer[a] += demons[best_demon_id].fragments_per_turn[b];
            }

            demons[best_demon_id].Fight();

            demon_id++;
        }

        // earn fragments
        final_score += fragments_buffer[current_turn];
    }

    ofstream archivo;

    archivo.open("./outputs/" + file_name + "-output.txt",ios::out);  // create an outputfile

    if (archivo.fail()) {
        cout<<"Couldn't open a new file";
        exit(1);
    }

    for (int i = 0; i < demon_id; i++) {  // write the demon queue in order
        if (i == demon_id - 1) {
            archivo << best_demon_list[i];
        } else {
            archivo << best_demon_list[i] << endl;
        }
    }

    archivo.close();  // close the file
    
    system("pause");
    return 0;
}