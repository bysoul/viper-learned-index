import re
import sys
import struct
import codecs

INSERT_RE = re.compile(r'^(INSERT|UPDATE) usertable user(\d+) \[ field0=(.{200}) .*$')
READ_RE = re.compile(r'^READ usertable user(\d+) .*$')
BIN_UINT64_T_FORMAT = '<Q'
BIN_UINT32_T_FORMAT = '<I'

OPS = { 'INSERT': 0, 'GET': 1, 'UPDATE': 2 }

def convert_file(in_file, out_file):
    with open(in_file) as in_f, open(out_file, 'wb') as out_f:
        count = 0
        list = []
        for line_num, line in enumerate(in_f):
            matched = False
            match = INSERT_RE.match(line)
            if match is not None:
                op = match.group(1)
                key = int(match.group(2))
                value = match.group(3)
                matched = True
                count=count+1
                list.append(key)

            if not matched:
                continue

            # print(f"op: {op} | key: {key} | value: {value}")

            # if line_num % 1000000 == 0:
            #     print(f"Converted {line_num} records")
        list.sort()
        out_f.write(struct.pack(BIN_UINT64_T_FORMAT, count))
        for num in list:
            bytes = struct.pack(BIN_UINT64_T_FORMAT, num)
            out_f.write(bytes)



if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Need two args, infile outfile")
        sys.exit(1)
    convert_file(sys.argv[1], sys.argv[2])
