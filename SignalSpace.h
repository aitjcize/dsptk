/**
 * @file   SignalSpace.h
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

#ifndef __ARITHMETIC_SIGNAL_SPACE_H__
#define __ARITHMETIC_SIGNAL_SPACE_H__

#include "Signal.h"

class SignalSpace {
public:
  SignalSpace(double freq = 0.0): freq(freq) {}
  Signal get_signal_at_coordinate(double x, double y);

  /*
   * @brief Perform inner product for Signal or an array of sampled signal
   * @param a first signal
   * @param b second signal
   * @param t_s start time of inner product
   * @param t_e end time of inner product
   * @param dt increamental time fraction
   * @param cnt reset the integral result after cnt * dt, 0 implies don't reset
   * @return a new signal
   */
  Signal inner_product(const Signal& a, const Signal& b,
                       double t_s, double t_e,
                       double dt, int cnt = 0);

  /* Member access */
  inline double get_freq(void) { return freq; }
  inline void set_freq(double f) { freq = f; }

private:
  double freq;
};

#endif /* __ARITHMETIC_SIGNAL_SPACE_H__ */
