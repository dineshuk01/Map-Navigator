#include <bits/stdc++.h>
using namespace std;

// Define a structure for the graph edges
struct Edge {
    string destination;
    int distance;
};

// Define a structure to hold road conditions
struct RoadCondition {
    bool isBlocked;
    int trafficDelay; // in minutes
};

// Function to add an edge to the graph
void addEdge(unordered_map<string, vector<Edge>> &graph, string u, string v, int distance) {
    graph[u].push_back({v, distance});
    graph[v].push_back({u, distance});
}

// Function to find the shortest path using Dijkstra's algorithm
pair<vector<string>, int> dijkstra(unordered_map<string, vector<Edge>> &graph, unordered_map<string, RoadCondition> &conditions, string start, string end) {
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    unordered_map<string, int> distances;
    unordered_map<string, int> trafficDelays;
    unordered_map<string, string> previous;

    for (auto &node : graph) {
        distances[node.first] = INT_MAX;
        trafficDelays[node.first] = 0;
    }

    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        string currentNode = pq.top().second;
        pq.pop();

        if (currentNode == end) break;

        for (auto &edge : graph[currentNode]) {
            // Skip blocked roads
            if (conditions[edge.destination].isBlocked) {
                cout << "Road from " << currentNode << " to " << edge.destination << " is blocked.\n";
                continue;
            }

            int newDistance = currentDistance + edge.distance;
            int newTrafficDelay = trafficDelays[currentNode] + conditions[edge.destination].trafficDelay;

            if (newDistance < distances[edge.destination]) {
                distances[edge.destination] = newDistance;
                trafficDelays[edge.destination] = newTrafficDelay;
                pq.push({newDistance, edge.destination});
                previous[edge.destination] = currentNode;
            }
        }
    }

    vector<string> path;
    int totalDistance = distances[end];
    int totalTrafficDelay = trafficDelays[end];

    // Check if a path exists
    if (totalDistance == INT_MAX || previous.find(end) == previous.end()) {
        cout << "No path available due to road blockages.\n";
        return {{}, 0};
    }

    for (string at = end; at != start; at = previous[at]) {
        path.push_back(at);
    }
    path.push_back(start);

    reverse(path.begin(), path.end());
    return {path, totalTrafficDelay};
}

int main() {
    unordered_map<string, vector<Edge>> graph;
    unordered_map<string, RoadCondition> conditions;

    int n;
    cout << "Enter the number of roads: ";
    cin >> n;

    cout << "Enter the roads (start city, end city, distance in km):\n";
    for (int i = 0; i < n; ++i) {
        string u, v;
        int distance;
        cin >> u >> v >> distance;
        addEdge(graph, u, v, distance);
    }

    int m;
    cout << "Enter the number of road conditions: ";
    cin >> m;

    cout << "Enter the road conditions (city, isBlocked (0/1), traffic delay in minutes):\n";
    for (int i = 0; i < m; ++i) {
        string city;
        bool isBlocked;
        int trafficDelay;
        cin >> city >> isBlocked >> trafficDelay;
        conditions[city] = {isBlocked, trafficDelay};
    }

    string start, end;
    cout << "Enter start location: ";
    cin >> start;
    cout << "Enter end location: ";
    cin >> end;

    auto result = dijkstra(graph, conditions, start, end);
    vector<string> shortestPath = result.first;
    int totalTrafficDelay = result.second;

    if (!shortestPath.empty()) {
        cout << "Shortest path: ";
        int totalDistance = 0;
        for (size_t i = 0; i < shortestPath.size() - 1; ++i) {
            for (auto &edge : graph[shortestPath[i]]) {
                if (edge.destination == shortestPath[i + 1]) {
                    totalDistance += edge.distance;
                    break;
                }
            }
            cout << shortestPath[i] << " -> ";
        }
        cout << shortestPath.back() << endl;

        cout << "Total distance: " << totalDistance << " km\n";
        cout << "Total traffic delay: " << totalTrafficDelay << " minutes\n";
    }

    return 0;
}
