#pragma once
#define Handle STHandle
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <time.h>
typedef uint8_t Byte, UInt8, Boolean;
typedef uint16_t Word, UInt16, UInt;
typedef int16_t SWord, Int16, Short, Coord;
typedef uint32_t DWord, UInt32, ULong;
typedef int32_t Int32, Long;
typedef char Char, *CharPtr, *Ptr;
typedef void *VoidPtr, *Handle, *VoidHand, *MemHandle, *DmOpenRef;
typedef uintptr_t LocalID;
typedef int Err, FontID;
typedef struct {Coord x,y;} PointType;
typedef struct {PointType topLeft,extent;} RectangleType;
typedef struct {int width,height,id;} BitmapType,*BitmapPtr;
typedef struct {int eType; Boolean penDown; Coord screenX,screenY; union {struct {Word controlID; void *pControl; Boolean on;} ctlSelect; struct {Word formID;} frmLoad; struct {Word chr,keyCode,modifiers;} keyDown; struct {Word itemID;} menu; } data;} EventType,*EventPtr;
typedef struct Object {struct {int frame;} attr; int id,type,x,y,w,h,font,value,style; Boolean hidden; char text[2048]; Handle handle; void *native;} Object,*ControlPtr;
typedef struct Form {int id,x,y,w,h,menu,def; char title[256]; Object objects[180]; int count; Boolean (*handler)(EventPtr);} FormType,*FormPtr;
typedef struct {int second,minute,hour,day,month,year,weekDay;} DateTimeType;
enum {nilEvent,penDownEvent,penUpEvent,keyDownEvent,ctlSelectEvent,menuEvent,frmLoadEvent,frmOpenEvent,frmUpdateEvent,frmCloseEvent,appStopEvent,winEnterEvent};
enum {stdFont,boldFont,largeFont,symbolFont,rectangleButtonFrame=1,dmModeReadWrite=3,noPreferenceFound=-1,evtWaitForever=-1,winScreenModeGet,winScreenModeSet,winScreenModeGetSupportedDepths,winScreenModeSetToDefaults,sysFtrCreator=1,sysFtrNumROMVersion=1,sysROMStageRelease=0,sysAppLaunchCmdNormalLaunch=0,sysAppLaunchFlagNewGlobals=1,sysAppLaunchFlagUIApp=2,sysErrRomIncompatible=1,sysAppLaunchCmdSaveData=1};
#define sysMakeROMVersion(a,b,c,d,e) ((a)<<24|(b)<<20)
#define bitmapRsc 'Tbmp'
#define strRsc 'tSTR'
#define vchrPageUp 11
#define vchrPageDown 12
#define vchrHard1 13
#define vchrHard2 14
#define vchrHard3 15
#define vchrHard4 16
#define StrLen strlen
#define StrCopy strcpy
#define StrNCopy strncpy
#define StrCat strcat
#define StrCompare strcmp
#define StrNCompare strncmp
#define StrCaselessCompare strcasecmp
#define StrAToI atol
#define StrPrintF sprintf
#define MemMove memmove
#define MemSet(p,n,v) memset(p,v,n)
char *StrIToA(char *,long);
char *StrToLower(char *,const char *);
FormPtr FrmGetActiveForm(void),FrmGetFormPtr(int),FrmInitForm(int);
void *FrmGetObjectPtr(FormPtr,int);
int FrmGetObjectIndex(FormPtr,int),FrmGetActiveFormID(void),FrmDoDialog(FormPtr);
char *FrmGetTitle(FormPtr),*CtlGetLabel(ControlPtr);
Handle MemHandleNew(size_t),DmGetResource(int,int),DmGetRecord(DmOpenRef,int),DmNewRecord(DmOpenRef,UInt*,size_t);
void *MemHandleLock(Handle);
DmOpenRef DmOpenDatabase(int,LocalID,int);
LocalID DmFindDatabase(int,const char*);
int FrmAlert(int),FrmCustomAlert(int,const char*,const char*,const char*);
int SysRandom(int),SysTicksPerSecond(void);
UInt32 TimGetSeconds(void),TimGetTicks(void),TimDateTimeToSeconds(DateTimeType*);
int FntSetFont(int);

typedef Boolean FormEventHandlerType(EventPtr);
#define chrPageUp vchrPageUp
#define chrPageDown vchrPageDown
#define hard1Chr vchrHard1
#define hard2Chr vchrHard2
#define hard3Chr vchrHard3
#define hard4Chr vchrHard4
#define winScreenModeGetSupportsColor 99
#define sysFileCDefaultApp 0
#define errNone 0
#define dmHdrAttrBackup 8
void FrmSetActiveForm(FormPtr),FrmSetEventHandler(FormPtr,FormEventHandlerType*),FrmDrawForm(FormPtr),FrmDeleteForm(FormPtr),FrmGotoForm(int),FrmCloseAllForms(void);
Boolean FrmDispatchEvent(EventPtr),SysHandleEvent(EventPtr),MenuHandleEvent(void*,EventPtr,Word*);
void FrmCopyLabel(FormPtr,int,const char*),FrmCopyTitle(FormPtr,const char*),FrmSetTitle(FormPtr,const char*),FrmHideObject(FormPtr,int),FrmShowObject(FormPtr,int),FrmSetFocus(FormPtr,int),FrmSetMenu(FormPtr,int),FrmHelp(int);
void CtlDrawControl(ControlPtr),CtlShowControl(ControlPtr),CtlSetLabel(ControlPtr,const char*),CtlSetValue(ControlPtr,int);
int CtlGetValue(ControlPtr),LstGetSelection(ControlPtr);
void LstSetSelection(ControlPtr,int),FldSetTextHandle(void*,Handle);
void MemHandleFree(Handle),MemHandleUnlock(Handle),MemPtrUnlock(void*);
void EvtGetEvent(EventPtr,int),EvtAddEventToQueue(EventPtr),MenuEraseStatus(int),GrfSetState(int,int,int);
void WinDrawChars(const char*,int,int,int),WinDrawBitmap(BitmapPtr,int,int),WinDrawLine(int,int,int,int),WinDrawRectangle(RectangleType*,int),WinEraseRectangle(RectangleType*,int),WinSetClip(RectangleType*),WinResetClip(void);
int WinScreenMode(int,UInt32*,UInt32*,UInt32*,Boolean*),FtrGet(int,int,UInt32*),AppLaunchWithCommand(int,int,void*);
void BmpGetDimensions(BitmapPtr,Coord*,Coord*,UInt16*),FntCharsInWidth(const char*,Int16*,Int16*,Boolean*),SysCopyStringResource(char*,int),SysStringByIndex(int,int,char*,int);
int PrefGetAppPreferences(int,int,void*,Word*,Boolean);
void PrefSetAppPreferences(int,int,int,const void*,size_t,Boolean);
int DmCreateDatabase(int,const char*,int,int,Boolean),DmWrite(void*,size_t,const void*,size_t),DmCloseDatabase(DmOpenRef),DmReleaseRecord(DmOpenRef,int,Boolean),DmReleaseResource(Handle);
int DmDatabaseInfo(int,LocalID,char*,Word*,void*,void*,void*,void*,void*,void*,void*,void*,void*),DmSetDatabaseInfo(int,LocalID,char*,Word*,void*,void*,void*,void*,void*,void*,void*,void*,void*),DmOpenDatabaseInfo(DmOpenRef,LocalID*,void*,void*,void*,void*);
