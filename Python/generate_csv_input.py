import csv
import random
import statistics
import numpy as np
import matplotlib.pyplot as plt
from collections import Counter
import os
from operator import itemgetter

first_value_timestamp = 100*1000
int_size_max = 4294967295

def delete_files(path_input_files):
    for fichier in os.listdir(path_input_files):
        chemin_fichier = os.path.join(path_input_files, fichier)
        if os.path.isfile(chemin_fichier):
            os.remove(chemin_fichier)

def generate_csv_input_360(csv_file_input, nb_words):
    list_csv_input = [[0]*3 for i in range(nb_words+1)]
    list_csv_input[0][0] = "Horodatage"
    list_csv_input[0][1] = "Channel"
    list_csv_input[0][2] = "Mot_A429"
    for i in range(1, len(list_csv_input)):
        if(i==1):
            list_csv_input[i][0] = first_value_timestamp
            list_csv_input[i][1] = 0
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
        else:
            list_csv_input[i][0] = list_csv_input[i-1][0]+ 360
            list_csv_input[i][1] = 0
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
    write_csv_file(csv_file_input,list_csv_input)

def generate_csv_input_HS_360(csv_file_input, nb_words, channel):
    list_csv_input = [[0]*3 for i in range(nb_words+1)]
    list_csv_input[0][0] = "Horodatage"
    list_csv_input[0][1] = "Channel"
    list_csv_input[0][2] = "Mot_A429"
    for i in range(1, len(list_csv_input)):
        if(i==1):
            list_csv_input[i][0] = first_value_timestamp
            list_csv_input[i][1] = channel
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
        else:
            list_csv_input[i][0] = list_csv_input[i-1][0]+ 360
            list_csv_input[i][1] = channel
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
    write_csv_file(csv_file_input,list_csv_input)

def generate_csv_input_LS_2880(csv_file_input, nb_words, channel):
    list_csv_input = [[0]*3 for i in range(nb_words+1)]
    list_csv_input[0][0] = "Horodatage"
    list_csv_input[0][1] = "Channel"
    list_csv_input[0][2] = "Mot_A429"
    for i in range(1, len(list_csv_input)):
        if(i==1):
            list_csv_input[i][0] = first_value_timestamp
            list_csv_input[i][1] = channel
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
        else:
            list_csv_input[i][0] = list_csv_input[i-1][0]+ 2880
            list_csv_input[i][1] = channel
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
    write_csv_file(csv_file_input,list_csv_input)

def generate_csv_input_rand(csv_file_input, nb_words):
    list_csv_input = [[0]*3 for i in range(nb_words+1)]
    list_csv_input[0][0] = "Horodatage"
    list_csv_input[0][1] = "Channel"
    list_csv_input[0][2] = "Mot_A429"
    for i in range(1, len(list_csv_input)):
        if(i==1):
            list_csv_input[i][0] = first_value_timestamp
            list_csv_input[i][1] = 0
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
        else:
            list_csv_input[i][0] = list_csv_input[i-1][0]+ random.randrange(360,1000, 1)
            list_csv_input[i][1] = 0
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
    write_csv_file(csv_file_input,list_csv_input)

def generate_csv_input_HS_rand(csv_file_input, nb_words, randmax, channel):
    if(randmax>360):
        list_csv_input = [[0]*3 for i in range(nb_words+1)]
        list_csv_input[0][0] = "Horodatage"
        list_csv_input[0][1] = "Channel"
        list_csv_input[0][2] = "Mot_A429"
        for i in range(1, len(list_csv_input)):
            if(i==1):
                list_csv_input[i][0] = first_value_timestamp
                list_csv_input[i][1] = channel
                list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
            else:
                list_csv_input[i][0] = list_csv_input[i-1][0]+ random.randrange(360,randmax, 1)
                list_csv_input[i][1] = channel
                list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
        write_csv_file(csv_file_input,list_csv_input)
    else:
        print("randmax must be superior than 360 µs (Min time to send a word in HS)")

