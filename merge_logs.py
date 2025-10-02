import glob
import re

# Tüm thread log dosyalarını bul
files = glob.glob("build/steps_thread*.log")

# EventID -> satır listesi
events = {}

for fname in files:
    with open(fname) as f:
        for line in f:
            # [Event X] bilgisini yakala
            match = re.match(r"\[Event (\d+)\]", line)
            if match:
                eid = int(match.group(1))
                if eid not in events:
                    events[eid] = []
                events[eid].append(line.strip())

# EventID'lere göre sırala
with open("all_steps.log", "w") as out:
    for eid in sorted(events.keys()):
        for line in events[eid]:
            out.write(line + "\n")
