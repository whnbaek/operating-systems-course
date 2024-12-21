sudo systemd-run --scope --property="CPUQuota=20%" --property="MemoryMax=1G" -- python3
