EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "TommyPROM 32 Nano"
Date "2022-12-19"
Rev "2.0"
Comp "github.com/TomNisbet"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 607CB85C
P 2450 2400
F 0 "A1" H 2200 3600 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 2200 3500 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 2450 2400 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 2450 2400 50  0001 C CNN
	1    2450 2400
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x16_Counter_Clockwise U3
U 1 1 607D18FC
P 2550 6600
F 0 "U3" H 2600 7550 50  0000 C CNN
F 1 "SST39SF040" H 2600 7450 50  0000 C CNN
F 2 "Socket:DIP_Socket-32_W11.9_W12.7_W15.24_W17.78_W18.5_3M_232-1285-00-0602J" H 2550 6600 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc0006.pdf" H 2550 6600 50  0001 C CNN
	1    2550 6600
	1    0    0    -1  
$EndComp
NoConn ~ 2550 1400
NoConn ~ 2350 1400
NoConn ~ 2950 1800
NoConn ~ 2950 1900
Wire Wire Line
	1450 4000 1450 2800
Wire Wire Line
	1450 2800 1950 2800
Wire Wire Line
	1550 2900 1950 2900
$Comp
L power:GND #PWR02
U 1 1 608154D3
P 2350 3500
F 0 "#PWR02" H 2350 3250 50  0001 C CNN
F 1 "GND" H 2355 3327 50  0000 C CNN
F 2 "" H 2350 3500 50  0001 C CNN
F 3 "" H 2350 3500 50  0001 C CNN
	1    2350 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 3400 2350 3450
$Comp
L power:GND #PWR08
U 1 1 608294BB
P 1450 7500
F 0 "#PWR08" H 1450 7250 50  0001 C CNN
F 1 "GND" H 1455 7327 50  0000 C CNN
F 2 "" H 1450 7500 50  0001 C CNN
F 3 "" H 1450 7500 50  0001 C CNN
	1    1450 7500
	1    0    0    -1  
$EndComp
Text Label 1600 2900 0    50   ~ 0
AD-BIT
Text Label 1600 2800 0    50   ~ 0
CLK-LO
Text Label 1600 2700 0    50   ~ 0
CLK-HI
NoConn ~ 1950 1800
NoConn ~ 1950 1900
NoConn ~ 1950 2200
Wire Wire Line
	2250 1350 2250 1400
NoConn ~ 1950 3000
NoConn ~ 1950 3100
Wire Wire Line
	2950 2000 3050 2000
Wire Wire Line
	2950 2100 3050 2100
Wire Wire Line
	2950 2200 3050 2200
Wire Wire Line
	2950 2300 3050 2300
Wire Wire Line
	2950 2400 3050 2400
Wire Wire Line
	2950 2500 3050 2500
Wire Wire Line
	2950 2600 3050 2600
Wire Wire Line
	2950 2700 3050 2700
$Comp
L Device:R_Small_US R1
U 1 1 608B3202
P 1300 2000
F 0 "R1" V 1250 2200 50  0000 C CNN
F 1 "10K" V 1250 2400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 1300 2000 50  0001 C CNN
F 3 "~" H 1300 2000 50  0001 C CNN
	1    1300 2000
	0    -1   1    0   
$EndComp
$Comp
L Device:R_Small_US R2
U 1 1 608B4927
P 1300 2100
F 0 "R2" V 1250 2300 50  0000 C CNN
F 1 "10K" V 1250 2500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 1300 2100 50  0001 C CNN
F 3 "~" H 1300 2100 50  0001 C CNN
	1    1300 2100
	0    -1   1    0   
$EndComp
$Comp
L Device:R_Small_US R3
U 1 1 608B68E4
P 1300 2200
F 0 "R3" V 1250 2400 50  0000 C CNN
F 1 "10K" V 1250 2600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 1300 2200 50  0001 C CNN
F 3 "~" H 1300 2200 50  0001 C CNN
	1    1300 2200
	0    -1   1    0   
$EndComp
Wire Wire Line
	1200 2200 750  2200
Wire Wire Line
	750  2100 1200 2100
Wire Wire Line
	750  1900 750  2000
Wire Wire Line
	1200 2000 750  2000
Connection ~ 750  2000
Wire Wire Line
	750  2000 750  2100
Wire Wire Line
	2950 3100 3350 3100
$Comp
L power:GND #PWR0102
U 1 1 6099119D
P 5600 5500
F 0 "#PWR0102" H 5600 5250 50  0001 C CNN
F 1 "GND" H 5605 5327 50  0000 C CNN
F 2 "" H 5600 5500 50  0001 C CNN
F 3 "" H 5600 5500 50  0001 C CNN
	1    5600 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 609A8765
