EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Memory_Flash:SST39SF040 U1
U 1 1 61AFB38D
P 3450 2450
F 0 "U1" H 3450 2450 50  0000 C CNN
F 1 "SST39SF040" H 3750 3750 50  0000 C CNN
F 2 "Package_DIP:DIP-32_W15.24mm_Socket" H 3450 2750 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/25022B.pdf" H 3450 2750 50  0001 C CNN
	1    3450 2450
	1    0    0    -1  
$EndComp
$Comp
L Memory_EEPROM:28C256 U2
U 1 1 61AFBB43
P 6650 2250
F 0 "U2" H 6650 2300 50  0000 C CNN
F 1 "28C256" H 6900 3300 50  0000 C CNN
F 2 "Package_DIP:DIP-28_W15.24mm" H 6650 2250 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc0006.pdf" H 6650 2250 50  0001 C CNN
	1    6650 2250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x14 J1
U 1 1 61AFC6D5
P 3350 5100
F 0 "J1" H 3300 5850 50  0000 L CNN
F 1 "t-left" H 3150 4250 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 3350 5100 50  0001 C CNN
F 3 "~" H 3350 5100 50  0001 C CNN
	1    3350 5100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x14 J2
U 1 1 61AFD052
P 3700 5100
F 0 "J2" H 3700 5850 50  0000 C CNN
F 1 "t-right" H 3600 4250 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 3700 5100 50  0001 C CNN
F 3 "~" H 3700 5100 50  0001 C CNN
	1    3700 5100
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x14 J3
U 1 1 61AFEB74
P 6450 5150
F 0 "J3" H 6400 5900 50  0000 L CNN
F 1 "b-left" H 6250 4300 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x14_P2.54mm_Vertical" H 6450 5150 50  0001 C CNN
F 3 "~" H 6450 5150 50  0001 C CNN
	1    6450 5150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x14 J4
U 1 1 61AFECD6
P 6800 5150
F 0 "J4" H 6800 5900 50  0000 C CNN
F 1 "b-right" H 6700 4300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x14_P2.54mm_Vertical" H 6800 5150 50  0001 C CNN
F 3 "~" H 6800 5150 50  0001 C CNN
	1    6800 5150
	-1   0    0    -1  
$EndComp
Text GLabel 6200 2950 0    50   Input ~ 0
~WE
Text GLabel 6200 3050 0    50   Input ~ 0
~OE
Text GLabel 6200 3150 0    50   Input ~ 0
~CE
Text GLabel 7100 1350 2    50   Input ~ 0
D0
Text GLabel 7100 1450 2    50   Input ~ 0
D1
Text GLabel 7100 1550 2    50   Input ~ 0
D2
Text GLabel 7100 1650 2    50   Input ~ 0
D3
Text GLabel 7100 1750 2    50   Input ~ 0
D4
Text GLabel 7100 1850 2    50   Input ~ 0
D5
Text GLabel 7100 1950 2    50   Input ~ 0
D6
Text GLabel 7100 2050 2    50   Input ~ 0
D7
Text GLabel 6200 1350 0    50   Input ~ 0
A0
Text GLabel 6200 1450 0    50   Input ~ 0
A1
Text GLabel 6200 1550 0    50   Input ~ 0
A2
Text GLabel 6200 1650 0    50   Input ~ 0
A3
Text GLabel 6200 1750 0    50   Input ~ 0
A4
Text GLabel 6200 1850 0    50   Input ~ 0
A5
Text GLabel 6200 1950 0    50   Input ~ 0
A6
Text GLabel 6200 2050 0    50   Input ~ 0
A7
Text GLabel 6200 2150 0    50   Input ~ 0
A8
Text GLabel 6200 2250 0    50   Input ~ 0
A9
Text GLabel 6200 2350 0    50   Input ~ 0
A10
Text GLabel 6200 2450 0    50   Input ~ 0
A11
Text GLabel 6200 2550 0    50   Input ~ 0
A12
Text GLabel 6200 2650 0    50   Input ~ 0
A13
Text GLabel 6200 2750 0    50   Input ~ 0
A14
Text GLabel 2800 2750 0    50   Input ~ 0
A15
Text GLabel 2800 2850 0    50   Input ~ 0
A16
Text GLabel 2800 2950 0    50   Input ~ 0
A17
Wire Wire Line
	6200 1350 6250 1350
