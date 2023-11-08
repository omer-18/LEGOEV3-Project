/*
Omer
November 6th
Version 1.06
Added Obstacle Detection Function, Driving around Obstacles, Integrated intoMain function
Still Needs: Testing and Vaulting Function
*/


const float WHEEL_RADIUS = 2.75;
const float CM_TO_ENC = (360.0 / (2 * PI * WHEEL_RADIUS));
const float ENC_TO_CM = ((2 * PI * WHEEL_RADIUS) / 360.0);



void configSensors();

void drive(int motor_power);

void rotateRobot(int motor_power, int angle);

void driveDist_ENC();

bool obstacleDetected();

int driveAround();

int driveOver();


task main()
{
	configSensors();

	//bool program_end = false;
	//bool canDriveAround;
	int colour_green = 3;

	bool green = false;
	bool detect;
	int middle_ENC = 0;
	int around = 0;


	while(!green)//driving before an object or finish line is detected
	{
		detect = obstacleDetected();

		if (!detect)
		{
			green = true;
		}
		else
		{
			rotateRobot(25, 90);
			middle_ENC = nMotorEncoder[motorB];

			around = driveAround();
		}

		if(!green && around == -10)
		{
			//move robot back to middle
			drive(30);
			while (abs(nMotorEncoder[motorB]) < (abs(nMotorEncoder[motorB]) + abs(middle_ENC)))
			{}
			drive(0);

			//make robot face forwards
			rotateRobot(25, -90);

			//run vault function, and store it into a variable
		}

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

	//SensorType[S1] = sensorEV3_Touch;// nah do gyro here
	SensorType[S2] = sensorEV3_Ultrasonic;
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;


	SensorType[S4] = sensorEV3_IRSensor;
	SensorMode[S4] = modeEV3IR_Proximity;
	// wait1Msec(50);


	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);


	// Motor A: Front Right
	// Motor B:	Back Right
	// Motor C: Back Left
	// Motor D: Front Left
}

void drive(int motor_power)
{
	// NOTE: Motor C is negative to drive forward due to hardware / gear configuration

	motor[motorA] = -motor_power;
	motor[motorB] = motor_power;
	motor[motorC] = -motor_power;
	motor[motorD] = motor_power;
	return;
}

void rotateRobot(int motor_power, int angle)
{

	if (angle > 0)	// Turn Counter clockwise
	{

		motor[motorA] = -motor_power;
		motor[motorB] = motor_power;
		motor[motorC] = motor_power;
		motor[motorD] = -motor_power;

		while (SensorValue[S4] > -90)
		{}
	}
	else
	{
		motor[motorC] = motor[motorD] = motor_power;
		motor[motorA] = motor[motorB] = -motor_power;
		while (SensorValue[S4] < 90)
		{}
	}
	drive(0);

	return;
}

void driveDist_ENC()
{

}

bool obstacleDetected()
{
	float initial_height = SensorValue[S2];
	drive(60);

	// while the change in height is less than 10cm, make this a constant number
	while (abs(SensorValue[S2]) - abs(initial_height) < 10 || SensorValue[S3] != (int)colorGreen)
	{}
	drive(0);
	if (SensorValue[S3] != (int)colorGreen)
	{
		return false;
	}

	return true;
}

int driveAround()
{
	//rotateRobot(25, 90);
	bool line = false;

	drive(30);
	while (SensorValue[S4] < 10 || !line)
	{
		
		if (SensorValue[S3] == (int)colorRed)
		{
			line = true;
		}
	}
	drive(0);
	if (!line)
	{
		rotateRobot (25, -90);
		return 1; // supposed to go back to driving
	}

	drive (-30);

	while (SensorValue[S4] < 10 || !line)
	{
		//drive(30);
		if (SensorValue[S3] == (int)colorRed)
		{
			line = true;
		}
	}
	drive(0);
	if (!line)
	{
		rotateRobot (25, -90);
		return 1; // supposed to go back to driving
	}

	//if it got here, means object has no end
	//drive back to middle
	return -10;// according to this value do obstacle vault
}

void driveOver()
{


}
