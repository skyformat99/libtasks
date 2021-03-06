/*
 * Copyright (c) 2013-2014 ADTECH GmbH
 * Licensed under MIT (https://github.com/adtechlabs/libtasks/blob/master/COPYING)
 *
 * Author: Andreas Pohl
 */

#ifndef _TASKS_TASK_H_
#define _TASKS_TASK_H_

#include <functional>
#include <vector>

namespace tasks {

class worker;

/// The base class for any task.
class task {
  public:
    typedef std::function<void(worker* worker)> finish_func_worker_t;
    typedef std::function<void()> finish_func_void_t;
    struct finish_func_t {
        finish_func_t(finish_func_worker_t f) : m_type(0), m_f_worker(f) {}
        finish_func_t(finish_func_void_t f) : m_type(1), m_f_void(f) {}

        void operator()(worker* worker) {
            switch (m_type) {
                case 0:
                    m_f_worker(worker);
                    break;
                case 1:
                    m_f_void();
                    break;
            }
        }

        int m_type = 0;
        finish_func_worker_t m_f_worker;
        finish_func_void_t m_f_void;
    };

    virtual ~task() {}

    /// Returns true if auto deletion is active.
    inline bool auto_delete() const { return m_auto_delete; }

    /// Call this to deactivate auto deletion.
    inline void disable_auto_delete() { m_auto_delete = false; }

    /// Called by a worker when a task has auto_deletion enabled. 
    void finish(worker* worker = nullptr);

    /// If a task finishes it can execute callback functions. Note that no locks will be used at this
    /// level.
    inline void on_finish(finish_func_worker_t f) {
        m_finish_funcs.push_back(finish_func_t(f));
    }

    /// If a task finishes it can execute callback functions. Note that no locks will be used at this
    /// level.
    inline void on_finish(finish_func_void_t f) {
        m_finish_funcs.push_back(finish_func_t(f));
    }

  private:
    // Default behavior is to delete a task when handle_event returns false. Change this by calling
    // disable_auto_delete().
    bool m_auto_delete = true;

    std::vector<finish_func_t> m_finish_funcs;
};

}  // tasks

#endif  // _TASKS_TASK_H_
