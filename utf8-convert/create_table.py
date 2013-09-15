#!/usr/bin/python3

with open('table.h', encoding='utf8', mode='w') as f:

  f.write("int count_table[] = {\n")

  for r in range(0,0x100):
    f.write("  ")
    if r < 0x80:
      f.write('0')
    elif r < 0xc0:
      f.write('1')
    elif r < 0xe0:
      f.write('2')
    elif r < 0xf0:
      f.write('3')
    elif r < 0xf8:
      f.write('4')
    elif r < 0xfc:
      f.write('5')
    elif r < 0xfe:
      f.write('6')
    elif r < 0xff:
      f.write('7')
    else:
      f.write('8')

    f.write(", // ")
    f.write(bin(r)[2:].zfill(8))
    f.write('\n')
  f.write("};\n")
