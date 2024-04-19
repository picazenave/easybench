import serial
import struct
import queue
import threading
import time


# Initialisation du port série
ser = serial.Serial('/dev/ttyACM0', 460800)

# # Création d'un exemple de données
words = [0xFFFFFFFF - i for i in range(32)]
timestamp = 23
nb_words = 32
channel_number = 0

words2 = [0xAAAAAAAA - i for i in range(32)]
timestamp2 = 230
nb_words2 = 32
channel_number2 = 0

# # Préparation du struct à envoyer
data_format = '<35I'
packed_data = struct.pack(data_format, *words, timestamp, nb_words, channel_number)
packed_data2 = struct.pack(data_format, *words2, timestamp2, nb_words2, channel_number2)

# Fonction pour lire la queueRx et récupérer les données série
def read_from_serial():
    while True:
        if ser.in_waiting > 0:
        # if not queueRx.empty():
            raw_data = queueRx.get()
            unpacked_data = struct.unpack(data_format, raw_data)
            words = unpacked_data[:64] 
            timestamp = unpacked_data[64]
            nb_words = unpacked_data[65]
            print(f"timestamp : {timestamp}")
            print(f"nb_words: {nb_words}")
            print("words: ", words)


# Fonction pour écrire les données de queueTx vers le port série
def write_to_serial():
    while True:
        if not queueTx.empty():
            data = queueTx.get()    #Récupérer la donnée stockée dans la queue (pas sur qu'il faille l'utiliser)
            ser.write(packed_data)

# Création des threads pour la lecture et l'écriture
# read_thread = threading.Thread(target=read_from_serial)
# write_thread = threading.Thread(target=write_to_serial)
# read_thread.start()
# write_thread.start()
print("timestamp")
print(type(len(words2)))
print("timestamp")
print(type(words2[0]))
print(hex(words2[0]))
print(type(timestamp2))
print(len(words2))
# z = 0
# ser.write(packed_data)                         # Emission d'un struct
# ser.write(packed_data2)                         # Emission d'un struct
# while(1):
#     if ser.in_waiting > 0:
#         data = ser.read(4*32+4*3)
#         data_reversed = data[::-1]            # Passage en little endian
#         datahex = data_reversed.hex().upper()
#         datahex3 = [datahex[i:i+8] for i in range(0, len(datahex), 8)][::-1]
#         words = datahex3[:32]
#         for i in words:
#             i = int(i,16)
#         timestamp = int(datahex3[32],16)
#         nb_words = int(datahex3[33],16)
#         channel_number = int(datahex3[34],16)
#         for mot in words:
#             print('0x' + mot)
#         print(timestamp)
#         print(nb_words)
#         print(channel_number)
#         # print('0x' + data_reversed.hex().upper())
#         # print('0x' + data.hex().upper())
#         z = z+1
#         print(f"l'iteration est {z}")
# #         data2 = data_reversed.hex()           # Conversion en hexa
#         # print(type(data2))                    # Type : string
# #         print(data2)
#         # int_hex_data = int(data2,16)          # Conversion en int
#         # print(type(int_hex_data))             # Type : int
#         # print(f"le int data {int_hex_data}")