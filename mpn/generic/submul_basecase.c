/*
dnl  Copyright 2009 Jason Moxham

dnl  This file is part of the MPIR Library.

dnl  The MPIR Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 2.1 of the License, or (at
dnl  your option) any later version.

dnl  The MPIR Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the MPIR Library; see the file COPYING.LIB.  If not, write
dnl  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
dnl  Boston, MA 02110-1301, USA.
*/

#include "mpir.h"
#include "gmp-impl.h"

#define ADD ADDC_LIMB
#define SUB SUBC_LIMB

#define ADC(co,w,x,y,ci) do{mp_limb_t _c1,_c2,_t;ADD(_c1,_t,x,y);ADD(_c2,w,_t,ci);co=_c1|_c2;}while(0)
#define SBB(co,w,x,y,ci) do{mp_limb_t _c1,_c2,_t;SUB(_c1,_t,x,y);SUB(_c2,w,_t,ci);co=_c1|_c2;}while(0)

// NOTE: compaired with submul_1 or submul_2 this does one more sub
// submul_basecase      (rp,xn+yn) = (rp,xn+yn) - (xp,xn)*(yp,yn) and return borrow 0 or 1
mp_limb_t
mpn_submul_basecase (mp_ptr rp, mp_srcptr xp, mp_size_t xn, mp_srcptr yp,
		     mp_size_t yn)
{
  mp_size_t i;
  mp_limb_t t, carry;

  ASSERT (xn != 0);
  ASSERT (yn != 0);
  ASSERT_MPN (xp, xn);
  ASSERT_MPN (yp, yn);
  ASSERT_MPN (rp, xn + yn);
  ASSERT (!MPN_OVERLAP_P (rp, xn + yn, xp, xn));
  ASSERT (!MPN_OVERLAP_P (rp, xn + yn, yp, yn));
  carry = 0;
  for (i = 0; i < yn; i++)
    {
      t = mpn_submul_1 (rp + i, xp, xn, yp[i]);
      SBB (carry, rp[xn + i], rp[xn + i], t, carry);
    }
  return carry;
}
