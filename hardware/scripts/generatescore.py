arrayOfValues = []
filename = "Scoreboard.mif"

def int2bin(integer, length):  #buggy if interger > length
    if (integer < 2**length):
        binaryArray = []
        for bit in range(length - 1):
            binaryArray.append('0')
        binary = bin(integer)
        binary = binary[2:len(binary)]
        lengthBinary = len(binary) - 2
        binaryArray[(len(binaryArray) - lengthBinary - 1):(len(binaryArray))] = binary
        return binaryArray
    else:
        return 0   #too big


arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 7, 7, 9, 9, 9, 9, 9, 9, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 9, 9, 9, 7, 7, 9, 9, 9, 9, 9, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 9, 7, 7, 7, 7, 9, 9, 9, 9, 9, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 9, 9, 7, 7, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 9, 9, 7, 7, 9, 9, 7, 9, 9, 9, 9, 9, 9, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])
arrayOfValues.append([ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7])


hexstring = "0" + str(3) + "x"
f = open(filename, "w")
f.write("DEPTH = 1280;                   -- The size of memory in words -- number of addresses - power of 2\nWIDTH =4;                    -- The size of data in bits -- binary size\nADDRESS_RADIX = HEX;          -- The radix for address values\nDATA_RADIX = BIN;             -- The radix for data values\nCONTENT                       -- start of (address : data pairs)\nBEGIN\n\n")
strb = ""
memorypositioncount = 0
for row in range(len(arrayOfValues)):

	for column in range(len(arrayOfValues[0])):
		strh = format(memorypositioncount, hexstring)
		f.write(strh)
		binstr = int2bin(arrayOfValues[row][column], 4)
		string = " : "+strb.join(binstr) + ";\n"
		f.write(string)
		memorypositioncount = memorypositioncount + 1
f.write("\nEND;")
f.close()

