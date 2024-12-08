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
from CONFIG import *


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


async def main_loop():
	global skeleton_addr

	await event.frameadvance()

	player_addr = memory.read_u32(0x8041BE4C)
	if player_addr == 0:
		return

	skeleton_addr = memory.read_u32(player_addr + 0xC0)

	headTransformA = Mat4()
	headTransformB = Mat4()

	headTransformA.load(skeleton_addr + Harry.Head * 64)
	headTransformB.load(skeleton_addr + Harry.Head * 64 + 0x1310)

	fwdA = headTransformA.getRow(1); fwdA.normalize()
	fwdB = headTransformB.getRow(1); fwdB.normalize()
	upA = headTransformA.getRow(2); upA.normalize()
	upB = headTransformB.getRow(2); upB.normalize()

	if modif == 1:
		pos = Vec4()
		pos.set(upA); pos.mul1(giraffe_amount); pos.add(headTransformA.getRow(3));
		headTransformA.setRow(3, pos);
		for i in HEAD_SET:
			headTransformA.store(skeleton_addr + i * 64)

		pos.set(upB); pos.mul1(giraffe_amount); pos.add(headTransformB.getRow(3));
		headTransformB.setRow(3, pos);
		for i in HEAD_SET:
			headTransformB.store(skeleton_addr + i * 64 + 0x1310)

	if modif == 2:
		eyeT = Mat4()
		eye = Vec4()

		eyeT.load(skeleton_addr + Harry.EyeL * 64)
		eye.set(fwdA); eye.mul1(-0.1); eye.add(eyeT.getRow(3))
		eyeT.setRow(3, eye)
		eyeT.store(skeleton_addr + Harry.EyeL * 64)

		eyeT.load(skeleton_addr + Harry.EyeL * 64 + 0x1310)
		eye.set(fwdB); eye.mul1(-0.1); eye.add(eyeT.getRow(3))
		eyeT.setRow(3, eye)
		eyeT.store(skeleton_addr + Harry.EyeL * 64 + 0x1310)

		eyeT.load(skeleton_addr + Harry.EyeR * 64)
		eye.set(fwdA); eye.mul1(-0.1); eye.add(eyeT.getRow(3))
		eyeT.setRow(3, eye)
		eyeT.store(skeleton_addr + Harry.EyeR * 64)

		eyeT.load(skeleton_addr + Harry.EyeR * 64 + 0x1310)
		eye.set(fwdB); eye.mul1(-0.1); eye.add(eyeT.getRow(3))
		eyeT.setRow(3, eye)
		eyeT.store(skeleton_addr + Harry.EyeR * 64 + 0x1310)


while True:
	await main_loop()
