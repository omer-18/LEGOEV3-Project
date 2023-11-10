/*
Omer
November 9th
Version 1.09
Added: Fixed obstacleDetected, Start of driveAround works
Still Needs: More Testing, and Vaulting Function
*/


const float WHEEL_RADIUS = 2.75;
const float CM_TO_ENC = (360.0 / (2 * PI * WHEEL_RADIUS));
const float ENC_TO_CM = ((2 * PI * WHEEL_RADIUS) / 360.0);



void configSensors();

void drive(int motor_power);

void rotateRobot(int motor_power, int angle);

void driveDist_ENC(int dist_cm, int motor_power);

bool obstacleDetected();

int driveAround();

int driveOver();


task main()
{
	configSensors();

	//bool program_end = false;
	//bool canDriveAround;
	// int colour_green = 3;

	bool green = false;
	bool detect = true;
	int middle_ENC = 0;
	int around = 0;


	while(green == false)//driving before an object or finish line is detected
	{
		detect = obstacleDetected();

		if (detect == false)
			green = true;

		else
		{
			rotateRobot(25, 90);
			middle_ENC = nMotorEncoder[motorB];

			around = driveAround();
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
		*/

	}
	drive(0);

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


}




void configSensors()
{


	SensorType[S2] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(50);


	SensorType[S1] = sensorEV3_IRSensor;
	wait1Msec(50);
	//SensorMode[S1] = modeEV3IR_Calibration;
	wait1Msec(50);
	SensorMode[S1] = modeEV3IR_Proximity;
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

	// Motor A: Front Right
	// Motor B:	Back Right
	// Motor C: Back Left
	// Motor D: Front Left
}

void drive(int motor_power)
{
	// NOTE: Motor A & C is negative to drive forward due to hardware / gear configuration

	motor[motorA] = -motor_power;
	motor[motorB] = motor_power;
	motor[motorC] = -motor_power;
	motor[motorD] = motor_power;
	return;
}

void rotateRobot(int motor_power, int angle)
{
	float current_angle = 0;
	current_angle = abs(getGyroDegrees(S4));

	if (angle > 0)	// Turn Counter clockwise
	{
		motor[motorA] = -motor_power;
		motor[motorB] = motor_power;
		motor[motorC] = motor_power;
		motor[motorD] = -motor_power;

	while (abs(getGyroDegrees(S4)) < abs(angle+ current_angle))
		{}
	}

	else if (angle < 0)		// Turn Clockwise
	{
		motor[motorB] = motor[motorC] = -motor_power;
		motor[motorA] = motor[motorD] = motor_power;

	while (abs(getGyroDegrees(S4)) > abs(angle+ current_angle))
		{}

	}

	drive(0);
}

void driveDist_ENC(int dist_cm, int motor_power)
{
	int initial_enc = nMotorEncoder[motorB];

	float dist_enc = dist_cm * CM_TO_ENC;
	drive(motor_power);

	if(motor_power > 0)
	{
	while(abs(nMotorEncoder[motorB]) < abs(dist_enc + initial_enc))
		{
			// eraseDisplay();
			// displayString(1, "%d", nMotorEncoder[motorB]);
			// displayString(2, "%f", abs(dist_enc + initial_enc));
		}
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
	drive(60);

	// while the change in height is less than 10cm, make this a constant number
	while (abs(SensorValue[S2] - initial_height) < HEIGHT_CHANGE && SensorValue[S3] != (int)colorGreen)
	{
		// Used to Display on EV3 Brick
		float curr_height = SensorValue[S2];
		displayString(1, "current Height: %f", curr_height);
		displayString(3, "inital Height: %f", initial_height);
		displayString(5, "change in Height %f", abs(SensorValue[S2] - initial_height));
	}

	drive(0);
	if (SensorValue[S3] != (int)colorGreen)
		return true;

	return false;
}

// Function is meant to drive in a certian way around a obstacle
int driveAround()
{
	bool red_line = false;		// Red line detector
	const int INF_DIST = 60;

	displayString(8, "Infra Value Inital: %d", getIRDistance(S1));

	//	displayString(9, "Infra Value ?: %i", getIRBeaconStrength(S1));
	//	displayString(10, "Infra Value ?: %i", SensorValue(S1));

	// Keeps driving if a line is not detected and there is an object along the side of the robot
	while (getIRDistance(S1) < INF_DIST && red_line == false)// based on a black object
	{
		displayString(11, "Infra Value After: %d", getIRDistance(S1));
		drive(30);

		if (SensorValue[S3] == (int)colorRed)
		{
			displayString(16, "RED");
			red_line = true;
			drive(0);
		}
	}

	// Drives a specified distance past the object and turns the robot to be paralel to object
	if (red_line == false)
	{
		displayString(15, "OBJECT DONE");
		drive(0);
		driveDist_ENC(20, 30);
		rotateRobot (25, -90);
		return 1; // supposed to go back to driving forward in main
	}


	eraseDisplay();
	displayString(15, "AFTER RED");

	red_line = false;
	displayString(4, "hi");
	driveDist_ENC(5, -10);

	displayString(11, "IR VAL B4", getIRDistance(S1));

	while (getIRDistance(S1) < 60 && red_line == false)// based on a black object
	{
		displayString(3, "INSIDE SECOND WHILE");
		drive(-40);
		displayString(11, "Infra Value After: %d", getIRDistance(S1));

		if (SensorValue[S3] == (int)colorRed)
			red_line = true;
	}
	drive(0);

	displayString(8, "Infra Value Inital: %d", getIRDistance(S1));

	if (red_line == false)
	{
		//drive a hardcoded amount
		displayString(12, "OBJECT DONE 2");
		drive(0);
		driveDist_ENC(30, -30);
		rotateRobot (25, -90);
		return 1; // supposed to go back to driving
	}

	return -10;// according to this value do obstacle vault
}
/*
void driveOver()
{}
*/
