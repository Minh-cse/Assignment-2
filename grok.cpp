// StudentID.cpp
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

// ==================== Custom Min-Heap Priority Queue ====================
struct Node {
    ll time;
    string vertex;
    bool operator>(const Node& other) const {
        return time > other.time;
    }
};

class MinHeap {
private:
    vector<Node> heap;

    void heapifyUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (heap[idx].time >= heap[parent].time) break;
            swap(heap[idx], heap[parent]);
            idx = parent;
        }
    }

    void heapifyDown(int idx) {
        int n = heap.size();
        while (true) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int smallest = idx;

            if (left < n && heap[left].time < heap[smallest].time)
                smallest = left;
            if (right < n && heap[right].time < heap[smallest].time)
                smallest = right;

            if (smallest == idx) break;
            swap(heap[idx], heap[smallest]);
            idx = smallest;
        }
    }

public:
    void push(ll time, string v) {
        heap.push_back({time, v});
        heapifyUp(heap.size() - 1);
    }

    Node pop() {
        if (heap.empty()) return {INF, ""};
        Node top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
        return top;
    }

    bool empty() const {
        return heap.empty();
    }
};
// =====================================================================

struct Edge {
    string to;
    ll w;
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " tests.txt output_StudentID.txt\n";
        return 1;
    }

    ifstream in(argv[1]);
    ofstream out(argv[2]);

    if (!in.is_open()) {
        cout << "Cannot open input file!\n";
        return 1;
    }

    int m;
    in >> m;

    for (int graph = 0; graph < m; ++graph) {
        int n;
        in >> n;

        map<string, ll> cycle;
        vector<string> vertices(n);

        for (int i = 0; i < n; ++i) {
            string v, comma;
            ll t;
            in >> v >> comma >> t;
            if (!v.empty() && v.back() == ',') v.pop_back();
            cycle[v] = t;
            vertices[i] = v;
        }

        // Build graph
        map<string, vector<Edge>> adj;
        for (int i = 0; i < n; ++i) {
            string u, v, comma;
            ll w;
            while (true) {
                if (in.eof()) break;
                char ch = in.peek();
                if (isdigit(ch)) break; // next graph starts with number
                string line;
                getline(in, line);
                if (line.empty()) continue;
                stringstream ss(line);
                if (!(ss >> u)) continue;
                if (!u.empty() && u.back() == ',') u.pop_back();
                ss >> v >> comma >> w;
                if (!v.empty() && v.back() == ',') v.pop_back();

                if (cycle.count(u) && cycle.count(v)) {
                    adj[u].push_back({v, w});
                    adj[v].push_back({u, w});
                }
            }
        }

        string source = vertices[0];
        string target = vertices.back();

        // Dijkstra with custom heap
        map<string, ll> dist;
        map<string, string> parent;
        for (auto& p : cycle) dist[p.first] = INF;
        dist[source] = 0;

        MinHeap pq;
        pq.push(0, source);

        while (!pq.empty()) {
            Node curr = pq.pop();
            ll time = curr.time;
            string u = curr.vertex;

            if (time > dist[u]) continue;

            // Waiting logic
            ll depart = time;
            if (time > 30) {
                ll t = cycle[u];
                depart = ((time + t - 1) / t) * t;
            }

            for (auto& e : adj[u]) {
                ll arrival = depart + e.w;
                if (arrival < dist[e.to]) {
                    dist[e.to] = arrival;
                    parent[e.to] = u;
                    pq.push(arrival, e.to);
                }
            }
        }

        ll shortestTime = dist[target];

        // Reconstruct path
        vector<string> path;
        string cur = target;
        while (!cur.empty()) {
            path.push_back(cur);
            if (cur == source) break;
            cur = parent[cur];
        }
        reverse(path.begin(), path.end());

        // Output
        cout << shortestTime << endl;
        out << shortestTime << endl;

        for (size_t i = 0; i < path.size(); ++i) {
            if (i > 0) {
                cout << " ";
                out << " ";
            }
            cout << path[i];
            out << path[i];
        }
        cout << endl;
        out << endl;
    }

    cout << "Done! Output written to " << argv[2] << endl;
    return 0;
}
