#!/usr/bin/env python

import serial
import json

# Serial port connection
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

pulses = '0';
liters = 0;

while True:
  line = ser.readline().strip()
  if line:
    print line
    # Remove possible garbage lines
    if line.startswith(pulses):
      print line
      piece = line.split(':')
      if 2==len(piece):
        pulses = int(piece[1].strip())
        print pulses
        liters = float(pulses)/5600
        print liters

