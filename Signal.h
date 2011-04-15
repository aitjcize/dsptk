/**
 * @file   Signal.h
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

#ifndef __SIGNAL_SIGNAL_H__
#define __SIGNAL_SIGNAL_H__

#include <ostream>
#include <vector>

using std::ostream;
using std::vector;

class Signal {
public:
  /*
   * @brief Signal type
   * @SIG_COS describe signal with a cosine function
   * @SIG_SAMP describe signal with an array of sample data
   */
  typedef enum { SIG_COS, SIG_SAMP } SigType;

  /*
   * @brief Up/Down sample algorithm
   * @S_STAIRCASE staircase algorithm
   */
  typedef enum { S_STAIRCASE } SampAlgo;

  /*
   * @brief Initialize a Siganl with type SIG_COS
   * @param amp signal amplititude
   * @param freq signal frequency
   * @param phi signal phi
   */
  Signal(double amp, double freq, double phi);

  /*
   * @brief Initialize a Siganl with type SIG_SAMP
   * @param length length of the sample, -1 implies
   * int(round(freq) * round(duration)))
   * @param samp_freq sample frequency
   * @param duration signal duration
   */
  Signal(double samp_freq = 0, double duration = 0);
  void sig_samp_init(double sf, double dura);

  Signal(const Signal& sig);
  ~Signal(void);

  void free_samples(void);

  /* Member access */
  inline SigType get_type(void) const { return type; }
  inline double get_amp(void) const { return amp; }
  inline double get_freq(void) const { return freq; }
  inline double get_phi(void) const { return phi; }
  inline double get_samp_freq(void) const { return samp_freq; }
  inline double get_duration(void) const { return duration; }
  inline unsigned get_len(void) const { return len; }

  void set_type(SigType t) { type = t; }
  void set_amp(double a) { amp = a; }
  void set_freq(double f) { freq = f; }
  void set_phi(double p) { phi = p; }
  void set_samp_freq(double sf) { samp_freq = sf; }
  void set_duration(double d);

  /*
   * @brief Evaluate signal at time t
   * @param t time to be evaluate
   * @return the signal value at time t
   */
  double at_t(double t) const;

  /*
   * @brief return sample[i] for SIG_SAMP type signal, return garbage for
   * SIG_COS signal
   * @param index the index of sample[i]
   * @return sample[i]
   */
  double& operator[](const unsigned index);
  double operator[](const unsigned index) const;

  Signal& operator=(const Signal& obj);
  Signal operator+(const Signal& obj) const;
  Signal& operator+=(const Signal& obj);

  Signal operator*(const double mag) const;
  Signal& operator*=(const double mag);

  /*
   * @brief Up sample a signal
   * @param ufreq up sample frequency
   * @param sample algorithm to use, see enum SampAlgo
   * @return 
   */
  Signal up_sample(double ufreq, SampAlgo algo) const;

  /*
   * @brief Down sample a signal
   * @param dfreq down sample frequency
   * @return 
   */
  Signal down_sample(double dfreq) const;

  friend ostream& operator<<(ostream& out, const Signal& sig);

private:
  SigType type;

  /* SIG_COS */
  double amp;
  double freq;
  double phi;

  /* SIG_SAMP */
  unsigned len;
  double samp_freq;
  double duration;
  double* sample;

  /* Reference counting */
  const Signal* ref;
  vector<Signal*> refed;

  /* Reference counting */
  void reref(void);
  void ref_inc(const Signal* obj);
  void ref_dec(const Signal* obj);
  void hcopy(const Signal& obj);
  void hfree(void);
  void deepcopy(void);
};

#endif /* __SIGNAL_SIGNAL_H__ */
