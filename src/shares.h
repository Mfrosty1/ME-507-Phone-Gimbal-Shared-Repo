/** @file shares.h
 *  This file contains extern declarations of shares and queues which are used
 *  in more than one file of a demonstration project.
 * 
 *  @author JR Ridgely
 *  @date   2021-Oct-23 Original file
 *  @copyright (c) 2021 by JR Ridgely, released under the LGPL 3.0. 
 */

#ifndef _SHARES_H_
#define _SHARES_H_

#include "taskqueue.h"
#include "taskshare.h"

// A share which holds a counter of how many times a simulated event occurred
extern Share<bool> begin_recording;
extern Share<float> pitchAngle;
extern Share<float> rollAngle;
extern Share<float> yawAngle;
extern Share<int16_t> pMotSpeed;
extern Share<int16_t> rMotSpeed;
extern Share<int16_t> yMotSpeed;



// A queue which triggers a task to print the count at certain times
// extern Queue<uint16_t> data_queue;


#endif // _SHARES_H_
