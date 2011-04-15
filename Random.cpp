/**
 * @file   Random.h
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

#include "Random.h"

#include <cmath>
#include <cstdlib>
#include <ctime>

Random::Random(void)
{
  srand(time(NULL));
}

int Random::randbit(void)
{
  double rn = uniform();
  if (rn >= 0.5)
    return 1;
  else
    return 0;
  return -1; /* can't happen */
}

double Random::uniform(void)
{
  return rand() / (RAND_MAX + 1.0);
}

double Random::gaussian(void)
{
  return gaussian(1.0);
}

double Random::gaussian(double variance)
{
  double radius = 0.0, theta = 0.0;

  radius = sqrt(-2 * variance * log(uniform()));
  theta = 2 * M_PI * uniform();

  return radius * cos(theta);
}

Signal Random::n_bit(int n, double samp_freq, double duration)
{
  Signal c(samp_freq, duration);
  int len = c.get_len();
  int max = pow(2, n);

  for (int i = 0; i < len; ++i)
    c[i] = rand() % max;

  return c;
}

Signal Random::AWGN(double variance, double samp_freq, double duration)
{
  Signal c(samp_freq, duration);
  int len = c.get_len();

  for (int i = 0; i < len; ++i)
    c[i] = gaussian(variance);

  return c;
}
