/**
 * @file   RandomBitsSource.h
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

#ifndef __SOURCE_RANDOM_BITS_SOURCE_H__
#define __SOURCE_RANDOM_BITS_SOURCE_H__

#include <cmath>

#include "Random.h"
#include "Signal.h"
#include "Source.h"

class RandomBitsSource : public Source {
public:
  RandomBitsSource(int bits, double samp_freq, double duration):
    bits(bits), samp_freq(samp_freq), Source(duration) {}

  virtual Signal generate(void)
  {
    Signal c(samp_freq, duration);
    int len = c.get_len();
    int max = pow(2, bits);

    for (int i = 0; i < len; ++i)
      c[i] = rand() % max;

    return c;
  }

private:
  int bits;
  double samp_freq;
};

#endif /* __SOURCE_RANDOM_BITS_SOURCE_H__ */
