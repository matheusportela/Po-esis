// @file   ActionScheduler.h
// @author Matheus Vieira Portela
// @date   27/04/2015
//
// @brief Schedule actions to execute sequentially.

#ifndef ACTION_SCHEDULER_H_
#define ACTION_SCHEDULER_H_

#include <memory>
#include <queue>

#include "Action.h"

// REMOVE AFTER TESTS
#include "Logger.h"

class ActionScheduler
{
  public:
    // Schedules an action by pushing it to the end of the queue.
    void Schedule(std::shared_ptr<Action> action);

    // Executes the front action, if any.
    void Execute();

  private:
    // Pointer to the action that is being currently executed.
    std::shared_ptr<Action> executingAction;

    // Actions to be executed.
    std::queue<std::shared_ptr<Action>> queue;
};

#endif // ACTION_SCHEDULER_H_