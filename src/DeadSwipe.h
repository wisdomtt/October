/*
 * DeadSwipe.h
 *
 *  Created on: Jul 14, 2015
 *      Author: Robotics
 */

#ifndef DEADSWIPE_H_
#define DEADSWIPE_H_

#define PI 3.14159
#define ENCODER_PULSE_PER_REV 250
#define DRIVE_WHEEL_DIAMETER 6.0
#define DRIVE_WHEEL_CIRCUMFERENCE PI * DRIVE_WHEEL_DIAMETER
#define GEARBOX_RATIO 5.95
#define SPROCKET_RATIO 0.5
#define DRIVE_GEAR_RATIO GEARBOX_RATIO / SPROCKET_RATIO
#define DRIVE_DISTANCE_PER_PULSE DRIVE_WHEEL_CIRCUMFERENCE / (ENCODER_PULSE_PER_REV * DRIVE_GEAR_RATIO)
#define MIDDLE_DISTANCE_PER_PULSE DRIVE_WHEEL_CIRCUMFERENCE / (ENCODER_PULSE_PER_REV * DRIVE_GEAR_RATIO)
#define ELEVATOR_VEX_RATIO 5/1
#define ELEVATOR_WORM_RATIO 16/1
#define ELEVATOR_GEAR_RATIO ELEVATOR_VEX_RATIO / ELEVATOR_WORM_RATIO
#define ELEVATOR_DIAMETER 11/8
#define ELEVATOR_CIRCUMFERENCE PI * ELEVATOR_DIAMETER
#define ELEVATOR_DISTANCE_PER_PULSE ELEVATOR_CIRCUMFERENCE / (ENCODER_PULSE_PER_REV * ELEVATOR_GEAR_RATIO)

#define L_OPEN 0
#define R_OPEN 1
#define L_CLOSE 2
#define R_CLOSE 3
#define B_OPEN 4
#define B_CLOSE 5
#define up  true
#define down  false


#endif /* DEADSWIPE_H_ */
