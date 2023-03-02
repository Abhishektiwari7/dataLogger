import serial
ser = serial.Serial()
ser.baudrate = 115200
ser.port = '/dev/ttyUSB0'
ser.open() #open com port
while True:
    size = ser.inWaiting() #waiting to get data from serial
    if size:
        data = ser.read(size)  #to read complete string of data coming from serial
        print(data) #print recieved data