Wire Wire Line
	6200 1450 6250 1450
Wire Wire Line
	6200 1550 6250 1550
Wire Wire Line
	6200 1650 6250 1650
Wire Wire Line
	6200 1750 6250 1750
Wire Wire Line
	6200 1850 6250 1850
Wire Wire Line
	6200 1950 6250 1950
Wire Wire Line
	6200 2050 6250 2050
Wire Wire Line
	6200 2150 6250 2150
Wire Wire Line
	6200 2250 6250 2250
Wire Wire Line
	6200 2350 6250 2350
Wire Wire Line
	6200 2450 6250 2450
Wire Wire Line
	6200 2550 6250 2550
Wire Wire Line
	6200 2650 6250 2650
Wire Wire Line
	6200 2750 6250 2750
Wire Wire Line
	6200 2950 6250 2950
Wire Wire Line
	6200 3050 6250 3050
Wire Wire Line
	6200 3150 6250 3150
Wire Wire Line
	7100 1350 7050 1350
Wire Wire Line
	7050 1450 7100 1450
Wire Wire Line
	7100 1550 7050 1550
Wire Wire Line
	7100 1650 7050 1650
Wire Wire Line
	7100 1750 7050 1750
Wire Wire Line
	7100 1850 7050 1850
Wire Wire Line
	7100 1950 7050 1950
Wire Wire Line
	7100 2050 7050 2050
Text GLabel 4100 1250 2    50   Input ~ 0
D0
Text GLabel 4100 1350 2    50   Input ~ 0
D1
Text GLabel 4100 1450 2    50   Input ~ 0
D2
Text GLabel 4100 1550 2    50   Input ~ 0
D3
Text GLabel 4100 1650 2    50   Input ~ 0
D4
Text GLabel 4100 1750 2    50   Input ~ 0
D5
Text GLabel 4100 1850 2    50   Input ~ 0
D6
Text GLabel 4100 1950 2    50   Input ~ 0
D7
Wire Wire Line
	4100 1250 4050 1250
Wire Wire Line
	4050 1350 4100 1350
Wire Wire Line
	4100 1450 4050 1450
Wire Wire Line
	4100 1550 4050 1550
Wire Wire Line
	4100 1650 4050 1650
Wire Wire Line
	4100 1750 4050 1750
Wire Wire Line
	4100 1850 4050 1850
Wire Wire Line
	4100 1950 4050 1950
Text GLabel 6200 5550 0    50   Input ~ 0
D0
Text GLabel 6200 5650 0    50   Input ~ 0
D1
Text GLabel 6200 5750 0    50   Input ~ 0
D2
Text GLabel 7050 5850 2    50   Input ~ 0
D3
Text GLabel 7050 5750 2    50   Input ~ 0
D4
Text GLabel 7050 5650 2    50   Input ~ 0
D5
Text GLabel 7050 5550 2    50   Input ~ 0
D6
Text GLabel 7050 5450 2    50   Input ~ 0
D7
Wire Wire Line
	6200 5550 6250 5550
Wire Wire Line
	6250 5650 6200 5650
Wire Wire Line
	6200 5750 6250 5750
Wire Wire Line
	7050 5850 7000 5850
Wire Wire Line
	7050 5750 7000 5750
Wire Wire Line
	7050 5650 7000 5650
Wire Wire Line
	7050 5550 7000 5550
Wire Wire Line
	7050 5450 7000 5450
