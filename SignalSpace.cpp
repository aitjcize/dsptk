/**
 * @file   SignalSpace.cpp
 * @brief  
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

#include "SignalSpace.h"

#include <cassert>
#include <cstdio>
#include <cmath>

Signal SignalSpace::get_signal_at_coordinate(double x, double y)
{
  return Signal(sqrt(x * x + y * y), freq, -atan(y / x));
}

Signal SignalSpace::inner_product(const Signal& a, const Signal& b,
                                  double t_s, double t_e,
                                  double dt, int cnt)
{
  assert(t_e > t_s);
  double t = 0.0, sum = 0.0;
  int i = 0, len = 0;
  Signal c(1 / dt, t_e - t_s);
  len = c.get_len();

  for (t = t_s, i = 0; t < t_e; t += dt, ++i) {
    if (cnt && i % cnt == 0)
      sum = 0.0;
    sum += a.at_t(t) * b.at_t(t) * dt;
    if (i < len)
      c[i] = sum;
  }
  return c;
}
