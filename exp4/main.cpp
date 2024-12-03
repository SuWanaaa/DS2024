#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <climits>

using namespace std;

class Graph {
public:
    vector<list<pair<int, int>>> adjList;

    Graph(int n) {
        adjList.resize(n);
    }

    void addEdge(int src, int dest, int weight) {
        adjList[src].push_back(make_pair(dest, weight));
        adjList[dest].push_back(make_pair(src, weight)); // 无向图
    }

    // 广度优先搜索（BFS）
    void BFS(int start) {
        vector<bool> visited(adjList.size(), false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";

            for (auto &neighbor : adjList[node]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        cout << endl;
    }

    // 深度优先搜索（DFS）
    void DFS(int start) {
        vector<bool> visited(adjList.size(), false);
        DFSUtil(start, visited);
        cout << endl;
    }

private:
    void DFSUtil(int node, vector<bool> &visited) {
        visited[node] = true;
        cout << node << " ";

        for (auto &neighbor : adjList[node]) {
            if (!visited[neighbor.first]) {
                DFSUtil(neighbor.first, visited);
            }
        }
    }

public:
    // Dijkstra算法实现最短路径
    void Dijkstra(int start) {
        vector<int> dist(adjList.size(), INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[start] = 0;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            // 如果当前节点的距离大于记录的最短距离，跳过
            if (dist[u] < pq.top().first) continue;

            for (auto &neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        // 输出每个节点的最短路径
        for (int i = 0; i < adjList.size(); i++) {
            if (dist[i] == INT_MAX) {
                cout << "节点 " << i << " 不可达" << endl;
            } else {
                cout << "从节点 " << start << " 到节点 " << i << " 的最短路径长度为 " << dist[i] << endl;
            }
        }
    }

    // Prim算法实现最小生成树
    void Prim() {
        vector<int> key(adjList.size(), INT_MAX);
        vector<bool> inMST(adjList.size(), false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        key[0] = 0;
        pq.push(make_pair(0, 0));

        int totalWeight = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (inMST[u]) continue;

            inMST[u] = true;
            totalWeight += key[u];

            for (auto &neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    pq.push(make_pair(key[v], v));
                }
            }
        }

        cout << "最小生成树的总权重为：" << totalWeight << endl;
    }
};

int main() {
    // 创建一个图，节点数为6
    Graph g(6);

    // 添加边 (源节点, 目标节点, 权重)
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(2, 1, 2);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 8);
    g.addEdge(3, 4, 3);
    g.addEdge(4, 5, 6);

    // 测试BFS
    cout << "广度优先搜索(BFS)结果: ";
    g.BFS(0);

	cout<<"/n";

    // 测试DFS
    cout << "深度优先搜索(DFS)结果: ";
    g.DFS(0);
    
    cout<<"/n";

    // 测试Dijkstra最短路径
    cout << "Dijkstra算法最短路径：\n";
    g.Dijkstra(0);

    // 测试Prim最小生成树
    cout << "Prim算法最小生成树：\n";
    g.Prim();

    return 0;
}
