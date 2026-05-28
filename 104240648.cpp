#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#define MAX_GRAPH 10
#define MAX_VERTEX 100
#define MAX_EDGE 1000
const long long INF = 1LL << 18;
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
struct AdjEdge {
    int to;
    int travelTime;
};
struct Graph {
    int numVertex = 0;
    Vertex vertices[MAX_VERTEX];
    vector<Edge> edgesHolder;
};
struct Node {
    int vertexIdx;
    long long dist;
};
void swapNode(Node & a, Node & b) {
    Node temp = a;
    a = b;
    b = temp;
}

class MinHeap {
private:
    vector<Node> heap;

    void heapifyUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (heap[idx].dist >= heap[parent].dist) {break;}
            swapNode(heap[idx], heap[parent]);
            idx = parent;
        }
    }
    void heapifyDown(int idx) {
        int n = heap.size();
        while(true) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int smallest = idx;

            if (left < n && heap[left].dist < heap[smallest].dist)
                {smallest = left;}
            if (right < n && heap[right].dist < heap[smallest].dist)
                {smallest = right;}
            if (smallest == idx) {break;}
            swapNode(heap[idx], heap[smallest]);
            idx = smallest;
        }
    }
public:
    void push(int vertexIdx, long long dist) {
        heap.push_back({vertexIdx, dist});
        heapifyUp(heap.size() - 1 );
    }
    Node pop() {
        if (heap.empty()) {return {INF, -1};}
        Node top = heap[0];

        heap[0] = heap.back();
        heap.pop_back();
        heap.pop_back();
        if (!heap.empty()) {heapifyDown(0);}
        return top;
    }
    bool isEmpty() const {
        return heap.empty();
    }
};
string trim(const string& s) {
    int left = 0;
    int right = s.length() - 1;

    while (left <= right && isspace(s[left])) {
        left++;
    }
    while (left <= right && isspace(s[right])) {
        right--;
    }
    if (left > right) {return ""; }

    return s.substr(left, right - left + 1);
}
bool isOnlyInt(const string& line) {
    if (line.empty()) {return false;}
    for (char c: line) {
        if (!isdigit(c) && c != ' ' && c != '\t' && c != '\r') {
            return false;
        }
    }
    return true;
}
int findVertexIndex(Graph& g, const string& name) {
    for (int i = 0; i < g.numVertex; i++) {
        if(g.vertices[i].name == name) {return i;}
    }
    return -1;
}

long long dijkstra(Graph& g, string startName, string targetName) {
    vector<AdjEdge> adj[MAX_EDGE];

    for (Edge e : g.edgesHolder) {
        int u = findVertexIndex(g, e.start);
        int v = findVertexIndex(g, e.end);

        if (u != -1 && v != -1) {
            adj[u].push_back({v, e.travelTime});
        }
    }
    int start = findVertexIndex(g, startName);
    int target = findVertexIndex(g, targetName);

    if (start == -1 || target == -1) {return -1;}

    long long dist[MAX_VERTEX];

    for (int i = 0; i< g.numVertex; i++) {
        dist[i] = INF;
    }

    dist[start] = 0;

    MinHeap pq;
    pq.push(start, 0);

    while(!pq.isEmpty()) {
        Node current = pq.pop();

        int u = current.vertexIdx;
        long long currentDist = current.dist;

        if (u == -1) {break;}
        
        if (currentDist != dist[u]) {continue;}
        
        if (u == target) {return dist[u];}

        for (AdjEdge edge : adj[u]) {
            int v = edge.to;
            int travelTime = edge.travelTime;

            long long wait = 0;
            int cycle = g.vertices[u].cycleTime;

            if (cycle > 0) {
                wait = (cycle - dist[u] % cycle) % cycle;
            }

            long long newDist = dist[u] + wait + travelTime;

            if (newDist < dist[v]) {
                dist[v] = newDist;
                pq.push(v, newDist);
            }
        }
    }
    return -1;
}

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
    for (int i = 0; i < numGraph; i++) {
        if (i == 0 || graphs[i].numVertex == 0){
            fin >> graphs[i].numVertex;
            fin.ignore(100, '\n');
        }
        for (int j = 0; j < graphs[i].numVertex; j++){
            string line;
            getline(fin, line);

            int pos = line.find(',');

            graphs[i].vertices[j].name = trim(line.substr(0, pos));
            graphs[i].vertices[j].cycleTime = stoi(trim(line.substr(pos + 1)));
        }
        string line;
        while(getline(fin, line)) {
            line = trim(line);
            if (line.empty()) {continue;}
            if (isOnlyInt(line)) {
                graphs[i + 1].numVertex = stoi(line);
                break;
            }
            
            int pos1 = line.find(',');
            int pos2 = line.find(',', pos1 + 1);

            Edge e;
            e.start = trim(line.substr(0, pos1));
            e.end = trim(line.substr(pos1 + 1, pos2 - pos1 - 1));
            e.travelTime = stoi(trim(line.substr(pos2 + 1)));

            graphs[i].edgesHolder.push_back(e);
        }
    }
    for(int i = 0; i < numGraph; i++) {
        string start = graphs[i].vertices[0].name;
        string target = graphs[i].vertices[graphs[i].numVertex - 1].name;

        long result = dijkstra(graphs[i], start, target);
        fout << result << endl;
    }

    
    fin.close();
    fout.close();
    return 0;
}