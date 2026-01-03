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

# Plot for values 0..8
plot_results("0..8", lock_free_set_results["0..8"])

# Plot for values 0..1028
plot_results("0..1028", lock_free_set_results["0..1028"])