P 6250 5350
F 0 "C2" H 6365 5396 50  0000 L CNN
F 1 "0.1uF" H 6365 5305 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 6288 5200 50  0001 C CNN
F 3 "~" H 6250 5350 50  0001 C CNN
	1    6250 5350
	1    0    0    -1  
$EndComp
Connection ~ 6250 5200
Connection ~ 6250 5500
Wire Wire Line
	6250 5200 6750 5200
Wire Wire Line
	6250 5500 6750 5500
$Comp
L Device:C C3
U 1 1 609B347E
P 6750 5350
F 0 "C3" H 6865 5396 50  0000 L CNN
F 1 "0.1uF" H 6865 5305 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 6788 5200 50  0001 C CNN
F 3 "~" H 6750 5350 50  0001 C CNN
	1    6750 5350
	1    0    0    -1  
$EndComp
Connection ~ 6750 5200
Connection ~ 6750 5500
$Comp
L Device:C C1
U 1 1 609B3CE9
P 5850 5350
F 0 "C1" H 5965 5396 50  0000 L CNN
F 1 "0.1uF" H 5965 5305 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 5888 5200 50  0001 C CNN
F 3 "~" H 5850 5350 50  0001 C CNN
	1    5850 5350
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 609C8C06
P 7650 5200
F 0 "#FLG0101" H 7650 5275 50  0001 C CNN
F 1 "PWR_FLAG" H 7650 5373 50  0000 C CNN
F 2 "" H 7650 5200 50  0001 C CNN
F 3 "~" H 7650 5200 50  0001 C CNN
	1    7650 5200
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 609C90E6
P 7650 5500
F 0 "#FLG0102" H 7650 5575 50  0001 C CNN
F 1 "PWR_FLAG" H 7650 5673 50  0000 C CNN
F 2 "" H 7650 5500 50  0001 C CNN
F 3 "~" H 7650 5500 50  0001 C CNN
	1    7650 5500
	1    0    0    1   
$EndComp
Text Notes 1550 900  0    50   ~ 0
Note that Vcc is provided by the USB\nand is fed from the Nano’s +5V pin
Wire Wire Line
	2450 3400 2450 3450
Wire Wire Line
	2450 3450 2350 3450
Connection ~ 2350 3450
Wire Wire Line
	2350 3450 2350 3500
Text GLabel 3050 2800 2    50   Output ~ 0
A16
Text GLabel 3050 2900 2    50   Output ~ 0
A17
Text GLabel 3050 3000 2    50   Output ~ 0
A18
Wire Wire Line
	2950 2800 3050 2800
Wire Wire Line
	2950 2900 3050 2900
Wire Wire Line
	2950 3000 3050 3000
Text GLabel 3050 2000 2    50   Output ~ 0
DQ0
Text GLabel 3050 2100 2    50   Output ~ 0
DQ1
Text GLabel 3050 2200 2    50   Output ~ 0
DQ2
Text GLabel 3050 2300 2    50   Output ~ 0
DQ3
Text GLabel 3050 2400 2    50   Output ~ 0
DQ4
Text GLabel 3050 2500 2    50   Output ~ 0
DQ5
Text GLabel 3050 2600 2    50   Output ~ 0
DQ6
Text GLabel 3050 2700 2    50   Output ~ 0
DQ7
Text GLabel 1250 2400 0    50   Output ~ 0
~WE
Text GLabel 1250 2500 0    50   Output ~ 0
~CE
Text GLabel 1250 2600 0    50   Output ~ 0
~OE
Connection ~ 750  2100
Wire Wire Line
	750  2100 750  2200
Wire Wire Line
	1250 2400 1550 2400
Wire Wire Line
	1250 2500 1650 2500
Wire Wire Line
	1250 2600 1750 2600
Wire Wire Line
	1400 2200 1550 2200
Wire Wire Line
	1550 2200 1550 2400
Connection ~ 1550 2400
Wire Wire Line
	1550 2400 1950 2400
Wire Wire Line
	1400 2100 1650 2100
Wire Wire Line
	1650 2100 1650 2500
Connection ~ 1650 2500
Wire Wire Line
	1650 2500 1950 2500
Wire Wire Line
	1400 2000 1750 2000
Wire Wire Line
	1750 2000 1750 2600
Connection ~ 1750 2600
Wire Wire Line
	1750 2600 1950 2600
Wire Wire Line
	1950 2700 1350 2700
$Comp
L Connector_Generic:Conn_02x16_Odd_Even J1
U 1 1 638C76BB
P 1700 6600
F 0 "J1" H 1750 7517 50  0000 C CNN
F 1 "left" H 1750 7426 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x16_P2.54mm_Vertical" H 1700 6600 50  0001 C CNN
F 3 "~" H 1700 6600 50  0001 C CNN
	1    1700 6600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x16_Odd_Even J2
