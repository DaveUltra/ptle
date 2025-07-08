#pragma once
#define ISOLATION_AWARE_ENABLED 1
#include <windows.h>
#include <cctype>
#include <string>
#include <shlobj.h>
#include <stdio.h>
#include <filesystem>
#include <functional>
#include <set>
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)

#include "dll/dll.h"



extern "C" __declspec(naked) void __stdcall _BinkAllocateFrameBuffers(int, int, int) { _asm { jmp[binkw32.BinkAllocateFrameBuffers] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferBlit(int, int, int) { _asm { jmp[binkw32.BinkBufferBlit] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferCheckWinPos(int, int, int) { _asm { jmp[binkw32.BinkBufferCheckWinPos] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferClear(int, int) { _asm { jmp[binkw32.BinkBufferClear] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferClose(int) { _asm { jmp[binkw32.BinkBufferClose] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferGetDescription(int) { _asm { jmp[binkw32.BinkBufferGetDescription] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferGetError() { _asm { jmp[binkw32.BinkBufferGetError] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferLock(int) { _asm { jmp[binkw32.BinkBufferLock] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferOpen(int, int, int, int) { _asm { jmp[binkw32.BinkBufferOpen] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferSetDirectDraw(int, int) { _asm { jmp[binkw32.BinkBufferSetDirectDraw] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferSetHWND(int, int) { _asm { jmp[binkw32.BinkBufferSetHWND] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferSetOffset(int, int, int) { _asm { jmp[binkw32.BinkBufferSetOffset] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferSetResolution(int, int, int) { _asm { jmp[binkw32.BinkBufferSetResolution] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferSetScale(int, int, int) { _asm { jmp[binkw32.BinkBufferSetScale] } }
extern "C" __declspec(naked) void __stdcall _BinkBufferUnlock(int) { _asm { jmp[binkw32.BinkBufferUnlock] } }
extern "C" __declspec(naked) void __stdcall _BinkCheckCursor(int, int, int, int, int) { _asm { jmp[binkw32.BinkCheckCursor] } }
extern "C" __declspec(naked) void __stdcall _BinkClose(int) { _asm { jmp[binkw32.BinkClose] } }
extern "C" __declspec(naked) void __stdcall _BinkCloseTrack(int) { _asm { jmp[binkw32.BinkCloseTrack] } }
extern "C" __declspec(naked) void __stdcall _BinkControlBackgroundIO(int, int) { _asm { jmp[binkw32.BinkControlBackgroundIO] } }
extern "C" __declspec(naked) void __stdcall _BinkControlPlatformFeatures(int, int) { _asm { jmp[binkw32.BinkControlPlatformFeatures] } }
extern "C" __declspec(naked) void __stdcall _BinkCopyToBuffer(int, int, int, int, int, int, int) { _asm { jmp[binkw32.BinkCopyToBuffer] } }
extern "C" __declspec(naked) void __stdcall _BinkCopyToBufferRect(int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.BinkCopyToBufferRect] } }
extern "C" __declspec(naked) void __stdcall _BinkCurrentSubtitle(int, int, int, int) { _asm { jmp[binkw32.BinkCurrentSubtitle] } }
extern "C" __declspec(naked) void __stdcall _BinkDDSurfaceType(int) { _asm { jmp[binkw32.BinkDDSurfaceType] } }
extern "C" __declspec(naked) void __stdcall _BinkDX8SurfaceType(int) { _asm { jmp[binkw32.BinkDX8SurfaceType] } }
extern "C" __declspec(naked) void __stdcall _BinkDX9SurfaceType(int) { _asm { jmp[binkw32.BinkDX9SurfaceType] } }
extern "C" __declspec(naked) void __stdcall _BinkDoFrame(int) { _asm { jmp[binkw32.BinkDoFrame] } }
extern "C" __declspec(naked) void __stdcall _BinkDoFrameAsync(int, int, int) { _asm { jmp[binkw32.BinkDoFrameAsync] } }
extern "C" __declspec(naked) void __stdcall _BinkDoFrameAsyncMulti(int, int, int) { _asm { jmp[binkw32.BinkDoFrameAsyncMulti] } }
extern "C" __declspec(naked) void __stdcall _BinkDoFrameAsyncWait(int, int) { _asm { jmp[binkw32.BinkDoFrameAsyncWait] } }
extern "C" __declspec(naked) void __stdcall _BinkDoFramePlane(int, int) { _asm { jmp[binkw32.BinkDoFramePlane] } }
extern "C" __declspec(naked) void __stdcall _BinkFindXAudio2WinDevice(int, int) { _asm { jmp[binkw32.BinkFindXAudio2WinDevice] } }
extern "C" __declspec(naked) void __stdcall _BinkFreeGlobals() { _asm { jmp[binkw32.BinkFreeGlobals] } }
extern "C" __declspec(naked) void __stdcall _BinkGetError() { _asm { jmp[binkw32.BinkGetError] } }
extern "C" __declspec(naked) void __stdcall _BinkGetFrameBuffersInfo(int, int) { _asm { jmp[binkw32.BinkGetFrameBuffersInfo] } }
extern "C" __declspec(naked) void __stdcall _BinkGetGPUDataBuffersInfo(int, int) { _asm { jmp[binkw32.BinkGetGPUDataBuffersInfo] } }
extern "C" __declspec(naked) void __stdcall _BinkGetKeyFrame(int, int, int) { _asm { jmp[binkw32.BinkGetKeyFrame] } }
extern "C" __declspec(naked) void __stdcall _BinkGetPalette(int) { _asm { jmp[binkw32.BinkGetPalette] } }
extern "C" __declspec(naked) void __stdcall _BinkGetPlatformInfo(int, int) { _asm { jmp[binkw32.BinkGetPlatformInfo] } }
extern "C" __declspec(naked) void __stdcall _BinkGetRealtime(int, int, int) { _asm { jmp[binkw32.BinkGetRealtime] } }
extern "C" __declspec(naked) void __stdcall _BinkGetRects(int, int) { _asm { jmp[binkw32.BinkGetRects] } }
extern "C" __declspec(naked) void __stdcall _BinkGetRects4(int) { _asm { jmp[binkw32.BinkGetRects4] } }
extern "C" __declspec(naked) void __stdcall _BinkGetSubtitleByIndex(int, int, int, int) { _asm { jmp[binkw32.BinkGetSubtitleByIndex] } }
extern "C" __declspec(naked) void __stdcall _BinkGetSummary(int, int) { _asm { jmp[binkw32.BinkGetSummary] } }
extern "C" __declspec(naked) void __stdcall _BinkGetTrackData(int, int) { _asm { jmp[binkw32.BinkGetTrackData] } }
extern "C" __declspec(naked) void __stdcall _BinkGetTrackData12(int, int, int) { _asm { jmp[binkw32.BinkGetTrackData12] } }
extern "C" __declspec(naked) void __stdcall _BinkGetTrackID(int, int) { _asm { jmp[binkw32.BinkGetTrackID] } }
extern "C" __declspec(naked) void __stdcall _BinkGetTrackMaxSize(int, int) { _asm { jmp[binkw32.BinkGetTrackMaxSize] } }
extern "C" __declspec(naked) void __stdcall _BinkGetTrackType(int, int) { _asm { jmp[binkw32.BinkGetTrackType] } }
extern "C" __declspec(naked) void __stdcall _BinkGoto(int, int, int) { _asm { jmp[binkw32.BinkGoto] } }
extern "C" __declspec(naked) void __stdcall _BinkIsSoftwareCursor(int, int) { _asm { jmp[binkw32.BinkIsSoftwareCursor] } }
extern "C" __declspec(naked) void __stdcall _BinkLoadSubtitles(int, int) { _asm { jmp[binkw32.BinkLoadSubtitles] } }
extern "C" __declspec(naked) void __stdcall _BinkLogoAddress() { _asm { jmp[binkw32.BinkLogoAddress] } }
extern "C" __declspec(naked) void __stdcall _BinkNextFrame(int) { _asm { jmp[binkw32.BinkNextFrame] } }
extern "C" __declspec(naked) void __stdcall _BinkOpen(int, int) { _asm { jmp[binkw32.BinkOpen] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenDirectSound(int) { _asm { jmp[binkw32.BinkOpenDirectSound] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenMiles(int) { _asm { jmp[binkw32.BinkOpenMiles] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenTrack(int, int) { _asm { jmp[binkw32.BinkOpenTrack] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenWaveOut(int) { _asm { jmp[binkw32.BinkOpenWaveOut] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenWithOptions(int, int, int) { _asm { jmp[binkw32.BinkOpenWithOptions] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenXAudio2(int) { _asm { jmp[binkw32.BinkOpenXAudio2] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenXAudio2_8(int, int) { _asm { jmp[binkw32.BinkOpenXAudio2_8] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenXAudio27(int, int) { _asm { jmp[binkw32.BinkOpenXAudio27] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenXAudio28(int, int) { _asm { jmp[binkw32.BinkOpenXAudio28] } }
extern "C" __declspec(naked) void __stdcall _BinkOpenXAudio29(int, int) { _asm { jmp[binkw32.BinkOpenXAudio29] } }
extern "C" __declspec(naked) void __stdcall _BinkPause(int, int) { _asm { jmp[binkw32.BinkPause] } }
extern "C" __declspec(naked) void __stdcall _BinkRegisterFrameBuffers(int, int) { _asm { jmp[binkw32.BinkRegisterFrameBuffers] } }
extern "C" __declspec(naked) void __stdcall _BinkRegisterGPUDataBuffers(int, int) { _asm { jmp[binkw32.BinkRegisterGPUDataBuffers] } }
extern "C" __declspec(naked) void __stdcall _BinkRequestStopAsyncThread(int) { _asm { jmp[binkw32.BinkRequestStopAsyncThread] } }
extern "C" __declspec(naked) void __stdcall _BinkRequestStopAsyncThreadsMulti(int, int) { _asm { jmp[binkw32.BinkRequestStopAsyncThreadsMulti] } }
extern "C" __declspec(naked) void __stdcall _BinkRestoreCursor(int) { _asm { jmp[binkw32.BinkRestoreCursor] } }
extern "C" __declspec(naked) void __stdcall _BinkService(int) { _asm { jmp[binkw32.BinkService] } }
extern "C" __declspec(naked) void __stdcall _BinkServiceSound() { _asm { jmp[binkw32.BinkServiceSound] } }
extern "C" __declspec(naked) void __stdcall _BinkSetError(int) { _asm { jmp[binkw32.BinkSetError] } }
extern "C" __declspec(naked) void __stdcall _BinkSetFileOffset(int, int) { _asm { jmp[binkw32.BinkSetFileOffset] } }
extern "C" __declspec(naked) void __stdcall _BinkSetFrameRate(int, int) { _asm { jmp[binkw32.BinkSetFrameRate] } }
extern "C" __declspec(naked) void __stdcall _BinkSetIO(int) { _asm { jmp[binkw32.BinkSetIO] } }
extern "C" __declspec(naked) void __stdcall _BinkSetIOSize(int) { _asm { jmp[binkw32.BinkSetIOSize] } }
extern "C" __declspec(naked) void __stdcall _BinkSetIOSize8(int, int) { _asm { jmp[binkw32.BinkSetIOSize8] } }
extern "C" __declspec(naked) void __stdcall _BinkSetMemory(int, int) { _asm { jmp[binkw32.BinkSetMemory] } }
extern "C" __declspec(naked) void __stdcall _BinkSetMixBinVolumes(int, int, int, int, int) { _asm { jmp[binkw32.BinkSetMixBinVolumes] } }
extern "C" __declspec(naked) void __stdcall _BinkSetMixBins(int, int, int, int) { _asm { jmp[binkw32.BinkSetMixBins] } }
extern "C" __declspec(naked) void __stdcall _BinkSetOSFileCallbacks(int, int, int, int) { _asm { jmp[binkw32.BinkSetOSFileCallbacks] } }
extern "C" __declspec(naked) void __stdcall _BinkSetPan(int, int, int) { _asm { jmp[binkw32.BinkSetPan] } }
extern "C" __declspec(naked) void __stdcall _BinkSetSimulate(int) { _asm { jmp[binkw32.BinkSetSimulate] } }
extern "C" __declspec(naked) void __stdcall _BinkSetSoundOnOff(int, int) { _asm { jmp[binkw32.BinkSetSoundOnOff] } }
extern "C" __declspec(naked) void __stdcall _BinkSetSoundSystem(int, int) { _asm { jmp[binkw32.BinkSetSoundSystem] } }
extern "C" __declspec(naked) void __stdcall _BinkSetSoundSystem2(int, int, int) { _asm { jmp[binkw32.BinkSetSoundSystem2] } }
extern "C" __declspec(naked) void __stdcall _BinkSetSoundTrack(int, int) { _asm { jmp[binkw32.BinkSetSoundTrack] } }
extern "C" __declspec(naked) void __stdcall _BinkSetSpeakerVolumes(int, int, int, int, int) { _asm { jmp[binkw32.BinkSetSpeakerVolumes] } }
extern "C" __declspec(naked) void __stdcall _BinkSetVideoOnOff(int, int) { _asm { jmp[binkw32.BinkSetVideoOnOff] } }
extern "C" __declspec(naked) void __stdcall _BinkSetVolume(int, int, int) { _asm { jmp[binkw32.BinkSetVolume] } }
extern "C" __declspec(naked) void __stdcall _BinkSetWillLoop(int, int) { _asm { jmp[binkw32.BinkSetWillLoop] } }
extern "C" __declspec(naked) void __stdcall _BinkShouldSkip(int) { _asm { jmp[binkw32.BinkShouldSkip] } }
extern "C" __declspec(naked) void __stdcall _BinkStartAsyncThread(int, int) { _asm { jmp[binkw32.BinkStartAsyncThread] } }
extern "C" __declspec(naked) void __stdcall _BinkUseTelemetry(int) { _asm { jmp[binkw32.BinkUseTelemetry] } }
extern "C" __declspec(naked) void __stdcall _BinkUseTmLite(int) { _asm { jmp[binkw32.BinkUseTmLite] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilCPUs() { _asm { jmp[binkw32.BinkUtilCPUs] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilFree(int) { _asm { jmp[binkw32.BinkUtilFree] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilMalloc(int, int) { _asm { jmp[binkw32.BinkUtilMalloc] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilMutexCreate(int, int) { _asm { jmp[binkw32.BinkUtilMutexCreate] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilMutexDestroy(int) { _asm { jmp[binkw32.BinkUtilMutexDestroy] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilMutexLock(int) { _asm { jmp[binkw32.BinkUtilMutexLock] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilMutexLockTimeOut(int, int) { _asm { jmp[binkw32.BinkUtilMutexLockTimeOut] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilMutexUnlock(int) { _asm { jmp[binkw32.BinkUtilMutexUnlock] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilSoundGlobalLock() { _asm { jmp[binkw32.BinkUtilSoundGlobalLock] } }
extern "C" __declspec(naked) void __stdcall _BinkUtilSoundGlobalUnlock() { _asm { jmp[binkw32.BinkUtilSoundGlobalUnlock] } }
extern "C" __declspec(naked) void __stdcall _BinkWait(int) { _asm { jmp[binkw32.BinkWait] } }
extern "C" __declspec(naked) void __stdcall _BinkWaitStopAsyncThread(int) { _asm { jmp[binkw32.BinkWaitStopAsyncThread] } }
extern "C" __declspec(naked) void __stdcall _BinkWaitStopAsyncThreadsMulti(int, int) { _asm { jmp[binkw32.BinkWaitStopAsyncThreadsMulti] } }
extern "C" __declspec(naked) void __stdcall _RADSetMemory(int, int) { _asm { jmp[binkw32.RADSetMemory] } }
extern "C" __declspec(naked) void __stdcall _RADTimerRead() { _asm { jmp[binkw32.RADTimerRead] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16a1bpp40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16a1bpp40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16a1bpp52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16a1bpp52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16a1bpp_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16a1bpp_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16a1bpp_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16a1bpp_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16a4bpp40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16a4bpp40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16a4bpp52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16a4bpp52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16a4bpp_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16a4bpp_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16a4bpp_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16a4bpp_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16bpp40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16bpp40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16bpp48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16bpp48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16bpp52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16bpp52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16bpp_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16bpp_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16bpp_mask56(int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16bpp_mask56] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_16bpp_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_16bpp_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24bpp40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24bpp40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24bpp48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24bpp48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24bpp52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24bpp52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24bpp_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24bpp_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24bpp_mask56(int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24bpp_mask56] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24bpp_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24bpp_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24rbpp40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24rbpp40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24rbpp48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24rbpp48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24rbpp52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24rbpp52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24rbpp_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24rbpp_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24rbpp_mask56(int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24rbpp_mask56] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_24rbpp_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_24rbpp_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32abpp40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32abpp40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32abpp52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32abpp52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32abpp_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32abpp_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32abpp_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32abpp_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32bpp40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32bpp40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32bpp48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32bpp48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32bpp52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32bpp52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32bpp_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32bpp_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32bpp_mask56(int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32bpp_mask56] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32bpp_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32bpp_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rabpp40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rabpp40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rabpp52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rabpp52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rabpp_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rabpp_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rabpp_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rabpp_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rbpp40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rbpp40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rbpp48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rbpp48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rbpp52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rbpp52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rbpp_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rbpp_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rbpp_mask56(int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rbpp_mask56] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_32rbpp_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_32rbpp_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_UYVY40(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_UYVY40] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_UYVY48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_UYVY48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_UYVY52(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_UYVY52] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_UYVY_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_UYVY_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_UYVY_mask56(int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_UYVY_mask56] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_UYVY_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_UYVY_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_YUY240(int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_YUY240] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_YUY248(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_YUY248] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_YUY252(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_YUY252] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_YUY2_mask48(int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_YUY2_mask48] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_YUY2_mask56(int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_YUY2_mask56] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_YUY2_mask60(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_YUY2_mask60] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_YV1244(int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_YV1244] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_YV1252(int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_YV1252] } }
extern "C" __declspec(naked) void __stdcall _YUV_blit_YV1256(int, int, int, int, int, int, int, int, int, int, int, int, int, int) { _asm { jmp[binkw32.YUV_blit_YV1256] } }
extern "C" __declspec(naked) void __stdcall _YUV_init4(int) { _asm { jmp[binkw32.YUV_init4] } }
extern "C" __declspec(naked) void __stdcall _radfree(int) { _asm { jmp[binkw32.radfree] } }
extern "C" __declspec(naked) void __stdcall _radmalloc(int) { _asm { jmp[binkw32.radmalloc] } }
