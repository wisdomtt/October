#ifndef _F310_H_
#define _F310_H_

#include "WPILib.h"
#include "Joystick.h"

/**
 * Class to facilitate input handling from a Logitech F310
 */
class F310 : public Joystick
{
public:
	typedef enum
	{
		kLeftStick,
		kRightStick,
	} StickType;

	typedef enum
	{
		kLeftXAxis,
		kLeftYAxis,
		kRightXAxis,
		kRightYAxis,
	} AxisType;

	typedef enum
	{
		kXButton,
		kAButton,
		kBButton,
		kYButton,
		kLeftBumper,
		kRightBumper,
		kLeftTrigger,
		kRightTrigger,
		kBackButton,
		kStartButton,
		kLeftStickButton,
		kRightStickButton,
	} ButtonType;

	F310(uint32_t port);
	~F310();

	float GetX(StickType stick);
	float GetY(StickType stick);
	float GetAxis(AxisType axis);
	float GetDPadX(void);
	float GetDPadY(void);

	bool GetButton(ButtonType button);

private:
    static const uint32_t kLeftXAxisNum = 1;
    static const uint32_t kLeftYAxisNum = 2;
    static const uint32_t kRightXAxisNum = 3;
    static const uint32_t kRightYAxisNum = 4;
    static const uint32_t kDPadXAxisNum = 5;
    static const uint32_t kDPadYAxisNum = 6;

    static const uint32_t kXButtonNum = 1;
    static const uint32_t kAButtonNum = 2;
    static const uint32_t kBButtonNum = 3;
    static const uint32_t kYButtonNum = 4;
    static const uint32_t kLeftBumperNum = 5;
    static const uint32_t kRightBumperNum = 6;
    static const uint32_t kLeftTriggerNum = 7;
    static const uint32_t kRightTriggerNum = 8;
    static const uint32_t kBackButtonNum = 9;
    static const uint32_t kStartButtonNum = 10;
    static const uint32_t kLeftStickButtonNum = 11;
    static const uint32_t kRightStickButtonNum = 12;
};

#endif // _F310_H_

