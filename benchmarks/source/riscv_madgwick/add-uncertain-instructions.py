#!/usr/bin/env python3
import fileinput
import re
import sys


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def check_instruction(line, mnemonic, lineno):
    if line.find(mnemonic) != -1:
        eprint(
            "Found `{}` instruction on line {}.\
Ignoring it"
            .format(mnemonic, lineno))


def replace_mnemonic(line, mnemonic):
    return line.replace(
        "\t{}\t".format(mnemonic),
        "\tun{}\t".format(mnemonic)
    )


with fileinput.input(inplace=True) as file:
    for line in file:
        lineno = file.lineno()
        line = replace_mnemonic(line, "fadd.s")
        line = replace_mnemonic(line, "fsub.s")
        line = replace_mnemonic(line, "fmul.s")
        line = replace_mnemonic(line, "fdiv.s")
        line = replace_mnemonic(line, "fsqrt.s")
        line = replace_mnemonic(line, "fsgnj.s")
        line = replace_mnemonic(line, "fsgnjn.s")
        line = replace_mnemonic(line, "fssnjx.s")
        # psuedo instructions
        line = replace_mnemonic(line, "fmv.s")
        line = replace_mnemonic(line, "fneg.s")

        # load and stores
        line = re.sub(
            r"^\tflw\t(\w*),(.*)\((\w*)\)\t(#.*)$",
            r"\tunflw\t\1,\2(\3),\2(\3)\t\4",
            line
        )
        line = re.sub(
            r"^\tfsw\t(\w*),(.*)\((\w*)\)\t(#.*)$",
            r"\tunfsw\t\1,\2(\3),\2(\3)\t\4",
            line
        )

        if re.search(r"^\tflw", line) != None:
            eprint(
                "Found flw instruction on line {}.\
Error: this should have been replaced"
                .format(lineno))
            sys.exit(1)

        if re.search(r"^\tfsw", line) != None:
            eprint(
                "Found fsw instruction on line {}.\
Error: this should have been replaced"
                .format(lineno))
            sys.exit(1)

        #check for instructions that have escaped

        # instructions we do not (yet) know how to replace
        check_instruction(line, "fmadd.s", lineno)
        check_instruction(line, "fmsub.s", lineno)
        check_instruction(line, "fnmsub.s", lineno)
        check_instruction(line, "fmin.s", lineno)
        check_instruction(line, "fmax.s", lineno)
        check_instruction(line, "fcvt.w.s", lineno)
        check_instruction(line, "fcvt.wu.s", lineno)
        check_instruction(line, "fmv.x.w", lineno)
        # check_instruction(line, "feq.s", lineno)
        check_instruction(line, "flt.s", lineno)
        check_instruction(line, "fle.s", lineno)
        check_instruction(line, "fclass.s", lineno)
        check_instruction(line, "fcvt.s", lineno)
        check_instruction(line, "fcvt.s.wu", lineno)
        check_instruction(line, "fmv.w.x", lineno)

        # depreciated instructions
        # check_instruction(line, "fmv.s.x", lineno)
        check_instruction(line, "fmv.w.s", lineno)

        # Misc notes:

        # gcc zeros floating point values using the fmv.s.w instruction

        print(line, end='')
