/* Problem statement:
You have a NxN map of the museum with rooms. Each room has doors to get to adjacent room (you can go left, right, up or down). The room can be either closed or has 1,2,3 or 4 doors open. We also have guards in the museum. Guards are located in the rooms. Propose solution where we return a 'metric' for each room. A metric is a distance to the nearest guard. She also asked me on testcases.
*/

#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <queue>
#include <utility>
#include <tuple>

using namespace std;

static const unsigned N = 10;
//static const unsigned UINT_MAX = numeric_limits<unsigned>::max()
typedef enum {OPEN, CLOSE} dstate_t;

typedef struct Room {
	dstate_t west, north, east, south;
	bool has_guard;
	unsigned dist;
	//Room() {};
	//Room(bool has_guard, dstate_t west, dstate_t north, dstate_t east, dstate_t south) : dist(UINT_MAX), has_guard(has_guard), west(west), north(north), east(east), south(south) {};
} Room;

typedef vector<vector<Room>> Floorplan;

ostream& operator<<(ostream& stream, const Floorplan& fp) {
	for(unsigned i=0; i<N; ++i) {
		for(unsigned j=0; j<N; ++j)
			stream << fp[i][j].dist << "(" << fp[i][j].has_guard << ") ";
		stream << endl;
	}
	return stream;
}

bool floorplans_equal(const Floorplan& fp1, const Floorplan& fp2) {
	for(unsigned i=0; i<N; ++i)
		for(unsigned j=0; j<N; ++j)
			if(fp1[i][j].dist != fp2[i][j].dist) {
				cout << "Smutny Pawel :(" << endl;
				return false;
			}
	return true;
}

Floorplan generate_random_floorplan() {
	Floorplan ret = vector<vector<Room>>(N, vector<Room>(N, Room()));

	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis4(1, 4); //uniform 1..4 (inclusive) distribution
    std::uniform_int_distribution<> dis8(1, 8); //uniform 1..8 (inclusive) distribution

	for(unsigned i=0; i<N; ++i)
		for(unsigned j=0; j<N; ++j) {
			if(i != 0) ret[i][j].north = ret[i-1][j].south;
			if(j != 0) ret[i][j].west = ret[i][j-1].east;
			ret[i][j].east = (dis4(gen) == 1 ? CLOSE : OPEN);
			ret[i][j].south = (dis4(gen) == 1 ? CLOSE : OPEN);
			if(dis8(gen) == 1) {
				ret[i][j].has_guard = true;
				ret[i][j].dist = 0;
			} else {
				ret[i][j].has_guard = false;
				ret[i][j].dist = UINT_MAX;
			}
		}
	return ret;
}

void solve_bfs(Floorplan& fp) {
	bool visited[N][N];
	queue<tuple<unsigned, unsigned, unsigned>> q;
	memset(visited, 0, N*N*sizeof(bool));

	for(unsigned i=0; i<N; ++i)
		for(unsigned j=0; j<N; ++j)
			if(fp[i][j].has_guard)
				q.push(make_tuple(i, j, 0));

	unsigned i, j, dist;
	while(!q.empty()) {
		tie(i, j, dist) = q.front();
		q.pop();
		if(visited[i][j])
			continue;
		visited[i][j] = true;
		fp[i][j].dist = dist;
		if(j != 0 && fp[i][j].west == OPEN && !visited[i][j-1]) {
			q.push(make_tuple(i, j-1, dist+1));
		}
		if(i != 0 && fp[i][j].north == OPEN && !visited[i-1][j]) {
			q.push(make_tuple(i-1, j, dist+1));
		}
		if(j != N-1 && fp[i][j].east == OPEN && !visited[i][j+1]) {
			q.push(make_tuple(i, j+1, dist+1));
		}
		if(i != N-1 && fp[i][j].south == OPEN && !visited[i+1][j]) {
			q.push(make_tuple(i+1, j, dist+1));
		}
	}
}
void solve_dp(Floorplan& fp) {
	//first pass
	for(unsigned i=0; i<N; ++i)
		for(unsigned j=0; j<N; ++j) {
			unsigned north = UINT_MAX, west = UINT_MAX;
			if(fp[i][j].north == OPEN && i != 0)
				north = (fp[i-1][j].dist == UINT_MAX ? UINT_MAX : fp[i-1][j].dist+1);
			if(fp[i][j].west == OPEN && j != 0)
				west = (fp[i][j-1].dist == UINT_MAX ? UINT_MAX : fp[i][j-1].dist+1);
			fp[i][j].dist = min({fp[i][j].dist, north, west});
		}
		cout << fp << endl;
	//second pass
	for(int i=N-1; i>=0; --i)
		for(int j=N-1; j>=0; --j) {
			unsigned south = UINT_MAX, east = UINT_MAX;
			if(fp[i][j].south == OPEN && i != N-1)
				south = (fp[i+1][j].dist == UINT_MAX ? UINT_MAX : fp[i+1][j].dist+1);
			if(fp[i][j].east == OPEN && j != N-1)
				east = (fp[i][j+1].dist == UINT_MAX ? UINT_MAX : fp[i][j+1].dist+1);
			fp[i][j].dist = std::min({fp[i][j].dist, south, east});
		}
}

int main() {
	auto fp_dp = generate_random_floorplan();
	auto fp_bfs = fp_dp;
	solve_dp(fp_dp);
	solve_bfs(fp_bfs);
	cout << fp_dp;
	cout << endl;
	cout << fp_bfs;

	floorplans_equal(fp_dp, fp_bfs);
}