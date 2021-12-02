/** @file shares.h
 *      This file contains extern declarations of shares and queues which are used
 *      in more than one file.
 * 
 *  @author Matthew Frost
 *  @author Ryan McLaughlin
 *  @author John Ridgely (code taken from in-class example)
 *  @date   2021-Nov-09 Original file
 */

#ifndef _SHARES_H_
#define _SHARES_H_

#include "taskqueue.h"
#include "taskshare.h"

/// Shared variable for the pitch angle
extern Share<float> pitchAngle;
/// Shared variable for the roll angle
extern Share<float> rollAngle;
/// Shared variable for the yaw angle
extern Share<float> yawAngle;
/// Shared variable for the next pitch motor speed
extern Share<int16_t> pMotSpeed;
/// Shared variable for the next roll motor speed
extern Share<int16_t> rMotSpeed;
/// Shared variable for the next yaw motor speed
extern Share<int16_t> yMotSpeed;

#endif // _SHARES_H_