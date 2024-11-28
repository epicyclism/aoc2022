#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <set>
#include <map>
#include <algorithm>
#include <bit>

using namespace std;

static int idx = 0;
map<string, int> ni;
map<string, vector<pair<string, long long>>> adj;
map<string, int> flow;

size_t make_key(tuple<string, long long, int, int>& k)
{
	size_t h1 = hash<string>{}(get<0>(k));
	size_t h2 = hash<long long>{}(get<1>(k));
	size_t h3 = hash<int>{}(get<2>(k));
	size_t h4 = hash<int>{}(get<3>(k));

	return h1 ^ (h3 << 1) ^ (h4 << 2);
}

void fill_in()
{
	//fill in the adj nodes that dont have direct paths, with inf
	for (auto it = adj.begin(); it != adj.end(); it++)
	{
		set<pair<string, long long>> ss;
		for (auto& s : it->second)
		{
			ss.insert(pair<string, long long>{s});
		}

		for (auto sd = adj.begin(); sd != adj.end(); sd++)
		{
			if (ss.find({ sd->first, 1 }) == ss.end())
			{
				ss.insert({ sd->first, INT_MAX });
			}
		}

		vector<pair<string, long long>> cpy(ss.size());
		copy(ss.begin(), ss.end(), cpy.begin());

		it->second = cpy;
	}
}

void floyd_warshall()
{
	for (auto it = adj.begin(); it != adj.end(); it++)
	{
		string inter = it->first;
		for (auto i = adj.begin(); i != adj.end(); i++)
		{
			for (auto j = adj.begin(); j != adj.end(); j++)
			{
				if (i->first == j->first)
				{
					adj[i->first][ni[j->first]].second = 0;
					adj[j->first][ni[i->first]].second = 0;
					continue;
				}

				long long val = min(adj[i->first][ni[j->first]].second,
					adj[i->first][ni[it->first]].second + adj[it->first][ni[j->first]].second);

				val = min((long long)INT_MAX, val);

				adj[i->first][ni[j->first]].second = val;
			}
		}
	}
}

void cull_flowless_nodes()
{
	set<string> keys_to_cull;
	for (auto it = adj.begin(); it != adj.end(); it++)
	{
		for (int i = 0; i < it->second.size(); i++)
		{
			if (flow[it->second[i].first] == 0)
			{
				it->second.erase(it->second.begin() + i);
				i--;
			}
		}

		if (flow[it->first] == 0)
		{
			keys_to_cull.insert(it->first);
		}
	}

	for (auto it = keys_to_cull.begin(); it != keys_to_cull.end(); it++)
	{
		if (*it == "AA") continue;

		adj.erase(*it);
	}
}

void printadj()
{
	cout << "-=-=-=-=-=-=-" << endl;
	for (auto it = adj.begin(); it != adj.end(); it++)
	{
		cout << it->first << ": " << flow[it->first] << endl;
		cout << "\t";
		for (auto& s : it->second)
		{
			cout << s.first << "-" << s.second << "|" << flow[s.first] << " ";
		}
		cout << endl;
	}
	cout << "-=-=-=-=-=-=-" << endl;
}

int MAX_OPENED_VALVES;

int countSetBits(unsigned n)
{
	return std::popcount(n);
}

bool getbit(uint64_t n, unsigned b)
{
	return n & (1ULL << b);
}

uint64_t set_bit(uint64_t n, unsigned b)
{
	uint64_t bb{ 1ULL << b };
	return n | bb;
}

map <size_t, long long> dp;
long long DFS(string& curr, long long pressure, uint64_t opened, int elapsed)
{
#if 0
	tuple<string, long long, int, int> k = { curr, pressure, opened, elapsed };
	size_t key = make_key(k);


	if (dp.find(key) != dp.end())
	{
		return dp[key];
	}
#endif
	if (countSetBits(opened) == MAX_OPENED_VALVES || elapsed >= 30)
	{
//		cout << opened << " - " << elapsed << ": " << pressure << endl;
		return pressure;
	}

	long long max_pressure = INT_MIN;
	for (auto& nb : adj[curr])
	{
		if (getbit(opened, ni[nb.first]) == 0)
		{
			int cost = nb.second;
			int future = elapsed + cost + 1;

			//if (future >= 30) continue; //would take too long

			max_pressure = max(max_pressure, DFS(nb.first, pressure + ((30 - future) * flow[nb.first]),
				set_bit(opened, ni[nb.first]), future));
		}
	}

//	dp[key] = max_pressure;

	return max_pressure;
}

void dfs_visit(string& curr, int pressure, uint64_t opened, int elapsed, int& mx)
{
	pressure += flow[curr] * (30 - elapsed);
	for (auto& nb : adj[curr])
	{
		if (nb.first == curr)
			continue;
		auto d = nb.second;
		if (elapsed + d <= 30)
		{
			if (!getbit(opened, ni[nb.first]))
				dfs_visit( nb.first, pressure, set_bit(opened, ni[nb.first]), elapsed + d + 1, mx);
		}
		else
		{
			if (pressure > mx)
				mx = pressure;
		}
	}
}

int main()
{
//	fstream f = get_input();
	string ln;
	while (getline(cin, ln))
	{
		stringstream ss(ln);
		string node, a;
		long f;

		ss >> node >> f;
		adj[node] = vector<pair<string, long long>>();
		flow[node] = f;
		ni[node] = idx++;

		while (ss >> a)
		{
			adj[node].push_back({ a, 1 });
		}
	}

	fill_in();
	floyd_warshall();
	cull_flowless_nodes();
	printadj();

	MAX_OPENED_VALVES = adj.size() - 1;
	string START = "AA";
//	cout << "Part 1: " << DFS(START, 0, 0, 0);
	int mx{ 0 };
	dfs_visit(START, 0, set_bit(0, ni[START]), 0, mx);
	cout << "Part 1: " << mx << "\n";

	return 0;
}