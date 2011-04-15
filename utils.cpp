/**
 * @file   utils.cpp
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


#include "utils.h"

#include <cmath>
#include <cstring>
#include <cstdio>

int* linear_graymap_create(int bits)
{
  int half = 0, m = 0;
  int* graymap = NULL;

  m = round(pow(2, bits));
  graymap = new int[m];
  memset(graymap, 0, sizeof(int) * m);

  for (int i = 0; i < bits; ++i) {
    half = round(pow(2, i));
    for (int j = half; j < 2 * half; ++j)
      graymap[j] |= (1 << i);

    if (i < bits - 1) {
      half = round(pow(2, i + 1));
      for (int j = half; j < 2 * half; ++j)
        graymap[j] = graymap[2 * half - j - 1];
    }
  }
  return graymap;
}

int** rect_graymap_create(int m_bits, int n_bits)
{
  int m = 0, n = 0;
  int** graymap = NULL;
  int* col_map = NULL;

  m = round(pow(2, m_bits));
  n = round(pow(2, n_bits));

  graymap = new int*[m];
  for (int i = 0; i < m; ++i)
    graymap[i] = linear_graymap_create(n_bits);
  col_map = linear_graymap_create(m_bits);

  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      graymap[i][j] = (graymap[i][j] << n_bits) | col_map[i];
  delete [] col_map;

  return graymap;
}
