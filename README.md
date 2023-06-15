# FiniteStateMachine
A simple, easy to use finite state machine in C++


FiniteStateMachine is a header-only library, so no special installation steps are needed. Add the header file directly to your project.


Example:

```c++
#include "statemachine.h"

enum class EventTypes {
  E1, E2
};

enum class StateTypes {
    S1, S2
};

int main() {
   StateMachine::FiniteStateMachine<StateTypes, EventTypes> fsm(StateTypes::S1);
   fsm.add(StateTypes::S1, EventTypes::E1, StateTypes::S2);
   fsm.add(StateTypes::S1, EventTypes::E2, StateTypes::S1);
   fsm.add(StateTypes::S2, EventTypes::E2, StateTypes::S1);

   StateMachine::stateCallback callback = [](StateMachine::StateMachineProcessResult res) {
       if(res == StateMachine::StateMachineProcessResult::SUCCESS) {
            std::cout << "Hello, World!" << std::endl;
       } else {
           std::cout << "Bye World!" << std::endl;
       }
   };

   fsm.process(EventTypes::E1, NULL, callback); // Hello World
   fsm.process(EventTypes::E2, NULL, callback); // Hello World
   fsm.process(EventTypes::E1, NULL, callback); // Hello World
   fsm.process(EventTypes::E1, NULL, callback); // Bye World, no trasition triggered by event E1 from source state S2
}

```
