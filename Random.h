/**
 * @file   Random.cpp
 * @brief  Generates random numbers
 * @author Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 *
 * Copyright (C) 2010 -  Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __SOURCE_RANDOM_H__
#define __SOURCE_RANDOM_H__

#include <cstdlib>

#include "Signal.h"

class Random {
public:
  Random();

  /*
   * Genrates random bit.
   */
  int randbit(void);

  /*
   * URNG that generates uniform random number at [0, 1).
   */
  double uniform(void);

  /*
   * GRNG using Box-Muller Algorithm that generates gaussian random number with
   * unit variance.
   */
  double gaussian(void);

  /*
   * GRNG using Box-Muller Algorithm that generates gaussian random number with
   * variance
   */
  double gaussian(double variance);

  /*
   * Generates n-bit random signal with duration duration and sample frequency
   * samp_freq
   */
  Signal n_bit(int n, double samp_freq, double duration);

  /*
   * Generate a AWGN Signal with duration duration and sample frequency
   * samp_freq
   */
  Signal AWGN(double variance, double samp_freq, double duration);
};

#endif /* __SOURCE_RANDOM_H__ */
