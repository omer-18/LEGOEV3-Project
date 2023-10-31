
void configSensors()
{

	//SensorType[S1] = sensorEV3_Touch;
	//SensorType[S2] = sensorEV3_Ultrasonic;
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;
	// wait1Msec(50);

	/*
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
	*/
}

void rotate(int clock_or_counter_clockwise, angle)
void drive(int motor_power)
{
	motor[motorA] = motor[motorD] = motor_power;
	return;
}


void driveAround()
{
 rotate(1, 90)
 drive(10)
 while {SensorValue[   
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
		drive(10);

		while(SensorValue[S3] != colour_green || obstacleDetected)//driving before an object or finish line is detected
		{}
		drive(0);

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
