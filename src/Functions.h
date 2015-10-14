#include  "WPILib.h"
void ElevatorMove(Encoder *enLeft, Encoder*enRight, Talon *Left, Talon *Right, bool Direction)
{
	if(Direction)
	{
		while((enLeft && enRight) < 24)
		{
			Left->Set(1.0);
			Right->Set(1.0);
		}
			Left->Set(0.0);
			Right->Set(0.0);
	}
	else if(!Direction)
	{
		while((enLeft && enRight) > 24)
			{
				Left->Set(1.0);
				Right->Set(1.0);
			}
				Left->Set(0.0);
				Right->Set(0.0);
	}
}