def generate_csv_input_LS_rand(csv_file_input, nb_words, randmax, channel):
    if(randmax>2880):
        list_csv_input = [[0]*3 for i in range(nb_words+1)]
        list_csv_input[0][0] = "Horodatage"
        list_csv_input[0][1] = "Channel"
        list_csv_input[0][2] = "Mot_A429"
        for i in range(1, len(list_csv_input)):
            if(i==1):
                list_csv_input[i][0] = first_value_timestamp
                list_csv_input[i][1] = channel
                list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
            else:
                list_csv_input[i][0] = list_csv_input[i-1][0]+ random.randrange(2880,randmax, 1)
                list_csv_input[i][1] = channel
                list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
        write_csv_file(csv_file_input,list_csv_input)
    else:
        print("randmax must be superior than 2880 µs (Min time to send a word in LS)")

def generate_csv_input_timestamp(csv_file_input, nb_words, timestamp):
    if(timestamp<360 and timestamp>=0):
        print("Timestamp is less than 360µs. So, deltas timestamp between words are random(up to 1ms max.)")
    list_csv_input = [[0]*3 for i in range(nb_words+1)]
    list_csv_input[0][0] = "Horodatage"
    list_csv_input[0][1] = "Channel"
    list_csv_input[0][2] = "Mot_A429"
    for i in range(1, len(list_csv_input)):
        if(i==1):
            list_csv_input[i][0] = first_value_timestamp
            list_csv_input[i][1] = 0
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
        else:
            if(timestamp<360 and timestamp>=0):
                list_csv_input[i][0] = list_csv_input[i-1][0]+ random.randrange(360,1000, 1)
            else:
                list_csv_input[i][0] = list_csv_input[i-1][0]+ timestamp
            list_csv_input[i][1] = 0
            list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
    write_csv_file(csv_file_input,list_csv_input)

def generate_csv_input_HS_timestamp(csv_file_input, nb_words, timestamp, channel):
    if(timestamp<360 and timestamp>=0):
        print("[WARNING] Timestamp is less than 360 µs. So, deltas timestamp between words are random(up to 1ms max.)")
        generate_csv_input_HS_rand(csv_file_input, nb_words, 1000, channel)
    elif(timestamp>=360):
        list_csv_input = [[0]*3 for i in range(nb_words+1)]
        list_csv_input[0][0] = "Horodatage"
        list_csv_input[0][1] = "Channel"
        list_csv_input[0][2] = "Mot_A429"
        for i in range(1, len(list_csv_input)):
            if(i==1):
                list_csv_input[i][0] = first_value_timestamp
                list_csv_input[i][1] = channel
                list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
            else:
                list_csv_input[i][0] = list_csv_input[i-1][0]+ timestamp
                list_csv_input[i][1] = channel
                list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
        write_csv_file(csv_file_input,list_csv_input)

def generate_csv_input_LS_timestamp(csv_file_input, nb_words, timestamp, channel):
    if(timestamp<2880 and timestamp>=0):
        print("[WARNING] Timestamp is less than 2880 µs. So, deltas timestamp between words are random(up to 10ms max.)")
        generate_csv_input_HS_rand(csv_file_input, nb_words, 10000, channel)
    elif(timestamp>=2880):
        list_csv_input = [[0]*3 for i in range(nb_words+1)]
        list_csv_input[0][0] = "Horodatage"
        list_csv_input[0][1] = "Channel"
        list_csv_input[0][2] = "Mot_A429"
        for i in range(1, len(list_csv_input)):
            if(i==1):
                list_csv_input[i][0] = first_value_timestamp
                list_csv_input[i][1] = channel
                list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
            else:
                list_csv_input[i][0] = list_csv_input[i-1][0]+ timestamp
                list_csv_input[i][1] = channel
                list_csv_input[i][2] = ((hex(random.randrange(0,int_size_max, 1)).upper())[2:]).zfill(8)
    write_csv_file(csv_file_input,list_csv_input)

def generate_csv_input_timestamp2(csv_file_input, nb_words, timestamp, channel, rate_mode):
    if(rate_mode == "HS"):
        generate_csv_input_HS_timestamp(csv_file_input, nb_words, timestamp, channel)
    elif(rate_mode == "LS"):
        generate_csv_input_LS_timestamp(csv_file_input, nb_words, timestamp, channel)
    else:
        print("[ERROR] The rate mode is not 'HS' or 'LS'")

