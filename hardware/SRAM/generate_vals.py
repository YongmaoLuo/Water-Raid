depth = 512
width = 40

f = open("SRAM.mif", "w")
f.write("DEPTH = "+str(depth)+";\n")
f.write("WIDTH = "+str(width)+";\n")
f.write("ADDRESS_RADIX = HEX;\n")
f.write("DATA_RADIX = BIN;\n")
f.write("CONTENT\n")
f.write("BEGIN\n")
f.write("\n")
for i in range(depth):
	string = format(i,'03X')
	bin_val = "0011011100"+"0110100100"+"0000000000"+"0000000000;"
	string = string + " : "+bin_val+"\n"
	f.write(string)
f.write("\nEND;")
f.close()
