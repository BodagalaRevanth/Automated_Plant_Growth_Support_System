\---------------------------------------------------------------

&#x09;Automated Plant Growth Support System

\---------------------------------------------------------------



An Arduino Nano and ESP32 Devkit based automated plant monitoring and growth support system.



The system monitors environmental conditions such as temperature, humidity, soil moisture and light intensity. Based on the selected plant and growth stage, the system automatically controls a water pump, misting system, cooling fan and grow light.



The ESP32 provides a Blynk-based dashboard for monitoring sensor values and modifying plant growth presets.



\---------------------------------------------------------------

&#x09;		Features

\---------------------------------------------------------------



\- Temperature monitoring

\- Humidity monitoring

\- Soil moisture monitoring

\- Light intensity monitoring

\- DS1307 real-time clock

\- Automatic water pump control

\- Automatic misting control

\- Automatic fan control

\- Automatic grow-light control

\- 16x2 LCD display

\- Physical stage navigation buttons

\- Plant and growth-stage presets

\- ESP32 Wi-Fi connectivity

\- Blynk dashboard

\- Remote preset modification

\- Arduino Nano + ESP32 UART communication



\---------------------------------------------------------------

&#x09;		System Architecture

\---------------------------------------------------------------



&#x20;                +----------------------+

&#x20;                |      Blynk App       |

&#x20;                |     / Dashboard      |

&#x20;                +----------+-----------+

&#x20;                           |

&#x20;                         Wi-Fi

&#x20;                           |

&#x20;                +----------v-----------+

&#x20;                |        ESP32         |

&#x20;                |                      |

&#x20;                | Blynk + Wi-Fi        |

&#x20;                | UART Communication   |

&#x20;                +----------+-----------+

&#x20;                           |

&#x20;                        UART

&#x20;                           |

&#x20;                +----------v-----------+

&#x20;                |     Arduino Nano     |

&#x20;                |                      |

&#x20;                | Sensor Processing    |

&#x20;                | Control Logic        |

&#x20;                +----+----+----+-------+

&#x20;                     |    |    |

&#x20;         +-----------+    |    +------------+

&#x20;         |                |                 |

&#x20;      Sensors          LCD/Buttons       Actuators

&#x20;         |                                  |

&#x20;      DHT11                               Pump

&#x20;      Soil Sensor                         Mist

&#x20;      LDR                                 Fan

&#x20;      DS1307                              NeoPixel LEDs





\---------------------------------------------------------------

&#x09;	ESP32 Devkit Pin Connections

\---------------------------------------------------------------

&#x20;-----------------------------------------------------------------------------------

| 		|						|		    |

| ESP32 Pin 	| Connection 					| Purpose 	    |

&#x20;-----------------------------------------------------------------------------------

| GPIO16 (RX2)  | Arduino Nano D1/TX via level shifter/divider  | UART receive      |

&#x20;-----------------------------------------------------------------------------------

| GPIO17 (TX2)  | Arduino Nano D0/RX 				| UART transmit     |

&#x20;-----------------------------------------------------------------------------------

| GND 		| Arduino Nano GND 				| Common ground     |

&#x20;-----------------------------------------------------------------------------------

| 3V3 		| 3.3 V peripherals 				| VCC 		    |

&#x20;-----------------------------------------------------------------------------------

| USB 		| PC / power supply 				| Power/programming |

&#x20;-----------------------------------------------------------------------------------



\---------------------------------------------------------------

&#x09;	Arduino Nano Connections

\---------------------------------------------------------------

&#x20;----------------------------------------------------------------------------------------

| 	   |   							  |		         |

| Nano Pin | Connection 					  | Purpose              |

&#x20;----------------------------------------------------------------------------------------

| 5V 	   | 5 V supply                                           | VCC                  |

&#x20;----------------------------------------------------------------------------------------

| GND 	   | Common GND                                           | Ground               |

&#x20;----------------------------------------------------------------------------------------

| D0       | ESP32 GPIO17                                         | UART RX              |

&#x20;----------------------------------------------------------------------------------------

| D1 	   | ESP32 GPIO16 via level shifter/divider               | UART TX              |

&#x20;----------------------------------------------------------------------------------------

| D13      | DHT11 DATA                                           | Temperature/humidity |

&#x20;----------------------------------------------------------------------------------------

| A1       | Soil sensor AO 					  | Soil moisture        |

&#x20;----------------------------------------------------------------------------------------

| A0       | LDR voltage-divider output 			  | Light                |

&#x20;----------------------------------------------------------------------------------------

| A4       | DS1307 SDA 					  | I²C data             |

&#x20;----------------------------------------------------------------------------------------

| A5       | DS1307 SCL 					  | I²C clock            |

&#x20;----------------------------------------------------------------------------------------

| D6       | LCD RS 						  | LCD                  |

&#x20;----------------------------------------------------------------------------------------

| D7       | LCD E 						  | LCD                  |

&#x20;----------------------------------------------------------------------------------------

| D8       | LCD D4 						  | LCD                  |

&#x20;----------------------------------------------------------------------------------------

| D9       | LCD D5 						  | LCD                  |

&#x20;----------------------------------------------------------------------------------------

| D10      | LCD D6 						  | LCD                  |

&#x20;----------------------------------------------------------------------------------------

| D11      | LCD D7 						  | LCD                  |

&#x20;----------------------------------------------------------------------------------------

| A3       | NeoPixel DIN 					  | Grow light           |

&#x20;----------------------------------------------------------------------------------------

| D5       | Pump relay IN 					  | Pump control         |

&#x20;----------------------------------------------------------------------------------------

| D4       | Mist relay IN 					  | Mist control         |

&#x20;----------------------------------------------------------------------------------------

| D2       | Fan relay IN 					  | Fan control          |

&#x20;----------------------------------------------------------------------------------------

| A2       | NEXT button 					  | Stage control        |

&#x20;----------------------------------------------------------------------------------------

| D12      | PREV button 					  | Stage control        |

&#x20;----------------------------------------------------------------------------------------

| 5V       | DHT11/soil/LCD/RTC/NeoPixel/relay VCC as appropriate | VCC                  |

&#x20;----------------------------------------------------------------------------------------

| GND      | All modules/common ground                            | GND                  |

&#x20;----------------------------------------------------------------------------------------



Note: Connect Arduino Nano D1/TX → ESP32 GPIO16 through a level shifter/ voltage divider, not a direct connection. The reverse direction, ESP32 GPIO17 → Nano D0/RX, is normally fine because a 3.3 V HIGH is generally recognized by the ATmega328P at a 5 V supply.

