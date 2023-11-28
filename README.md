# Arduino M-BUS Master node for Arduino MKR M-BUS Shield

This software will read out a M-BUS slave device connected to an Arduino MKR board equipped with our [Arduino MKR M-BUS Master shield](https://www.hwhardsoft.de/english/projects/m-bus-mkr-shield/).

![My image](https://private-user-images.githubusercontent.com/3049858/286342795-c4f2cda1-ab28-45bd-a090-7284bd47ff43.jpg?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTEiLCJleHAiOjE3MDExOTE5NTksIm5iZiI6MTcwMTE5MTY1OSwicGF0aCI6Ii8zMDQ5ODU4LzI4NjM0Mjc5NS1jNGYyY2RhMS1hYjI4LTQ1YmQtYTA5MC03Mjg0YmQ0N2ZmNDMuanBnP1gtQW16LUFsZ29yaXRobT1BV1M0LUhNQUMtU0hBMjU2JlgtQW16LUNyZWRlbnRpYWw9QUtJQUlXTkpZQVg0Q1NWRUg1M0ElMkYyMDIzMTEyOCUyRnVzLWVhc3QtMSUyRnMzJTJGYXdzNF9yZXF1ZXN0JlgtQW16LURhdGU9MjAyMzExMjhUMTcxNDE5WiZYLUFtei1FeHBpcmVzPTMwMCZYLUFtei1TaWduYXR1cmU9OGU4MmU1M2ZmNzViMGNhMmEwYzU0N2IwOGMwY2Q1NjM0MzU3N2M1MzVmZWEwMWE5MzVjOTZjNGVjOGI1ZDlhYiZYLUFtei1TaWduZWRIZWFkZXJzPWhvc3QmYWN0b3JfaWQ9MCZrZXlfaWQ9MCZyZXBvX2lkPTAifQ.hF5yBF35-CZS67LZv3n_rKtdEuv5FxKdeEf7o-eOj30)


## Data decoding
The decoded data is output in JSON format via the UART0 (USB port) and can be viewed via the serial monitor of the Arduino IDE.
Make sure to adjust the primary MBUS address of the slave and the baud rate of the slave in the program.


## Credits
[MbusduinoLib](https://github.com/Zeppelin500/MBusino) by Zeppelin500 


Based on the awesome work of [Open energy monitor](https://github.com/openenergymonitor/HeatpumpMonitor) code by Trystan Lea, Glyn Hudson, and others

[emonMbus](https://github.com/btm/emonMbus) by Bryan McLellan  <btm@loftninjas.org>



## License

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

