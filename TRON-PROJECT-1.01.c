//TRON GROUP PROJECT
//VERSION 1.01
//STAYING INSIDE RED LANES AND STOPS BEFORE GREEN FINISH LINE
// ASSUMPTIONS: Using 4 Motors, 1 Color Sensor
// Commit By Omer



int drive(int motorPowerBack = 0, int motorPowerFront = 0)
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


void rotateRobot (float angle)
{
	float current_angle = 0;

	current_angle = abs(getGyroDegrees(S4));

	if (angle > 0)
	{
		motor[motorC] = 20;
		motor[motorD] = -20;
	}
	else
	{
		motor[motorC] = 20;
		motor[motorD] = -20;
	}

	while (abs(getGyroDegrees(S4))<(abs(angle + current_angle)))
	{
	}

	drive(0, 0);


}

void driveBack(int current_enc)
{

drive(-30, -30);

while(nMotorEncoder[motorA] > current_enc)
{}

}


task main()
{
	bool flag_border = false;
	int flag_border_check = 0;
	int middle_enc = 0;
	middle_enc = nMotorEncoder[motorA];


	while (!flag_border)
	{
			while(!flag_border && flag_border_check >= 0)
			{
				flag_border_check = drive(40,40);


				if (flag_border_check == -10)
				{
					flag_border = true;
				}

			}
	}

	driveBack(middle_enc);
	rotateRobot(90);




}
