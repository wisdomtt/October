#include  "WPILib.h"
#include "DeadSwipe.h"
void ElevatorMove(Encoder *enLeft, Encoder*enRight, Talon *Left, Talon *Right, bool Direction, Timer *clock)
{
	clock->Reset();
	if(Direction)
	{
		while((SuperGetDistance(enLeft, Direction) && (SuperGetDistance(enRight, Direction))) < 24)
		{
			Left->Set(1.0);
			Right->Set(1.0);
			if(clock->Get() > 2.2)
			{
				break;
			}
		}
			Left->Set(0.0);
			Right->Set(0.0);
	}
	else if(!Direction)
	{
		while((SuperGetDistance(enLeft,Direction) && (SuperGetDistance(enRight,Direction))) > 0)
			{
				Left->Set(-1.0);
				Right->Set(-1.0);
				if(clock->Get() > 2.2)
				{
						break;
				}
			}
				Left->Set(0.0);
				Right->Set(0.0);
	}
}
float EncoderRevolutions(Encoder *enc)
{
	return enc->GetDistance()/EDPP;
}

float SuperGetDistance(Encoder *enc, bool direction)
{
	if(direction)
	return enc->GetDistance()+EncoderRevolutions(enc)(ropethickness);
	else
	return enc->GetDistance()-EncoderRevolutions(enc)(ropethickness);
}