U 1 1 638C98A0
P 3400 6600
F 0 "J2" H 3450 7517 50  0000 C CNN
F 1 "right" H 3450 7426 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x16_P2.54mm_Vertical" H 3400 6600 50  0001 C CNN
F 3 "~" H 3400 6600 50  0001 C CNN
	1    3400 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 5900 3200 5900
Wire Wire Line
	2850 6000 3200 6000
Wire Wire Line
	2850 6100 3200 6100
Wire Wire Line
	2850 6200 3200 6200
Wire Wire Line
	2850 6300 3200 6300
Wire Wire Line
	2850 6400 3200 6400
Wire Wire Line
	2850 6500 3200 6500
Wire Wire Line
	2850 6600 3200 6600
Wire Wire Line
	2850 6700 3200 6700
Wire Wire Line
	2850 6800 3200 6800
Wire Wire Line
	2850 6900 3200 6900
Wire Wire Line
	2850 7000 3200 7000
Wire Wire Line
	2850 7100 3200 7100
Wire Wire Line
	2850 7200 3200 7200
Wire Wire Line
	2850 7300 3200 7300
Wire Wire Line
	2850 7400 3200 7400
Wire Wire Line
	2000 5900 2350 5900
Wire Wire Line
	2000 6000 2350 6000
Wire Wire Line
	2000 6100 2350 6100
Wire Wire Line
	2000 6200 2350 6200
Wire Wire Line
	2000 6300 2350 6300
Wire Wire Line
	2000 6400 2350 6400
Wire Wire Line
	2000 6500 2350 6500
Wire Wire Line
	2000 6600 2350 6600
Wire Wire Line
	2000 6700 2350 6700
Wire Wire Line
	2000 6800 2350 6800
Wire Wire Line
	2000 6900 2350 6900
Wire Wire Line
	2000 7000 2350 7000
Wire Wire Line
	2000 7100 2350 7100
Wire Wire Line
	2000 7200 2350 7200
Wire Wire Line
	2000 7300 2350 7300
Wire Wire Line
	2000 7400 2350 7400
Wire Wire Line
	1500 7400 1450 7400
Wire Wire Line
	1450 7400 1450 7500
Wire Wire Line
	3750 5900 3700 5900
Wire Wire Line
	3750 5100 3750 5300
$Comp
L Connector_Generic:Conn_01x01 J3
U 1 1 639820FD
P 3550 5300
F 0 "J3" H 3700 5300 50  0000 C CNN
F 1 "PWR28" H 3550 5400 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 3550 5300 50  0001 C CNN
F 3 "~" H 3550 5300 50  0001 C CNN
	1    3550 5300
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J4
U 1 1 639829C2
P 3550 5500
F 0 "J4" H 3700 5500 50  0000 C CNN
F 1 "PWR24" H 3550 5600 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 3550 5500 50  0001 C CNN
F 3 "~" H 3550 5500 50  0001 C CNN
	1    3550 5500
	-1   0    0    -1  
$EndComp
Connection ~ 3750 5300
Wire Wire Line
	3750 5300 3750 5500
Connection ~ 3750 5500
Wire Wire Line
	3750 5500 3750 5900
$Comp
L Connector_Generic:Conn_01x04 J7
U 1 1 639D2A68
P 9100 4600
F 0 "J7" H 9180 4592 50  0000 L CNN
F 1 "+5V" H 9180 4501 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9100 4600 50  0001 C CNN
F 3 "~" H 9100 4600 50  0001 C CNN
	1    9100 4600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J8
U 1 1 639D3274
P 9100 5100
F 0 "J8" H 9180 5092 50  0000 L CNN
F 1 "+5V" H 9180 5001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9100 5100 50  0001 C CNN
F 3 "~" H 9100 5100 50  0001 C CNN
	1    9100 5100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J9
