import csv
import time
import struct
import serial
import random
import numpy
import matplotlib.pyplot as plt
import threading

data_format = '<3I'

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

def write_to_serial(list_csv_input):    #Depracated (askip)
    global offset_RPi
    global data_format

    # print("synchro 2")
    temp=struct.pack(data_format,0,99,0)
    ser.write(temp)
    
    # print("debut envoi csv")
    i_end=len(list_csv_input)
    i = 0
    print("BEGIN EMISSION")
    while(i<i_end):
        
        if(list_csv_input[0][0]+offset_RPi<=int(time.time()*1e6)+1*1000): #La vérif est possiblement bonne (A checker)
            # Struct la data
            # print("Mot n°",i) #TITITI
            # print(list_csv_input[i][0])   #TITITI
            # print(list_csv_input[i][1])   #TITITI
            # print(list_csv_input[i][2])   #TITITI
            packed_data = struct.pack(data_format, list_csv_input[i][0], list_csv_input[i][1], list_csv_input[i][2]) # Vérifier si on peut changer l'ordre des variables sans tout casser cote ESP32
            # print(packed_data)    #TITITI
            ser.write(packed_data)
            i += 1    
    print("END EMISSION")

def send_synchro():
    global offset_RPi
    global data_format

    temp=struct.pack(data_format,0,99,0)
    ser.write(temp)

def send_one_struct(timestamp,channel,data):    #Deprecated (askip)
    packed_data = struct.pack(data_format, timestamp, channel, data)
    ser.write(packed_data)

def read_nb_structs(nb_structs,list_csv_output):
    serial_data_array=[]
    i=0
    nb_to_read=nb_structs*4*3

    t0=time.time()
    while(i<nb_to_read and (time.time()-t0)<1):
        if(ser.in_waiting > 0):
            t0=time.time()
            #print('i=',i," in waiting=",ser.in_waiting," nb structs=",i/12 )
            s=ser.read_all()
            n_read=len(s)
            serial_data_array.extend(s)
            i=i+n_read
            
    
    print("done reading ",i/12 ," structs from serial")
    list_csv_output.append(["Horodatage","Channel","Mot_A429"])
    i=0
    while(i<nb_structs):
        index=(i*4*3)
        data = serial_data_array[index:(index+4*3)]
        timestamp=int.from_bytes(data[0:3],'little')
        channel=int.from_bytes(data[4:7],'little')
        a429_data=int.from_bytes(data[8:12],'little')
        list_datahex=[str(timestamp),str(channel),str(hex(a429_data)[2:].upper().zfill(8))]
    #Modifier le timestamp (enlever l'offset de l'ESP32)
        if(i == 0):
            offset_ESP = timestamp
        list_csv_output.append(list_datahex)
        i=i+1

def write_csv_file(csv_file_output, list_csv_output):
    with open(csv_file_output, 'w') as fichier_csv:
        writer = csv.writer(fichier_csv)
        for i in range(len(list_csv_output)):
            writer.writerow(list_csv_output[i])

def send_receive_old(nb_structs,list_csv_output,list_csv_input) :
    global offset_RPi
    global data_format

    print("synchro send_receive")
    temp=struct.pack(data_format,0,99,0)
    ser.write(temp)
    
    serial_data_array=[]
    i_rcv=0
    nb_to_read=nb_structs*4*3
    
    i_send_end=len(list_csv_input)
    i_send = 0
    print("BEGIN EMISSION")
    while(i_send<i_send_end):
        if(list_csv_input[0][0]+offset_RPi<=int(time.time()*1e6)+10*1000):
            # Struct la data
            packed_data = struct.pack(data_format, list_csv_input[i_send][0], list_csv_input[i_send][1], list_csv_input[i_send][2])
            ser.write(packed_data)
            i_send += 1  

        if(ser.in_waiting > 0):
            s=ser.read(ser.in_waiting)
            n_read=len(s)
            serial_data_array.extend(s)
            i_rcv=i_rcv+n_read  

    print("END EMISSION")
    print("done reading ",i_rcv/12 ," structs from serial while sending")

    t0=time.time()
    while(i_rcv<nb_to_read and (time.time()-t0)<2):
        if(ser.in_waiting > 0):
            t0=time.time()
            s=ser.read(ser.in_waiting)
            n_read=len(s)
            serial_data_array.extend(s)
            i_rcv=i_rcv+n_read  

    print("done reading ",i_rcv/12 ," structs from serial")

    ##
    ## Decode received packets
    ##
    list_csv_output.append(["Horodatage","Channel","Mot_A429"])
    i=0
    packet_size=12
    while(i<nb_structs):
        index=(i*packet_size)
        data = serial_data_array[index:(index+packet_size)]
        timestamp=int.from_bytes(data[0:3],'little')
        channel=int.from_bytes(data[4:7],'little')
        a429_data=int.from_bytes(data[8:12],'little')
        list_datahex=[str(timestamp),str(channel),str(hex(a429_data)[2:].upper().zfill(8))]
    #Modifier le timestamp (enlever l'offset de l'ESP32)
        if(i == 0):
            offset_ESP = timestamp
        list_csv_output.append(list_datahex)
        i=i+1

def send_receive(nb_structs,list_csv_output,list_csv_input) :
    global offset_RPi
    global data_format
    
    serial_data_array=[]
    i_rcv=0
    nb_to_read=nb_structs*4*3
    i_send_end=len(list_csv_input)
    # print("Nombre de mots à envoyer :", i_send_end)
    i_send = 0
    print("BEGIN EMISSION")
    delta_sleep=0
    while(i_send<i_send_end):
        #f ((temp_timestamp) >= (offset_init - offset_send_delay + peek_struct_A429.timestamp))
        if(int(time.time()*1e6)>= list_csv_input[i_send][0] + offset_RPi - 100*1000): #send words Xms before timestamp
            # Struct la data
            packed_data = struct.pack(data_format, list_csv_input[i_send][0], list_csv_input[i_send][1], list_csv_input[i_send][2])
            ser.write(packed_data)
            i_send += 1
        else:
            new_time=time.time()
            print("sleep, i_send=",i_send," delta_sleep=",(new_time-delta_sleep)*1000,"ms")
            delta_sleep=new_time
            time.sleep(0.09)

        if(ser.in_waiting > 0):
            s=ser.read(ser.in_waiting)
            n_read=len(s)
            serial_data_array.extend(s)
            i_rcv=i_rcv+n_read

    print("END EMISSION")
    print("done reading ",i_rcv/12 ," structs from serial while sending")

    t0=time.time()
    while(i_rcv<nb_to_read and (time.time()-t0)<1):
        if(ser.in_waiting > 0):
            t0=time.time()
            s=ser.read(ser.in_waiting)
            n_read=len(s)
            serial_data_array.extend(s)
            i_rcv=i_rcv+n_read  
    # print("i_rcv=",i_rcv," nb to read",nb_to_read)
    print("done reading ",i_rcv/12 ," structs from serial")

    ##
    ## Decode received packets
    ##
    list_csv_output.append(["Horodatage","Channel","Mot_A429"])
    i=0
    packet_size=12
    while(i<nb_structs):
        index=(i*packet_size)
        data = serial_data_array[index:(index+packet_size)]
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
fix_timestamp(list_words_input)
# Initialisation du port série
print("Ouverture serial") 
ser = serial.Serial('/dev/ttyACM0', 3000000,timeout=5)

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