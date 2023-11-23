#include "mindsensors-motormux.h";

const float WHEEL_RADIUS = 2.75;
const float CM_TO_ENC = (360.0 / (2 * PI * WHEEL_RADIUS));

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

void moveFrontArmDown(float arm_enc, int motor_power)
{
	float current_enc = MSMMotorEncoder(mmotor_S4_2);
	while (abs(MSMMotorEncoder(mmotor_S4_2)) < abs(arm_enc + current_enc))
	{
		MSMMotor(mmotor_S4_2, -100);
		wait1Msec(1000);
		motor[motorB] = motor_power;
		motor[motorC] = -motor_power;
	}
	MSMMotor(mmotor_S4_2, 0);
	drive(0);
}

void moveFrontArmUp(float arm_enc)
{
	int current_enc = MSMMotorEncoder(mmotor_S4_2);
	while (abs(MSMMotorEncoder(mmotor_S4_2)) > abs(arm_enc + current_enc))
	{
		MSMMotor(mmotor_S4_2, 50);
	}
	MSMMotor(mmotor_S4_2, 0);
}

void moveBackArmDown(float arm_enc)
{
	int current_enc = MSMMotorEncoder(mmotor_S4_1);
	while (abs(MSMMotorEncoder(mmotor_S4_1)) < abs(arm_enc + current_enc))
	{
		MSMMotor(mmotor_S4_1, -100);
		motor[motorA] = -30;
		motor[motorD] = 30;
		displayString(1, "ENC %d",abs(MSMMotorEncoder(mmotor_S4_1)));
	}
	MSMMotor(mmotor_S4_1, 0);
	drive(0);

}

void moveBackArmUp(float arm_enc)
{
	float current_enc = MSMMotorEncoder(mmotor_S4_1);
	while (abs(MSMMotorEncoder(mmotor_S4_1)) > abs(arm_enc + current_enc))
	{
		MSMMotor(mmotor_S4_1, 50);
	}
	MSMMotor(mmotor_S4_1, 0);
}


void moveFrontArmOrg(int ENC_LIM)
{
		MSMMotor(mmotor_S4_2, 30);
		while (abs(MSMMotorEncoder(mmotor_S4_2)) > ENC_LIM)
		{}
		MSMMotor(mmotor_S4_2, 0);
}
void driveEnd()
{
	const float HEIGHT_CHANGE = 3.0;
	drive(30);
	float initial_height = SensorValue[S2];
	while (abs(SensorValue[S2] - initial_height) < HEIGHT_CHANGE)
	{}
	drive(0);
}

void moveBackArmOrg()
{
		MSMMotor(mmotor_S4_1, 30);
		//drive(30);
		bool end = false;

		while (abs(MSMMotorEncoder(mmotor_S4_1)) > 80)
		{
			if (end == false)
			{
				driveEnd();
				end = true;
			}
		}
		MSMMotor(mmotor_S4_1, 0);
		//drive(0);
}

void driveDist_CM(int dist_cm, int motor_power)
// Function Detects obstacles infront of robot
{
	float initial_enc = nMotorEncoder[motorB];

	float dist_enc = dist_cm * CM_TO_ENC;
	drive(motor_power);

	if(motor_power > 0)
	{
		while(abs(nMotorEncoder[motorB]) < abs(dist_enc + initial_enc))
		{
			displayString(10, "FRONT ENC B4 %d", abs(MSMMotorEncoder(mmotor_S4_2)));
		}
	}

	else
	{
		while(abs(nMotorEncoder[motorB]) > abs(initial_enc - dist_enc))
		{}
	}
	drive(0);

}

task main()
{
	const float FRONT_ARM_ENC = 1600;
	const float BACK_ARM_ENC =  1500;

	configSensors();


  moveFrontArmDown(FRONT_ARM_ENC, 50);
	//wait1Msec(3000);

	moveFrontArmOrg(60);
	//wait1Msec(3000);

	moveBackArmDown(BACK_ARM_ENC);
	//wait1Msec(3000);`

	moveBackArmOrg();
	//wait1Msec(3000);

	driveDist_CM(18, 30);

	moveFrontArmDown(1375, 0);

	driveDist_CM(75, 40);
	//displayString(8, "FRONT ENC %d", abs(MSMMotorEncoder(mmotor_S4_2)));
	moveFrontArmOrg(20);

	driveDist_CM(50,25);

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
