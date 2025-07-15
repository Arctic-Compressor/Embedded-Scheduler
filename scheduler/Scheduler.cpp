/**
 * @file Scheduler.cpp
 * @author Niel Cansino (nielcansino@gmail.com)
 * @brief A lean, lightweight, and portable scheduler designed for embedded C++ applications.
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

#include "Scheduler.hpp"

bool Scheduler::init(Task* const taskTable, const uint16_t num_tasks, const uint32_t systick_interval) {
    this->systick_interval_ = systick_interval;
    bool retval = false;

    /* Checks for null pointer */
    if( taskTable == NULL ) return retval;

    /* Checks whether the functions are not NULL */
    for( uint16_t i = 0; i < num_tasks; ++i )
    {
        if( taskTable[i].func == NULL )
            return retval;
    }

    /* Attaches the taskTable and num_tasks to internal variables */
    task_table_ = taskTable;
    num_tasks_ = num_tasks;

    /*  Initializes the last_called_ to
    *   (UINT32_MAX - interval + 1) so that function is called
    *   on first instance of run().
    */
    for( uint16_t i = 0; i < num_tasks; ++i )
    {
        task_table_[i].last_called_ = UINT32_MAX - task_table_[i].interval + 1;
    }

    /* Initialize system tick counter to zero */
    sys_tick_ctr_ = 0;

    retval = true;
    return retval;
}

bool Scheduler::init(Task* const taskTable, const uint16_t num_tasks) {
    return init(taskTable, num_tasks, 1);
}

inline uint32_t Scheduler::tick(void)
{
    return sys_tick_ctr_ += systick_interval_;
}

inline uint32_t Scheduler::getTickCount(void)
{
    return sys_tick_ctr_;
}

inline void Scheduler::setTickInterval(const uint32_t systick_interval) {
    this->systick_interval_ = systick_interval;
}

void Scheduler::run(void)
{
    uint32_t sysctr;

    /* Loop across the tasks */
    for( uint16_t i = 0; i < num_tasks_; ++i )
    {
        /* obtain a copy of the sys_tick_ctr at the execution to avoid concurrency */
        sysctr = sys_tick_ctr_;

        /* Breaks the loop on NULL existence */
        if( task_table_[i].func == NULL )
            break;

        /* Run the tasks */
        if( task_table_[i].interval == 0 )
        {
            /* Run continuous tasks */
            (*(task_table_[i].func))();
        }
        else if ( sysctr - task_table_[i].last_called_ >= task_table_[i].interval )
        {
            /* Run the tasks that are already due */
            (*(task_table_[i].func))();

            /* Update last_called_.
             * using sysctr instead of sys_tick_ctr makes sure that
             * the counter value is the same at the start and end of the function
             */
            task_table_[i].last_called_ = sysctr;
        }
        else
        {
            /* do nothing */
            continue;
        }

    }
}
