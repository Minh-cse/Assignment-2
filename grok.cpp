#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

#define MAX_GRAPH 10
#define MAX_VERTEX 100

using namespace std;

const long long INF = 1000000000000000000LL;

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

struct Result {
    long long shortestTime;
    vector<int> path;
};

void swapNode(Node& a, Node& b) {
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

            if (heap[idx].dist >= heap[parent].dist) {
                break;
            }

            swapNode(heap[idx], heap[parent]);
            idx = parent;
        }
    }

    void heapifyDown(int idx) {
        int n = heap.size();

        while (true) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int smallest = idx;

            if (left < n && heap[left].dist < heap[smallest].dist) {
                smallest = left;
            }

            if (right < n && heap[right].dist < heap[smallest].dist) {
                smallest = right;
            }

            if (smallest == idx) {
                break;
            }

            swapNode(heap[idx], heap[smallest]);
            idx = smallest;
        }
    }

public:
    void push(int vertexIdx, long long dist) {
        heap.push_back({vertexIdx, dist});
        heapifyUp(heap.size() - 1);
    }

    Node pop() {
        if (heap.empty()) {
            return {-1, INF};
        }

        Node top = heap[0];

        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapifyDown(0);
        }

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

    while (right >= left && isspace(s[right])) {
        right--;
    }

    if (left > right) {
        return "";
    }

    return s.substr(left, right - left + 1);
}

bool isOnlyInt(const string& line) {
    if (line.empty()) {
        return false;
    }

    for (char c : line) {
        if (!isdigit(c) && c != ' ' && c != '\t' && c != '\r') {
            return false;
        }
    }

    return true;
}

int findVertexIndex(Graph& g, const string& name) {
    for (int i = 0; i < g.numVertex; i++) {
        if (g.vertices[i].name == name) {
            return i;
        }
    }

    return -1;
}

Result dijkstra(Graph& g, string startName, string targetName) {
    vector<AdjEdge> adj[MAX_VERTEX];

    for (int i = 0; i < g.edgesHolder.size(); i++) {
        Edge e = g.edgesHolder[i];

        int u = findVertexIndex(g, e.start);
        int v = findVertexIndex(g, e.end);

        if (u != -1 && v != -1) {
            adj[u].push_back({v, e.travelTime});
            adj[v].push_back({u, e.travelTime}); 
        }
    }

    int start = findVertexIndex(g, startName);
    int target = findVertexIndex(g, targetName);

    Result result;
    result.shortestTime = -1;

    if (start == -1 || target == -1) {
        return result;
    }

    long long dist[MAX_VERTEX];
    int parent[MAX_VERTEX];

    for (int i = 0; i < g.numVertex; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    dist[start] = 0;

    MinHeap pq;
    pq.push(start, 0);

    while (!pq.isEmpty()) {
        Node cur = pq.pop();

        int u = cur.vertexIdx;
        long long currentDist = cur.dist;

        if (u == -1) {
            break;
        }

        if (currentDist != dist[u]) {
            continue;
        }

        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i].to;
            int travelTime = adj[u][i].travelTime;

            long long wait = 0;

            if (dist[u] > 30) {
                int cycle = g.vertices[u].cycleTime;

                if (cycle > 0) {
                    wait = (cycle - dist[u] % cycle) % cycle;
                }
            }

            long long newDist = dist[u] + wait + travelTime;

            if (newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;
                pq.push(v, newDist);
            }
        }
    }

    if (dist[target] == INF) {
        result.shortestTime = -1;
        return result;
    }

    result.shortestTime = dist[target];

    vector<int> reversedPath;

    int current = target;

    while (current != -1) {
        reversedPath.push_back(current);
        current = parent[current];
    }

    for (int i = reversedPath.size() - 1; i >= 0; i--) {
        result.path.push_back(reversedPath[i]);
    }

    return result;
}

void printResult(Graph& g, Result result, ostream& out) {
    out << result.shortestTime << endl;

    if (result.shortestTime == -1) {
        out << "No path" << endl;
        return;
    }

    for (int i = 0; i < result.path.size(); i++) {
        int vertexIdx = result.path[i];

        out << g.vertices[vertexIdx].name;

        if (i != result.path.size() - 1) {
            out << " ";
        }
    }

    out << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Use: 104240648 tests.txt output_104240648.txt\n";
        return 1;
    }

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    if (!fin) {
        cout << "Cannot open input file\n";
        return 1;
    }

    if (!fout) {
        cout << "Cannot open output file\n";
        return 1;
    }

    int numGraph;
    fin >> numGraph;
    fin.ignore(100, '\n');

    Graph graphs[MAX_GRAPH];

    for (int i = 0; i < numGraph; i++) {
        if (graphs[i].numVertex == 0) {
            fin >> graphs[i].numVertex;
            fin.ignore(100, '\n');
        }

        for (int j = 0; j < graphs[i].numVertex; j++) {
            string line;
            getline(fin, line);

            int pos = line.find(',');

            graphs[i].vertices[j].name = trim(line.substr(0, pos));
            graphs[i].vertices[j].cycleTime = stoi(trim(line.substr(pos + 1)));
        }

        string line;

        while (getline(fin, line)) {
            line = trim(line);

            if (line.empty()) {
                continue;
            }

            if (isOnlyInt(line)) {
                if (i + 1 < numGraph) {
                    graphs[i + 1].numVertex = stoi(line);
                }
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

    for (int i = 0; i < numGraph; i++) {
        string start = graphs[i].vertices[0].name;
        string target = graphs[i].vertices[graphs[i].numVertex - 1].name;

        Result result = dijkstra(graphs[i], start, target);

        printResult(graphs[i], result, fout);
        printResult(graphs[i], result, cout);
    }

    fin.close();
    fout.close();

    return 0;
}