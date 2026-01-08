import matplotlib.pyplot as plt

lock_free_set_results = {
    "0..8": {
        "ctn10_add54_rmv36": [
            {"threads": 1,  "time_ms": 2.9690,   "total_ops": 100000},
            {"threads": 2,  "time_ms": 19.5320,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 46.7830,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 130.1699, "total_ops": 800000},
            {"threads": 16, "time_ms": 270.7478, "total_ops": 1600000},
        ],
        "ctn50_add30_rmv20": [
            {"threads": 1,  "time_ms": 4.0750,   "total_ops": 100000},
            {"threads": 2,  "time_ms": 30.7190,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 56.9338,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 160.7891, "total_ops": 800000},
            {"threads": 16, "time_ms": 341.1479, "total_ops": 1600000},
        ],
        "ctn90_add6_rmv4": [
            {"threads": 1,  "time_ms": 3.2190,   "total_ops": 100000},
            {"threads": 2,  "time_ms": 23.5620,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 49.2151,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 134.0471, "total_ops": 800000},
            {"threads": 16, "time_ms": 282.9980, "total_ops": 1600000},
        ],
    },
    "0..1028": {
        "ctn10_add54_rmv36": [
            {"threads": 1,  "time_ms": 13.2070,  "total_ops": 100000},
            {"threads": 2,  "time_ms": 37.9119,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 85.4670,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 198.7373, "total_ops": 800000},
            {"threads": 16, "time_ms": 422.6028, "total_ops": 1600000},
        ],
        "ctn50_add30_rmv20": [
            {"threads": 1,  "time_ms": 48.7710,  "total_ops": 100000},
            {"threads": 2,  "time_ms": 62.8350,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 106.0959, "total_ops": 400000},
            {"threads": 8,  "time_ms": 217.9990, "total_ops": 800000},
            {"threads": 16, "time_ms": 496.6282, "total_ops": 1600000},
        ],
        "ctn90_add6_rmv4": [
            {"threads": 1,  "time_ms": 97.6602,  "total_ops": 100000},
            {"threads": 2,  "time_ms": 110.1350, "total_ops": 200000},
            {"threads": 4,  "time_ms": 152.4871, "total_ops": 400000},
            {"threads": 8,  "time_ms": 234.9009, "total_ops": 800000},
            {"threads": 16, "time_ms": 494.9448, "total_ops": 1600000},
        ],
    }
}

def throughput(entry):
    return entry["total_ops"] / (entry["time_ms"] / 1000.0)

def plot_results(value_range, data):
    plt.figure()
    
    for config, measurements in data.items():
        threads = [m["threads"] for m in measurements]
        thrpt = [throughput(m) for m in measurements]
        
        plt.plot(threads, thrpt, marker='o', label=config)
    
    plt.xlabel("Number of threads")
    plt.ylabel("Throughput (operations / second)")
    plt.title(f"LockFreeSet throughput for values {value_range}")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

# run on tussigalo
lock_free_set_results_2 = {
    "0..8": {
        "ctn10_add54_rmv36": [
            {"threads": 1,  "time_ms": 17.2620,   "total_ops": 100000},
            {"threads": 2,  "time_ms": 185.9329,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 329.9109,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 975.0449,  "total_ops": 800000},
            {"threads": 16, "time_ms": 2049.9070, "total_ops": 1600000},
        ],
        "ctn50_add30_rmv20": [
            {"threads": 1,  "time_ms": 20.1838,   "total_ops": 100000},
            {"threads": 2,  "time_ms": 209.6760,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 393.2988,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 1093.7590, "total_ops": 800000},
            {"threads": 16, "time_ms": 2281.4531, "total_ops": 1600000},
        ],
        "ctn90_add6_rmv4": [
            {"threads": 1,  "time_ms": 17.0911,   "total_ops": 100000},
            {"threads": 2,  "time_ms": 194.3540,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 342.0000,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 1009.5698, "total_ops": 800000},
            {"threads": 16, "time_ms": 2094.2019, "total_ops": 1600000},
        ],
    },
    "0..1028": {
        "ctn10_add54_rmv36": [
            {"threads": 1,  "time_ms": 40.9670,   "total_ops": 100000},
            {"threads": 2,  "time_ms": 205.5222,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 489.7544,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 1268.1211, "total_ops": 800000},
            {"threads": 16, "time_ms": 2594.9719, "total_ops": 1600000},
        ],
        "ctn50_add30_rmv20": [
            {"threads": 1,  "time_ms": 234.6738,  "total_ops": 100000},
            {"threads": 2,  "time_ms": 344.6682,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 666.6741,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 1387.1208, "total_ops": 800000},
            {"threads": 16, "time_ms": 2832.4360, "total_ops": 1600000},
        ],
        "ctn90_add6_rmv4": [
            {"threads": 1,  "time_ms": 386.7471,  "total_ops": 100000},
            {"threads": 2,  "time_ms": 478.7290,  "total_ops": 200000},
            {"threads": 4,  "time_ms": 753.6157,  "total_ops": 400000},
            {"threads": 8,  "time_ms": 1403.9729, "total_ops": 800000},
            {"threads": 16, "time_ms": 2912.6753, "total_ops": 1600000},
        ],
    }
}


# Plot for values 0..8
plot_results("0..8", lock_free_set_results_2["0..8"])

# Plot for values 0..1028
plot_results("0..1028", lock_free_set_results_2["0..1028"])