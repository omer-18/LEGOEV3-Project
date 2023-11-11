/*
Name: Jayden Mangaroo
Date: November 10th
Version: 1.12

Changed:
- Added comments and made come more readable, made Code more efficient
- Moved the calling of the drive function to the main code from obstacle Detected function
- In main changed the initial value of detect as it was initalized as true (which logically doesn't make sense)
    so I changed that then reversed the return statements of true and false in the function obstacle Detected
- Completely revamped the Drive around function in order to optimize it
- Changed the logic of the main code
- Added functions to check for Green and Red

Additions to be made:
- Drive to middle after red is detected on both sides
- Vaulting up capabilities
- Vaulting down capabilities
*/

// Global Variables
const float WHEEL_RADIUS = 2.75;
const float CM_TO_ENC = (360.0 / (2 * PI * WHEEL_RADIUS));
// const float ENC_TO_CM = ((2 * PI * WHEEL_RADIUS) / 360.0);

// Function Prototyping
void configSensors();
void drive(int motor_power);
void rotateRobot(int motor_power, int angle);
void driveDist_CM(int dist_cm, int motor_power);
void driveOver();
bool GreenCheck();
bool RedCheck();
bool obstacleDetected();
bool driveAround(int motor_power, bool drive_direction_left_right);
void driveMiddle(int motor_power, float middle_ENC);

// Main Function
task main()
{
  // Declaration of Variables
	bool green_line;
	bool red_line;
	bool obstacle_detected;
	float middle_of_obstacle_ENC = 0;

	configSensors();

  // Our Robot is going to continue driving forward until green line or obstacle is detected
	while(!green_line)
	{
        drive(50);

        green_line = GreenCheck();
				obstacle_detected = obstacleDetected();

        if (obstacle_detected)
        {
            rotateRobot(25,90);
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
}
		// COMMENT THIS OUT FOR NOW
		/*
		if(green == false && around == -10)
		{
			//move robot back to middle
			int initial_enc = abs(nMotorEncoder[motorB]);
			drive(30);
			while (abs(nMotorEncoder[motorB]) < (abs(initial_enc + middle_ENC)))
			{}
			drive(0);

			//make robot face forwards
			rotateRobot(25, -90);

			//run vault function, and store it into a variable
		}

	}
	*/

	/* drive(0);

	rotateRobot(10, 90);

	if(SensorValue[S3] == colour_green)//if it was finish line that stops the drive, run endcode
	program_end = true;

	else if (obstacleDetected) //else IF it was an obstacle that stops the drive, run obstacle avoidance code
	{
	if(canDriveAround)//function(not created) to check if the clearance is enough to drive around
	driveAround();

	else
	driveOver();
	}
	*/

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
        Sensor 3: Colour
        Sensor 4: Gyro
    */

    SensorType[S1] = sensorEV3_IRSensor;
	wait1Msec(50);
	//SensorMode[S1] = modeEV3IR_Calibration;
	wait1Msec(50);
	SensorMode[S1] = modeEV3IR_Proximity;
	wait1Msec(50);

	SensorType[S2] = sensorEV3_Ultrasonic;
	wait1Msec(50);

	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(50);

	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
	resetGyro(S4);
	wait1Msec(50);

	nMotorEncoder[motorB] = 0;
}

bool GreenCheck()
// Function checks if green is detected
{
    if (SensorValue[S3] == (int)colorGreen)
    {
        drive(0);
        return true;
    }
    return false;
}

bool RedCheck()
// Function checks if green is detected
{
    if (SensorValue[S3] == (int)colorRed)
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

void rotateRobot(int motor_power, int angle)
// Function rotates the robot a specified angle based the current position
{
	float current_angle = 0;
	current_angle = abs(getGyroDegrees(S4));

	if (angle > 0)	// Turn Counter clockwise
	{
        motor[motorA] = motor[motorD] = -motor_power;
        motor[motorB] = motor[motorC] = motor_power;

        while (abs(getGyroDegrees(S4)) < abs(angle + current_angle))
        {}
	}

	else if (angle < 0)		// Turn Clockwise
	{
		motor[motorA] = motor[motorD] = motor_power;
		motor[motorB] = motor[motorC] = -motor_power;

	    while (abs(getGyroDegrees(S4)) > abs(angle + current_angle))
		{}
	}

	drive(0);
    return;
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
	const float HEIGHT_CHANGE = 4.0;

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
	while (getIRDistance(S1) < INF_DIST && RedCheck() == false)
	{}

    if (RedCheck() == false)
    {
    		if (pos_motor_power)
    			driveDist_CM(extra_drive_dist, motor_power);

      	else
      		driveDist_CM(extra_drive_dist, -motor_power);

      		rotateRobot (25, -90);
        return false;
    }

    else
    {
    	drive(0);
    		driveDist_CM(8,-motor_power);
        return true;
    }
}

void driveMiddle(int motor_power, float middle_ENC)
{
	drive(0);
	float initial_enc = nMotorEncoder[motorB];
	drive(motor_power);

	while(abs(nMotorEncoder[motorB]) < abs(middle_ENC)
	{
		displayString(1, "Current MotorEncoder: %d", abs(nMotorEncoder[motorB]));
		displayString(2, "middle_ENC: + inital %f", abs(middle_ENC);


	}

	drive(0);

	rotateRobot(25, -90);

	driveDist_CM(10, -30);

}