U 1 1 639D368F
P 9700 4600
F 0 "J9" H 9780 4592 50  0000 L CNN
F 1 "GND" H 9780 4501 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9700 4600 50  0001 C CNN
F 3 "~" H 9700 4600 50  0001 C CNN
	1    9700 4600
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J10
U 1 1 639D3B3A
P 9700 5100
F 0 "J10" H 9780 5092 50  0000 L CNN
F 1 "GND" H 9780 5001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9700 5100 50  0001 C CNN
F 3 "~" H 9700 5100 50  0001 C CNN
	1    9700 5100
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J5
U 1 1 639D3EE3
P 7650 1800
F 0 "J5" H 7800 1800 50  0000 C CNN
F 1 "high" H 7650 1700 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 7650 1800 50  0001 C CNN
F 3 "~" H 7650 1800 50  0001 C CNN
	1    7650 1800
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 639D4239
P 7650 1300
F 0 "J6" H 7800 1250 50  0000 C CNN
F 1 "low" H 7650 1400 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7650 1300 50  0001 C CNN
F 3 "~" H 7650 1300 50  0001 C CNN
	1    7650 1300
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_SPDT SW1
U 1 1 639D456E
P 8200 1800
F 0 "SW1" H 8200 2085 50  0000 C CNN
F 1 "SW_SPDT" H 8200 1994 50  0000 C CNN
F 2 "digikey-footprints:Switch_Slide_11.6x4mm_EG1218" H 8200 1800 50  0001 C CNN
F 3 "~" H 8200 1800 50  0001 C CNN
	1    8200 1800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J11
U 1 1 639EBC7A
P 9150 1300
F 0 "J11" V 9350 1200 50  0000 L CNN
F 1 "VPP out 1" V 9250 1150 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9150 1300 50  0001 C CNN
F 3 "~" H 9150 1300 50  0001 C CNN
	1    9150 1300
	0    -1   -1   0   
$EndComp
$Comp
L Diode:1N5818 D1
U 1 1 639FADD3
P 8700 1300
F 0 "D1" H 8700 1083 50  0000 C CNN
F 1 "1N5818" H 8700 1174 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 8700 1125 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 8700 1300 50  0001 C CNN
	1    8700 1300
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N5818 D2
U 1 1 63A28EFA
P 8700 1700
F 0 "D2" H 8700 1483 50  0000 C CNN
F 1 "1N5818" H 8700 1574 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 8700 1525 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 8700 1700 50  0001 C CNN
	1    8700 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	7850 1800 8000 1800
Wire Wire Line
	8550 1300 7850 1300
Wire Wire Line
	8850 1300 9000 1300
Wire Wire Line
	9000 1300 9000 1500
Wire Wire Line
	9000 1700 8850 1700
Connection ~ 9000 1500
Wire Wire Line
	9000 1500 9000 1700
Wire Wire Line
	8400 1700 8550 1700
Wire Wire Line
	8900 5300 8900 5200
Connection ~ 8900 4500
Connection ~ 8900 4600
Wire Wire Line
	8900 4600 8900 4500
Connection ~ 8900 4700
Wire Wire Line
	8900 4700 8900 4600
Connection ~ 8900 4800
Wire Wire Line
	8900 4800 8900 4700
Connection ~ 8900 5000
Wire Wire Line
	8900 5000 8900 4800
Connection ~ 8900 5100
Wire Wire Line
	8900 5100 8900 5000
Connection ~ 8900 5200
Wire Wire Line
	8900 5200 8900 5100
Wire Wire Line
	9900 4500 9900 4600
Connection ~ 9900 4600
Wire Wire Line
	9900 4600 9900 4700
Connection ~ 9900 4700
Wire Wire Line
	9900 4700 9900 4800
Connection ~ 9900 4800
Wire Wire Line
	9900 4800 9900 5000
Connection ~ 9900 5000
Wire Wire Line
	9900 5000 9900 5100
Connection ~ 9900 5100
Wire Wire Line
	9900 5100 9900 5200
Connection ~ 9900 5200
Wire Wire Line
	9900 5200 9900 5300
Connection ~ 9900 5300
$Comp
L power:GND #PWR011
U 1 1 63A7E853
P 9900 5350
F 0 "#PWR011" H 9900 5100 50  0001 C CNN
F 1 "GND" H 9905 5177 50  0000 C CNN
F 2 "" H 9900 5350 50  0001 C CNN
F 3 "" H 9900 5350 50  0001 C CNN
	1    9900 5350
	1    0    0    -1  
