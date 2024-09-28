#include "stdio.h"
#include "random.h"
#include "kalman.h"

int main() {
	KalmanFilter filter = KALMAN_INIT(1, 1, 1, 0.01, 0.1);
	for(int i = 0; i < 100; i++) {
		float input = 1;
		float m = randfloat(i + -0.1, i + 0.1);
		float x = kalmanUpdate(&filter, input, m);
		printf("%f\n", x);
	}
}