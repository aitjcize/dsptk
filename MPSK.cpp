/**
 * @file   MPSK.cpp
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

#include "MPSK.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Signal.h"
#include "SignalSpace.h"
#include "utils.h"

MPSK::MPSK(int m, double sig_freq, double carr_freq):
  m(m), sig_freq(sig_freq), carr_freq(carr_freq)
{
  graymap = linear_graymap_create(round(log2(m)));
}

MPSK::~MPSK(void)
{
  delete [] graymap;
}

Signal MPSK::modulation(const Signal& src, double snr) const
{
  int n = 0;
  Signal moded = src;
  Signal carrier(sqrt(2 * sig_freq), carr_freq, 0.0);

  carrier *= sqrt(2 * round(log2(m)) * pow(10.0, snr / 10.0));

  for (unsigned long i = 0; i < src.get_len(); ++i) {
    for (int j = 0; j < m; ++j)
      if (graymap[j] == (int)round(src[i])) {
        n = j;
        break;
      }
    carrier.set_phi(2 * M_PI * n / m);
    moded[i] = carrier.at_t(i / moded.get_samp_freq());
  }
  return moded;
}

Signal MPSK::demodulation(const Signal& src, double snr) const
{
  SignalSpace space;
  Signal x_hat(sqrt(2 * sig_freq), carr_freq, 0.0),
         y_hat(sqrt(2 * sig_freq), carr_freq, M_PI / 2);
  Signal x_inted, y_inted;
  Signal result(sig_freq, src.get_duration());
  double samp_freq = 0, theta = 0, xv = 0, yv = 0;
  int idx = 0;

  samp_freq = src.get_samp_freq();
  x_inted = space.inner_product(x_hat, src, 0.0, src.get_duration(),
                                1 / samp_freq, samp_freq);

  y_inted = space.inner_product(y_hat, src, 0.0, src.get_duration(),
                                1 / samp_freq, samp_freq);

  for (int t = 0; t < src.get_duration(); ++t) {
    xv = x_inted[(int)((t + 1) * samp_freq) -1];
    yv = y_inted[(int)((t + 1) * samp_freq) -1];
    if (xv == 0)
      theta = M_PI / 2;
    else
      theta = atan(yv / xv);

    theta += (xv < 0) * M_PI + (xv > 0 && yv < 0) * 2 * M_PI;
    idx = round(theta * m / (2 * M_PI));
    result[(int)round(t * sig_freq)] = graymap[idx];
  }
  return result;
}
