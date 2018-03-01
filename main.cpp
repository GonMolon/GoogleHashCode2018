#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;


int R;
int C;
int F;
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

    car() {
        p = pos(0, 0);
        time = 0;
    }

    void print() {
        cout << tasks.size() << " ";
        for(int i = 0; i < tasks.size(); ++i) {
            cout << tasks[i] << " ";
        }
        cout << endl;
    }

    int update_task(task t) {
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

vector<car> cars;


void read(ifstream& input) {
    input >> R >> C >> F >> N >> B >> T;

    m = matrix(R, vector<cell>(C));

    for(int i = 0; i < N; ++i) {
        task t = task(input, i);
        m[t.init.x][t.init.y].push(t);
    }

    cars = vector<car>(F);
}


void solve() {
    for(int i = 0; i < cars.size(); ++i) {

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

    for(int i = 0; i < F; ++i) {
        cars[i].print();
    }
}