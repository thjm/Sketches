#! /usr/bin/env python
#
# readFLASH.py: example for reading from an EPROM/EEPROM 
#               plugged into the FlashReader hardware
#

import sys
import time

# for parsing command line options
from argparse import ArgumentParser
# reading/writing Intel Hex files
# see also: http://pythonhosted.org/IntelHex
from intelhex import IntelHex
# import serial interface stuff
import serial

# ---

EEprom = {}

EEprom['2716']  = { 'size' : 0x0800,
                    'type' : 1 }
EEprom['2732']  = { 'size' : 0x1000,
                    'type' : 2 }
EEprom['2764']  = { 'size' : 0x2000,
                    'type' : 3 }
EEprom['27128'] = { 'size' : 0x4000,
                    'type' : 4 }
EEprom['27256'] = { 'size' : 0x8000,
                    'type' : 5 }
EEprom['27512'] = { 'size' : 0x8000,
                    'type' : 6 }
EEprom['27010'] = { 'size' : 0x10000,
                    'type' : 7 }
EEprom['27020'] = { 'size' : 0x20000,
                    'type' : 8 }
EEprom['27040'] = { 'size' : 0x40000,
                    'type' : 9 }

SERIAL_DEVICE = '/dev/ttyACM0'

# ---

KNOWN_EPROMS = [eprom for eprom in EEprom]

description = """Read data from the FLASH of the specified type in given address range."""
parser = ArgumentParser(description=description,
                        version="1.0")

parser.add_argument('-p', '--serial-port', type=str, default=SERIAL_DEVICE,
                          help='name of the serial interface (device)')
parser.add_argument('-o', '--output-file', type=str, default='FLASH.hex',
                          help='name of the output file for the data of the EPROMs')
parser.add_argument('-s', '--show-eproms', action='store_true', default=False,
                          help='show known E(E)PROM types and exit')
parser.add_argument('-t', '--type', type=str, default='2732',
                          help='set the E(E)PROM type')
parser.add_argument('--start-address', type=long, default=0x0,
                          help='start address of E(E)PROM from where to read')
parser.add_argument('--end-address', type=long, default=0x0,
                          help='end address of E(E)PROM until where to read')

args = parser.parse_args()

if args.show_eproms:
    print "Known E(E)PROM types are:"
    for eprom in KNOWN_EPROMS:
        print "\t%s" % eprom
    sys.exit(0)

if args.type not in KNOWN_EPROMS:
    print "EPROM type args.type is not known!"
    sys.exit(1)

# ---

eprom = EEprom[args.type]

if args.end_address == 0:
    args.end_address = eprom['size']

# array for EPROM data, pre-initialized with 0xff
eprom_data = [0xff]*eprom['size']

print "Reading from E(E)PROM of type %s from 0x%04x to 0x%04x..." % \
      (args.type, args.start_address, args.end_address-1)

# with filename argument means open for reading
ih = IntelHex()

# readout part, toggle LEDs and measure the time

t_start = time.time()

ser = None

try:
   ser = serial.Serial(args.serial_port, 9600, 
                       timeout=0.05, bytesize=8, stopbits=1, 
                       parity=serial.PARITY_NONE, rtscts=False, xonxoff=False)
except:
   print "Exception while trying to open %s!" % args.serial_port
   sys.exit(1)

nBytes = 0

# set the E(E)PROM type, '\r' is not really needed
response = ''
while response != 'OK':
    request = "t %d\r\n" % eprom['type']
    print "request='%s'" % request.strip()
    ser.write(request)
    # read from serial interface, strip CR/LF
    response = ser.readline().strip()
    print "response='%s'" % response

# read data from FLASH in chunks of 16 bytes
for address in range(args.start_address, args.end_address, 16):
    request = "r %d %d\r\n" % (address, 16)
    print "request='%s'" % request.strip()
    ser.write(request)
    time.sleep(0.1)
    response = ser.readline().strip()
    #print "response='%s'" % response
    response = ser.readline().strip()
    print "response='%s'" % response
    response = ser.readline().strip()
    print "response='%s'" % response

#ser.write("r %d %d\n" % (args.start_address, args.end_address - args.start_address))
#serial_data = ser.read(100).strip()
#print serial_data

ser.close()

t_stop = time.time()

print "Read %d bytes from EPROM type %s in %f s" % \
      (nBytes, args.type, t_stop - t_start)

# file part

hexfile = open(args.output_file, 'w')
ih.tofile(hexfile, 'hex')
hexfile.close()

print "Written %d bytes to Intel Hex-record formatted file %s!" % \
      (nBytes, args.output_file)

# eof
