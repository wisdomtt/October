#include "WPILib.h"
#include "DeadSwipe.h"
#include "F310.h"
#include <iostream>
#include "Functions.h"

using namespace std;
class Robot : public SampleRobot
{
	// boolean values for elevator limit switches
	bool TopFinish;
	bool BotFinish;
	bool ElevatorControl;
	int auton;
	// class wide declarations

	// drive train declarations - anything prefixed with db is a part of the drive train

	// motor declarations - motors are controlled by motor controllers. We use Talons
	Talon *dbFrontLeft; // pwm 6
	Talon *dbRearLeft; // pwm 4
	Talon *dbFrontRight; // pwm 7
	Talon *dbRearRight; // pwm 9
	RobotDrive *dbDrive; // easy tools ftw
	Talon *dbMiddlElevatorTalonLeft; // pwm 5
	Talon *dbMiddlElevatorTalonRight; // pwm 8

	// encoder declarations - encoders require 2 digital I/O pins and are used to measured how far a rotating object
	// has moved
	Encoder *dbLeftEncoder; // DIO 0 and 1
	Encoder *dbRightEncoder; // DIO 2 and 3
	Encoder *dbMidEncoder; // DIO 4 and 5

	Timer *clock;
	// gyro declaration - gyros allow us to measure how much an object has rotated along an axis
	// NOTE: gyros can(as of this writing) ONLY be used on AIN 0 and 1
	Gyro *dbGyro; // AIN 0

	SendableChooser *autoChooser; // this is how we'll select different
	// Elevator declarations

	Talon *ElevatorTalonLeft; // pwm 0
	Talon *ElevatorTalonRight; // pwm 1

	DoubleSolenoid *epLeft; // PCM ports 0 and 1
	DoubleSolenoid *epRight; // PCM ports 2 and 3

	// intake declarations

	Talon *iLeft; // pwm 1
	Talon *iRight; // pwm 0

	DoubleSolenoid *ipLeft; // PCM ports 4 and 5
	DoubleSolenoid *ipRight; // PCM ports 6 and 7

	// operator interface declarations - prefixed by oi
	Joystick *oiLeft;
	Joystick *oiRight;
	F310 *oiGamepad;

	Relay* ElevatorRight;
	Relay* ElevatorLeft;

public:


	float SafetyUpElevator;
	float SafetyDownElevator;
	float Distance;
	float rElevatorDistance;
	float lElevatorDistance;

	Robot()
	{
		clock = new Timer();

		autoChooser = new SendableChooser();
		Distance = 0;
		auton = 0;
		SafetyUpElevator = 1.0;
		SafetyDownElevator = 1.0;
		ElevatorControl = false;
		rElevatorDistance = 0;
		lElevatorDistance = 0;

		TopFinish = false;
		BotFinish = false;

		oiLeft = new Joystick(0);
		oiRight = new Joystick(1);
		oiGamepad = new F310(3);

		dbFrontLeft = new Talon(6);
		dbRearLeft = new Talon(4);
		dbFrontRight = new Talon(7);
		dbRearRight = new Talon(9);

		dbDrive = new RobotDrive(dbFrontLeft, dbRearLeft, dbFrontRight, dbRearRight);
		dbDrive->SetSafetyEnabled(true);
		dbDrive->SetExpiration(0.1);
		dbDrive->SetSensitivity(0.5);
		dbDrive->SetMaxOutput(1.0);

		dbMiddlElevatorTalonLeft = new Talon(5);
		dbMiddlElevatorTalonRight = new Talon(8);

		dbLeftEncoder = new Encoder(0, 1, false, Encoder::k4X);
		dbLeftEncoder->SetDistancePerPulse(DRIVE_DISTANCE_PER_PULSE);

		dbRightEncoder = new Encoder(2, 3, false, Encoder::k4X);
		dbRightEncoder->SetDistancePerPulse(DRIVE_DISTANCE_PER_PULSE);

		dbMidEncoder = new Encoder(4, 5, false, Encoder::k4X);
		dbMidEncoder->SetDistancePerPulse(MIDDLE_DISTANCE_PER_PULSE);

		dbGyro = new Gyro(0);
		dbGyro->SetSensitivity(0.007);



		ElevatorTalonLeft = new Talon(2);
		ElevatorTalonRight = new Talon(3);


		iLeft = new Talon(1);
		iRight= new Talon(0);

		ElevatorRight = new Encoder(6, 7, false, Encoder::k4X);
		ElevatorLeft = new Encoder(8, 9, false, Encoder::k4X);

		//Pneumatics i:intake e:elevator
		ipLeft = new DoubleSolenoid(0, 1, 6);
		ipRight = new DoubleSolenoid(0, 0, 7);
		epLeft = new DoubleSolenoid(0, 2, 5);
		epRight = new DoubleSolenoid(0, 3, 4);

}
	void Autonomous(void)
	{
			int aSelectedMode = (int) autoChooser->GetSelected();
					while(IsEnabled() && IsAutonomous())
					{


					}

	}

