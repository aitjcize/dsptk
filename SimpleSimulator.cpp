/**
 * @file   SimpleSimulator.cpp
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

#include "SimpleSimulator.h"

#include <cassert>
#include <cmath>
#include <cstdio>

SimpleSimulator::SimpleSimulator(int bits, double samp_freq, Source* src,
                                 Modem* modem, int ber_n, BHfunc ber_handle):
  bits(bits), samp_freq(samp_freq), src(src), modem(modem), ber_n(ber_n),
  ber_handle(ber_handle)
{
  ++ber_n;
  ber.resize(ber_n);
  for (int i = 0; i < ber_n; ++i)
    ber[i] = 0;
}

BERset& SimpleSimulator::run(const vector<double>& snr_dbs, int ser_thres)
{
  for (size_t i = 0; i < snr_dbs.size(); ++i) {
    fprintf(stderr, "Running SNR = %1.1f ...\n", snr_dbs[i]);
    for (size_t j = 0; j < ber.size(); ++j)
      ber[j] = 0;
    do {
      run_single_round(snr_dbs[i]);
      ++ber[0];
    } while (ber[1] < ser_thres);
    snr_bers.push_back(ber);
  }
  return snr_bers;
}

vector<int>& SimpleSimulator::run_single_round(double snr)
{
  Signal packet, mod_packet;

  assert(modem != NULL);
  assert(src != NULL);

  /* Generate initial signal */
  packet = src->generate().up_sample(samp_freq, Signal::S_STAIRCASE);

  /* Start modulation */
  mod_packet = modem->modulation(packet, snr);

  /* AWGN Channel */
  mod_packet += rng.AWGN(1, samp_freq, src->get_duration()) * sqrt(samp_freq);
  
  /* Start demodulation */
  demod_packet = modem->demodulation(mod_packet, snr);

  /* Compute error rate */
  if (ber_handle) {
    for (double t = 0; t < src->get_duration(); ++t)
      ber_handle(ber, round(packet[I(t, 0, samp_freq)]),
          round(demod_packet[t]));
  }
  return ber;
}
