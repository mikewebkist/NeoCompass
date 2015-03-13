#include <LSM303.h>
#include "MyLSM303.h"

float MyLSM303::pitch(void) {
    double amag = sqrt(vector_dot(&a, &a));
    return asin(-a.x / amag) * 180 / M_PI;
}

float MyLSM303::roll(void) {
    double amag = sqrt(vector_dot(&a, &a));
    return asin(a.y/amag / cos(asin(-a.x/amag))) * 180 / M_PI;
}

float MyLSM303::yaw(void) {
    vector<int32_t> temp_m = {m.x, m.y, m.z};
    temp_m.x -= ((int32_t)m_min.x + m_max.x) / 2;
    temp_m.y -= ((int32_t)m_min.y + m_max.y) / 2;
    temp_m.z -= ((int32_t)m_min.z + m_max.z) / 2;
    
    double amag = sqrt(vector_dot(&a, &a));
    vector<double> anorm = { a.x / amag, a.y / amag, a.z / amag };

    float pitch = asin(-anorm.x);
    float roll = asin(anorm.y/cos(pitch));

    float magxcomp = temp_m.x * cos(pitch) + temp_m.z * sin(pitch);
    float magycomp = temp_m.x * sin(roll) * sin(pitch) + temp_m.y * cos(roll) - temp_m.z * sin(roll) * cos(pitch);

    float yaw = atan2(magycomp, magxcomp) * 180 / M_PI;

    if(yaw < 0) yaw += 360;
    return yaw;
}

