#ifndef LOCAL_SEARCH_STATE_H
#define LOCAL_SEARCH_STATE_H

#include <list>
#include <memory>
#include <iostream>

using namespace std;

class State {

protected:
//    shared_ptr<State> child;

    bool synced;

public:

    State();

    State(const State& state) : State() {}

    virtual list<shared_ptr<State>> generate_children() { return list<shared_ptr<State>>(); };

    virtual shared_ptr<State> generate_random_child() { return nullptr; };

    virtual double get_score() const = 0;

    // Sets this state as a current one in the local search
    virtual void sync() = 0;

//    class Modifier {
//
//        virtual bool modify(State* state) = 0;
//    };
};

#endif