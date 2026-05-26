#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define MAX_GRAPH 10
#define MAX_VERTEX 100
#define MAX_EDGE 1000
#define INF 10e9
using namespace std;

struct Vertex {
    string name;
    int cycleTime;
};
struct Edge {
    string start;
    string end;
    int travelTime;
};
struct Graph {
    int numVertex;
    int numEdge;
    Vertex vertices[MAX_VERTEX];
    vector<Edge> edges;
};
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Use 104240648 tests.txt output_104240648.txt\n";
        return 1;
    }
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    if (!fin) {
        cout << "Cannot open input file\n";
        return 1;
    }
    int numGraph;
    fin >> numGraph;
    fin.ignore(100, '\n');

    
    Graph graphs[MAX_GRAPH];
    Edge edges[MAX_EDGE];
    for (int i = 0; i < numGraph; i++) {
        if (i == 0 || graphs[i].numVertex == 0){
            fin >> graphs[i].numVertex;
            fin.ignore(100, '\n');
        }
        for (int j = 0; j < graphs[i].numVertex; j++){
            getline(fin, graphs[i].vertices[j].name, ',');
            fin >> graphs[i].vertices[j].cycleTime;
            fin.ignore(100, '\n');
        }
        string line;
        while(getline(fin, line)) {
            if (line.empty()) continue;
            if (isOnlyInt(line)) {
                graphs[i + 1].numVertex = stoi(line);
                break;
            }
            Edge e;
            int pos1 = line.find(',');
            int pos2 = line.find(',', pos1 + 1);
            e.start = line.substr(0, pos1);
            e.end = line.substr(pos1 + 2, pos2 - pos1 - 2);
            e.travelTime = stoi(line.substr(pos2 + 2));
            graphs[i].edges.push_back(e);
        }
    }
    fin.close();
    fout.close();
    return 0;
}
bool isOnlyInt(const string& line) {
    if (line.empty()) return false;
    for (char c: line) {
        if (!isdigit(c) && c != ' ' && c != '\t' && c != '\r') {
            return false;
        }
    }
    return true;
}