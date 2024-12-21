import subprocess
import time
import sys

def count_root_processes():
    try:
        # Run the `ps` command to list processes
        result = subprocess.run(['ps', 'aux'], stdout=subprocess.PIPE, text=True)
        
        # Count the number of processes that belong to root (UID 0)
        root_processes = [line for line in result.stdout.splitlines() if line.startswith('root')]
        
        # Print the number of root processes
        print(f"Number of root processes: {len(root_processes)}")
        sys.stdout.flush()

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    while True:
        count_root_processes()
        time.sleep(3)  # Wait for 5 minutes (300 seconds)