$EndComp
Text GLabel 1400 7000 0    50   Input ~ 0
A0
Text GLabel 1400 6900 0    50   Input ~ 0
A1
Text GLabel 1400 6800 0    50   Input ~ 0
A2
Text GLabel 1400 6700 0    50   Input ~ 0
A3
Text GLabel 1400 6600 0    50   Input ~ 0
A4
Text GLabel 1400 6500 0    50   Input ~ 0
A5
Text GLabel 1400 6400 0    50   Input ~ 0
A6
Text GLabel 1400 6300 0    50   Input ~ 0
A7
Text GLabel 3800 6400 2    50   Input ~ 0
A8
Text GLabel 3800 6500 2    50   Input ~ 0
A9
Text GLabel 3800 6800 2    50   Input ~ 0
A10
Text GLabel 3800 6600 2    50   Input ~ 0
A11
Text GLabel 1400 6200 0    50   Input ~ 0
A12
Text GLabel 3800 6300 2    50   Input ~ 0
A13
Text GLabel 3800 6200 2    50   Input ~ 0
A14
Text GLabel 1400 6100 0    50   Input ~ 0
A15
Text GLabel 1400 6000 0    50   Input ~ 0
A16
Text GLabel 3800 6100 2    50   Input ~ 0
A17
Text GLabel 1400 5900 0    50   Input ~ 0
A18
Text GLabel 1400 7100 0    50   Input ~ 0
DQ0
Text GLabel 1400 7200 0    50   Input ~ 0
DQ1
Text GLabel 1400 7300 0    50   Input ~ 0
DQ2
Text GLabel 3800 7400 2    50   Input ~ 0
DQ3
Text GLabel 3800 7300 2    50   Input ~ 0
DQ4
Text GLabel 3800 7200 2    50   Input ~ 0
DQ5
Text GLabel 3800 7100 2    50   Input ~ 0
DQ6
Text GLabel 3800 7000 2    50   Input ~ 0
DQ7
Text GLabel 3800 6000 2    50   Input ~ 0
~WE
Text GLabel 3800 6900 2    50   Input ~ 0
~CE
Text GLabel 3800 6700 2    50   Input ~ 0
~OE
Wire Wire Line
	3700 6000 3800 6000
Wire Wire Line
	3700 6100 3800 6100
Wire Wire Line
	3700 6200 3800 6200
Wire Wire Line
	3700 6300 3800 6300
Wire Wire Line
	3700 6400 3800 6400
Wire Wire Line
	3700 6500 3800 6500
Wire Wire Line
	3700 6600 3800 6600
Wire Wire Line
	3700 6700 3800 6700
Wire Wire Line
	3700 6800 3800 6800
Wire Wire Line
	3700 6900 3800 6900
Wire Wire Line
	3700 7000 3800 7000
Wire Wire Line
	3700 7100 3800 7100
Wire Wire Line
	3800 7200 3700 7200
Wire Wire Line
	3700 7300 3800 7300
Wire Wire Line
	3700 7400 3800 7400
Wire Wire Line
	1400 7300 1500 7300
Wire Wire Line
	1400 7200 1500 7200
Wire Wire Line
	1400 7100 1500 7100
Wire Wire Line
	1400 7000 1500 7000
Wire Wire Line
	1400 6900 1500 6900
Wire Wire Line
	1400 6800 1500 6800
Wire Wire Line
	1400 6700 1500 6700
Wire Wire Line
	1400 6600 1500 6600
Wire Wire Line
	1400 6500 1500 6500
Wire Wire Line
	1400 6400 1500 6400
Wire Wire Line
	1400 6300 1500 6300
Wire Wire Line
	1400 6200 1500 6200
Wire Wire Line
	1400 6100 1500 6100
Wire Wire Line
	1400 6000 1500 6000
Wire Wire Line
	1400 5900 1500 5900
$Comp
L Connector_Generic:Conn_01x01 J12
U 1 1 63D0158D
P 3550 3100
F 0 "J12" H 3468 3317 50  0000 C CNN
F 1 "D13" H 3468 3226 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 3550 3100 50  0001 C CNN
F 3 "~" H 3550 3100 50  0001 C CNN
	1    3550 3100
	1    0    0    1   
$EndComp
NoConn ~ 8400 1900
Wire Wire Line
	5600 5200 5850 5200
Wire Wire Line
	5600 5500 5850 5500
Connection ~ 5850 5200
Wire Wire Line
	5850 5200 6250 5200
Connection ~ 5850 5500
Wire Wire Line
	5850 5500 6250 5500
Wire Wire Line
	6750 5200 7650 5200
Wire Wire Line
	6750 5500 7650 5500
$Comp
L Connector_Generic:Conn_01x01 J14
U 1 1 63D6D984
P 8900 2000
F 0 "J14" H 9050 2000 50  0000 C CNN
F 1 "GND" H 9050 1900 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 8900 2000 50  0001 C CNN
F 3 "~" H 8900 2000 50  0001 C CNN
	1    8900 2000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9250 1500 9150 1500
Wire Wire Line
	9150 1500 9000 1500
Connection ~ 9150 1500
$Comp
L Connector_Generic:Conn_01x04 J16
U 1 1 6399E89F
P 9700 6050
F 0 "J16" H 9780 6042 50  0000 L CNN
F 1 "Tie" H 9780 5951 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9700 6050 50  0001 C CNN
F 3 "~" H 9700 6050 50  0001 C CNN
	1    9700 6050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9900 5950 9900 6050
Connection ~ 9900 6050
Wire Wire Line
	9900 6050 9900 6150
Connection ~ 9900 6150
Wire Wire Line
	9900 6150 9900 6250
