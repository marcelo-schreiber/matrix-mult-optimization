#!/bin/python
from os import system

num_of_cores = int(input("Número de cores do seu CPU: "))
num_of_cores -= 1 # likwid conta a partir do 0

likwid_metrics = {
  "flops": {
    "metric": "FLOPS_DP",
    "label": "MFLOP/s",
  },
  "l2cache": {
    "metric": "CACHE",
    "label": "ratio",
  },
  "mem": {
    "metric": "MEM",
    "label": "Memory bandwidth [MBytes/s]",
  },
  "energy": {
    "metric": "ENERGY",
    "label": "Energy Core [J]", # meu likwid não contém label 'Energy [J]' somente CORE e PKG, n sei pq :(
  }
}

likwid_markers = ["mult-matvet-sem-otimizacao", "mult-matvet-com-otimizacao", "mult-matmat-sem-otimizacao", "mult-matmat-com-otimizacao"]

sample_inputs = [64, 96, 128, 208, 256, 512, 608, 896, 1024, 2000, 2048, 2992, 4000]    

system(f"echo \"performance\" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor")    

def get_metric_number_from_file(filename, metric):
    tracked_metrics = {}
    current_marker = None

    with open(filename, 'r') as f:
        for line in f:
            for markers in likwid_markers:
                if markers in line:
                    current_marker = markers
            if metric in line:
                if current_marker is None:
                    raise Exception(f"Marker não encontrado para a métrica {metric}")

                tracked_metrics[current_marker] = (line.split('|')[-2].strip())
    
    return tracked_metrics

def get_runtime_from_file(filename):
    runtimes = {
      "mult-matvet-sem-otimizacao": None,
      "mult-matmat-sem-otimizacao": None,
      "mult-matvet-com-otimizacao": None,
      "mult-matmat-com-otimizacao": None,
    }
    
    with open(filename, 'r') as f:
        lines = f.readlines()
        # grab number from string
        runtimes["mult-matvet-sem-otimizacao"] = lines[5].split(':')[-1].strip()
        runtimes["mult-matmat-sem-otimizacao"] = lines[6].split(':')[-1].strip()
        runtimes["mult-matvet-com-otimizacao"] = lines[7].split(':')[-1].strip()
        runtimes["mult-matmat-com-otimizacao"] = lines[8].split(':')[-1].strip()
      
    return runtimes
  
for i in sample_inputs:
    for metric in likwid_metrics:
        system(f"likwid-perfctr -C {num_of_cores} -g {likwid_metrics[metric]['metric']} -m ./matmult {i} > {metric}_tmp.txt")

        tracked_metrics = get_metric_number_from_file(f"{metric}_tmp.txt", likwid_metrics[metric]['label'])

        for marker in tracked_metrics:
            with open(f"{metric}_{marker}.txt", 'a') as f:
                f.write(f"{i} {tracked_metrics[marker]}\\\\\n")
  
    
    runtimes = get_runtime_from_file(f"{metric}_tmp.txt")

    for marker in runtimes:
        with open(f"runtime_{marker}.txt", 'a') as f:
            f.write(f"{i} {runtimes[marker]}\\\\\n")


# system(f"rm *_tmp.txt")
system(f"echo \"powersave\" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor") 
print('Done!')