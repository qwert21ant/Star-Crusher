#ifndef TimerH
#define TimerH

#include <algorithm>

using namespace std;

class Timer{
private:
	double counter;
	double speed;

//---------------------------------------------------------------------------
public:
	Timer() : counter(0), speed(0) {};
	Timer(double Speed) : counter(0), speed(Speed) {};

    double GetSpeed() const { return speed; };

	void SetSpeed(double Speed) { speed = Speed; counter = 0; };

	bool Check(){
		counter += speed;
		if(counter >= 1.0){
			counter -= 1.0;
			return true;
		}
		return false;
	};
};

//---------------------------------------------------------------------------
#endif