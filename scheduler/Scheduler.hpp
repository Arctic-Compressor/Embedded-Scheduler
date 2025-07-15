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

/**
 * Scheduler Class Declaration
 */
class Scheduler
{
public:

    /**
     * Task class
     * This represents each tasks handled by the scheduler
     */
    class Task
    {
        public:
            friend class Scheduler; /* Give Scheduler access to internal variables */

            /* Constructor */
            Task(){}
            Task(void (*func)(), volatile uint32_t interval) :
                func(func),
                interval(interval)
            {
            }


            /* Public members */
            void (*func)();
            volatile uint32_t interval;

        private:
            /* Internal variables */
            uint32_t last_called_ = 0;
    };

    /* Constructor */
    Scheduler(/* args */);
    ~Scheduler();

    /**
     * APIs
     */
    bool init(Task* const taskTable, const uint16_t num_tasks, const uint32_t systick_interval);
    void run(void);
    uint32_t tick(void);
    uint32_t getTickCount(void);

    volatile uint32_t sys_tick_ctr_ = 0;    /*!< System tick counter */
private:
    /* Internal variables */

    uint16_t num_tasks_ = 0;                /*!< Number of tasks in the task table */
    Task* task_table_ = NULL;               /*!< Pointer to the task table */

};
