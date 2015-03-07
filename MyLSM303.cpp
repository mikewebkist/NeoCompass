#include <LSM303.h>
#include "MyLSM303.h"

float MyLSM303::vertical_magnitude() {
    vector<int32_t> temp_m = {m.x, m.y, m.z};
    temp_m.x -= ((int32_t)m_min.x + m_max.x) / 2;
    temp_m.y -= ((int32_t)m_min.y + m_max.y) / 2;
    temp_m.z -= ((int32_t)m_min.z + m_max.z) / 2;

    double mag = sqrt(sq((double) a.x) + sq((double) a.y) + sq((double) a.z));
    Serial.println(mag);
    float heading = atan2(a.z, mag) * 180 / M_PI;

    /*
    // return heading;
    vector<float> E;
    vector<float> N;
    vector_cross(&temp_m, &a, &E);
    vector_normalize(&E);
    vector_cross(&a, &E, &N);
    
    vector_normalize(&N);
    vector<int> foo = {0, -1, 0};

    float heading = atan2(vector_dot(&a, &foo), vector_dot(&a, &foo)) * 180 / M_PI;
    // float heading = vector_dot(&N, &a);
    */
    return heading;
}
