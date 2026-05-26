#include <iostream>
#include <fstream>
#include <string>
#define MAX_GRAPH 10
#define MAX_VERTEX 100
using namespace std;
struct Vertex {
    string name;
    int cycleTime;
};
int main(int argc, char* argv[]) {
    // if (argc != 3) {
    //     cout << "Use 104240648 tests.txt output_104240648.txt\n";
    //     return 1;
    // }
    // ifstream fin(argv[1]);
    // ofstream fout(argv[2]);

    // if (!fin) {
    //     cout << "Cannot open input file\n";
    //     return 1;
    // }
    int numGraph = 1;
    int numVertex = 2;
    //fin >> numGraph;
    Vertex vertices[MAX_GRAPH][MAX_VERTEX];
        for (int j = 0; j < numVertex; j++){
            getline(cin, vertices[0][j].name, ',');
            cin >> vertices[0][j].cycleTime;
            cin.ignore();
        }
        for (int j = 0; j < numVertex; j++){
            cout << vertices[0][j].name << vertices[0][j].cycleTime << "\n";
        }
    return 0;
}