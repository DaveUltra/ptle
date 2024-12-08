# https://github.com/Felk/dolphin/tree/scripting-preview2

import os
import sys
from pathlib import Path

from dolphin import event, memory

import time
from enum import IntEnum


dolphin_path = Path().absolute()
print("Dolphin path:", dolphin_path)
real_scripts_path = os.path.realpath(dolphin_path / "Scripts")
print("Real Scripts path:", real_scripts_path)
sys.path.append(f"{real_scripts_path}/GiraffeHarry")
# Wait for the first frame before scanning the game for constants
await event.frameadvance()

from lib.math import *


class Harry(IntEnum):
	Pelvis     = 2
	LegUpperL  = 4
	LegLowerL  = LegUpperL + 1
	FootL      = LegUpperL + 2
	ToeL       = LegUpperL + 3
	LegUpperR  = 9
	LegLowerR  = LegUpperR + 1
	FootR      = LegUpperR + 2
	ToeR       = LegUpperR + 3

	ShoulderL  = 21
	ArmUpperL  = ShoulderL + 1
	ArmLowerL  = ShoulderL + 2
	HandL      = ShoulderL + 3

	Head       = 38
	# 39 is a dummy
	Eyelids    = 40

	# 42 is a dummy
	Chin       = 43
	LipBottom  = 44
	EyeL       = 45
	EyebrowL   = 46

	LipCornerL = 48
	EyeR       = 49
	EyebrowR   = 50

	LipCornerR = 52
	# 53 is a dummy
	LipTop     = 54
	ShoulderR  = 55
	ArmUpperR  = ShoulderR + 1
	ArmLowerR  = ShoulderR + 2
	HandR      = ShoulderR + 3


HEAD_SET = [
	Harry.Head,
	Harry.Chin,
	Harry.LipTop,
	Harry.LipBottom,
	Harry.LipCornerL,
	Harry.LipCornerR,
	Harry.EyeL,
	Harry.EyeR,
	Harry.EyebrowL,
	Harry.EyebrowR,
	Harry.Eyelids,
	41, 47, 51
]

def printMat(m):
	print(m.data[0])
	print(m.data[1])
	print(m.data[2])
	print(m.data[3])
	print("---------------")


skeleton_addr = 0

def set(bone, z):
	global skeleton_addr
	addr = bone * 64 + 0x38
	memory.write_f32(skeleton_addr + addr, z)
	memory.write_f32(skeleton_addr + addr + 0x1310, z)

async def main_loop():
	global skeleton_addr

	await event.frameadvance()

	player_addr = memory.read_u32(0x8041BE4C)
	skeleton_addr = memory.read_u32(player_addr + 0xC0)

	headTransform = Mat4()

	headTransform.load(skeleton_addr + Harry.Head * 64)
	headTransform.data[3][2] += 2.0
	for i in HEAD_SET:
		headTransform.store(skeleton_addr + i * 64)

	headTransform.load(skeleton_addr + Harry.Head * 64 + 0x1310)
	headTransform.data[3][2] += 2.0
	for i in HEAD_SET:
		headTransform.store(skeleton_addr + i * 64 + 0x1310)


while True:
	await main_loop()
