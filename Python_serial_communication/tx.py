import serial
#working code	https://techtutorialsx.com/2017/12/02/esp32-esp8266-arduino-serial-communication-with-python/
ser = serial.Serial()
ser.baudrate = 115200
ser.port = '/dev/ttyUSB0'
ser.open()
 
#values = bytearray([4, 9, 62, 144, 56, 30, 147, 3, 210, 89, 111, 78, 184, 151, 17, 129])
#ser.write(values) 
#total = 0
#while total < len(values):
#   print(ord(ser.read(1)))
#    total=total+1
#ser.close()

#continous string of data
while 1:
    ser.write(b"hello World") #continous sending string to usb to ttl
#ser.close()
