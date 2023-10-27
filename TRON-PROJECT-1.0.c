//TRON GROUP PROJECT
//VERSION 1.0
//FIGURING OUT HOW TO STAY BETWEEN COLOR LANES AND ALL THAT GOOD STUFF



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
	bool flag_border = false;
	int flag_border_check = 0;

	while (!flag_border)
	{

		flag_border_check = drive(40, 40);

		if (


	}




}
