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

#include "Signal.h"

#include <cassert>
#include <cstdio>
#include <cmath>
#include <cstring>

Signal::Signal(double amp, double freq, double phi)
  : amp(amp), freq(freq), phi(phi), ref(NULL)
{
  type = SIG_COS;
  freq = 0.0;
  samp_freq = 0.0;
  phi = 0.0;
  duration = 0.0;
  len = 0;
  sample = NULL;
}

Signal::Signal(double samp_freq, double duration)
  : samp_freq(samp_freq), duration(duration), ref(NULL)
{
  sample = NULL;
  sig_samp_init(samp_freq, duration);
}

void Signal::sig_samp_init(double sf, double dura)
{
  if (sample)
    delete [] sample;

  type = SIG_SAMP;
  len = (sf + 1) * (dura + 1);
  sample = new double[len];
}

void Signal::set_duration(double d)
{
  hfree();
  duration = d;
  sig_samp_init(samp_freq, d);
}

Signal::Signal(const Signal& sig): ref(NULL)
{
  sample = NULL;
  hcopy(sig);
}

Signal::~Signal(void)
{
  hfree();
}

void Signal::free_samples(void)
{
  if (ref) {
    const_cast<Signal*>(ref)->ref_dec(this);
  } else {
    if (refed.size())
      reref();
    else if (sample) {
      delete [] sample;
      sample = NULL;
    }
  }
}

double Signal::at_t(double t) const
{
  unsigned loc = 0;
  if (type == SIG_COS) {
    return amp * cos(2 * M_PI * freq * t + phi);
  } else if (type == SIG_SAMP) {
    loc = round(t * samp_freq);
    assert(loc < len);
    return sample[loc];
  }
  return 0.0;
}

double& Signal::operator[](const unsigned index)
{
  assert(index < len);
  deepcopy();

  if (type == SIG_SAMP)
    return sample[index];
  return sample[0];
}

double Signal::operator[](const unsigned index) const
{
  assert(index < len);

  if (type == SIG_SAMP)
    return sample[index];
  return sample[0];
}

Signal& Signal::operator=(const Signal& obj)
{
  hcopy(obj);
  return *this;
}

Signal Signal::operator+(const Signal& obj) const
{
  unsigned length = 0;
  double sfreq = 0.0, dura = 0.0;

  assert(type != SIG_COS && obj.type != SIG_COS);

  if (type == SIG_SAMP && obj.type == SIG_SAMP)
    assert(samp_freq == obj.samp_freq && len == obj.len);

  if (type == SIG_SAMP && obj.type == SIG_SAMP) {
    length = len;
    sfreq = samp_freq;
    dura = duration;
  } else if (type == SIG_SAMP) {
    length = len;
    sfreq = samp_freq;
    dura = duration;
  } else {
    length = obj.len;
    sfreq = obj.samp_freq;
    dura = obj.duration;
  }

  Signal c(sfreq, dura);

  for (unsigned i = 0; i < length; ++i) {
    if (type == SIG_SAMP && obj.type == SIG_SAMP)
      c.sample[i] = sample[i] + obj.sample[i];
    else if (type == SIG_SAMP)
      c.sample[i] = sample[i] + obj.at_t(i / obj.samp_freq);
    else
      c.sample[i] = at_t(i / samp_freq) + obj.sample[i];
  }
  return c;
}

Signal& Signal::operator+=(const Signal& obj)
{
  deepcopy();
  return (*this) = (*this) + obj;
}

Signal Signal::operator*(const double mag) const
{
  Signal c(*this);
  if (type == SIG_COS)
    c.amp *= mag;
  else  {
    for (unsigned i = 0; i < len; ++i)
      c.sample[i] *= mag;
  }
  return c;
}

Signal& Signal::operator*=(const double mag)
{
  deepcopy();
  return (*this) = (*this) * mag;
}

Signal Signal::up_sample(double ufreq, SampAlgo algo) const
{
  assert(ufreq >= samp_freq);
  Signal res(ufreq, duration);
  
  for (unsigned i = 0; i < res.get_len(); ++i) {
    if (algo == S_STAIRCASE) {
      res[i] = sample[(unsigned)(i * samp_freq / ufreq)];
    }
  }
  return res;
}

Signal Signal::down_sample(double dfreq) const
{
  assert(dfreq < samp_freq);
  Signal res(dfreq, duration);
  
  for (unsigned i = 0; i < res.get_len(); ++i)
    res[i] = sample[(unsigned)(i * dfreq / samp_freq)];
  return res;
}

void Signal::hcopy(const Signal& obj)
{
  type = obj.type;
  amp = obj.amp;
  freq = obj.freq;
  samp_freq = obj.samp_freq;
  phi = obj.phi;
  duration = obj.duration;
  len = obj.len;

  if (type == SIG_SAMP) {
    const Signal* topref = &obj;
    hfree();

    while (topref->ref)
      topref = topref->ref;
    ref = topref;
    const_cast<Signal*>(ref)->ref_inc(this);
    len = obj.len;
    sample = obj.sample;
  }
}

void Signal::hfree(void)
{
  if (ref) {
    const_cast<Signal*>(ref)->ref_dec(this);
  } else {
    if (refed.size())
      reref();
    else if (sample) {
      delete [] sample;
      sample = NULL;
    }
  }
}

ostream& operator<<(ostream& out, const Signal& sig)
{
  unsigned len = round(sig.get_samp_freq() * sig.get_duration());
  out << "[";
  for (unsigned i = 0; i < len; ++i)
    out << sig[i] << ((i != len -1)? ", ": "]");
  return out;
}

void Signal::reref(void)
{
  vector<Signal*>::iterator next, iter;
  if (refed.size()) {
    next = refed.begin();
    (*next)->ref = NULL;

    for (iter = refed.begin() + 1; iter != refed.end(); ++iter) {
      (*next)->ref_inc(*iter);
      (*iter)->ref = (*next);
      (*iter)->sample = (*next)->sample;
    }
  }
  refed.clear();
}

void Signal::ref_inc(const Signal* pobj)
{
  vector<Signal*>::iterator iter;
  for (iter = refed.begin(); iter != refed.end(); ++iter)
    if (*iter == pobj) return;
  refed.push_back(const_cast<Signal*>(pobj));
}

void Signal::ref_dec(const Signal* pobj)
{
  vector<Signal*>::iterator iter;
  for (iter = refed.begin(); iter != refed.end(); ++iter) {
    if (*iter == pobj) {
      refed.erase(iter);
      break;
    }
  }
}

void Signal::deepcopy(void) {
  double* tmp = NULL;
  if (ref) {
    tmp = new double[len];
    memcpy(tmp, sample, sizeof(double) * len);
    sample = tmp;
    const_cast<Signal*>(ref)->ref_dec(this);
    ref = NULL;
  }
}