$Comp
L Connector_Generic:Conn_01x04 J15
U 1 1 639AA711
P 9100 6050
F 0 "J15" H 9180 6042 50  0000 L CNN
F 1 "Tie" H 9180 5951 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9100 6050 50  0001 C CNN
F 3 "~" H 9100 6050 50  0001 C CNN
	1    9100 6050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9300 5950 9300 6050
Connection ~ 9300 6050
Wire Wire Line
	9300 6050 9300 6150
Connection ~ 9300 6150
Wire Wire Line
	9300 6150 9300 6250
Wire Wire Line
	8900 4400 8900 4500
Wire Wire Line
	7900 1150 7900 1200
Wire Wire Line
	7900 1200 7850 1200
Text Notes 7550 1600 0    50   ~ 0
VPP In
$Comp
L Connector_Generic:Conn_01x01 J17
U 1 1 639FC50F
P 7650 3150
F 0 "J17" H 7800 3150 50  0000 C CNN
F 1 "high" H 7650 3050 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 7650 3150 50  0001 C CNN
F 3 "~" H 7650 3150 50  0001 C CNN
	1    7650 3150
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J13
U 1 1 639FCA2F
P 7650 2650
F 0 "J13" H 7800 2600 50  0000 C CNN
F 1 "low" H 7650 2750 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7650 2650 50  0001 C CNN
F 3 "~" H 7650 2650 50  0001 C CNN
	1    7650 2650
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_SPDT SW2
U 1 1 639FCA39
P 8200 3150
F 0 "SW2" H 8200 3435 50  0000 C CNN
F 1 "SW_SPDT" H 8200 3344 50  0000 C CNN
F 2 "digikey-footprints:Switch_Slide_11.6x4mm_EG1218" H 8200 3150 50  0001 C CNN
F 3 "~" H 8200 3150 50  0001 C CNN
	1    8200 3150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J18
U 1 1 639FCA43
P 9150 2650
F 0 "J18" V 9350 2550 50  0000 L CNN
F 1 "VPP out 2" V 9250 2500 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9150 2650 50  0001 C CNN
F 3 "~" H 9150 2650 50  0001 C CNN
	1    9150 2650
	0    -1   -1   0   
$EndComp
$Comp
L Diode:1N5818 D3
U 1 1 639FCA4D
P 8700 2650
F 0 "D3" H 8700 2433 50  0000 C CNN
F 1 "1N5818" H 8700 2524 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 8700 2475 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 8700 2650 50  0001 C CNN
	1    8700 2650
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N5818 D4
U 1 1 639FCA57
P 8700 3050
F 0 "D4" H 8700 2833 50  0000 C CNN
F 1 "1N5818" H 8700 2924 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 8700 2875 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 8700 3050 50  0001 C CNN
	1    8700 3050
	-1   0    0    1   
$EndComp
Wire Wire Line
	7850 3150 8000 3150
Wire Wire Line
	8550 2650 7850 2650
Wire Wire Line
	8850 2650 9000 2650
Wire Wire Line
	9000 2650 9000 2850
Wire Wire Line
	9000 3050 8850 3050
Connection ~ 9000 2850
Wire Wire Line
	9000 2850 9000 3050
Wire Wire Line
	8400 3050 8550 3050
NoConn ~ 8400 3250
Wire Wire Line
	9250 2850 9150 2850
Wire Wire Line
	9150 2850 9000 2850
Connection ~ 9150 2850
$Comp
L power:+5V #PWR0104
U 1 1 639FCA6D
P 7900 2500
F 0 "#PWR0104" H 7900 2350 50  0001 C CNN
F 1 "+5V" H 7915 2673 50  0000 C CNN
F 2 "" H 7900 2500 50  0001 C CNN
F 3 "" H 7900 2500 50  0001 C CNN
	1    7900 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 2500 7900 2550
Wire Wire Line
	7900 2550 7850 2550
Text Notes 7550 2950 0    50   ~ 0
VPP In
Wire Wire Line
	1550 3900 1550 2900
Wire Wire Line
	1350 2700 1350 4100
Text GLabel 5500 3600 2    50   Output ~ 0
A15
Text GLabel 5500 3500 2    50   Output ~ 0
A14
Text GLabel 5500 3400 2    50   Output ~ 0
A13
Text GLabel 5500 3300 2    50   Output ~ 0
A12
Text GLabel 5500 3200 2    50   Output ~ 0
A11
Text GLabel 5500 3100 2    50   Output ~ 0
A10
Text GLabel 5500 3000 2    50   Output ~ 0
A9
Text GLabel 5500 2900 2    50   Output ~ 0
A8
Wire Wire Line
	5250 3600 5500 3600
