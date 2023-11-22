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

	Front Large Motor Needs negative motor power to go down
	Back Large Motor needs negative motor power

	Sensors:
	Sesnor 1: Infared
	Sensor 2: Ultrasonic

	Sensor 3:
	C2 - Colour
	C1 - Gyro
	Sensor 4: Motor Multiplexor
	*/

	// Configure sensor ports

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

	SensorType[S4] = sensorI2CCustom;
	wait1Msec(50);
	MSMMUXinit();
	wait1Msec(50);

	MSMMotorEncoderReset(mmotor_S4_1);
	MSMMotorEncoderReset(mmotor_S4_2);
}

void drive(int motor_power)
{
	// Motor A & C is negative to drive forward due to hardware / gear configuration

    motor[motorA] = motor[motorC] = -motor_power;
    motor[motorB] = motor[motorD] = motor_power;
}

void moveFrontArmDown(int arm_enc)
{
	int current_enc = MSMMotorEncoder(mmotor_S4_2);
	while (abs(MSMMotorEncoder(mmotor_S4_2)) < abs(arm_enc + current_enc))
	{
		MSMMotor(mmotor_S4_2, -100);
		wait1Msec(1000);
		motor[motorB] = 10;
		motor[motorC] = -10;
	}
	MSMMotor(mmotor_S4_2, 0);
	drive(0);
}

void moveFrontArmUp(int arm_enc)
{
	int current_enc = MSMMotorEncoder(mmotor_S4_2);
	while (abs(MSMMotorEncoder(mmotor_S4_2)) > abs(arm_enc + current_enc))
	{
		MSMMotor(mmotor_S4_2, 50);
	}
	MSMMotor(mmotor_S4_2, 0);
}

void moveBackArmDown(int arm_enc)
{
	int current_enc = MSMMotorEncoder(mmotor_S4_1);
	while (abs(MSMMotorEncoder(mmotor_S4_1)) < abs(arm_enc + current_enc))
	{
		MSMMotor(mmotor_S4_1, -100);
		motor[motorA] = -10;
		motor[motorD] = 10;
	}
	MSMMotor(mmotor_S4_2, 0);

}

void moveBackArmUp(int arm_enc)
{
	int current_enc = MSMMotorEncoder(mmotor_S4_1);
	while (abs(MSMMotorEncoder(mmotor_S4_1)) > abs(arm_enc + current_enc))
	{
		MSMMotor(mmotor_S4_1, 50);
	}
	MSMMotor(mmotor_S4_1, 0);
}


void moveFrontArmOrg()
{
		MSMMotor(mmotor_S4_2, 30);
		while (abs(MSMMotorEncoder(mmotor_S4_2)) > 0)
		{}
		MSMMotor(mmotor_S4_2, 0);
}

task main()
{

	const int FRONT_ARM_ENC = 1600;
	const int BACK_ARM_ENC = 2500;

	configSensors();
	//MSMMotor(mmotor_S4_1, 30);
	//wait1Msec(2000);

	moveFrontArmDown(FRONT_ARM_ENC);
	wait1Msec(1000);

	moveBackArmDown(BACK_ARM_ENC);
	wait1Msec(1000);

	moveFrontArmUp(FRONT_ARM_ENC);
	moveBackArmUp(BACK_ARM_ENC);

	//moveFrontArmOrg();

	// stop both motors
	MSMotorStop(mmotor_S4_1);
	MSMMotor(mmotor_S4_1, 0);
	MSMotorStop(mmotor_S4_2);
	MSMMotor(mmotor_S4_2, 0);
}

/*
To move the front arm up onto the object then we need have these values:
motorEncoder = 1600
wait 1 second
drive at 10 speed

void moveFrontArmDown()
{
	const int FRONT_ARM_ENC = 1600;

	int current_enc = MSMMotorEncoder(mmotor_S4_2);

	while (abs(MSMMotorEncoder(mmotor_S4_2)) < abs(FRONT_ARM_ENC + current_enc))
	{
		MSMMotor(mmotor_S4_2, -100);
		wait1Msec(1000);

		motor[motorB] = 10;
		motor[motorC] = -10;

		displayString(1, "ENC %d",abs(MSMMotorEncoder(mmotor_S4_2)));
		displayString(2, "Target %d",abs(FRONT_ARM_ENC + current_enc));
	}
	MSMMotor(mmotor_S4_2, 0);



}

*/
