#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <map>
#include <cassert>
#include <string>
#include <iostream>

namespace StateMachine {

enum StateMachineProcessResult {
    SUCCESS, FAILURE
};

typedef void (*stateCallback)(StateMachineProcessResult);

template <typename T>
class StateMachine;

template <typename T, typename E>
class StateTransition {
public:
    E mEvent;
    T mSrcState;
    T mEndState;

    explicit StateTransition(E event, T srcState, T endState)
        : mEvent(event), mSrcState(srcState), mEndState(endState) {}
};

template <typename T, typename E>
class FiniteStateMachine {
protected:
    T mCurrentState;
    std::map<E, std::map<T, std::unique_ptr<StateTransition<T, E>>>> mStateTransitions;

public:
    explicit FiniteStateMachine<T,E> (T initialState): mCurrentState(initialState) {
    }

    void add(T srcState, E event, T endState) {
        if (mStateTransitions.count(event) != 0) {
            auto& transitions = mStateTransitions[event];
            if (transitions.count(srcState) != 0) {
                assert(false && "Transition with src and event already exists");
            }
            auto newTransition = std::make_unique<StateTransition<T, E>>(event, srcState, endState);
            transitions[srcState] = std::move(newTransition);
        } else {
            auto newTransition = std::make_unique<StateTransition<T, E>>(event, srcState, endState);
            auto transitions = std::map<T, std::unique_ptr<StateTransition<T, E>>>();
            transitions[srcState] = std::move(newTransition);
            mStateTransitions[event] = std::move(transitions);
        }
    }

    void process(E event, void* execution, stateCallback callback) {
        if (mStateTransitions.count(event) != 0) {
            auto& transitions = mStateTransitions[event];
            if (transitions.count(mCurrentState) == 0) {
                callback(FAILURE);
                return;
            }
            auto& performableTransition = transitions[mCurrentState];
            mCurrentState = performableTransition->mEndState;
            callback(SUCCESS);
        }
    }
};

} // namespace StateMachine

#endif // STATEMACHINE_H
