/**
 * @file   QAM.h
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

#ifndef __MODEM_QAM_H__
#define __MODEM_QAM_H__

#include "Modem.h"
#include "Signal.h"

class QAM : public Modem {
public:
  QAM(int m, int n, double sig_freq, double carr_freq);
  ~QAM(void);

  virtual Signal modulation(const Signal& src, double snr) const;
  virtual Signal demodulation(const Signal& src, double snr) const;
  
private:
  int m;
  int n;
  double d_base;
  double sig_freq;
  double carr_freq;
  int** graymap;
};

#endif /* __MODEM_QAM_H__ */
