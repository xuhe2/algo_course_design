#include <bits/stdc++.h>
using namespace std;

#include "json.hpp"
using json = nlohmann::json;

int V;
int dis[1010];
bool vis[1010];
int head[1010], EdgeCount;
typedef pair<int, int> P; // first是最短距离，second是顶点的编号

int from[1010];                       // 记录最短路径的边
std::map<int, std::string> from_type; // 记录来到这个点的方式
int final_distance[1010];             // 记录最短路径的距离
int final_money[1010];                // 记录最短路径的花费

struct EDGE
{
    int u, v, w, nxt;
    int distance, money;
    std::string from_type;
} edge[1010 * 10];
//
void init()
{ // 初始化
    fill(head, head + V + 1, -1);
    EdgeCount = 0;
}
//
void add_edge(int u, int v, int w, int distance, int money, std::string from_type)
{
    edge[EdgeCount] = EDGE{u, v, w, head[u], distance, money, from_type}; // nxt
    head[u] = EdgeCount++;
}
//
void dijkstra(int pos_begin)
{
    priority_queue<P, vector<P>, greater<P>> heap;
    memset(dis, 0x3f, sizeof(dis));
    memset(vis, 0, sizeof(vis));

    dis[pos_begin] = 0;

    from[pos_begin] = pos_begin;
    final_distance[pos_begin] = 0;
    final_money[pos_begin] = 0;
    from_type[pos_begin] = "start";

    heap.push(P(0, pos_begin));

    while (!heap.empty())
    {
        P p = heap.top();
        heap.pop();
        int v = p.second;
        if (dis[v] < p.first)
            continue;
        for (int i = head[v]; ~i; i = edge[i].nxt)
        {
            int now = edge[i].u;
            int to = edge[i].v;
            int w = edge[i].w;
            if (dis[to] > dis[now] + w)
            {
                dis[to] = dis[now] + w;
                // 记录距离和费用
                final_distance[to] = final_distance[now] + edge[i].distance;
                final_money[to] = final_money[now] + edge[i].money;
                from_type[to] = edge[i].from_type;

                heap.push(P{dis[to], to});
                from[to] = now;
            }
        }
    }
}

int getWeight(int distance, int money, double percent)
{
    return (int)(distance * percent + money * (1 - percent));
}

json dijkstra_run(const std::string &start_pos, const std::string &end_pos, double percent, int car_cost, int plane_cost, int train_cost, int min_transition)
{
    // 初始化链式前向星
    V = 500;
    init();
    // 读取MAP
    std::ifstream f;
    f.open("map.json", std::ios::out | std::ios::binary);
    if (!f)
        std::cout << "error: file" << std::endl;
    json data = json::parse(f);
    // 把文字地名转化为数字，下标从1开始
    int index = 0;
    map<string, int> name2id;     // 从文字名字到ID
    map<int, string> id2name;     // 从ID到文字名字
    set<string> name_set;         // 存储文字名字，用于去重
    for (auto &p : data["roads"]) // 遍历全部的路线
    {
        string s1 = p["start"];
        string s2 = p["end"];
        // 给名字一个ID值方便索引
        if (name_set.find(s1) == name_set.end())
        {
            name_set.insert(s1);
            name2id[s1] = ++index;
            id2name[index] = s1;
        }
        if (name_set.find(s2) == name_set.end())
        {
            name_set.insert(s2);
            name2id[s2] = ++index;
            id2name[index] = s2;
        }
        // 加入一个边
        int id1 = name2id[s1];
        int id2 = name2id[s2];
        int distance = p["distance"];
        int money = p["money"];
        int weight = getWeight(distance, money, percent);
        std::string travel_type = p["type"];
        // 对不同的出行方式做修正
        if (p["type"] == "train")
            weight += train_cost;
        if (p["type"] == "plane")
            weight += plane_cost;
        if (p["type"] == "car")
            weight += car_cost;
        // 如果查找最少的站数
        if (min_transition == 1)
            weight = 1;
        // 加入边
        add_edge(id1, id2, weight, distance, money, travel_type);
    }
    // 使用dijkstra找最短路
    dijkstra(name2id[start_pos]);
    // json存储结果
    json result;
    result["start"] = start_pos;
    result["end"] = end_pos;
    result["distance"] = final_distance[name2id[end_pos]];
    result["money"] = final_money[name2id[end_pos]];
    // 使用stack获得路径
    stack<int> path_stk;
    path_stk.push(name2id[end_pos]);
    while (path_stk.top() != name2id[start_pos])
    {
        path_stk.push(from[path_stk.top()]);
    }
    // 输出结果
    string path_str = "";
    while (!path_stk.empty())
    {
        path_str += id2name[path_stk.top()];
        path_stk.pop();
        // 非空的话,加上`->`作为连接
        if (!path_stk.empty())
        {
            path_str += "-[";
            path_str += from_type[path_stk.top()];
            path_str += "]->";
        }
    }
    // 把路径存入JSON
    result["path"] = path_str;

    return result;
}

// int main()
// {
//     json data = run("beijing", "shanghai", 1, 100, 100, 100, false);
//     std::cout << data << endl;
//     return 0;
// }

// int main()
// {
//     V = 3;
//     init();
//     add_edge(1, 2, 1);
//     add_edge(1, 3, 100);
//     add_edge(2, 3, 2);
//     dijkstra(1);

//     stack<int> s;
//     s.push(3);
//     while (s.top() != 1)
//     {
//         s.push(from[s.top()]);
//     }

//     while (!s.empty())
//     {
//         cout << s.top() << " ";
//         s.pop();
//     }
//     return 0;
// }
