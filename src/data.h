#include<cmath>
#include <deque>

using namespace std;

const double GYRO_THRESHOLD=200;
const float FILTET_ALPHA=0.5;  // parameter for low pass filter


double applyDeadZone(double gyroValue, double threshold);
float lowPassFilter(float current_value, float previous_value);
float angularVelocityToLinearVelocity(float angularVelocity_dps, float radius_m);