Wire Wire Line
	4850 2600 4850 2650
Wire Wire Line
	4850 3800 4850 3850
$Comp
L power:GND #PWR06
U 1 1 60828993
P 4850 3850
F 0 "#PWR06" H 4850 3600 50  0001 C CNN
F 1 "GND" H 4855 3677 50  0000 C CNN
F 2 "" H 4850 3850 50  0001 C CNN
F 3 "" H 4850 3850 50  0001 C CNN
	1    4850 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3000 4450 3000
Wire Wire Line
	4350 3300 4350 3000
Wire Wire Line
	4450 3300 4350 3300
Wire Wire Line
	5250 3500 5500 3500
Wire Wire Line
	5250 3400 5500 3400
Wire Wire Line
	5250 3300 5500 3300
Wire Wire Line
	5250 3200 5500 3200
Wire Wire Line
	5250 3100 5500 3100
Wire Wire Line
	5250 3000 5500 3000
Wire Wire Line
	5250 2900 5500 2900
$Comp
L 74xx:74HCT164 U2
U 1 1 607D08C8
P 4850 3200
F 0 "U2" H 5100 3750 50  0000 C CNN
F 1 "74HCT164" H 5100 3650 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 5750 2900 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/74HC_HCT164.pdf" H 5750 2900 50  0001 C CNN
	1    4850 3200
	1    0    0    -1  
$EndComp
Text GLabel 5500 1850 2    50   Output ~ 0
A7
Text GLabel 5500 1750 2    50   Output ~ 0
A6
Text GLabel 5500 1650 2    50   Output ~ 0
A5
Text GLabel 5500 1550 2    50   Output ~ 0
A4
Text GLabel 5500 1450 2    50   Output ~ 0
A3
Text GLabel 5500 1350 2    50   Output ~ 0
A2
Text GLabel 5500 1250 2    50   Output ~ 0
A1
Text GLabel 5500 1150 2    50   Output ~ 0
A0
Wire Wire Line
	4850 900  4850 950 
Connection ~ 4850 900 
Wire Wire Line
	4350 900  4850 900 
Wire Wire Line
	4850 800  4850 900 
Wire Wire Line
	4850 2050 4850 2100
$Comp
L power:GND #PWR04
U 1 1 60826661
P 4850 2100
F 0 "#PWR04" H 4850 1850 50  0001 C CNN
F 1 "GND" H 4855 1927 50  0000 C CNN
F 2 "" H 4850 2100 50  0001 C CNN
F 3 "" H 4850 2100 50  0001 C CNN
	1    4850 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 1750 4450 1750
Wire Wire Line
	4450 1450 4050 1450
$Comp
L 74xx:74HCT164 U1
U 1 1 607CEBFA
P 4850 1450
F 0 "U1" H 5100 2000 50  0000 C CNN
F 1 "74HCT164" H 5100 1900 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm" H 5750 1150 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/74HC_HCT164.pdf" H 5750 1150 50  0001 C CNN
	1    4850 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1550 4350 1550
Wire Wire Line
	4350 1550 4350 1250
Wire Wire Line
	4450 1250 4350 1250
Connection ~ 4350 1250
Wire Wire Line
	4350 1250 4350 900 
Wire Wire Line
	5250 1150 5500 1150
Wire Wire Line
	5250 1250 5500 1250
Wire Wire Line
	5250 1350 5500 1350
Wire Wire Line
	5250 1450 5500 1450
Wire Wire Line
	5250 1550 5500 1550
Wire Wire Line
	5250 1650 5500 1650
Wire Wire Line
	5250 1750 5500 1750
Wire Wire Line
	5250 1850 5500 1850
Wire Wire Line
	4850 2650 4350 2650
Wire Wire Line
	4350 2650 4350 3000
Connection ~ 4850 2650
Wire Wire Line
	4850 2650 4850 2700
Connection ~ 4350 3000
Wire Wire Line
	4300 3500 4300 4100
Wire Wire Line
	4300 3500 4450 3500
Wire Wire Line
	4200 4000 4200 1750
Wire Wire Line
	4050 3900 4050 3200
Wire Wire Line
	4450 3200 4050 3200
Connection ~ 4050 3200
Wire Wire Line
	4050 3200 4050 1450
Wire Wire Line
	1550 3900 4050 3900
Wire Wire Line
	1450 4000 4200 4000
Wire Wire Line
	1350 4100 4300 4100
$Comp
L Device:C C4
U 1 1 63BAD949
P 9150 1800
F 0 "C4" H 9265 1846 50  0000 L CNN
F 1 "0.1uF" H 9265 1755 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 9188 1650 50  0001 C CNN
F 3 "~" H 9150 1800 50  0001 C CNN
	1    9150 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 1500 9150 1650
