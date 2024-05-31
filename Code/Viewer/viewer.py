from cProfile import label
from math import pi
import serial

import matplotlib.pyplot as plt 
from collections import deque 
import struct
import threading

plt.ion()

plt.pause(0.1) 

baudrate = 40000
com_port = "COM11"
visible_meas_count = 100
packet_data_count = 9

packet_length = packet_data_count*4 + 4
period = 1/(baudrate/packet_length)


def read_port(period, packet_data_count, time_points, data_points_arr):
    t = 0
    ser = serial.Serial(com_port, baudrate)
    
    while True:
        data = ser.read_until(b'\n\n\n\n')
        if len(data) == packet_length:
            data = struct.unpack("<" + str(packet_data_count-1) + "f" + "L" + "4b", data)
            
            time_points.append(t)
            
            for i in range(packet_data_count):
                data_points_arr[i].append(data[i])
                    
        t += period

time_points = deque(maxlen=visible_meas_count) 

data_points_arr = [deque(maxlen=visible_meas_count) for _ in range(packet_data_count)]

fig, ((cur, vol), (w, theta)) = plt.subplots(2, 2, sharex=True) 
lines = [cur.plot([], label=r"$i_W$")[0], cur.plot([], label=r"$i_V$")[0], cur.plot([], label=r"$i_U$")[0], vol.plot([], label=r"$v_W$")[0], vol.plot([], label=r"$v_V$")[0], vol.plot([], label=r"$v_U$")[0], w.plot([])[0], theta.plot([])[0], theta.plot([])[0]]

cur.set_xlim(0, period*visible_meas_count) 
cur.set_ylim(-5, 5)
cur.legend()

vol.set_ylim(-12, 12)
vol.legend()

w.set_ylim(0, 700)
theta.set_ylim(0, 100000)

t1 = threading.Thread(target=read_port, args=(period, packet_data_count, time_points, data_points_arr))
t1.start()

while True:
    xlim = cur.get_xlim()
    if len(time_points) > 0 and time_points[-1] > xlim[1]:
        cur.set_xlim(time_points[0], time_points[-1])
    for i in range(packet_data_count):
        if i != packet_data_count - 2:
            lines[i].set_data(time_points, data_points_arr[i])
    plt.pause(0.1) 
    
    


