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

/**
 * @brief Class constructor
 * 
 */
Scheduler::Scheduler(/* args */)
{
}

/**
 * @brief Destroy the Scheduler:: Scheduler object
 * 
 */
Scheduler::~Scheduler()
{
}

/**
 * @brief   Initializes the scheduler object.
 *          This function binds the array of tasks [taskTable] 
 *          to be executed by the scheduler.
 *          This also gives the object information on how long a systick is.
 * 
 * @param taskTable Array of type [Task*] that has the pointer to the tasks
 *                  that will be used by the scheduler.
 * @param num_tasks Number of members in array [taskTable]
 * @param systick_interval  Actual duration of a single systick, in microseconds
 * @return true     On successful initialization
 * @return false    Returns false when one of the functions in the [taskTable] is null.
 */
bool Scheduler::init(Task* const taskTable, const uint16_t num_tasks, const uint32_t systick_interval)
{
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

/**
 * @brief Increments the system tick 
 * 
 * @return uint32_t 
 */
uint32_t Scheduler::tick(void)
{
    return ++sys_tick_ctr_;
}

/**
 * @brief Get the system tick counter value
 * 
 * @return uint32_t System Tick Counter Value
 */
uint32_t Scheduler::getTickCount(void)
{
    return sys_tick_ctr_;
}

/**
 * @brief Runs the tasks registered via init().
 * 
 */
void Scheduler::run(void)
{
    /* Loop across the tasks */
    for( uint16_t i = 0; i < num_tasks_; ++i )
    {   
        /* Breaks the loop on NULL existence */
        if( task_table_[i].func == NULL ) 
            break;

        /* Run the tasks */
        if( task_table_[i].interval == 0 )
        {
            /* Run continuous tasks */
            (*(task_table_[i].func))();
        }
        else if ( sys_tick_ctr_ - task_table_[i].last_called_ >= task_table_[i].interval )
        {
            /* Run the tasks that are already due */
            (*(task_table_[i].func))();

            /* Update last_called_ */
            task_table_[i].last_called_ = sys_tick_ctr_;
        }
        else
        {
            /* do nothing */
            continue;
        }
        
    }
}
