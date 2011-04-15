/**
 * @file   MultiThreadedSimulator.h
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

#ifndef __TOOL_MULTITHREADED_SIMULATOR_H__
#define __TOOL_MULTITHREADED_SIMULATOR_H__

#include <pthread.h>

#include "Source.h"
#include "SimpleSimulator.h"

class MultiThreadedSimulator;

class ThreadArg {
public:
  ThreadArg(SimpleSimulator* sim, int thread_idx, vector<double> snr_dbs,
      int ser_thres):
    sim(sim), thread_idx(thread_idx), snr_dbs(snr_dbs), ser_thres(ser_thres) {}

  pthread_t tid;
  SimpleSimulator* sim;
  int thread_idx;
  vector<double> snr_dbs;
  int ser_thres;
  BERset bers;
};

void* simulation_thread(void* obj);

class MultiThreadedSimulator : public SimpleSimulator {
public:
  /*
   * @brief SimpleSimulator constructor
   * @param bits the number of bits per symbol
   * @param samp_freq sample frequency
   * @param modem The modem class
   * @param ber_n number of fields of the ber vector
   * @param ber_handle ber counting handler
   */
  MultiThreadedSimulator(int nthreads, int bits, double samp_freq, Source* src,
                         Modem* modem, int ber_n, BHfunc ber_handle);
  ~MultiThreadedSimulator(void);

  /*
   * @brief Run simulation
   * @param snr_dbs a vector of snr in dB
   * @param duration duration of a singal iteration
   * @param ser_thres the SER threshold of stopping simulation
   */
  virtual BERset& run(const vector<double>& snr_dbs, int ser_thres);
  void run_single_round(int thread_idx, double snr);

private:
  int nthreads;
  vector<SimpleSimulator*> sims;
  BERset bers;
};

#endif /* __TOOL_MULTITHREADED_SIMULATOR_H__ */
