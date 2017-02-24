#
from time import sleep
import serial

serial_port ='/dev/ttyUSB0' 
baud_rate = 9600

serial_connection = serial.Serial(serial_port, baud_rate) # Establish the connection on a specific port
serial_connection.write('2;100:101:102:103:') # Convert the decimal number to ASCII then send it to the Arduino