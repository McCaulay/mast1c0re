#!/usr/bin/env python3
import argparse
import re

# Generate from EBOOT.BIN
# Generate rop.txt file: rp-win.exe -f .\eboot.bin --va 0x3FC000 --raw=x64 --rop=5 > eboot-1.01-rop.txt
# Re-save eboot-1.01-rop.txt to UTF-8
# Usage: python3 gadgets.py eboot-1.01-rop.txt ../../sdk/include/offsets/eboot/gadgets/1.01.hpp

RE_RPLINE = re.compile(r"(0x[0-9a-f]+): (.*) \(([0-9]+) found\)")
RE_SPECIAL = re.compile(r"[^a-zA-Z0-9_]")
RE_UNDERSCORE = re.compile(r"_+")

def convertGadgetToDefine(gadget):
    # Replace certain characters for text
    gadget = gadget.replace('+', '_PLUS_')
    gadget = gadget.replace('[', '_OB_')
    gadget = gadget.replace(']', '_CB_')
    gadget = gadget.replace('?', '_Q_')

    # Replace special characters for an underscore
    gadget = re.sub(RE_SPECIAL, '_', gadget)

    # Replace multiple consecutive underscores for a single underscore
    gadget = re.sub(RE_UNDERSCORE, '_', gadget)

    # Remove leading and trailing underscore then convert all characters to uppercase
    gadget = gadget.lstrip('_').rstrip('_').upper()
    return gadget

def main(args):

    gadgets = {}

    # Read .txt file
    with open(args.input, 'r') as i:

        # Write .hpp
        with open(args.output, 'w') as o:
            o.write(f'#pragma once\n\n')

            # Write each gadget
            for line in i:
                line = line.rstrip()

                # Ignore non-matching lines
                matches = RE_RPLINE.match(line)
                if not matches:
                    continue

                # Get the address and gadget from the .txt file
                address = matches.group(1)
                gadget = matches.group(2)

                # Ignore already defined gadget
                if gadget in gadgets:
                    continue

                # Write the hpp definition
                o.write(f'#define {convertGadgetToDefine(gadget)} {address} // {gadget}\n')
                gadgets[gadget] = address

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate an EBOOT gadgets header definition file.')
    parser.add_argument('input', help='The rp++ text file output to be parsed.')
    parser.add_argument('output', help='The generated .h file.')
    main(parser.parse_args())