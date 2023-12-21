
#include <data.h> // need π（M_PI）

/**
 * @brief Converts angular velocity to linear velocity.
 *
 * This function is used to calculate the linear velocity of a point on a rotating object,
 * given the angular velocity of the object and the radius of the point from the center of rotation.
 * The linear velocity is calculated as the product of the angular velocity and the radius.
 *
 * @param angularVelocity_dps Angular velocity in degrees per second.
 * @param radius_m Radius in meters from the center of rotation to the point.
 * @return Linear velocity in meters per second at the specified radius.
 */
float angularVelocityToLinearVelocity(float angularVelocity_dps, float radius_m) {
    // First, convert angular velocity from degrees per second to radians per second
    float angularVelocity_rps = angularVelocity_dps * (M_PI / 180.0);

    // Then, calculate the linear velocity
    float linearVelocity = radius_m * angularVelocity_rps;

    return linearVelocity;
}


/**
 * @brief Applies a dead zone to a gyro value.
 * 
 * This function is used to filter out small gyro values that are likely just noise.
 * If the absolute value of the gyroValue is less than the specified threshold,
 * it is considered as noise and the function returns 0. Otherwise, it returns the original gyroValue.
 *
 * @param gyroValue The gyro value to be filtered.
 * @param threshold The threshold value for the dead zone.
 * @return The filtered gyro value, or 0 if the value is within the dead zone.
 */
double applyDeadZone(double gyroValue) {
    if (std::abs(gyroValue) < GYRO_THRESHOLD) {
        return 0.0; // 如果读数小于阈值，则视为零
    }
    return gyroValue; // 否则返回实际读数
}

/**
 * @brief Implements a simple low pass filter.
 *
 * This function is used to smoothen the gyro data by filtering out high-frequency noise.
 * It computes a new value based on the current and previous gyro readings, giving more weight to the previous value.
 * This results in a smoother output that changes less abruptly over time.
 *
 * @param current_value The current gyro value to be filtered.
 * @param previous_value The previous gyro value.
 * @return The filtered (smoothed) gyro value.
 */
float lowPassFilter(float current_value, float previous_value) {
    current_value=applyDeadZone(current_value);
    return FILTET_ALPHA * current_value + (1 - FILTET_ALPHA) * previous_value;
}


