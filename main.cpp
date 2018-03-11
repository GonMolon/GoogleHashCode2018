#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

int R;
int C;
int F;
int N;
int N_act;
int B;
int T;
int total_assigned_tasks;
int score;

struct pos {
    int x, y;

    pos() : pos(0, 0) {}

    pos(int x, int y) {
        this->x = x;
        this->y = y;
    }

    int dist(const pos& p) const {
        return abs(x - p.x) + abs(y - p.y);
    }
};


struct task {
public:
    pos init;
    pos end;
    int length;
    int s, f;
    bool available;
    int possible_cars;

    task() {}

    task(ifstream& input) {
        input >> init.x >> init.y >> end.x >> end.y >> s >> f;
        available = true;
        possible_cars = F;
        length = init.dist(end);
    }

    void rejected_from_car() {
        if(--possible_cars == 0 && available) {
            --N_act;
        }
    }
};

vector<task> tasks;

struct car {
    pos p;
    int time;
    list<int> available_tasks;
    list<int> assigned_tasks;

    car() {
        p = pos(0, 0);
        time = 0;
        for(int i = 0; i < N; ++i) {
            available_tasks.push_back(i);
        }
    }

    void print() {
        cout << assigned_tasks.size() << " ";
        for(int id : assigned_tasks) {
            cout << id << " ";
        }
        cout << endl;
    }

    void assign_task(int id) {
        task& t = tasks[id];

        assigned_tasks.push_back(id);
        t.available = false;
        --N_act;
        ++total_assigned_tasks;
        score += get_score(t);

        int dist = p.dist(t.init);
        time = time + max(dist, t.s - time) + t.length;
        p = t.end;
        if(T < time) {
            cout << "ERROR" << endl;
            exit(1);
        }
    }

    int get_score(task& t) {
        int dist = p.dist(t.init);
        bool bonus = max(dist, t.s - time) > dist; // Maybe greater equal?
        return bonus * B + t.length;
    }

    double get_heuristic(task& t) {
        int dist = p.dist(t.init);
        return get_score(t) - dist; // TODO penalize waiting time
    }

    bool is_reachable(task& t) {
        int dist = p.dist(t.init);
        return time + dist + t.length <= t.f;
    }

    int choose_task() {
        int best_task = -1;
        double best_heuristic = 0;

        auto it = available_tasks.begin();
        while(it != available_tasks.end()) {
            int id = *it;
            if(not tasks[id].available || not is_reachable(tasks[id])) {
                tasks[id].rejected_from_car();
                it = available_tasks.erase(it);
            } else {
                double heuristic = get_heuristic(tasks[id]);
                if(best_task == -1 || heuristic > best_heuristic) {
                    best_task = id;
                    best_heuristic = heuristic;
                }
                ++it;
            }
        }

        return best_task;
    }
};

list<car> cars;
list<car> used_cars;

void read(ifstream& input) {
    input >> R >> C >> F >> N >> B >> T;
    N_act = N;
    total_assigned_tasks = 0;
    score = 0;

    tasks = vector<task>(N);
    cars = list<car>(F);

    for(int i = 0; i < N; ++i) {
        task t = task(input);
        tasks[i] = t;
    }
}

void solve() {
    while(cars.size() > 0) {
//        cout << "tasks: " << N_act << endl;
//        cout << "cars: " << cars.size() << endl;
        double best_heuristic = 0;
        list<car>::iterator selected_car = cars.end();
        int assigned_task = 0;

        auto it = cars.begin();
        while(it != cars.end()) {
            int id = it->choose_task();
            if(id == -1) {
                used_cars.push_back(*it);
                it = cars.erase(it);
            } else {
                double heuristic = it->get_heuristic(tasks[id]);
                if(selected_car == cars.end() || heuristic > best_heuristic) {
                    best_heuristic = heuristic;
                    selected_car = it;
                    assigned_task = id;
                }
                ++it;
            }
        }
        if(selected_car != cars.end()) {
            selected_car->assign_task(assigned_task);
        }
    }
}


int main(int num_args, char* args[]) {

    if(num_args != 2) {
        cout << "usage: ./main input_file.in" << endl;
        exit(1);
    }

    ifstream input;
    input.open(args[1]);

    read(input);

    solve();

    for(car c : used_cars) {
        c.print();
    }

//    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
//    cout << "score: " << score << endl;
//    cout << "ratio: " << ((double)total_assigned_tasks)/N << endl;

}