#!/usr/bin/python3

with open('test-input.utf8', encoding='utf8', mode='w') as f:
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
