#pragma once
#include <Keyboard.h>
class SpeedControl {
public:
	float speed ;
	
	SpeedControl() {
		speed = 0.5f;
	}
	void SpeedInput();
	void decrement() {
		speed += 0.01f; 
	}
	void increment() {
		if (speed > 0) {
			speed -= 0.01f; 
		}
}
	void reset() { speed = 0.5f; }
	void backward() { speed *= -1; }

public:
	
};