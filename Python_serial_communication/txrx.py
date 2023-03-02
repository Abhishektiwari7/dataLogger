import serial #install serial module

ser = serial.Serial()
ser.baudrate = 115200 #baudrate
ser.port = '/dev/ttyUSB0' #comport, 
ser.open() #open com port
while True:
    ser.write(b"hello World\n") #write string of data to usb to ttl com port
    size = ser.inWaiting() #waiting to get data from serial
    if size:
        #data = ser.read(size) #to read complete string of data coming from serial
        data = ser.read_until(b'\n')   # read until '\n' appears
        print(data) #print recieved data