Text GLabel 2800 3050 0    50   Input ~ 0
A18
Text GLabel 2800 1250 0    50   Input ~ 0
A0
Text GLabel 2800 1350 0    50   Input ~ 0
A1
Text GLabel 2800 1450 0    50   Input ~ 0
A2
Text GLabel 2800 1550 0    50   Input ~ 0
A3
Text GLabel 2800 1650 0    50   Input ~ 0
A4
Text GLabel 2800 1750 0    50   Input ~ 0
A5
Text GLabel 2800 1850 0    50   Input ~ 0
A6
Text GLabel 2800 1950 0    50   Input ~ 0
A7
Text GLabel 2800 2050 0    50   Input ~ 0
A8
Text GLabel 2800 2150 0    50   Input ~ 0
A9
Text GLabel 2800 2250 0    50   Input ~ 0
A10
Text GLabel 2800 2350 0    50   Input ~ 0
A11
Text GLabel 2800 2450 0    50   Input ~ 0
A12
Text GLabel 2800 2550 0    50   Input ~ 0
A13
Text GLabel 2800 2650 0    50   Input ~ 0
A14
Wire Wire Line
	2800 1250 2850 1250
Wire Wire Line
	2800 1350 2850 1350
Wire Wire Line
	2800 1450 2850 1450
Wire Wire Line
	2800 1550 2850 1550
Wire Wire Line
	2800 1650 2850 1650
Wire Wire Line
	2800 1750 2850 1750
Wire Wire Line
	2800 1850 2850 1850
Wire Wire Line
	2800 1950 2850 1950
Wire Wire Line
	2800 2050 2850 2050
Wire Wire Line
	2800 2150 2850 2150
Wire Wire Line
	2800 2250 2850 2250
Wire Wire Line
	2800 2350 2850 2350
Wire Wire Line
	2800 2450 2850 2450
Wire Wire Line
	2800 2550 2850 2550
Wire Wire Line
	2800 2650 2850 2650
Text GLabel 2800 3250 0    50   Input ~ 0
~WE
Text GLabel 2800 3550 0    50   Input ~ 0
~OE
Text GLabel 2800 3450 0    50   Input ~ 0
~CE
Wire Wire Line
	2800 3250 2850 3250
Wire Wire Line
	2800 3550 2850 3550
Wire Wire Line
	2800 3450 2850 3450
Wire Wire Line
	2800 2750 2850 2750
Wire Wire Line
	2800 2850 2850 2850
Wire Wire Line
	2800 2950 2850 2950
Wire Wire Line
	2800 3050 2850 3050
Text GLabel 2250 4000 0    50   Input ~ 0
A15
Text GLabel 2250 4100 0    50   Input ~ 0
A16
Text GLabel 2250 4200 0    50   Input ~ 0
A17
Text GLabel 2250 4300 0    50   Input ~ 0
A18
Text GLabel 6200 5450 0    50   Input ~ 0
A0
Text GLabel 6200 5350 0    50   Input ~ 0
A1
Text GLabel 6200 5250 0    50   Input ~ 0
A2
Text GLabel 6200 5150 0    50   Input ~ 0
A3
Text GLabel 6200 5050 0    50   Input ~ 0
A4
Text GLabel 6200 4950 0    50   Input ~ 0
A5
Text GLabel 6200 4850 0    50   Input ~ 0
A6
Text GLabel 6200 4750 0    50   Input ~ 0
A7
Text GLabel 7050 4850 2    50   Input ~ 0
A8
Text GLabel 7050 4950 2    50   Input ~ 0
A9
Text GLabel 7050 5250 2    50   Input ~ 0
A10
Text GLabel 7050 5050 2    50   Input ~ 0
A11
Text GLabel 6200 4650 0    50   Input ~ 0
A12
Text GLabel 7050 4750 2    50   Input ~ 0
A13
Text GLabel 6200 4550 0    50   Input ~ 0
A14
Wire Wire Line
	6200 5450 6250 5450
Wire Wire Line
	6200 5350 6250 5350
Wire Wire Line
	6200 5250 6250 5250
Wire Wire Line
	6200 5150 6250 5150
Wire Wire Line
	6200 5050 6250 5050
Wire Wire Line
	6200 4950 6250 4950
Wire Wire Line
	6200 4850 6250 4850
Wire Wire Line
	6200 4750 6250 4750
Wire Wire Line
	7050 4850 7000 4850
Wire Wire Line
	7050 4950 7000 4950
Wire Wire Line
	7050 5250 7000 5250
