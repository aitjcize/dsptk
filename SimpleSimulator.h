/**
 * @file   SimpleSimulator.h
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

#ifndef __TOOL_SIMPLE_SIMULATOR_H__
#define __TOOL_SIMPLE_SIMULATOR_H__

#include <vector>

#include "Modem.h"
#include "Random.h"
#include "Simulator.h"
#include "Source.h"
#include "utils.h"

#define I(x, y, samp) int(round((x) * (samp) + (y)))

using std::vector;

class SimpleSimulator : public SimulatorBase {
public:
  /*
   * @brief SimpleSimulator constructor
   * @param bits the number of bits per symbol
   * @param samp_freq sample frequency
   * @param src Signal source, the simulation duration is determined by
   * src->get_duration(), allow NULL but need to be set using set_source before
   * simulation
   * @param modem The modem class, allow NULL but need to be set using set_modem
   * before simulation
   * @param uer_n number of fields of the ber vector
   * @param ber_handle ber counting handler
   */
  SimpleSimulator(int bits, double samp_freq, Source* src, Modem* modem,
                  int ber_n, BHfunc ber_handle);

  /*
   * @brief Run simulation
   * @param snr_dbs a vector of snr in dB
   * @param ser_thres the SER threshold of stopping simulation
   */
  virtual BERset& run(const vector<double>& snr_dbs, int ser_thres);
  vector<int>& run_single_round(double snr);

  void set_modem(Modem* m) { modem = m; }
  void set_source(Source* s) { src = s; }

  Signal& get_result(void) { return demod_packet; }

protected:
  Random rng;
  vector<int> ber;
  int bits;
  double samp_freq;
  Source* src;
  Modem* modem;
  int ber_n;
  BHfunc ber_handle;
  BERset snr_bers;
  Signal demod_packet;
};

#endif /* __TOOL_SIMPLE_SIMULATOR_H__ */
