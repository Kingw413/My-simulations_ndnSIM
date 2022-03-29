def rate_compute(filename):
    import pandas as pd
    f = open("./data/log/multicast/"+filename, "r")
    rate_file = []
    rate = {i+1:0 for i in range(20)}
    line = f.readline()
    while line:
        if "responding with Data" in line:
            time = line.split()[0].split('s')[0]
            rate_file.append(int(float(time)))
        line = f.readline()
    f.close()
    packet_counts = sorted(pd.value_counts(rate_file))

    for i in range(20):
        rate[i+1] = sum(packet_counts[:(i+1)])*8*1024/(i+1)/1000
    result_filename = "results/rate/multicast/" + filename.split(".")[0] + "_rate.txt"
    result_file = open(result_filename, "w")
    for key, value in sorted(rate.items()):
        result_file.write(str(key) +' '+str(value)+'\n')
    result_file.close()

import os
file_names = os.listdir('./data/log/multicast')
for filename in file_names:
    rate_compute(filename)