def generate_multi_csv_input(nb_words, timestamp, channel_min, channel_max, rate_mode):
    csv_file_input = [0]*((channel_max-channel_min)+1)
    for i in range(channel_min, channel_max+1):
        csv_file_input[i-channel_min] = 'input_files/input_' + 'TX#' + str(i) + '_' + str(nb_words) + '_' + str(timestamp) + '_' + rate_mode + '.csv'
        generate_csv_input_timestamp2(csv_file_input[i-channel_min], nb_words, timestamp, i, rate_mode)

def fusion_csv_files(path_input_files, path_output_file):
    all_files = [file for file in os.listdir(path_input_files) if(file.startswith("input") and file.endswith(".csv"))]
    fusion_list = []
    for file in range(len(all_files)):
        read_csv_file(path_input_files + '/' + all_files[file],fusion_list)
    for i in range(len(fusion_list)):
        fusion_list[i][0] = int(fusion_list[i][0]) 
    sorted_fusion_list = sorted(fusion_list, key=itemgetter(0,1))
    sorted_fusion_list.insert(0,["Horodatage", "Channel", "Mot_A429"])
    write_csv_file(path_output_file,sorted_fusion_list)

def write_csv_file(csv_file_input, list_csv_input):
    with open(csv_file_input, 'w') as fichier_csv:
        writer = csv.writer(fichier_csv)
        for i in range(len(list_csv_input)):
            writer.writerow(list_csv_input[i])

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

def compare_csv_files(input_csv, output_csv, rate_mode):
    list_input = []
    list_output = []
    list_timestamp_input = []
    delay = [0]
    nb_words_received = [0]
    min_timestamp = [0]
    max_timestamp = [0]
    average_timestamp = [0]
    median_timestamp = [0]
    nb_data_incorrect = [0]
    fig, axs = plt.subplots(2, 2, figsize=(10,7))
    fig.delaxes(axs[0][1])
    fig.delaxes(axs[1][1])
    read_csv_file(input_csv, list_input)
    read_csv_file(output_csv, list_output)
    get_delay(list_input, delay)
    compare_nb_words2(list_input, list_output, nb_words_received)
    compare_data_words2(list_input, list_output, nb_data_incorrect)
    list_timestamp = [0]*len(list_input)
    compare_timestamp(list_input, list_output, list_timestamp)
    get_min_deviation(list_timestamp, min_timestamp)
    get_max_deviation(list_timestamp, max_timestamp)
    get_average_deviation(list_timestamp, average_timestamp)
    get_median_deviation(list_timestamp, median_timestamp)
    plot_time_graph(list_timestamp_input, list_input, list_timestamp, axs[0][0])
    plot_density_graph(list_timestamp, axs[1][0])
    texte = "Rate : " + rate_mode + " \n\n\n"
    texte += "Delay means : " + str(delay[0]) + " µs\n\n\n"
    texte += "Number of words received : " + str(nb_words_received[0]) + " \n\n\n"
    texte += "Number of inconsistents datas : " + str(nb_data_incorrect[0]) + " \n\n\n"
    texte += "Min deviation : " + str(min_timestamp[0]) + " µs\n\n\n"
    texte += "Max deviation : " + str(max_timestamp[0]) + " µs\n\n\n" 
    texte += "Average deviation : " + str(average_timestamp[0])[:5] + " µs\n\n\n" 
    texte += "Median deviation : " + str(median_timestamp[0]) + " µs\n\n\n"
    # print(texte)
    fig.text(0.6,0.1,texte,fontsize=12)
    plt.suptitle('Deviations performances RX#'+str(list_input[0][1]), fontsize=16)
    plt.tight_layout()
    plt.show()

def compare_timestamp(list_input, list_output, list_timestamp):
    for i in range(len(list_input)):
        list_timestamp[i] = int(list_output[i][0])-(int(list_input[i][0])+360)

def compare_data_words(list_input, list_output):
    for i in range(len(list_input)):
        if(list_input[i][2]!=list_output[i][2]):
            print("[Error] At word", i, "the data is not the same.")
            print("Input word :", list_input[i][2])
            print("Output word :", list_output[i][2])

def compare_data_words2(list_input, list_output, nb_data_incorrect):
    for i in range(len(list_input)):
        if(list_input[i][2]!=list_output[i][2]):
            nb_data_incorrect[0] += 1
            print("[Error] At word", i+1, "the data is not the same.")
            print("Input word :", list_input[i][2])
            print("Output word :", list_output[i][2])

