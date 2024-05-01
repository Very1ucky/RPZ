import serial

import matplotlib.pyplot as plt 
from collections import deque 
import struct
import threading

plt.ion()

plt.pause(1) 

baudrate = 115200
meas_counts = 5000


def read_port(period, packet_data_count, lines, time_points, data_points_arr):
    t = 0
    ser = serial.Serial("COM11", baudrate)
    
    while True:
        data = ser.read_until(b'\n\n\n\n')
        if len(data) == packet_length:
            data = struct.unpack("<" + str(packet_data_count) + "f" + "4b", data)
            
            time_points.append(t)
            
            for i in range(packet_data_count):
                data_points_arr[i].append(data[i])
                    
        t += period

packet_data_count = 6
packet_length = packet_data_count*4 + 4
period = 1/(baudrate/packet_length)

# Create a fixed-length deque to store the data points 
time_points = deque(maxlen=meas_counts) 

data_points_arr = [deque(maxlen=meas_counts) for _ in range(packet_data_count)]

# Create an empty plot 
fig, (cur, vol) = plt.subplots(2, 1, sharex=True) 
lines = [cur.plot([])[0], cur.plot([])[0], cur.plot([])[0], vol.plot([])[0], vol.plot([])[0], vol.plot([])[0]]

# Set the x-axis and y-axis limits 
cur.set_xlim(0, period*meas_counts) 
cur.set_ylim(-5, 5)

vol.set_ylim(-12, 12) 

t1 = threading.Thread(target=read_port, args=(period, packet_data_count, lines, time_points, data_points_arr))
t1.start()

while True:
    xlim = cur.get_xlim()
    if len(time_points) > 0 and time_points[-1] > xlim[1]:
        cur.set_xlim(time_points[0], time_points[-1])
    for i in range(packet_data_count):
        lines[i].set_data(time_points, data_points_arr[i])
    plt.pause(0.5) 
    
    