	void OperatorControl(void)
	{
		while (IsOperatorControl() && IsEnabled())
			{
			if(oiGamepad->GetButton(F310::kAButton)){
				iLeft->Set(-1.0);
				iRight->Set(-1.0);
			}
			if(oiGamepad->GetButton(F310::kYButton)){
				iLeft->Set(1.0);
				iRight->Set(1.0);
			}
			if(oiGamepad->GetButton(F310::kLeftTrigger)){
				iLeft->Set(0.0);
				iRight->Set(0.0);
			}
			//////////////////////////////////////////////////
			if(oiGamepad->GetButton(F310::kRightTrigger)){
				ipLeft->Set(DoubleSolenoid::kForward);
				ipRight->Set(DoubleSolenoid::kForward);
			}
			if(oiGamepad->GetButton(F310::kRightBumper)){
				ipLeft->Set(DoubleSolenoid::kReverse);
				ipRight->Set(DoubleSolenoid::kReverse);
			}
			//////////////////////////////////////////////////
			if(oiGamepad->GetButton(F310::kXButton)){
				epLeft->Set(DoubleSolenoid::kForward);
				epRight->Set(DoubleSolenoid::kForward);
			}

			if(oiGamepad->GetButton(F310::kBButton)){
				epLeft->Set(DoubleSolenoid::kReverse);
				epRight->Set(DoubleSolenoid::kReverse);
			}

			ElevatorTalonLeft->Set(oiGamepad->GetY(F310::kLeftStick));
			ElevatorTalonRight->Set(oiGamepad->GetY(F310::kRightStick));


			}
	}

	// h drive function
		void HDrive(float speed)
		{
			dbMiddlElevatorTalonLeft->Set(speed);
			dbMiddlElevatorTalonRight->Set(speed);
		}

		// elevator pneumatic function
		void ePneumaticControl(int x)
		{
			switch(x)
			{
			case L_OPEN:
				this->epLeft->Set(DoubleSolenoid::kReverse);
				break;
			case L_CLOSE:
				this->epLeft->Set(DoubleSolenoid::kForward);
				break;
			case R_OPEN:
				this->epRight->Set(DoubleSolenoid::kReverse);
				break;
			case R_CLOSE:
				this->epRight->Set(DoubleSolenoid::kForward);
				break;
			case B_OPEN:
				this->epLeft->Set(DoubleSolenoid::kReverse);
				this->epRight->Set(DoubleSolenoid::kReverse);
				break;
			case B_CLOSE:
				this->epLeft->Set(DoubleSolenoid::kForward);
				this->epRight->Set(DoubleSolenoid::kForward);
				break;
			default:
				break;
			}
		}

		// intake pnematic function
		void iPneumaticControl(int x)
			{
				switch(x)
				{
				case L_OPEN:
					this->ipLeft->Set(DoubleSolenoid::kReverse);
					break;
				case L_CLOSE:
					this->ipLeft->Set(DoubleSolenoid::kForward);
					break;
				case R_OPEN:
					this->ipRight->Set(DoubleSolenoid::kReverse);
					break;
				case R_CLOSE:
					this->ipRight->Set(DoubleSolenoid::kForward);
					break;
				case B_OPEN:
					this->ipLeft->Set(DoubleSolenoid::kReverse);
					this->ipRight->Set(DoubleSolenoid::kReverse);
					break;
				case B_CLOSE:
					this->ipLeft->Set(DoubleSolenoid::kForward);
					this->ipRight->Set(DoubleSolenoid::kForward);
					break;
				default:
					break;
				}
			}


		void Forward(RobotDrive *drive, float forward, Encoder *A, Encoder *B)
		{
			while(forward < ((A->GetDistance() + B->GetDistance())/2))
			{
				drive->TankDrive(1.0, 1.0);
			}
			drive->TankDrive(0.0,0.0);
			A->Reset();
			B->Reset();
		}
		void GYROTurn(Gyro *gyro, float turn, RobotDrive *drive)
		{
			while(-5 < turn && turn < 5)
			{
				if(turn > 0)
				{
					drive->TankDrive(0.7, 0.0);
				}
				else if(turn < 0)
				{
					drive->TankDrive(0.0,0.7);
				}
			}
			drive->TankDrive(0.0, 0.0);
		}



	void Disabled(void)
	{

	}

	void Test(void)
	{

	}
};
START_ROBOT_CLASS(Robot);
