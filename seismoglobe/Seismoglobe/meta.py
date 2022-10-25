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
"KAPI",

# None,
"SAML",
"SACV",
"SDV",
"XMAS",
None,
None,
None,
None,
None,
None,
"DAV",
None,
None,
None,
"PALK",
None,
None,
None,
"FURI",
"RAYN",
"PAB",
"ANWB",
None,
"GTBY",
"TEIG",
"SLBS",
None,
None,
None,
"POHA",
None,
None,
None,
"KWJN",
None,
"GUMO",
None,
None,
None,
"CHTO",
None,
None,
None,
"UOSS",
"BBSR",
None,
None,
None,
"PFO",
None,
None,
None,
None,
"MIDW",
"MAJO",
"XAN",
None,
"OSA",
"KIV",
None,
"GRFO",
None,
"ESK",
"HIV",
None,
"HRV",
None,
"RSSD",
None,
"COR",
"ADK",
"HIA",
"AAK",
"OBN",
"BORG",
"FFC",
'KDAK',
'MA2',
'YAK',
'BORK',
'KIV',
'SFJD',
'COLA',
'BILL',
'TIXI',
'TATO',
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
    if station is None:
        ledsToStations.append('' + str(255))
        continue

    if station not in data:
        ledsToStations.append('' + str(254));
        continue
    
    stationMap[station] = lastStation

    templ = "{\n"
    templ += data[station]
    templ += "}"

    dataMap.append(templ)

    ledsToStations.append('' + str(lastStation))

    lastStation += 1
out = f"""
const uint8_t ledsToStationsLength = {len(ledsToStations)};
const uint8_t ledsToStations[] = {{
    {','.join(ledsToStations)}
}};

const uint8_t PROGMEM stationsToData[][{lines}] = {{
{','.join(dataMap)}
}};
"""

pathlib.Path('out.ino').write_text(out)

# print(out)