Wire Wire Line
	7050 5050 7000 5050
Wire Wire Line
	6200 4650 6250 4650
Wire Wire Line
	7050 4750 7000 4750
Wire Wire Line
	6200 4550 6250 4550
Text GLabel 7050 4650 2    50   Input ~ 0
~WE
Text GLabel 7050 5150 2    50   Input ~ 0
~OE
Text GLabel 7050 5350 2    50   Input ~ 0
~CE
Wire Wire Line
	7050 4650 7000 4650
Wire Wire Line
	7050 5150 7000 5150
Wire Wire Line
	7050 5350 7000 5350
Wire Wire Line
	2250 4000 2300 4000
Wire Wire Line
	2250 4100 2300 4100
Wire Wire Line
	2250 4200 2300 4200
Wire Wire Line
	2250 4300 2300 4300
$Comp
L power:GND #PWR0101
U 1 1 61B30053
P 6650 3400
F 0 "#PWR0101" H 6650 3150 50  0001 C CNN
F 1 "GND" H 6655 3227 50  0000 C CNN
F 2 "" H 6650 3400 50  0001 C CNN
F 3 "" H 6650 3400 50  0001 C CNN
	1    6650 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 61B302F1
P 3450 3700
F 0 "#PWR0102" H 3450 3450 50  0001 C CNN
F 1 "GND" H 3455 3527 50  0000 C CNN
F 2 "" H 3450 3700 50  0001 C CNN
F 3 "" H 3450 3700 50  0001 C CNN
	1    3450 3700
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0103
U 1 1 61B30ADD
P 6650 1100
F 0 "#PWR0103" H 6650 950 50  0001 C CNN
F 1 "VCC" H 6665 1273 50  0000 C CNN
F 2 "" H 6650 1100 50  0001 C CNN
F 3 "" H 6650 1100 50  0001 C CNN
	1    6650 1100
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0104
U 1 1 61B31282
P 3450 1100
F 0 "#PWR0104" H 3450 950 50  0001 C CNN
F 1 "VCC" H 3465 1273 50  0000 C CNN
F 2 "" H 3450 1100 50  0001 C CNN
F 3 "" H 3450 1100 50  0001 C CNN
	1    3450 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 1100 6650 1150
Wire Wire Line
	3450 1100 3450 1150
Wire Wire Line
	3450 3650 3450 3700
Wire Wire Line
	6650 3350 6650 3400
$Comp
L power:GND #PWR0105
U 1 1 61B42F42
P 6150 5900
F 0 "#PWR0105" H 6150 5650 50  0001 C CNN
F 1 "GND" H 6155 5727 50  0000 C CNN
F 2 "" H 6150 5900 50  0001 C CNN
F 3 "" H 6150 5900 50  0001 C CNN
	1    6150 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 61B4351D
P 3050 5850
F 0 "#PWR0106" H 3050 5600 50  0001 C CNN
F 1 "GND" H 3055 5677 50  0000 C CNN
F 2 "" H 3050 5850 50  0001 C CNN
F 3 "" H 3050 5850 50  0001 C CNN
	1    3050 5850
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0107
U 1 1 61B441C1
P 7100 4500
F 0 "#PWR0107" H 7100 4350 50  0001 C CNN
F 1 "VCC" H 7115 4673 50  0000 C CNN
F 2 "" H 7100 4500 50  0001 C CNN
F 3 "" H 7100 4500 50  0001 C CNN
	1    7100 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 4500 7100 4550
Wire Wire Line
	7100 4550 7000 4550
Wire Wire Line
	6250 5850 6150 5850
Wire Wire Line
	6150 5850 6150 5900
Wire Wire Line
	3150 5800 3050 5800
Wire Wire Line
	3050 5800 3050 5850
Text GLabel 3950 5800 2    50   Input ~ 0
D3
Text GLabel 3950 5700 2    50   Input ~ 0
D4
Text GLabel 3950 5600 2    50   Input ~ 0
D5
Text GLabel 3950 5500 2    50   Input ~ 0
D6
Text GLabel 3950 5400 2    50   Input ~ 0
D7
Wire Wire Line
	3950 5800 3900 5800