$Comp
L power:GND #PWR012
U 1 1 63BBBF7A
P 9150 2050
F 0 "#PWR012" H 9150 1800 50  0001 C CNN
F 1 "GND" H 9155 1877 50  0000 C CNN
F 2 "" H 9150 2050 50  0001 C CNN
F 3 "" H 9150 2050 50  0001 C CNN
	1    9150 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 1950 9150 2000
$Comp
L Device:C C5
U 1 1 63BC9615
P 9150 3150
F 0 "C5" H 9265 3196 50  0000 L CNN
F 1 "0.1uF" H 9265 3105 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 9188 3000 50  0001 C CNN
F 3 "~" H 9150 3150 50  0001 C CNN
	1    9150 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 2850 9150 3000
$Comp
L power:GND #PWR013
U 1 1 63BC9BFA
P 9150 3400
F 0 "#PWR013" H 9150 3150 50  0001 C CNN
F 1 "GND" H 9155 3227 50  0000 C CNN
F 2 "" H 9150 3400 50  0001 C CNN
F 3 "" H 9150 3400 50  0001 C CNN
	1    9150 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 3300 9150 3350
Wire Wire Line
	9900 5300 9900 5350
Wire Wire Line
	9100 2000 9150 2000
Connection ~ 9150 2000
Wire Wire Line
	9150 2000 9150 2050
$Comp
L Connector_Generic:Conn_01x01 J19
U 1 1 63D1AE9B
P 8900 3350
F 0 "J19" H 9050 3350 50  0000 C CNN
F 1 "GND" H 9050 3250 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 8900 3350 50  0001 C CNN
F 3 "~" H 8900 3350 50  0001 C CNN
	1    8900 3350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9100 3350 9150 3350
Connection ~ 9150 3350
Wire Wire Line
	9150 3350 9150 3400
$Comp
L power:+5V #PWR0101
U 1 1 63DF9749
P 7900 1150
F 0 "#PWR0101" H 7900 1000 50  0001 C CNN
F 1 "+5V" H 7915 1323 50  0000 C CNN
F 2 "" H 7900 1150 50  0001 C CNN
F 3 "" H 7900 1150 50  0001 C CNN
	1    7900 1150
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0103
U 1 1 63DF9DE5
P 8900 4400
F 0 "#PWR0103" H 8900 4250 50  0001 C CNN
F 1 "+5V" H 8915 4573 50  0000 C CNN
F 2 "" H 8900 4400 50  0001 C CNN
F 3 "" H 8900 4400 50  0001 C CNN
	1    8900 4400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0105
U 1 1 63DFAB4C
P 2250 1350
F 0 "#PWR0105" H 2250 1200 50  0001 C CNN
F 1 "+5V" H 2265 1523 50  0000 C CNN
F 2 "" H 2250 1350 50  0001 C CNN
F 3 "" H 2250 1350 50  0001 C CNN
	1    2250 1350
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0106
U 1 1 63DFB5A2
P 750 1900
F 0 "#PWR0106" H 750 1750 50  0001 C CNN
F 1 "+5V" H 765 2073 50  0000 C CNN
F 2 "" H 750 1900 50  0001 C CNN
F 3 "" H 750 1900 50  0001 C CNN
	1    750  1900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0107
U 1 1 63DFBE34
P 4850 800
F 0 "#PWR0107" H 4850 650 50  0001 C CNN
F 1 "+5V" H 4865 973 50  0000 C CNN
F 2 "" H 4850 800 50  0001 C CNN
F 3 "" H 4850 800 50  0001 C CNN
	1    4850 800 
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0108
U 1 1 63DFD091
P 4850 2600
F 0 "#PWR0108" H 4850 2450 50  0001 C CNN
F 1 "+5V" H 4865 2773 50  0000 C CNN
F 2 "" H 4850 2600 50  0001 C CNN
F 3 "" H 4850 2600 50  0001 C CNN
	1    4850 2600
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0109
U 1 1 63DFD9AD
P 5600 5200
F 0 "#PWR0109" H 5600 5050 50  0001 C CNN
F 1 "+5V" H 5615 5373 50  0000 C CNN
F 2 "" H 5600 5200 50  0001 C CNN
F 3 "" H 5600 5200 50  0001 C CNN
	1    5600 5200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0110
U 1 1 63DFE777
P 3750 5100
F 0 "#PWR0110" H 3750 4950 50  0001 C CNN
F 1 "+5V" H 3765 5273 50  0000 C CNN
F 2 "" H 3750 5100 50  0001 C CNN
F 3 "" H 3750 5100 50  0001 C CNN
	1    3750 5100
	1    0    0    -1  
$EndComp
$EndSCHEMATC
