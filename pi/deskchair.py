
#!/usr/bin/python3

from time import sleep
import serial
import sys



def get_values():
    serial_connection.write('1:') # Convert the decimal number to ASCII then send it to the Arduino
    return_value = serial_connection.readline() 
    print(return_value)
    return return_value

def set_values(val1, val2, val3, val4):
    serial_connection.write('2:') # Convert the decimal number to ASCII then send it to the Arduino
    sleep(.1)
    serial_connection.write(str(val1)+':'+str(val2)+':'+str(val3)+':'+str(val4)+'\n')

def do_the_thing():
    serial_connection.write('42;')


serial_port ='/dev/ttyACM0' 
baud_rate = 9600 ;



serial_connection = serial.Serial(serial_port, baud_rate,timeout = 10) # Establish the connection on a specific port
# serial_connection.write('2;100:101:102:103:') # Convert the decimal number to ASCII then send it to the Arduino
sleep(1)
#serial_connection.isOpen()
#serial_connection.reset_input_buffer()

# set_values(1,2,3,4)
get_values()
#do_the_thing()

serial_connection.close()
