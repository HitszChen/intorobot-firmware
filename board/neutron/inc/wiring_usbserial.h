/**
 ******************************************************************************
 * @file     : wiring_usartserial.h
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
 ******************************************************************************
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
 */

#ifndef   WIRING_USBSERIAL_H_
#define   WIRING_USBSERIAL_H_

#include "wiring_stream.h"
#include "wiring_usbserial_hal.h"

class USBSerial : public Stream
{
    private:
        bool started;

    public:
        // public methods
        USBSerial();

        void begin(void);
        void begin(long speed);
        void end(void);
        int peek(void);

        virtual size_t write(uint8_t byte);
        virtual int read(void);
        virtual int available(void);
        virtual void flush(void);

        using Print::write;

        operator bool(void);
};


extern USBSerial SerialUSB;

#endif /*WIRING_USBSERIAL_H_*/

