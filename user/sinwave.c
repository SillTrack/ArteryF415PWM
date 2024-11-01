
#include <stdlib.h>

#define PI 3.14159265358979323846f

int sin_wave(uint16_t max_value, uint16_t buf[], uint16_t n_samples)
{
    if (buf == NULL)
    {
        return -1;
    }

    uint16_t tmp_buf[n_samples + 1];
    for (int i = 0; i < (n_samples + 1); i++)
    {
        tmp_buf[i] = (uint16_t)(max_value * sin(PI * i / n_samples)); // half period
    }

    // average
    for (int i = 0; i < n_samples ; i++)
    {
        buf[i] = (tmp_buf[i] + tmp_buf[i + 1]) / 2;
    }

    return 0;
}
