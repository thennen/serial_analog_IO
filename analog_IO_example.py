'''
A short python example how to use my serial_analog_IO arduino program.

Arduino is using some library called CmdMessenger to handle serial commands.
I think it's a little too complex for this application, but it's the newest
hippest library so whatever.

There's a corresponding python library called pyCmdMessenger that is supposed
to take care of this, but it was not working properly when I tried it.  Who
wants to install more libraries anyway?

These are the commands as defined in the arduino sketch.
The beginning of every message addresses each command by number
'AO':0,
'AI':1,
'Reply':2

The full command syntax is like
cmdnum, arg1, arg2, arg3;
'''

import serial
s = serial.Serial(port='COM8', baudrate=9600)

def analogOut(channel, voltage):
    # Arduino will take the set voltage in bits.
    vmax = 5.
    numbits = 8
    # Find the closest value that can be output.
    vstep = vmax / (2**numbits - 1)
    value = round(voltage / vstep)
    cmd_str = '0,{},{};'.format(channel, value).encode()
    s.write(cmd_str)
    actualvoltage = vstep * value
    print('Sent command to output {} volts on pin {}'.format(actualvoltage, channel))


def analogIn(channel):
    vmax = 5
    numbits = 10
    vstep = vmax / (2**numbits - 1)
    cmd_str = '1,{};'.format(channel).encode()
    s.write(cmd_str)
    reply = s.readline().decode()
    adc_value = int(reply.split(',')[-1].strip().strip(';'))
    voltage = adc_value * vstep
    print('Sent command to read analog input on pin {}'.format(channel))
    return voltage

if __name__ == '__main__':
    analogOut(10, 2.6)
    volts = analogIn(0)
    print(volts)
    s.close()
