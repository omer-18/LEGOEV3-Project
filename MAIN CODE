


void configSensors()
{

	//SensorType[S1] = sensorEV3_Touch;
	//SensorType[S2] = sensorEV3_Ultrasonic;
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;
	// wait1Msec(50);


	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

}


void drive(int motor_power_l,int motor_power_r)
{
	motor[motorB] = motor_power_l;//changr after
	motor[motorC] = motor[motorD] = motor[motorA] = -motor_power_r;
	return;
}

void rotateRobot(int motor_power, int angle)
{
	if (angle > 0)	// Turn Counter clockwise
	{
		motor[motorA] = 25;
		motor[motorB] = -25;
		motor[motorC] = -25;
		motor[motorD] = -25;
		
		
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
	drive(0,0);

	return;
}
void driveAround()
{

}

void driveOver()
{

}

task main()
{
	configSensors();

	bool program_end = false;
	bool canDriveAround;
	bool obstacleDetected = false;
	int colour_green = 3;

	while(!program_end)//loop to reiterate for EACH object it'll climn
	{
		drive(10,10);

		while(SensorValue[S3] != colour_green)// || obstacleDetected)//driving before an object or finish line is detected
		{}
		drive(0,0);

		if(SensorValue[S3] == colour_green)//IF it was finish line that stops the drive, run endcode
			program_end = true;

		else if (obstacleDetected) //else IF it was an obstacle that stops the drive, run obstacle avoidance code
		{
	   if(canDriveAround)//function(not created) to check if the clearance is enough to drive around
	   	driveAround();

	   else
	    driveOver();
		}
	}

}