Wire Wire Line
	3950 5700 3900 5700
Wire Wire Line
	3950 5600 3900 5600
Wire Wire Line
	3950 5500 3900 5500
Wire Wire Line
	3950 5400 3900 5400
Text GLabel 3950 4800 2    50   Input ~ 0
A8
Text GLabel 3950 4900 2    50   Input ~ 0
A9
Text GLabel 3950 5200 2    50   Input ~ 0
A10
Text GLabel 3950 5000 2    50   Input ~ 0
A11
Text GLabel 3950 4700 2    50   Input ~ 0
A13
Wire Wire Line
	3950 4800 3900 4800
Wire Wire Line
	3950 4900 3900 4900
Wire Wire Line
	3950 5200 3900 5200
Wire Wire Line
	3950 5000 3900 5000
Wire Wire Line
	3950 4700 3900 4700
Text GLabel 3950 4600 2    50   Input ~ 0
~WE
Text GLabel 3950 5100 2    50   Input ~ 0
~OE
Text GLabel 3950 5300 2    50   Input ~ 0
~CE
Wire Wire Line
	3950 4600 3900 4600
Wire Wire Line
	3950 5100 3900 5100
Wire Wire Line
	3950 5300 3900 5300
Text GLabel 3100 5500 0    50   Input ~ 0
D0
Text GLabel 3100 5600 0    50   Input ~ 0
D1
Text GLabel 3100 5700 0    50   Input ~ 0
D2
Wire Wire Line
	3100 5500 3150 5500
Wire Wire Line
	3150 5600 3100 5600
Wire Wire Line
	3100 5700 3150 5700
Text GLabel 3100 5400 0    50   Input ~ 0
A0
Text GLabel 3100 5300 0    50   Input ~ 0
A1
Text GLabel 3100 5200 0    50   Input ~ 0
A2
Text GLabel 3100 5100 0    50   Input ~ 0
A3
Text GLabel 3100 5000 0    50   Input ~ 0
A4
Text GLabel 3100 4900 0    50   Input ~ 0
A5
Text GLabel 3100 4800 0    50   Input ~ 0
A6
Text GLabel 3100 4700 0    50   Input ~ 0
A7
Text GLabel 3100 4600 0    50   Input ~ 0
A12
Text GLabel 3100 4500 0    50   Input ~ 0
A14
Wire Wire Line
	3100 5400 3150 5400
Wire Wire Line
	3100 5300 3150 5300
Wire Wire Line
	3100 5200 3150 5200
Wire Wire Line
	3100 5100 3150 5100
Wire Wire Line
	3100 5000 3150 5000
Wire Wire Line
	3100 4900 3150 4900
Wire Wire Line
	3100 4800 3150 4800
Wire Wire Line
	3100 4700 3150 4700
Wire Wire Line
	3100 4600 3150 4600
Wire Wire Line
	3100 4500 3150 4500
$Comp
L power:VCC #PWR0108
U 1 1 61BBEB12
P 4000 4450
F 0 "#PWR0108" H 4000 4300 50  0001 C CNN
F 1 "VCC" H 4015 4623 50  0000 C CNN
F 2 "" H 4000 4450 50  0001 C CNN
F 3 "" H 4000 4450 50  0001 C CNN
	1    4000 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 4450 4000 4500
Wire Wire Line
	4000 4500 3900 4500
$Comp
L power:GND #PWR0109
U 1 1 61BD7779
P 2300 4350
F 0 "#PWR0109" H 2300 4100 50  0001 C CNN
F 1 "GND" H 2305 4177 50  0000 C CNN
F 2 "" H 2300 4350 50  0001 C CNN
F 3 "" H 2300 4350 50  0001 C CNN
	1    2300 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 4000 2300 4100
Connection ~ 2300 4100
Wire Wire Line
	2300 4100 2300 4200
Connection ~ 2300 4200
Wire Wire Line
	2300 4200 2300 4300
Connection ~ 2300 4300
Wire Wire Line
	2300 4300 2300 4350
$EndSCHEMATC
