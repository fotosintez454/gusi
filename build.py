#!/usr/bin/env python3
"""Build the offline Windows x64 game using Linux GCC + GNU binutils.
No network, pip, external Python packages or resource generation required.
"""
import argparse
import pathlib
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent
BUILD = ROOT / 'build'
BUILD.mkdir(exist_ok=True)

def run(*args):
    print('+', ' '.join(map(str, args)), flush=True)
    subprocess.run(list(map(str, args)), cwd=ROOT, check=True)

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--test', action='store_true', help='run portable headless tests and render screenshots')
parser.add_argument('--safe', action='store_true', help='build CPU fallback without OpenGL')
parser.add_argument('--test-fast', action='store_true', help='same tests, faster-compiling O1 diagnostic binaries')
args = parser.parse_args()
if args.test or args.test_fast:
    (ROOT / 'release_v160').mkdir(exist_ok=True)
    for name in ['qa_v160', 'qa_panel160', 'qa_life160', 'qa_repairs160', 'qa_layout160', 'qa_roomlayout160', 'qa_architecture170', 'qa_world180', 'qa_district190', 'qa_crowd190', 'qa_v200', 'qa_save200', 'qa_v201', 'qa_v202', 'qa_v203', 'qa_v204']:
        run('gcc', '-O1' if args.test_fast else '-O2', ROOT/('tools/'+name+'.c'), '-lm', '-o', BUILD/name)
        run(BUILD/name)

else:
    run('gcc', '-c', ROOT/'src/imports.s', '-o', BUILD/'imports.o')
    run('gcc', *(['-DFORCE_SOFTWARE'] if args.safe else []), ('-O1' if args.safe else '-O2'), '-c', '-mabi=ms', '-maccumulate-outgoing-args', '-mno-red-zone',
        '-ffreestanding', '-fno-builtin', '-fno-pie', '-fno-stack-protector',
        '-fno-asynchronous-unwind-tables', '-fno-ident', ROOT/'src/main.c', '-o', BUILD/'game.o')
    run('ld', '-mi386pep', '--subsystem', 'windows', '--image-base', '0x400000',
        '--stack', '16777216,1048576', '--disable-dynamicbase', '--disable-high-entropy-va', '--disable-reloc-section',
        '-e', 'start', BUILD/'game.o', BUILD/'imports.o', '-o', ROOT/('OKNO_SAFE.exe' if args.safe else 'OKNO.exe'))
    print('Built:', ROOT/('OKNO_SAFE.exe' if args.safe else 'OKNO.exe'))
