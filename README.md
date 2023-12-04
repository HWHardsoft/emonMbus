# Arduino M-BUS Master node for Arduino MKR M-BUS Shield

This software will read out a M-BUS slave device connected to an Arduino MKR board equipped with our [Arduino MKR M-BUS Master shield](https://www.hwhardsoft.de/english/projects/m-bus-mkr-shield/).

![My image](https://github.com/HWHardsoft/emonMbus/assets/3049858/ced0fc9a-9ee6-4e34-9a0c-d57d86f6fc5c)


## Data decoding
The decoded data is output in JSON format via the UART0 (USB port) and can be viewed via the serial monitor of the Arduino IDE.
Make sure to adjust the primary MBUS address of the slave and the baud rate of the slave in the program.

![Monitor_output](https://github.com/HWHardsoft/emonMbus/assets/3049858/6ef558bd-514f-41fe-a4d4-2f18750ba42e)

## Credits
[MbusinoLib](https://github.com/Zeppelin500/MBusino) by Zeppelin500 


Based on the awesome work of [Open energy monitor](https://github.com/openenergymonitor/HeatpumpMonitor) code by Trystan Lea, Glyn Hudson, and others

[emonMbus](https://github.com/btm/emonMbus) by Bryan McLellan  <btm@loftninjas.org>



## License

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

