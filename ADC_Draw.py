import serial, time
from matplotlib import pyplot as plt
from drawnow import *
#initialization and open the port

#possible timeout values:
#    1. None: wait forever, block call
#    2. 0: non-blocking mode, return immediately
#    3. x, x is bigger than 0, float allowed, timeout block call

ser = serial.Serial()
#ser.port = "/dev/ttyUSB0"
# no need no for virtual serial port 
ser.port = "COM2"
#ser.port = "/dev/ttyS2"
ser.baudrate = 9600
ser.bytesize = serial.EIGHTBITS #number of bits per bytes
ser.parity = serial.PARITY_NONE #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE #number of stop bits
#ser.timeout = None          #block read
#unblock read to reduce application excution time 
ser.timeout = 1            #non-block read
#ser.timeout = 2              #timeout block read
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 2     #timeout for write
ser.open()
x=0
adc_array=[]
count=0
ser.flushInput()
def ADC_PLOT():
#making upper limit 5v
    plt.ylim(0.0,200.00)
    global count
    count =count+1
    #print(count)
    #plt.axis([0, count, 0, 5])
    plt.plot(adc_array,'r-')
    plt.title('plot ADC Value')
    plt.xlabel('Time in (ms)')
    plt.ylabel('Volt')
    plt.grid(1)
    end=time.time()
    print((end-start)*1000)

#for i in range(0,3):
    #adc_array.append(x)

#Main Function
while(1):
    # starting time
    start = time.time()
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    #Send real 
    while(ser.inWaiting()==0):
        pass
    
    x=ser.read(1)
    y=hex( int.from_bytes(x, 'big', signed=True) )
    z=int(y,16)
    

    # Send fraction part
    while(ser.inWaiting()==0):
        pass
    a=ser.read(1)
    b=hex( int.from_bytes(a, 'big', signed=True) )
    c=int(b,16)

    # get float 
    fraction=float(z)+(float(c)/100)
# remrmber to make graph float if you can do not forget sub plot & sending duty
    if (z<=100):
        adc_array.append(fraction)
        '''
     This instrcution is used for deleting pervous data why???
     for making the curve move not shrink the data 
        '''
    
        #adc_array.pop(0)
        drawnow(ADC_PLOT)
    else:
        print("error")







    '''
    print(z)
    print(c)
    print(fraction)
    print("--------------------------------------")
    '''
