#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;


int R;
int C;
int F;
int F_act;
int N;
int N_act;
int B;
int T;

struct pos {
    int x, y;

    pos() : pos(0, 0) {}

    pos(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator==(const pos& p) const {
        return x == p.x && y == p.y;
    }

};

struct pos_hasher {
public:
    size_t operator()(const pos &p) const {
        return hash<int>()(p.x) ^ hash<int>()(p.y);
    }
};

struct task {
public:
    pos init;
    pos end;
    int s, f;
    int id;

    task(ifstream& input, int id) {
        input >> init.x >> init.y >> end.x >> end.y >> s >> f;
        this->id = id;
    }
};

struct car {
    pos p;
    int time;
    vector<int> tasks;
    queue<pos> cache;
    unordered_set<pos, pos_hasher> used;
    bool deleted;

    car() {
        p = pos(0, 0);
        time = 0;
        deleted = false;
    }

    void print() {
        cout << tasks.size() << " ";
        for(int i = 0; i < tasks.size(); ++i) {
            cout << tasks[i] << " ";
        }
        cout << endl;
    }

    int update_task(task t) {
        tasks.push_back(t.id);

        int D = abs(p.x - t.init.x) + abs(p.y - t.init.y );
        int Ben = abs(t.init.x - t.end.x) + abs(t.end.y - t.init.y);
        int Te = max(D,t.s - time);
        p = t.end;
        if (time + Te > t.s) {
            time = Te + time + Ben;
            return Ben;
        }
        else {
            time = time + Te + Ben;
            return Ben + B;
        }

    }

    int propose_task(task t) {
        int D = abs(p.x - t.init.x) + abs(p.y - t.init.y );
        int Ben = abs(t.init.x - t.end.x) + abs(t.end.y - t.init.y);
        int Te = max(D,t.s - time);
        int Bonus = 1;
        if (Te > D) Bonus = 0;

        //POSSIBLE O NO
        if (time + D + Ben > t.f) return -1;

        double Dw = Te / (R+C);
        double Benw = Ben/ (R+C);
        double Bw = Bonus*B / (Benw*10000);

        double Taskw = N_act / N;

        double c1 = 0.1;
        double c2 = 0.3;
        double c3 = 0.3;
        double c4 = 1- c1 - c2 - c3;
        double Pref = 0.001;
        double prob = c1 * Dw + c2 * Benw + c3 * Bw + c4 * Taskw;

        if (prob > Pref) return 1;
        return 1;
        return 0;

    }
};

class comparator {
public:
    bool operator()(task a, task b) {
        if(a.s != b.s) {
            return a.s > b.s;
        } else {
            return a.f <= b.f;
        }
    }
};

typedef priority_queue<task, vector<task>, comparator> cell;

typedef vector<vector<cell>> matrix;

matrix m;
int score;

vector<car> cars;

void read(ifstream& input) {
    input >> R >> C >> F >> N >> B >> T;
    N_act = N;
    F_act = F;
    score = 0;

    m = matrix(R, vector<cell>(C));

    for(int i = 0; i < N; ++i) {
        task t = task(input, i);
        m[t.init.x][t.init.y].push(t);
    }

    cars = vector<car>(F);
}


void solve() {

    while(F_act > 0) {
        for(int i = 0; i < cars.size() && N_act > 0; ++i) {
//            cout << i << endl;
//            cout << N_act << endl
            cout << F_act << endl;

            car& car = cars[i];
            if(car.deleted) {
                break;
            }

            queue<pos> q;
            if(!car.cache.empty()) {
                q = car.cache;
            } else {
                q.push(car.p);
            }

            int dist = 0;
            int current_level = 1;
            int next_level = 0;

            bool stop = false;

            while(!q.empty() && !stop) {
                pos p = q.front();
//                cout << "current pos: " << p.x << ", " << p.y << endl;
                q.pop();
//                cout << "dist: " << dist << endl;

                cell& cell = m[p.x][p.y];
                if(!cell.empty()) {

                    int res = car.propose_task(cell.top());
                    if(res == 0) {
                        car.cache = q;
                        break;
                    } else if(res == 1) {
                        N_act--;
                        score += car.update_task(cell.top());
                        cell.pop();
                    } else { // This task is not feasible, keep looking for another one
                        // Propose the rest of tasks in this cell
                        // NOT BY NOW
                    }
                }

                for(int r = -1; r <= 1; ++r) {
                    for(int c = -1; c <= 1; ++c) {
                        if((r == 0 || c == 0) && (r != 0 || c != 0)) {
                            pos next = p;
                            next.x += r;
                            next.y += c;
                            if(next.x >= 0 && next.y >= 0 && next.x < R && next.y < C) {
                                int d = abs(next.x - car.p.x) + abs(next.y - car.p.y );
                                if(d > dist) {
                                    pair<int, int> aux(next.x, next.y);
                                    if(car.used.count(next) == 0) {
                                        car.used.insert(next);
                                        q.push(next);
                                        ++next_level;
                                    }
                                }
                            }
                        }
                    }
                }


                if(--current_level == 0) {
                    current_level = next_level;
                    next_level = 0;
                    ++dist;

                    car.used.clear();
                }

            }

            if(q.empty()) {
                --F_act;
                car.deleted = true;
                car.used.clear();
                // TODO delete this car since it can't get another task ever
            }

        }
    }
}


int main(int num_args, char* args[]) {

//    if(num_args != 2) {
//        cout << "usage: ./main input_file.in" << endl;
//        exit(1);
//    }

    ifstream input;
    input.open(args[1]);

    read(input);

    solve();

    cout << "score: " << score << endl;

    for(int i = 0; i < F; ++i) {
        cars[i].print();
    }
}