/**
 * @file   MultiThreadedSimulator.cpp
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

#include "MultiThreadedSimulator.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include <sys/wait.h>

#include <pthread.h>

using std::vector;
using std::fill;

void* simulation_thread(void* obj)
{
  ThreadArg* arg = (ThreadArg*)obj;
  fprintf(stderr, "Thread %d running...\n", arg->thread_idx);
  fflush(stderr);
  arg->bers = arg->sim->run(arg->snr_dbs, arg->ser_thres);
  fprintf(stderr, "Thread %d Done.\n", arg->thread_idx);
  fflush(stderr);
  return NULL;
}

MultiThreadedSimulator::MultiThreadedSimulator(int nthreads, int bits,
    double samp_freq, Source* src, Modem* modem, int ber_n, BHfunc ber_handle):
    SimpleSimulator(bits, samp_freq, src, modem, ber_n, ber_handle),
    nthreads(nthreads)
{
  for (int i = 0; i < nthreads; ++i) {
    sims.push_back(new SimpleSimulator(bits, samp_freq, src, modem, ber_n,
                                       ber_handle));
  }
}

MultiThreadedSimulator::~MultiThreadedSimulator(void)
{
  for (int i = 0; i < nthreads; ++i)
    delete sims[i];
}

BERset& MultiThreadedSimulator::run(const vector<double>& snr_dbs,
                                    int ser_thres)
{
  vector<ThreadArg*> tcs;
  vector<BERset> bersets;
  void* ret = 0;

  for (int i = 0; i < nthreads; ++i) {
      tcs.push_back(new ThreadArg(sims[i], i, snr_dbs, ser_thres / nthreads));
      pthread_create(&tcs[i]->tid, NULL, simulation_thread, tcs[i]);
  }

  bers.resize(snr_dbs.size());
  for (size_t i = 0; i < snr_dbs.size(); ++i) {
    bers[i].resize(ber_n);
    for (int j = 0; j < ber_n; ++j)
      bers[i][j] = 0;
  }

  for (int i = 0; i < nthreads; ++i)
     pthread_join(tcs[i]->tid, &ret);

  for (int i = 0; i < nthreads; ++i) {
     for (size_t j = 0; j < snr_dbs.size(); ++j)
       for (int k = 0; k < ber_n; ++k)
         bers[j][k] += tcs[i]->bers[j][k];
  }
  
  for (int i = 0; i < nthreads; ++i)
    delete tcs[i];
  return bers;
}
