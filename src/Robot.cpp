#include "WPILib.h"
#include "DeadSwipe.h"
#include "F310.h"
#include <iostream>
#include "Functions.h"

using namespace std;
class Robot : public SampleRobot
{
	float SafetyUpElevator;
	float SafetyDownElevator;
	float Distance;
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

	Talon *iLeft; // pwm 2
	Talon *iRight; // pwm 3

	DoubleSolenoid *ipLeft; // PCM ports 4 and 5
	DoubleSolenoid *ipRight; // PCM ports 6 and 7

	// operator interface declarations - prefixed by oi
	Joystick *oiLeft;
	Joystick *oiRight;
	F310 *oiGamepad;

	Encoder *ElevatorRight;
	Encoder *ElevatorLeft;

public:

	Robot()
	{
		autoChooser = new SendableChooser();
		Distance = 0;
		auton = 0;
		SafetyUpElevator = 1.0;
		SafetyDownElevator = 1.0;
		ElevatorControl = false;

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

		epLeft = new DoubleSolenoid(0, 0, 1);
		epRight = new DoubleSolenoid(0, 2, 3);

		ElevatorTalonLeft = new Talon(0);
		ElevatorTalonRight = new Talon(1);

		ipLeft = new DoubleSolenoid(0, 4, 5);
		ipRight = new DoubleSolenoid(0, 6, 7);

		iLeft = new Talon(2);
		iRight= new Talon(3);

		ElevatorRight = new Encoder(6, 7, false, Encoder::k4X);
		ElevatorLeft = new Encoder(8, 9, false, Encoder::k4X);
}
	void Autonomous(void)
	{
			int aSelectedMode = (int) autoChooser->GetSelected();
					while(IsEnabled() && IsAutonomous())
					{
						this->Debug();
						switch (aSelectedMode)
						{
							case 1:
							{
								ElevatorMove(ElevatorLeft, ElevatorRight,
										ElevatorTalonLeft, ElevatorTalonRight, down);
								break;
							}
							case 2:
								break;
							default:
							{
								ElevatorMove(ElevatorLeft, ElevatorRight,
									ElevatorTalonLeft, ElevatorTalonRight, down);
								break;
							}
						}
						this->Debug();

					} // end of while loop

	}

	void OperatorControl(void)
	{
		while (IsOperatorControl() && IsEnabled())
			{
			//Press Y, Elevator goes up if you didn't press A
			//Press A, Elevator goes down if you didn't press Y
			//Press Both and Elevator stops

				if(oiGamepad->GetButton(F310::kYButton) &&
						!(oiGamepad->GetButton(F310::kAButton)))
				{
					ElevatorTalonLeft->Set(1.0 * SafetyUpElevator);
					ElevatorTalonRight->Set(1.0*SafetyUpElevator);
				}
				else if(oiGamepad->GetButton(F310::kAButton) &&
						!(oiGamepad->GetButton(F310::kYButton)))
				{
					ElevatorTalonLeft->Set(-1.0*SafetyDownElevator);
					ElevatorTalonRight->Set(-1.0*SafetyDownElevator);
				}
				else if(oiGamepad->GetButton(F310::kAButton) &&
						(oiGamepad->GetButton(F310::kYButton)))
				{
					ElevatorTalonLeft->Set(0.0);
					ElevatorTalonRight->Set(0.0);
				}
				if((ElevatorRight->GetDistance() && ElevatorLeft->GetDistance()) > 24)
				{
					SafetyUpElevator = 0;
				}
				else
				{
					SafetyUpElevator = 1;
				}
				if((ElevatorRight->GetDistance() && ElevatorLeft->GetDistance()) < 0)
				{
					SafetyDownElevator = 0;
				}
				else
				{
					SafetyDownElevator = 1;
				}

				this->Debug();

				if(oiGamepad->GetButton(F310::kXButton) == true)
					{
						this->ePneumaticControl(B_CLOSE);
					}

					if(oiGamepad->GetButton(F310::kBButton) == true)
						{
							this->ePneumaticControl(B_OPEN);
						}

					if(oiGamepad->GetButton(F310::kXButton) && oiGamepad->GetButton(F310::kBButton))
						{
								// do nothing.
						}


					dbDrive->TankDrive(oiLeft->GetY(), oiRight->GetY());
					if (oiRight->GetRawButton(1) == true)
						{
							this->HDrive(oiRight->GetX());
						}


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
		// drive train debug function
		void dbDebug()
		{
			SmartDashboard::PutNumber("dbMiddle:", dbMidEncoder->GetDistance());
			SmartDashboard::PutNumber("dbLeft:", dbLeftEncoder->GetDistance());
			SmartDashboard::PutNumber("dbRight:", dbRightEncoder->GetDistance());
			SmartDashboard::PutNumber("dbGyro:", dbGyro->GetAngle());
		}

		// elevator debug function
		void eDebug()
		{
			SmartDashboard::PutNumber("ElevatorTalonLeft", ElevatorLeft->GetDistance());
			SmartDashboard::PutNumber("ElevatorTalonRight", ElevatorRight->GetDistance());
		}
		void DebugTeleop()
		{
			SmartDashboard::GetNumber("Inches Forward", Distance);

			//Constantly Updates Elevator Encoders
			SmartDashboard::PutNumber("Left Elevator Encoder", ElevatorLeft->GetDistance());
			SmartDashboard::PutNumber("Right Elevator Encoder", ElevatorRight->GetDistance());

			//Constantly Updates Drive Base Encoders
			SmartDashboard::PutNumber("Drive Base Encoder Left", dbLeftEncoder->GetDistance());
			SmartDashboard::PutNumber("Drive Base Encoder Middle", dbMidEncoder->GetDistance());
			SmartDashboard::PutNumber("Drive Base Encoder Right", dbRightEncoder->GetDistance());

			//Elevator values
			SmartDashboard::PutNumber("SafetyUpElevator (Ask A Programmer)", SafetyUpElevator);
			SmartDashboard::PutNumber("SafetyDownElevator (Ask A Programmer)", SafetyDownElevator);
		}
		void Robot::Debug()
		{
			this->dbDebug();
			this->eDebug();
			this->DebugTeleop();
		}


	void Disabled(void)
	{

	}

	void Test(void)
	{

	}
};
START_ROBOT_CLASS(Robot);