def compare_nb_words(list_input, list_output):
    if(len(list_input) == len(list_output)):
        print("Number of words is right : ",len(list_input), " words")
    else:
        print("[Error] Number of words is wrong. Input : ", len(list_input), " words and output : ", len(list_output))
        print("Input : ", len(list_input))
        print("Output : ", len(list_output))

def compare_nb_words2(list_input, list_output, nb_words_received):
    if(len(list_input) == len(list_output)):
        nb_words_received[0]=len(list_output)
        # print("Number of words is right : ",len(list_input), " words")
    else:
        nb_words_received[0]=len(list_output)
        # print("[Error] Number of words is wrong. Input : ", len(list_input), " words and output : ", len(list_output))
        # 
        # print("Input : ", len(list_input))
        # print("Output : ", len(list_output))

def get_delay(list_input, delay):
    list_delay = [[]]*(len(list_input)-1)
    for i in range(1, len(list_input)):
        list_delay[i-1] = int(list_input[i][0])-int(list_input[i-1][0])
    if(all(element_delay ==list_delay[0] for element_delay in list_delay)):
        delay[0] = list_delay[0]
    else :
        delay = "random"    
    
def get_min_deviation(list_timestamp, min_timestamp):
    min_timestamp[0] = min(list_timestamp)

def get_max_deviation(list_timestamp, max_timestamp):
    max_timestamp[0] = max(list_timestamp)

def get_average_deviation(list_timestamp, average_timestamp):
    average_timestamp[0] = sum(list_timestamp)/len(list_timestamp)

def get_median_deviation(list_timestamp, median_timestamp):
    median_timestamp[0] = statistics.median(list_timestamp)

def plot_time_graph(list_timestamp_input, list_input, list_timestamp, ax):
    for i in range(len(list_input)):
        list_timestamp_input.append((int(list_input[i][0])+360)/1000)
    ax.plot(list_timestamp_input, list_timestamp,'x-',linewidth=1)
    ax.set_title('Deviations in time')
    ax.set_xlabel('Time (in ms)')
    ax.set_ylabel('Deviation (in µs)')
    ax.grid()

def plot_density_graph(list_timestamp, ax):
    occurences = Counter(list_timestamp)
    sorted_deviations = sorted(occurences.keys())
    sorted_occurences = [occurences[i] for i in sorted_deviations]
    ax.bar(sorted_deviations, sorted_occurences, width=1.0, edgecolor="black")
    xlim = 0
    if(min(sorted_deviations)<0):
        if(min(sorted_deviations)*(-1)<max(sorted_deviations)):
            xlim = max(sorted_deviations)
        else:
            xlim = min(sorted_deviations)*(-1)
    else:
        if(min(sorted_deviations)<max(sorted_deviations)):
            xlim = max(sorted_deviations)
        else:
            xlim = min(sorted_deviations)        
    ax.set_xlim(-xlim-0.1*xlim, xlim+0.1*xlim)
    ax.set_ylim(min(sorted_occurences)-1, max(sorted_occurences)+1)
    ax.set_title('Deviations density graph')
    ax.set_xlabel('Deviation (in µs)')
    ax.set_ylabel('Occurences')
    ax.grid()

def sim_timestamp_A429(input_csv, output_csv):
    list_input = []
    read_csv_file(input_csv, list_input)
    list_csv_input = [[0]*3 for i in range(len(list_input)+1)]
    list_csv_input[0][0] = "Horodatage"
    list_csv_input[0][1] = "Channel"
    list_csv_input[0][2] = "Mot_A429"
    for i in range(len(list_input)):
        list_csv_input[i+1][0] = str(int(list_input[i][0])+random.randint(350,370))
        list_csv_input[i+1][1] = list_input[i][1]
        list_csv_input[i+1][2] = list_input[i][2]
    write_csv_file(output_csv,list_csv_input)


if(0):
    delete_files('input_files')
    generate_multi_csv_input(100, 361, 0, 3, 'HS')

    fusion_csv_files('input_files','fusion_input_file/input_file.csv')
else :
    # compare_csv_files('generate_timestamp.csv', 'timestamp_output.csv', 'HS')
    compare_csv_files('fusion_input_file/input_file.csv', 'fusion_output_file/output_file.csv', 'HS')
