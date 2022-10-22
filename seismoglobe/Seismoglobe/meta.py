import os
import pathlib

datadir = "data/testfiles/"

stationOrder = [
"SBA",
"SNZO",
"MSVF",
"HNR",
"TAU",
"CASY",
"HOPE",
"EFI",
"PMSA",
"RAO",
"RAR",
"PTCN",
"RPN",
"TRQA",
"SHEL",
"LBTB",
"ABPO",
"NWAO",
"CTAO",
"PAYG",
"LCO",
"RCBR",
"MBAR",
"MSEY",
"DGAR",
"COCO",
"KAPI"
]

data = {}

files = os.listdir(datadir)

entrycount = None;

for path in files:
    _, name, chan, *_ = path.split('.')
    if name not in stationOrder:
        print('not looking for', name)
        continue
    if chan != '00':
        print('wrong channel', chan)
        continue

    print('have data for', name, chan)

    data[name] = pathlib.Path(datadir + path).read_text()

    lines = entrycount = len(data[name].split('\n'))

    if not entrycount:
        entrycount = lines
    else:
        if entrycount != lines:
            print('something has gone wrong', path, 'has wrong lines', lines, 'vs', entrycount)
            exit(1)

lastStation = 0
stationMap = {}

dataMap = []

ledsToStations = []

for station in stationOrder:
    if station not in data:
        ledsToStations.append('' + str(255));
        continue
    
    stationMap[station] = lastStation

    templ = "{\n"
    templ += data[station]
    templ += "}"

    dataMap.append(templ)

    ledsToStations.append('' + str(lastStation))

    lastStation += 1
out = f"""
const uint8_t PROGMEM ledsToStations[] = {{
    {','.join(ledsToStations)}
}};

const uint8_t PROGMEM stationsToData[][{lines}] = {{
{','.join(dataMap)}
}};
"""

pathlib.Path('out.ino').write_text(out)

# print(out)