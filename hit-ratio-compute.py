def compute_hit_ratio(file_name):
    hits = 0
    packets = 0
    second = 39
    f = open('./data/cs/multicast/'+file_name, "r")
    f.readline()
    file = f.readlines()
    f_list = [line.strip("\n").split("\t") for line in file]
    node_num = int(len(f_list) / second)
    hit_ratios = {(k+1)*0.5: 0 for k in range(second)}
    for i in range(second):
        for j in range((i+1)*node_num):
            packets += int(f_list[j][3])
            if j % 2 == 0:
                hits += int(f_list[j][3])
        hit_ratios[(i+1)*0.5] = hits/packets
        packets = 0
        hits = 0
    result_file = "results/hit-ratio/multicast/" + file_name.split("_")[0] + "_hit-ratio.txt"
    result = open(result_file, "w")
    for k, v in sorted(hit_ratios.items()):
        result.write(str(k)+' '+str(v) + '\n')
    result.close()


import os
file_names = os.listdir('./data/cs/multicast')
for filename in file_names:
    compute_hit_ratio(filename)
