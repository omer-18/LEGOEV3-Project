#include "mindsensors-motormux.h";
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

	displayString(1,"HERE");
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

}
void drive(int motor_power)
{
	// Motor A & C is negative to drive forward due to hardware / gear configuration

    motor[motorA] = motor[motorC] = -motor_power;
    motor[motorB] = motor[motorD] = motor_power;
}

void rotateRobot(int motor_power)
// Function rotates the robot a specified angle based the current position
{
	int currentEncoder = nMotorEncoder[motorA];
	const int TURNENC = 1250;
	if (motor_power > 0)	// Turn Counter clockwise
	{
				eraseDisplay();
        motor[motorA] = motor[motorD] = -motor_power;
        motor[motorB] = motor[motorC] = motor_power;

        while (nMotorEncoder[motorA] > (currentEncoder - TURNENC))
        {
        displayString(1, "%d", abs(nMotorEncoder[motorA]));
        displayString(2, "%d", abs(currentEncoder - TURNENC));
        }
	}

	else		// Turn Clockwise
	{
		motor[motorA] = motor[motorD] = -motor_power;
		motor[motorB] = motor[motorC] = motor_power;

	   while ((nMotorEncoder[motorA]) < (currentEncoder + TURNENC))
		{
			  displayString(1, "%d", abs(nMotorEncoder[motorA]));
        displayString(2, "%d", abs(currentEncoder - TURNENC));
		}
	}

	drive(0);

}

task main()
{

	SensorType[S4] = sensorI2CCustom;
	wait1Msec(50);
	MSMMUXinit();
	wait1Msec(50);

	displayString(4,"START");
	configSensors();
	displayString(2,"END");
	rotateRobot(25);

	MSMMotor(mmotor_S4_1, 30);
	wait1Msec(5000);

	// stop both motors – this does not happen automatically

	MSMotorStop(mmotor_S4_1);
	// MSMotorStop(mmotor_S4_2);
}
