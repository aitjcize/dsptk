/**
 * @file   QAM.cpp
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

#include "QAM.h"

#include <cmath>
#include <cstdio>

#include "SignalSpace.h"
#include "utils.h"

QAM::QAM(int m, int n, double sig_freq, double carr_freq):
  m(m), n(n), sig_freq(sig_freq), carr_freq(carr_freq)
{
  double e = 0;
  graymap = rect_graymap_create(round(log2(m)), round(log2(n)));

  for (int i = 0; i < m / 2; ++i)
    for (int j = 0; j < n / 2; ++j)
      e += ((2 * i + 1) * (2 * i + 1) + (2 * j + 1) * (2 * j + 1));
  d_base = sqrt(round(log2(m) + log2(n)) * m * n / e);
}

QAM::~QAM(void)
{
  for (int i = 0; i < m; ++i)
    delete [] graymap[i];
  delete [] graymap;
}


Signal QAM::modulation(const Signal& src, double snr) const
{
  int nn = 0, mm = 0;
  Signal moded = src;
  Signal carrier(sqrt(2 * sig_freq), carr_freq, 0.0);
  double base = 0, d = 0, x = 0, y = 0, theta = 0;

  d = d_base * sqrt(2 * pow(10.0, snr / 10.0));
  base = carrier.get_amp();

  for (unsigned long i = 0; i < src.get_len(); ++i) {
    for (int j = 0; j < m; ++j)
      for (int k = 0; k < n; ++k)
        if (graymap[j][k] == (int)round(src[i])) {
          mm = j;
          nn = k;
          break;
        }
    x = ((nn - n / 2) + 0.5) * d;
    y = ((mm - m / 2) + 0.5) * d;
    theta = atan(y / x) + (x < 0) * M_PI + (x > 0 && y < 0) * 2 * M_PI;

    carrier.set_amp(base * sqrt(x * x + y * y));
    carrier.set_phi(theta);
    moded[i] = carrier.at_t(i / moded.get_samp_freq());
  }
  return moded;
}

Signal QAM::demodulation(const Signal& src, double snr) const
{
  SignalSpace space;
  Signal x_hat(sqrt(2 * sig_freq), carr_freq, 0.0),
         y_hat(sqrt(2 * sig_freq), carr_freq, M_PI / 2);
  Signal x_inted, y_inted;
  Signal result(sig_freq, src.get_duration());
  double samp_freq = 0, xv = 0, yv = 0, d = 0;
  int mm = 0, nn = 0;

  d = d_base * sqrt(2 * pow(10.0, snr / 10.0));
  samp_freq = src.get_samp_freq();
  x_inted = space.inner_product(x_hat, src, 0.0, src.get_duration(),
                                1 / samp_freq, samp_freq);

  y_inted = space.inner_product(y_hat, src, 0.0, src.get_duration(),
                                1 / samp_freq, samp_freq);

  for (int t = 0; t < src.get_duration(); ++t) {
    xv = x_inted[(int)((t + 1) * samp_freq) -1];
    yv = y_inted[(int)((t + 1) * samp_freq) -1];
    nn = round(xv / d - 0.5 + n / 2);
    mm = round(yv / d - 0.5 + m / 2);
    nn = CLAMP(nn, 0, n -1);
    mm = CLAMP(mm, 0, m -1);
    result[(int)round(t * sig_freq)] = graymap[mm][nn];
  }
  return result;
}
