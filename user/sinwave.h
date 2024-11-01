#pragma once

/**
 * @brief Generate sine wave
 * @param max_value amplitude
 * @param buf buffer to store the wave
 * @param n_samples number of samples
 * @return 0 on success, -1 on error
 */
int sin_wave(uint16_t max_value, uint16_t buf[], uint16_t n_samples);
