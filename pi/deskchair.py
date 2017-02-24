
#!/usr/bin/python

from time import sleep
import serial
import sys



def get_values():
	serial_connection.write('1:') # Convert the decimal number to ASCII then send it to the Arduino
	return_value = ser.readline() ;
	return return_value

def set_values(val1, val2, val3, val4):
	serial_connection.write('2:') # Convert the decimal number to ASCII then send it to the Arduino
	sleep(.1)
	serial_connection.write(str(val1)+':'+str(val2)+':'+str(val3)+':'+str(val4)+':')


serial_port ='/dev/ttyUSB0' 
baud_rate = 9600

serial_connection = serial.Serial(serial_port, baud_rate) # Establish the connection on a specific port
# serial_connection.write('2;100:101:102:103:') # Convert the decimal number to ASCII then send it to the Arduino

# set_values(1,2,3,4)
get_values()