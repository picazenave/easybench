import csv
import time
import struct
import serial
import random
import numpy
import matplotlib.pyplot as plt
import threading

# data_format_OUTPUT = '<IBI'
data_format_OUTPUT = '<3I'

import sys
import os
import psutil

os_used = sys.platform
process = psutil.Process(os.getpid())  # Set highest priority for the python script for the CPU
if os_used == "win32":  # Windows (either 32-bit or 64-bit)
    process.nice(psutil.REALTIME_PRIORITY_CLASS)
elif os_used == "linux":  # linux
    process.nice(psutil.IOPRIO_CLASS_RT)
else:  # MAC OS X or other
    process.nice(20)  


def read_csv_file(csv_file_input, list_csv_input):
    lignes_csv = []
    with open(csv_file_input, mode='r') as fichier_csv:
        lecteur_csv = csv.reader(fichier_csv)
        next(lecteur_csv)
        for ligne in lecteur_csv:
            lignes_csv.append(ligne)
        for sousliste in lignes_csv:
            timestamp, channel, *rest = sousliste
            for words in rest:
                list_csv_input.append([timestamp, channel, words])

def convert_data_types(list_csv_input):
    for i in range(len(list_csv_input)):
        list_csv_input[i][0] = int(list_csv_input[i][0])
        list_csv_input[i][1] = int(list_csv_input[i][1])
        list_csv_input[i][2] = int(list_csv_input[i][2],16)
    # print(list_csv_input)

def fix_timestamp(list_csv_input):
    for i in range(1, len(list_csv_input)):
        if(list_csv_input[i][0]<=list_csv_input[i-1][0] + 360):
            list_csv_input[i][0] = list_csv_input[i-1][0] + 360
    # print(list_csv_input)


def send_synchro():
    global offset_RPi
    global data_format

    temp=struct.pack(data_format_OUTPUT,0,99,0)
    ser.write(temp)

def send_one_struct(timestamp,channel,data):    #Deprecated (askip)
    packed_data = struct.pack(data_format_OUTPUT, timestamp, channel, data)
    ser.write(packed_data)

def write_csv_file(csv_file_output, list_csv_output):
    with open(csv_file_output, 'w') as fichier_csv:
        writer = csv.writer(fichier_csv)
        for i in range(len(list_csv_output)):
            writer.writerow(list_csv_output[i])

def send_receive(nb_structs,list_csv_output,list_csv_input) :
    global offset_RPi
    global data_format
    
    serial_data_array=[]
    i_rcv=0
    nb_to_read=nb_structs*12
    i_send_end=len(list_csv_input)
    # print("Nombre de mots à envoyer :", i_send_end)
    i_send = 0
    print("BEGIN EMISSION")
    while(i_send<i_send_end):
        #f ((temp_timestamp) >= (offset_init - offset_send_delay + peek_struct_A429.timestamp))
        if(int(time.time()*1e6)>= list_csv_input[i_send][0] + offset_RPi - 10*1000): #send words Xms before timestamp
            # Struct la data
            packed_data = struct.pack(data_format_OUTPUT, list_csv_input[i_send][0], list_csv_input[i_send][1], list_csv_input[i_send][2])
            ser.write(packed_data)
            i_send += 1

        if(ser.in_waiting > 0):
            s=ser.read(ser.in_waiting)
            n_read=len(s)
            serial_data_array.append(s)
            i_rcv=i_rcv+n_read
            

    print("END EMISSION")
    print("done reading ",i_rcv/12 ," structs from serial while sending")

    t0=time.time()
    while(i_rcv<nb_to_read and (time.time()-t0)<1):
        if(ser.in_waiting > 0):
            t0=time.time()
            s=ser.read(ser.in_waiting)
            n_read=len(s)
            serial_data_array.append(s)
            i_rcv=i_rcv+n_read  
    # print("i_rcv=",i_rcv," nb to read",nb_to_read)
    print("done reading ",i_rcv/12 ," structs from serial")
    serial_data_array=b''.join(serial_data_array)
    ##
    ## Decode received packets
    ##
    list_csv_output.append(["Horodatage","Channel","Mot_A429"])
    i=0
    packet_size=12
    while(i<nb_structs):
        index=(i*packet_size)
        data = serial_data_array[index:(index+packet_size)]
        #print(data)
        timestamp=int.from_bytes(data[0:4],'little',signed=False)
        channel=int.from_bytes(data[4:8],'little',signed=False)
        a429_data=int.from_bytes(data[8:12],'little',signed=False)
        list_datahex=[str(timestamp),str(channel),str(hex(a429_data)[2:].upper().zfill(8))]
        list_csv_output.append(list_datahex)
        i=i+1    


# Initialisation des listes CSV des mots à émettre et à recevoir
list_words_input = []
list_words_output = []
# Initialiser la variable de mots reçus
nb_word_read = 0

# Lecture fichier CSV
read_csv_file('fusion_input_file/input_file.csv', list_words_input)
# print(list_words_input)
# Conversion des données pour l'envoi (str->int) 
convert_data_types(list_words_input)
# fix_timestamp(list_words_input)
# print(list_words_input)

# Initialisation du port série
print("Ouverture serial") 
ser = serial.Serial('/dev/ttyACM0', 2200000,timeout=1)

ser.reset_output_buffer()
ser.reset_input_buffer()
# Génration de l'offset RPi pour la gestion du débit d'émission
print("synchro")       
offset_RPi = int(time.time()*1e6)
send_synchro()

# write_to_serial(list_words_input)
# # Réception
# read_nb_structs(10,list_words_output)

send_receive(len(list_words_input),list_words_output,list_words_input)
#Ecrire dans le fichier CSV
write_csv_file('fusion_output_file/output_file.csv', list_words_output)

print("guigui il est bo")