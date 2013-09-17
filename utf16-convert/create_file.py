#!/usr/bin/python3

def dump_chars(f):
  for c in range(0, 0x10ffff):
    if (c < 32):
      f.write('\n')
      continue
    if (c >= 0xd800 and c <= 0xdfff):
      f.write('\n')
      continue # reserved for UTF-16 surrogate pairs
    f.write(chr(c))
    f.write('\n')
  f.write('\n')


with open('test-input.utf16be', encoding='UTF-16-BE', mode='w') as f:
  dump_chars(f)

with open('test-input.utf16le', encoding='UTF-16-LE', mode='w') as f:
  dump_chars(f)

with open('test-input.utf16', encoding='UTF-16', mode='w') as f:
  dump_chars(f)
