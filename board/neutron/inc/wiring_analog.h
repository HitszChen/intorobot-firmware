/**
 ******************************************************************************
 * @file     : wring_analog.h
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
 *******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
**/

#ifndef WIRING_ANALOG_H_
#define WIRING_ANALOG_H_

#include "variant.h"


#ifdef __cplusplus
extern "C" {
#endif

int32_t analogRead(uint16_t pin);
void analogWrite(uint16_t pin, uint8_t value);
void analogWriteAndSetF(uint16_t pin, uint8_t value,uint32_t fre);

#ifdef __cplusplus
}
#endif

#endif /* WIRING_ANALOG_H_ */