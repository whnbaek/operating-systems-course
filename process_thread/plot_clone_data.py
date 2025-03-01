import matplotlib.pyplot as plt
import re

# Read the benchmark results from a file
with open("benchmark_results.txt", "r") as file:
    data = file.read()

# Extract labels and times using regex
pattern = re.compile(r'Benchmarking (.*?):\s*(\d+\.\d+) seconds')
matches = pattern.findall(data)

if not matches:
    print("No valid benchmark data found.")
    exit(1)

labels, times = zip(*matches)
times = [float(time) for time in times]

plt.figure(figsize=(12, 6))
plt.barh(labels, times, color='skyblue')
plt.xlabel('Time (seconds)')
plt.ylabel('Benchmark')
plt.title('Benchmarking Clone Variants')
plt.gca().invert_yaxis()  # Invert y-axis for better readability
plt.grid(axis='x', linestyle='--', alpha=0.7)
plt.show()
