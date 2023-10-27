//TRON GROUP PROJECT
//VERSION 1.0
//FIGURING OUT HOW TO STAY BETWEEN COLOR LANES AND ALL THAT GOOD STUFF


void configSensors()
{
	// Motor Configuration
	// Front Left: motorA
	// Front Right: motorB
	// Back Left: motorC
	// Back Right: motorD

	// Sensors Configuration
	SensorType[S1] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);

	// Calibrate Gyro Sensor
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

	// Calibrate Colour Sensor
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(50);

	// Reset Gyro at the begining
	resetGyro(S4);
	wait1Msec(50);

	// Reset nMotorEncoder
	nMotorEncoder[motorA] = 0;

	return;
}

void driveFront(int m_pow_f)
{
	motor[motorA] = motor[motorB] = m_pow_f;
	return;
}

void driveBack(int m_pow_b)
{
	motor[motorA] = motor[motorB] = m_pow_b;
	return;
}


int drive(motorPowerBack = 0, motorPowerFront = 0)
{
	//assuming motor C and Motor D are connected to the back two wheels
	motor[motorC] = motor[motorD] = motorPowerBack;
	//assuming motor C and Motor D are connected to the front two wheels
	motor[motorA] = motor[motorB] = motorPowerFront;

	while (SensorValue[S3] != (int)colorGreen)
	{
		if (SensorValue[S3] == (int)colorRed)
		{
			return -10;
		}
	}
	return 10;
}




task main()
{
	configSensors();
	
	bool flag_border = false;
	int flag_border_check = 0;

	while (!flag_border)
	{

		flag_border_check = drive(40, 40);

		if (


	}




}
