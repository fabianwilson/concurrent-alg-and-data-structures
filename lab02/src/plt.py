import matplotlib.pyplot as plt

threads = [1, 2, 4, 8, 16]
total_ops = [100000, 200000, 400000, 800000, 1600000]

# Data format:
# data[value_range][workload][set_name] = list of times in ms
data = {
    "0..8": {
        "10/54/36": {
            "FineSet":       [13.0730, 61.9871, 240.5042, 530.0420, 1118.7432],
            "OptimisticSet": [12.7510, 47.3750, 118.7439, 327.6941, 811.3611],
            "LazySet":       [13.9817, 50.9460, 125.4570, 340.2109, 817.4089],
        },
        "50/30/20": {
            "FineSet":       [18.9141, 120.7908, 374.5630, 680.0430, 1430.4849],
            "OptimisticSet": [17.0420, 62.7183, 200.9541, 517.9070, 1086.7629],
            "LazySet":       [17.4443, 61.2832, 192.4990, 506.8770, 1090.7969],
        },
        "90/6/4": {
            "FineSet":       [17.0132, 123.4250, 391.3650, 696.5347, 1379.7749],
            "OptimisticSet": [15.6851, 49.1040, 218.3389, 580.4780, 1211.1692],
            "LazySet":       [16.2307, 45.2710, 186.7319, 529.8589, 1178.6680],
        },
    },

    "0..1028": {
        "10/54/36": {
            "FineSet":       [120.2441, 346.5859, 3065.3792, 6099.0879, 13582.9331],
            "OptimisticSet": [80.9128, 101.8491, 134.0090, 252.3599, 525.1970],
            "LazySet":       [56.6589, 73.5891, 113.2959, 230.6819, 503.1919],
        },
        "50/30/20": {
            "FineSet":       [424.4312, 795.2009, 7882.0959, 16166.0601, 34600.8850],
            "OptimisticSet": [294.3621, 342.1440, 337.6201, 471.2651, 900.5767],
            "LazySet":       [170.7131, 197.2661, 225.1660, 331.3579, 644.8003],
        },
        "90/6/4": {
            "FineSet":       [616.7471, 1058.5791, 9888.1570, 21637.2710, 45329.6609],
            "OptimisticSet": [460.3560, 504.2700, 537.2300, 692.4761, 1295.9121],
            "LazySet":       [247.5759, 272.9338, 299.7400, 406.1599, 786.7620],
        },
    }
}
for value_range, workloads in data.items():
    for workload, sets in workloads.items():

        plt.figure()

        for set_name, times in sets.items():
            throughput = [
                ops / (t / 1000)
                for ops, t in zip(total_ops, times)
            ]
            plt.plot(threads, throughput, marker='o', label=set_name)

        plt.xlabel("Number of Threads")
        plt.ylabel("Throughput (operations per second)")
        plt.title(f"Throughput vs Threads\nValues {value_range}, workload {workload}")
        plt.legend()
        plt.grid(True)

        plt.savefig(f"throughput_{value_range.replace('..','_')}_{workload.replace('/','_')}.png")