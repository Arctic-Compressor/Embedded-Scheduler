/**
 * @file Scheduler.hpp
 * @author Niel Cansino (nielcansino@gmail.com)
 * @brief header for Scheduler.hpp
 * @version 0.1
 * @date 2020-07-23
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

/* Make sure UINT32_MAX is present*/
#ifndef UINT32_MAX
    #define UINT32_MAX  (0xFFFFFFFF)
#endif

#ifndef NULL
    #define NULL (0)
#endif

class Scheduler {
public:
    /**
     * @brief A single task to be ran by the scheduler.
     *
     */
    class Task {
        public:
            friend class Scheduler;

            /**
             * @brief Construct a new Task to be ran by the scheduler. This task
             * should be initialized as part of an array.
             *
             * @param func Function point to be ran by the scheduler.
             * @param interval Interval (typically in microseconds) that the scheduler runs the function.
             */
            Task(void (*func)(), volatile uint32_t interval) : func(func), interval(interval) {}

            void (*func)();
            volatile uint32_t interval;

        private:
            uint32_t last_called_ = 0;
    };

    /**
     * @brief System tick count, typically represented in microseconds.
     * Public access is given to allow for control within ISR without a function call.
     * Do not decrement this value.
     */
    volatile uint32_t sys_tick_ctr_ = 0;    /*!< System tick counter */


    /**
     * @brief   Initializes the scheduler object.
     *          This function binds the array of tasks [taskTable]
     *          to be executed by the scheduler.
     *          This also gives the object information on how long a systick is.
     *
     * @param taskTable Array of type [Task*] that has the pointer to the tasks
     *                  that will be used by the scheduler.
     * @param num_tasks Number of members in array [taskTable]
     * @param systick_interval  Actual duration of a single systick, typically in microseconds
     * @return true     On successful initialization
     * @return false    Returns false when one of the functions in the [taskTable] is null.
     */
    bool init(Task* const taskTable, const uint16_t num_tasks, const uint32_t systick_interval);

    /**
     * @brief   Initializes the scheduler object.
     *          This function binds the array of tasks [taskTable]
     *          to be executed by the scheduler.
     *
     * @param taskTable Array of type [Task*] that has the pointer to the tasks
     *                  that will be used by the scheduler.
     * @param systick_interval  Actual duration of a single systick, typically in microseconds
     * @return true     On successful initialization
     * @return false    Returns false when one of the functions in the [taskTable] is null.
     */
    bool init(Task* const taskTable, const uint16_t num_tasks);

    /**
     * @brief Runs the tasks registered via init().
     *
     */
    void run(void);

    /**
     * @brief Increments the system tick by the systick_interval.
     *
     * @return uint32_t Current tick
     */
    uint32_t tick(void);

    /**
     * @brief Get the system tick counter value
     *
     * @return uint32_t System Tick Counter Value
     */
    uint32_t getTickCount(void);

    /**
     * @brief Set the system tick interval
     *
     * @param systick_interval Duration of a single systick, typically in microseconds
     */
    void setTickInterval(const uint32_t systick_interval);

private:
    uint32_t systick_interval_ = 1;
    uint16_t num_tasks_ = 0;                /*!< Number of tasks in the task table */
    Task* task_table_ = NULL;               /*!< Pointer to the task table */

};
