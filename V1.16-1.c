/*
Name: Jayden Mangaroo
Date: November 10th
Version: 1.18

Changed:
WORKING DRIVE AROUND FUNCTION

Additions to be made:
VAULT FUNCTION
*/

// Global Variables

//BIG WHEELS: 4.2, NORMAL WHEELS: 2.75
const float WHEEL_RADIUS = 2.75;
const float CM_TO_ENC = (360.0 / (2 * PI * WHEEL_RADIUS));
// const float ENC_TO_CM = ((2 * PI * WHEEL_RADIUS) / 360.0);

#include "mindsensors-motormux.h"

// Function Prototyping
void configSensors();
void drive(int motor_power);
void rotateRobot(int motor_power);
void driveDist_CM(int dist_cm, int motor_power);
void driveOver();
void ButtonPressed();
bool GreenCheck();
bool RedCheck();
bool obstacleDetected();
bool driveAround(int motor_power, bool drive_direction_left_right);
void driveMiddle(int motor_power, float middle_ENC);

// Main Function
task main()
{

	// initialize, for the multiplexer connected to S1
  // Declaration of Variables
	bool green_line;
	bool red_line;
	bool obstacle_detected;
	float middle_of_obstacle_ENC = 0;

	configSensors();
	//ButtonPressed();
	//time1[T1] = 0;

  // Our Robot is going to continue driving forward until green line or obstacle is detected
	while(!green_line)
	{
    drive(50);


    green_line = GreenCheck();
		obstacle_detected = obstacleDetected();

        if (obstacle_detected)
        {
            rotateRobot(50);
            middle_of_obstacle_ENC = nMotorEncoder[motorB];

            red_line = driveAround(50, true);

            if (red_line)
            {

                red_line = driveAround(50, false);

                if (red_line)
                {
                    driveMiddle(30, middle_of_obstacle_ENC);
                }
            }
        }
	}


	// stop both motors  this does not happen automatically
	MSMotorStop(mmotor_S4_1);
	MSMotorStop(mmotor_S4_2);

}

void configSensors()
{
    /*
	Robot Configuration:

    Motors:
        Motor A: Front Right
        Motor B: Back Right - Used for Motor Encoders
        Motor C: Back Left
        Motor D: Front Left

    Sensors:
        Sesnor 1: Infared
        Sensor 2: Ultrasonic

        Sensor 3:
        C2 - Colour
        C1 - Gyro
       Sensor 4: Motor Multiplexor
    */
  // Configure sensor port

	// configure each channel on the sensor mux


	//displayString(1,"HERE");
	SensorType[S3] = sensorEV3_IRSensor;
	wait1Msec(50);
	SensorMode[S3] = modeEV3IR_Proximity;
	wait1Msec(50);

	SensorType[S2] = sensorEV3_Ultrasonic;
	wait1Msec(50);

	SensorType[S1] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Color_Color;
	wait1Msec(50);

	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorD] = 0;

	SensorType[S4] = sensorI2CCustom;
	wait1Msec(50);
	MSMMUXinit();
	wait1Msec(50);

}

void ButtonPressed()
{
	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{}
	return;
}


bool GreenCheck()
// Function checks if green is detected
{
    if (SensorValue[S1] == (int)colorGreen)
    {
        drive(0);
        return true;
    }
    return false;
}

bool RedCheck()
// Function checks if green is detected
{
    if (SensorValue[S1] == (int)colorRed)
        return true;
    return false;
}

void drive(int motor_power)
{
	// Motor A & C is negative to drive forward due to hardware / gear configuration

    motor[motorA] = motor[motorC] = -motor_power;
    motor[motorB] = motor[motorD] = motor_power;

	return;
}

void rotateRobot(int motor_power)
// Function rotates the robot a specified angle based the current position
{
	int currentEncoder = nMotorEncoder[motorA];
	int currentEncoder2 = nMotorEncoder[motorD];
	const int TURNENC = 1125;
	const int TURNENC2 = 1000;
	if (motor_power > 0)	// Turn Counter clockwise
	{
				eraseDisplay();
        motor[motorA] = motor[motorD] = -motor_power;
        motor[motorB] = motor[motorC] = motor_power;

        while (nMotorEncoder[motorA] > (currentEncoder - TURNENC))
        {
       // displayString(1, "%d", abs(nMotorEncoder[motorA]));
       // displayString(2, "%d", abs(currentEncoder - TURNENC));
        }
	}

	else		// Turn Clockwise
	{
		motor[motorA] = motor[motorD] = -motor_power;
		motor[motorB] = motor[motorC] = motor_power;

	   while ((nMotorEncoder[motorD]) < (currentEncoder2 + TURNENC2))
		{
			  //displayString(1, "%d", abs(nMotorEncoder[motorA]));
        //displayString(2, "%d", abs(currentEncoder - TURNENC));
		}
	}

	drive(0);

}

void driveDist_CM(int dist_cm, int motor_power)
// Function Detects obstacles infront of robot
{
	int initial_enc = nMotorEncoder[motorB];

	float dist_enc = dist_cm * CM_TO_ENC;
	drive(motor_power);

	if(motor_power > 0)
	{
		while(abs(nMotorEncoder[motorB]) < abs(dist_enc + initial_enc))
		{}
	}

	else
	{
		while(abs(nMotorEncoder[motorB]) > abs(initial_enc - dist_enc))
		{}
	}
	drive(0);

}

bool obstacleDetected()
{
	float initial_height = SensorValue[S2];
	const float HEIGHT_CHANGE = 8.0;

	// while the change in height is less than 10cm, make this a constant number
	while (abs(SensorValue[S2] - initial_height) < HEIGHT_CHANGE && GreenCheck() == false)
	{}

	drive(0);

	if (GreenCheck() == false)
		return true;

	return false;
}

bool driveAround(int motor_power, bool drive_direction_left_right)

// Function is drives around an obstacle
{
	const int INF_DIST = 60;    // Some number which the inafred will cause a obstacle detection, based on a black object
  int extra_drive_dist = 0;
  bool pos_motor_power;

    if (drive_direction_left_right)
	{
  		drive(motor_power);
      extra_drive_dist = 20;
      pos_motor_power = true;
	}
    else
	{
        drive(-motor_power);
        extra_drive_dist = 30;
        pos_motor_power = false;

	}
    // Keeps driving if a red line is not detected and there is an object along the side of the robot
	while (getIRDistance(S3) < INF_DIST && RedCheck() == false)
	{}

    if (RedCheck() == false)
    {
    		if (pos_motor_power)
    			driveDist_CM(extra_drive_dist, motor_power);

      	else
      		driveDist_CM(extra_drive_dist, -motor_power);

      		rotateRobot (-50);
        return false;
    }

    else
    {
    		driveDist_CM(8,-motor_power);
        return true;
    }
}

void driveMiddle(int motor_power, float middle_ENC)
{
	drive(0);
	drive(motor_power);

	while(abs(nMotorEncoder[motorB]) < abs(middle_ENC))
	{}

	drive(0);

	rotateRobot(-50);

	driveDist_CM(10, -30);

}
