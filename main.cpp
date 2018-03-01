#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

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
    int t;
    vector<int> tasks;

    car() {
        p = pos(0, 0);
        t = 0;
    }

    void print() {
        cout << tasks.size() << " ";
        for(int i = 0; i < tasks.size(); ++i) {
            cout << tasks[i] << " ";
        }
        cout << endl;
    }

    int propose_task(task t) {

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

int R;
int C;
int F;
int N;
int B;
int T;

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