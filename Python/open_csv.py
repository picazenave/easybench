import csv
import time
import struct
import serial
import random
import numpy
import matplotlib.pyplot as plt

data_format = '<3I'

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

def write_to_serial(list_csv_input):
    global offset_RPi
    global data_format

    temp=struct.pack(data_format,0,99,0)
    ser.write(temp)
    print("debut envoi csv")
    while(len(list_csv_input)>0):
        
        # if ser.in_waiting > 0:
        #     print("bbb")
        #     print("\"")
        #     print(ser.readall())
        #     print("\"")

        if(list_csv_input[0][0]+offset_RPi<=int(time.time()*1e6)+10000): #La vérif est possiblement bonne (A checker)
            # Struct la data
            packed_data = struct.pack(data_format, list_csv_input[0][0], list_csv_input[0][1], list_csv_input[0][2]) # Vérifier si on peut changer l'ordre des variables sans tout casser cote ESP32
            
            # Envoyer la data
            ser.write(packed_data)

            # Supprimer la data de la liste
            poped = list_csv_input.pop(0)
            print(poped)
            # print(list_csv_input)     
    print("END EMISSION")

def send_synchro():
    global offset_RPi
    global data_format

    temp=struct.pack(data_format,0,99,0)
    ser.write(temp)

def send_one_struct(timestamp,channel,data):
    packed_data = struct.pack(data_format, timestamp, channel, data)
    ser.write(packed_data)

def read_one_struct():#return timestamp,channel,data
    data = ser.read(4*3)   
    #La formater dans le bon sens
    data_reversed = data[::-1]            # Passage en little endian
    datahex = data_reversed.hex().upper()
    list_datahex = [datahex[i:i+8] for i in range(0, len(datahex), 8)][::-1]
    list_datahex[0] = int(list_datahex[0],16)
    list_datahex[1] = int(list_datahex[1],16)
    return list_datahex[0],list_datahex[1],int(list_datahex[2],16)

def read_from_serial(list_csv_output,nb_words_read):
    offset_ESP = 0
    # while(True):
    while(nb_words_read != 10):
        #Si message sur le lien serie
        if(ser.in_waiting > 0):

        #Lire la donnée
            data = ser.read(4*3)
        
        #La formater dans le bon sens
            data_reversed = data[::-1]            # Passage en little endian
            datahex = data_reversed.hex().upper()
            list_datahex = [datahex[i:i+8] for i in range(0, len(datahex), 8)][::-1]
            list_datahex[0] = str(int(list_datahex[0],16))
            list_datahex[1] = str(int(list_datahex[1],16))
            
        #Modifier le timestamp (enlever l'offset de l'ESP32)
            if(nb_words_read == 0):
                offset_ESP = int(list_datahex[0])
            # print(list_datahex)
        #La mettre dans une liste pour le csv
            delta_offset_esp=str(int(list_datahex[0])-offset_ESP)

            if(nb_words_read>0):
                timing_mot_avant=int(list_csv_output[-1][0])
                delta_entre_word=str(int(list_datahex[0])-timing_mot_avant)
            else : 
                delta_entre_word="0"

            list_datahex.extend([delta_offset_esp,delta_entre_word])
            list_csv_output.append(list_datahex)
            nb_words_read += 1
            print(list_csv_output)

def write_csv_file(csv_file_output, list_csv_output):
    with open(csv_file_output, 'w') as fichier_csv:
        writer = csv.writer(fichier_csv)
        for i in range(len(list_csv_output)):
            writer.writerow(list_csv_output[i])

# Initialisation des listes CSV des mots à émettre et à recevoir
list_words_input = []
list_words_output = []
# Initialiser la variable de mots reçus
nb_word_read = 0


# Lecture fichier CSV
read_csv_file('fichier_test_csv.csv', list_words_input)   # DONE
# print(list_words_input)
# Conversion des données pour l'envoi (str->int) 
convert_data_types(list_words_input)                      # DONE (les mots sont des int et par contre doivent etre converti en string pour l'affichage en hexa)
fix_timestamp(list_words_input)
# Initialisation du port série
ser = serial.Serial('/dev/ttyACM0', 460800,timeout=1)
# Génration de l'offset RPi pour la gestion du débit d'émission
offset_RPi = int(time.time()*1e6)
send_synchro()

counter=0
endcounter=1000
local_timestamp=0
struct_list=numpy.zeros((endcounter,3),dtype=numpy.int64)
ref_struct_list=numpy.zeros((endcounter,3),dtype=numpy.int64)
while(counter<endcounter):
    local_rand=random.randrange(1000,2000)
    local_timestamp=local_timestamp+local_rand
    ref_struct_list[counter][0]=local_timestamp
    ref_struct_list[counter][1]=0
    ref_struct_list[counter][2]=0xAABBCAFE
    send_one_struct(local_timestamp,0,0xAABBCAFE)
#    time.sleep(local_rand/1e6)
    r_timestamp,r_channel,r_data=read_one_struct()
    struct_list[counter][0]=r_timestamp
    struct_list[counter][1]=r_channel
    struct_list[counter][2]=r_data
    counter=counter+1
#    print("local_timestamp=",local_timestamp,"r_timestamp=",r_timestamp)
#    print("diff timestamp=",r_timestamp-local_timestamp)

timestamp_delta=numpy.zeros(endcounter,dtype=numpy.int64)
timestamp_delta=struct_list[:,0]-ref_struct_list[:,0]-360

x=numpy.arange(0,endcounter)
plt.plot(x,timestamp_delta)
plt.grid()
plt.show()

exit()
##CODE DE GUILLLLLLL LE GRTOS BG DE LA STREET JTM BEBOU
# Emission
print("EMISSION")
write_to_serial(list_words_input)
# Réception
read_from_serial(list_words_output, nb_word_read)
#Ecrire dans le fichier CSV
write_csv_file('output_file.csv', list_words_output)
# Conversion des données pour l'envoi (str->int) (pour les futurs traitement de données)

#Penser a mettre l'émission et la réception en multithreading (travail en parallèle)



print("AAAAA")

