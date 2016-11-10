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

def decodeRecord(ih, record='', line=0):
    """Decode a record in Intel HEX format and write the resulting data
       in case of no error into the object of type Intelhex.
       
       This function is similar (but simplified) to the the method 
       _decode_record() of the class IntelHex. It is only able to read
       the record types 0 (data) and -1 (end of file).
       
       Return the number of bytes read or -1 in the case the EOF record 
       was found.
    """

    from array import array
    from intelhex import unhexlify, asbytes

    record = record.rstrip('\r\n')
    if not record:
    	return 0	 # empty line

    if record[0] == ':':
    	try:
    	    bin = array('B', unhexlify(asbytes(record[1:])))
    	except (TypeError, ValueError):
    	    # this might be raised by unhexlify when odd hexascii digits
    	    raise HexRecordError(line=line)
    	length = len(bin)
    	if length < 5:
    	    raise HexRecordError(line=line)
    else:
    	raise HexRecordError(line=line)

    record_length = bin[0]
    if length != (5 + record_length):
    	raise RecordLengthError(line=line)

    addr = bin[1]*256 + bin[2]

    record_type = bin[3]
    #if not (0 <= record_type <= 5):
    if not (0 <= record_type <= 1):
    	raise RecordTypeError(line=line)

    crc = sum(bin)
    crc &= 0x0FF
    if crc != 0:
    	raise RecordChecksumError(line=line)

    # data record
    if record_type == 0:
        for i in range(record_length):
   	    ih[addr+i] = bin[i+4]

    elif record_type == 1:
    	# end of file record
    	if record_length != 0:
    	    raise EOFRecordError(line=line)
    	return -1

    return record_length

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

# http://stackoverflow.com/questions/3056048/filename-and-line-number-of-python-script
from inspect import currentframe, getframeinfo

# read data from FLASH in chunks of 16 bytes
for address in range(args.start_address, args.end_address, 16):
    request = "r %d %d\r\n" % (address, 16)
    #print "request='%s'" % request.strip()
    ser.write(request)
    time.sleep(0.1)
    response = ser.readline().strip()
    #print "response='%s'" % response
    if 'OK' in response:
        response = ser.readline().strip()
        print "response='%s'" % response
        #ih._decode_record(response,getframeinfo(currentframe()).lineno)
	nBytes += decodeRecord(ih, response)
        response = ser.readline().strip()
        print "response='%s'" % response
    elif 'ERR' in response:
        print "Error in response to request '%s'!" % request.strip()
    else:
        print "Unknown response '%s' to request '%s'!" % (response,request.strip())

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
