// SPDX-License-Identifier: GPL-3.0-or-later
// Native iOS compatibility layer for Pieter Spronck's Space Trader 1.2.2.
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include "PalmOS.h"
#include "external.h"
#undef min
#undef max
static NSMutableDictionary *handleSizes;
static NSDictionary *resources;
static NSMutableArray<NSData*> *events;
static FormPtr active;
static NSMutableDictionary *forms;
static CGContextRef canvas;
static int fontID,dialogDepth;
static BOOL dirty,clipSet;
static UIViewController *controller;
static UIImageView *screenView;
static NSMutableDictionary *images;
static NSString *saveDirectory;
static UIFont *font(int f) {return [UIFont systemFontOfSize:f==2?12:8 weight:f==1?UIFontWeightSemibold:UIFontWeightRegular];}
static NSString *str(const char *s){return s?([NSString stringWithUTF8String:s]?:[NSString stringWithCString:s encoding:NSMacOSRomanStringEncoding]):@"";}
static void copy(char *d,NSString*s,size_t n){snprintf(d,n,"%s",s.UTF8String?:"");}
static void pushContext(void){UIGraphicsPushContext(canvas);}
static void popContext(void){UIGraphicsPopContext();dirty=YES;}
static void textAt(NSString *s,int x,int y,int f){pushContext();[s drawAtPoint:CGPointMake(x,y-1) withAttributes:@{NSFontAttributeName:font(f),NSForegroundColorAttributeName:UIColor.blackColor}];popContext();}
static void fill(CGRect r,UIColor*c){CGContextSetFillColorWithColor(canvas,c.CGColor);CGContextFillRect(canvas,r);dirty=YES;}
static UIImage *bitmap(int n){NSString*k=[NSString stringWithFormat:@"%d",n];UIImage*i=images[k];if(!i){i=[UIImage imageWithContentsOfFile:[NSBundle.mainBundle pathForResource:k ofType:@"png"]];if(i)images[k]=i;}return i;}
static void updateDashboard(void);
static void flush(void){if(!dirty)return;CGImageRef im=CGBitmapContextCreateImage(canvas);screenView.image=[UIImage imageWithCGImage:im];CGImageRelease(im);dirty=NO;updateDashboard();}
static void enqueue(EventType e){[events addObject:[NSData dataWithBytes:&e length:sizeof(e)]];}
static void pump(void){flush();[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:.02]];}
static void drawObject(FormPtr f,Object*o){if(!f||o->hidden)return;int x=f->x+o->x,y=f->y+o->y;
 if(o->type==8){textAt(str(o->text),x,y,o->font);return;}
 if(o->type==4){UIImage*i=bitmap(o->id);pushContext();[i drawAtPoint:CGPointMake(x,y)];popContext();return;}
 if(o->type==0){fill(CGRectMake(x,y,o->w,o->h),UIColor.whiteColor);CGContextSetStrokeColorWithColor(canvas,UIColor.lightGrayColor.CGColor);CGContextStrokeRect(canvas,CGRectMake(x,y+o->h-1,o->w,.3));textAt(str(o->handle?:o->text),x,y,o->font);return;}
 if(o->type!=1)return;
 fill(CGRectMake(x-1,y-1,o->w+2,o->h+2),UIColor.whiteColor);
 pushContext();CGContextSetStrokeColorWithColor(canvas,[UIColor colorWithRed:.34 green:.44 blue:.52 alpha:1].CGColor);CGContextSetLineWidth(canvas,.35);
 if(o->style==2){CGContextStrokeRect(canvas,CGRectMake(x,y+2,7,7));if(o->value){CGContextMoveToPoint(canvas,x+1,y+5);CGContextAddLineToPoint(canvas,x+3,y+8);CGContextAddLineToPoint(canvas,x+7,y+2);CGContextStrokePath(canvas);}textAt(str(o->text),x+10,y,o->font);}
 else {if(o->style==0||o->style==1){UIBezierPath*b=[UIBezierPath bezierPathWithRoundedRect:CGRectMake(x,y,o->w,o->h) cornerRadius:1.5];[[UIColor colorWithRed:.89 green:.94 blue:.97 alpha:1]setFill];[b fill];[b stroke];}NSString*s=str(o->text);CGSize z=[s sizeWithAttributes:@{NSFontAttributeName:font(o->font)}];textAt(s,x+(o->w-z.width)/2,y+(o->h-9)/2,o->font);}
 popContext();}
FormPtr FrmGetActiveForm(void){return active;}
FormPtr FrmGetFormPtr(int n){return [forms[@(n)] pointerValue];}
FormPtr FrmInitForm(int n){NSDictionary*d=resources[@"forms"][[NSString stringWithFormat:@"%d",n]];if(!d){NSLog(@"Missing form %d",n);abort();}FormPtr f=calloc(1,sizeof(*f));f->id=n;f->x=[d[@"x"] intValue];f->y=[d[@"y"] intValue];f->w=[d[@"w"] intValue];f->h=[d[@"h"] intValue];f->def=[d[@"default"] intValue];f->menu=[d[@"menu"] intValue];copy(f->title,d[@"title"],sizeof(f->title));for(NSDictionary*q in d[@"objects"]){Object*o=&f->objects[f->count++];o->id=[q[@"id"] intValue];o->type=[q[@"type"] intValue];o->x=[q[@"x"] intValue];o->y=[q[@"y"] intValue];o->w=[q[@"w"] intValue];o->h=[q[@"h"] intValue];o->font=[q[@"font"] intValue];o->style=[q[@"style"] intValue];o->hidden=[q[@"hidden"] boolValue];copy(o->text,q[@"text"],sizeof(o->text));}forms[@(n)]=[NSValue valueWithPointer:f];return f;}
void FrmSetActiveForm(FormPtr f){active=f;}
int FrmGetActiveFormID(void){return active?active->id:0;}
void FrmSetEventHandler(FormPtr f,FormEventHandlerType*h){f->handler=h;}
int FrmGetObjectIndex(FormPtr f,int id){for(int i=0;i<f->count;i++)if(f->objects[i].id==id)return i;NSLog(@"Missing object %d in %d",id,f->id);return -1;}
void *FrmGetObjectPtr(FormPtr f,int i){return f&&i>=0&&i<f->count?&f->objects[i]:NULL;}
void FrmDrawForm(FormPtr f){if(!f)return;if(!dialogDepth)fill(CGRectMake(0,0,160,160),UIColor.whiteColor);fill(CGRectMake(f->x,f->y,f->w,f->h),UIColor.whiteColor);if(f->x||f->y){CGContextSetStrokeColorWithColor(canvas,UIColor.blackColor.CGColor);CGContextStrokeRect(canvas,CGRectMake(f->x-.5,f->y-.5,f->w+1,f->h+1));}if(f->title[0]){textAt(str(f->title),f->x+2,f->y+1,1);WinDrawLine(f->x,f->y+12,f->x+f->w-1,f->y+12);}for(int i=0;i<f->count;i++)drawObject(f,&f->objects[i]);dirty=YES;}
void FrmDeleteForm(FormPtr f){if(!f)return;if([forms[@(f->id)] pointerValue]==f)[forms removeObjectForKey:@(f->id)];if(active==f)active=NULL;free(f);}
void FrmGotoForm(int n){EventType e={0};e.eType=frmLoadEvent;e.data.frmLoad.formID=n;enqueue(e);e.eType=frmOpenEvent;enqueue(e);}
Boolean FrmDispatchEvent(EventPtr e){return active&&active->handler?active->handler(e):false;}
void FrmCloseAllForms(void){}
void FrmCopyLabel(FormPtr f,int id,const char*s){Object*o=FrmGetObjectPtr(f,FrmGetObjectIndex(f,id));if(o){if(f==active)fill(CGRectMake(f->x+o->x,f->y+o->y,MAX(1,strlen(o->text)*5),10),UIColor.whiteColor);copy(o->text,str(s),sizeof(o->text));if(f==active)drawObject(f,o);}}
void FrmCopyTitle(FormPtr f,const char*s){copy(f->title,str(s),sizeof(f->title));if(f!=active)return;fill(CGRectMake(f->x,f->y,f->w,12),UIColor.whiteColor);textAt(str(s),f->x+2,f->y+1,1);}
void FrmSetTitle(FormPtr f,const char*s){FrmCopyTitle(f,s);}
char *FrmGetTitle(FormPtr f){return f->title;}
void FrmHideObject(FormPtr f,int i){Object*o=FrmGetObjectPtr(f,i);if(o){o->hidden=YES;if(f!=active)return;fill(CGRectMake(f->x+o->x-1,f->y+o->y-1,o->w?o->w+2:strlen(o->text)*5+2,o->h?o->h+2:11),UIColor.whiteColor);}}
void FrmShowObject(FormPtr f,int i){Object*o=FrmGetObjectPtr(f,i);if(o){o->hidden=NO;if(f==active)drawObject(f,o);}}
void FrmSetFocus(FormPtr f,int i){}
void FrmSetMenu(FormPtr f,int n){f->menu=n;}
void CtlDrawControl(ControlPtr o){if(active&&(uintptr_t)o>=(uintptr_t)active->objects&&(uintptr_t)o<(uintptr_t)(active->objects+active->count))drawObject(active,o);}
void CtlShowControl(ControlPtr o){if(o){o->hidden=NO;CtlDrawControl(o);}}
void CtlSetLabel(ControlPtr o,const char*s){if(o){copy(o->text,str(s),sizeof(o->text));CtlDrawControl(o);}}
char *CtlGetLabel(ControlPtr o){return o->text;}
void CtlSetValue(ControlPtr o,int v){if(o){o->value=v;CtlDrawControl(o);}}
int CtlGetValue(ControlPtr o){return o?o->value:0;}
int LstGetSelection(ControlPtr o){return CtlGetValue(o);}
void LstSetSelection(ControlPtr o,int n){if(o)o->value=n;}
void FldSetTextHandle(void*p,Handle h){Object*o=p;if(o){o->handle=h;CtlDrawControl(o);}}
int FrmDoDialog(FormPtr f){FormPtr previous=active;CGImageRef behind=CGBitmapContextCreateImage(canvas);active=f;dialogDepth++;FrmDrawForm(f);int result=f->def;for(;;){EventType e;EvtGetEvent(&e,evtWaitForever);if(f->handler&&f->handler(&e))continue;if(e.eType==ctlSelectEvent){Object*o=FrmGetObjectPtr(f,FrmGetObjectIndex(f,e.data.ctlSelect.controlID));if(o&&o->style==0){result=o->id;break;}}}dialogDepth--;active=previous;CGContextSaveGState(canvas);CGContextTranslateCTM(canvas,0,160);CGContextScaleCTM(canvas,1,-1);CGContextDrawImage(canvas,CGRectMake(0,0,160,160),behind);CGContextRestoreGState(canvas);CGImageRelease(behind);dirty=YES;return result;}
int FrmCustomAlert(int n,const char*a,const char*b,const char*c){NSDictionary*d=resources[@"alerts"][[NSString stringWithFormat:@"%d",n]];NSString*msg=d[@"message"]?:[NSString stringWithFormat:@"Message %d",n];NSArray*subs=@[str(a),str(b),str(c)];for(int i=0;i<3;i++)msg=[msg stringByReplacingOccurrencesOfString:[NSString stringWithFormat:@"^%d",i] withString:subs[i]];UIAlertController*alert=[UIAlertController alertControllerWithTitle:d[@"title"]?:@"Space Trader" message:msg preferredStyle:UIAlertControllerStyleAlert];__block int result=-1;NSArray*buttons=d[@"buttons"]?:@[@"OK"];for(int i=0;i<buttons.count;i++){[alert addAction:[UIAlertAction actionWithTitle:buttons[i] style:UIAlertActionStyleDefault handler:^(UIAlertAction*x){result=i;}]];}dialogDepth++;flush();[controller presentViewController:alert animated:YES completion:nil];
#if TARGET_OS_SIMULATOR
if([NSProcessInfo.processInfo.arguments containsObject:@"--selftest"])[NSTimer scheduledTimerWithTimeInterval:.3 repeats:NO block:^(NSTimer*t){result=0;[alert dismissViewControllerAnimated:NO completion:nil];}];
#endif
while(result<0)pump();while(controller.presentedViewController)pump();dialogDepth--;return result;}
int FrmAlert(int n){return FrmCustomAlert(n,"","","");}
void FrmHelp(int n){NSString*s=resources[@"strings"][[NSString stringWithFormat:@"%d",n]]?:@"Tap Menu for the original game commands. Tap a field to enter text. Use Up and Down to scroll.";UIAlertController*a=[UIAlertController alertControllerWithTitle:@"Space Trader Help" message:s preferredStyle:UIAlertControllerStyleAlert];[a addAction:[UIAlertAction actionWithTitle:@"Done" style:UIAlertActionStyleCancel handler:nil]];[controller presentViewController:a animated:YES completion:nil];}
void EvtAddEventToQueue(EventPtr e){enqueue(*e);}
void EvtGetEvent(EventPtr e,int ticks){flush();if(!dialogDepth&&CurForm!=MainForm&&CurForm!=NewCommanderForm&&NameCommander[0])SaveGame(0);CFTimeInterval end=CACurrentMediaTime()+(ticks<0?1e12:ticks/100.0);while(!events.count&&CACurrentMediaTime()<end)pump();memset(e,0,sizeof(*e));if(events.count){[events[0] getBytes:e length:sizeof(*e)];[events removeObjectAtIndex:0];if(e->eType==frmLoadEvent&&active&&!dialogDepth){FormPtr old=active;if(old->handler){EventType close={.eType=frmCloseEvent};old->handler(&close);}FrmDeleteForm(old);}}}
Boolean SysHandleEvent(EventPtr e){return false;}
Boolean MenuHandleEvent(void*p,EventPtr e,Word*err){return false;}
void MenuEraseStatus(int n){}
void GrfSetState(int a,int b,int c){}
void WinDrawChars(const char*s,int n,int x,int y){NSData*d=[NSData dataWithBytes:s length:MAX(0,n)];NSString*t=[[NSString alloc]initWithData:d encoding:NSUTF8StringEncoding]?:[[NSString alloc]initWithData:d encoding:NSMacOSRomanStringEncoding];textAt(t?:@"",x,y,fontID);}
void WinDrawBitmap(BitmapPtr p,int x,int y){if(!p)return;pushContext();[bitmap(p->id) drawAtPoint:CGPointMake(x,y)];popContext();}
void WinDrawLine(int x,int y,int xx,int yy){CGContextSetStrokeColorWithColor(canvas,UIColor.blackColor.CGColor);CGContextSetLineWidth(canvas,.7);CGContextMoveToPoint(canvas,x+.5,y+.5);CGContextAddLineToPoint(canvas,xx+.5,yy+.5);CGContextStrokePath(canvas);dirty=YES;}
void WinDrawRectangle(RectangleType*r,int c){fill(CGRectMake(r->topLeft.x,r->topLeft.y,r->extent.x,r->extent.y),UIColor.blackColor);}
void WinEraseRectangle(RectangleType*r,int c){fill(CGRectMake(r->topLeft.x,r->topLeft.y,r->extent.x,r->extent.y),UIColor.whiteColor);}
void WinResetClip(void){if(clipSet){CGContextRestoreGState(canvas);clipSet=NO;}}
void WinSetClip(RectangleType*r){WinResetClip();CGContextSaveGState(canvas);clipSet=YES;CGContextClipToRect(canvas,CGRectMake(r->topLeft.x,r->topLeft.y,r->extent.x,r->extent.y));}
int WinScreenMode(int op,UInt32*w,UInt32*h,UInt32*d,Boolean*c){if(w)*w=160;if(h)*h=160;if(d)*d=op==winScreenModeGetSupportedDepths?0xff:8;if(c)*c=true;return 0;}
int FtrGet(int a,int b,UInt32*v){*v=sysMakeROMVersion(4,0,0,0,0);return 0;}
int AppLaunchWithCommand(int a,int b,void*c){return 0;}
void BmpGetDimensions(BitmapPtr p,Coord*w,Coord*h,UInt16*r){if(w)*w=p?p->width:0;if(h)*h=p?p->height:0;if(r)*r=p?p->width:0;}
int FntSetFont(int n){int old=fontID;fontID=n;return old;}
void FntCharsInWidth(const char*s,Int16*w,Int16*n,Boolean*fits){int count=0;CGFloat width=0;for(;count<*n&&s[count];count++){char ch[2]={s[count],0};CGFloat z=[str(ch) sizeWithAttributes:@{NSFontAttributeName:font(fontID)}].width;if(width+z>*w)break;width+=z;}*fits=count==*n||s[count]==0;*n=count;*w=ceil(width);}
char *StrIToA(char*p,long n){sprintf(p,"%ld",n);return p;}
char *StrToLower(char*d,const char*s){for(int i=0;;i++){d[i]=tolower((unsigned char)s[i]);if(!s[i])break;}return d;}
int SysRandom(int seed){if(seed)srandom(seed);return (int)(random()&0x7fff);}
int SysTicksPerSecond(void){return 100;}
UInt32 TimGetTicks(void){return (UInt32)(CACurrentMediaTime()*100);}
UInt32 TimGetSeconds(void){return (UInt32)time(NULL);}
UInt32 TimDateTimeToSeconds(DateTimeType*d){struct tm t={.tm_year=d->year-1900,.tm_mon=d->month-1,.tm_mday=d->day,.tm_hour=d->hour,.tm_min=d->minute,.tm_sec=d->second};return (UInt32)mktime(&t);}
void SysCopyStringResource(char*d,int n){strcpy(d,[resources[@"strings"][[NSString stringWithFormat:@"%d",n]] UTF8String]?:"");}
void SysStringByIndex(int n,int i,char*d,int max){NSArray*a=resources[@"lists"][[NSString stringWithFormat:@"%d",n]];copy(d,i>=0&&i<a.count?a[i]:@"",max);}
Handle MemHandleNew(size_t n){void*p=calloc(1,n);handleSizes[[NSValue valueWithPointer:p]]=@(n);return p;}
void *MemHandleLock(Handle h){return h;}
void MemHandleUnlock(Handle h){}
void MemPtrUnlock(void*p){}
void MemHandleFree(Handle h){[handleSizes removeObjectForKey:[NSValue valueWithPointer:h]];free(h);}
Handle DmGetResource(int type,int n){if(type==bitmapRsc){BitmapPtr p=calloc(1,sizeof(*p));UIImage*i=bitmap(n);p->id=n;p->width=i.size.width;p->height=i.size.height;return p;}NSString*s=resources[@"strings"][[NSString stringWithFormat:@"%d",n]];return strdup(s.UTF8String?:"");}
int DmReleaseResource(Handle h){free(h);return 0;}
static NSString *prefPath(int n){return [saveDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"preferences-%d.dat",n]];}
int PrefGetAppPreferences(int creator,int n,void*p,Word*size,Boolean saved){NSData*d=[NSData dataWithContentsOfFile:prefPath(n)];if(d.length!=*size)return noPreferenceFound;memcpy(p,d.bytes,*size);return 1;}
void PrefSetAppPreferences(int creator,int n,int version,const void*p,size_t size,Boolean saved){[[NSData dataWithBytes:p length:size] writeToFile:prefPath(n) atomically:YES];}
typedef struct {char name[128];void*data;size_t size;} DB;
static DB dbs[16];static int dbCount;
static NSString *dbPath(DB*d){return [saveDirectory stringByAppendingPathComponent:[str(d->name) stringByAppendingString:@".dat"]];}
LocalID DmFindDatabase(int card,const char*name){for(int i=0;i<dbCount;i++)if(!strcmp(name,dbs[i].name))return i+1;DB tmp={0};snprintf(tmp.name,sizeof(tmp.name),"%s",name);NSData*d=[NSData dataWithContentsOfFile:dbPath(&tmp)];if(!d)return 0;DB*q=&dbs[dbCount++];*q=tmp;q->size=d.length;q->data=malloc(MAX(1,d.length));memcpy(q->data,d.bytes,d.length);return dbCount;}
int DmCreateDatabase(int card,const char*name,int c,int t,Boolean res){if(dbCount>=16)return 1;DB*d=&dbs[dbCount++];snprintf(d->name,sizeof(d->name),"%s",name);return 0;}
DmOpenRef DmOpenDatabase(int card,LocalID id,int mode){return id&&id<=dbCount?&dbs[id-1]:NULL;}
Handle DmGetRecord(DmOpenRef ref,int index){return ref?((DB*)ref)->data:NULL;}
Handle DmNewRecord(DmOpenRef ref,UInt*i,size_t n){DB*d=ref;free(d->data);d->size=n;d->data=calloc(1,n);return d->data;}
int DmWrite(void*p,size_t o,const void*s,size_t n){memcpy((char*)p+o,s,n);return 0;}
int DmReleaseRecord(DmOpenRef ref,int n,Boolean dirtyFlag){DB*d=ref;if(d&&dirtyFlag)return [[NSData dataWithBytes:d->data length:d->size] writeToFile:dbPath(d) atomically:YES]?0:1;return 0;}
int DmCloseDatabase(DmOpenRef ref){return DmReleaseRecord(ref,0,true);}
int DmDatabaseInfo(int card,LocalID id,char*n,Word*a,void*b,void*c,void*d,void*e,void*f,void*g,void*h,void*i,void*j){if(a)*a=0;return 0;}
int DmSetDatabaseInfo(int card,LocalID id,char*n,Word*a,void*b,void*c,void*d,void*e,void*f,void*g,void*h,void*i,void*j){return 0;}
int DmOpenDatabaseInfo(DmOpenRef ref,LocalID*id,void*a,void*b,void*c,void*d){if(id)*id=((DB*)ref-dbs)+1;return 0;}
@interface GameController:UIViewController
@end
@implementation GameController
- (UILabel*)label:(NSInteger)tag size:(CGFloat)size color:(UIColor*)color {UILabel*l=[UILabel new];l.tag=tag;l.font=[UIFont systemFontOfSize:size weight:UIFontWeightMedium];l.textColor=color;l.adjustsFontSizeToFitWidth=YES;l.minimumScaleFactor=.7;[self.view addSubview:l];return l;}
- (void)viewDidLoad {
 [super viewDidLoad];self.view.backgroundColor=[UIColor colorWithRed:.89 green:.92 blue:.93 alpha:1];
 UIVisualEffect*material=[UIBlurEffect effectWithStyle:UIBlurEffectStyleSystemMaterial];if(@available(iOS 26.0,*)){material=[UIGlassEffect effectWithStyle:UIGlassEffectStyleRegular];}UIView*header=[[UIVisualEffectView alloc]initWithEffect:material];header.tag=980;header.layer.cornerRadius=24;header.clipsToBounds=YES;[self.view addSubview:header];
 UIImageView*icon=[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"AppIcon180"]];icon.tag=981;icon.layer.cornerRadius=9;icon.clipsToBounds=YES;[self.view addSubview:icon];
 [self label:982 size:23 color:UIColor.labelColor].text=@"Space Trader";
 [self label:983 size:11 color:UIColor.secondaryLabelColor].text=@"COMMANDER’S CONSOLE";
 UIView*panel=[UIView new];panel.tag=984;panel.backgroundColor=UIColor.whiteColor;panel.layer.cornerRadius=10;panel.layer.shadowColor=UIColor.blackColor.CGColor;panel.layer.shadowOpacity=.12;panel.layer.shadowRadius=8;panel.layer.shadowOffset=CGSizeMake(0,3);[self.view addSubview:panel];
 screenView=[UIImageView new];screenView.userInteractionEnabled=YES;screenView.backgroundColor=UIColor.whiteColor;screenView.layer.magnificationFilter=kCAFilterLinear;[self.view addSubview:screenView];[screenView addGestureRecognizer:[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tap:)]];
 [self label:985 size:13 color:[UIColor colorWithRed:.16 green:.33 blue:.41 alpha:1]];
 [self label:986 size:12 color:UIColor.darkGrayColor];
 NSArray*titles=@[@"Trade",@"Shipyard",@"Equipment",@"Charts",@"Commander",@"Bank"];
 for(int i=0;i<6;i++){UIButton*b=[UIButton buttonWithType:UIButtonTypeSystem];b.tag=910+i;[b setTitle:titles[i] forState:UIControlStateNormal];b.titleLabel.font=[UIFont systemFontOfSize:12 weight:UIFontWeightSemibold];b.tintColor=[UIColor colorWithRed:.12 green:.29 blue:.39 alpha:1];b.backgroundColor=[UIColor colorWithWhite:1 alpha:.8];b.layer.cornerRadius=6;[b addTarget:self action:@selector(navigate:) forControlEvents:UIControlEventTouchUpInside];if(@available(iOS 26.0,*)){UIButtonConfiguration*c=[UIButtonConfiguration glassButtonConfiguration];c.title=titles[i];c.contentInsets=NSDirectionalEdgeInsetsMake(6,4,6,4);c.titleTextAttributesTransformer=^NSDictionary*(NSDictionary*a){NSMutableDictionary*d=[a mutableCopy];d[NSFontAttributeName]=[UIFont systemFontOfSize:12 weight:UIFontWeightSemibold];return d;};c.baseForegroundColor=UIColor.labelColor;c.cornerStyle=UIButtonConfigurationCornerStyleCapsule;b.configuration=c;b.backgroundColor=UIColor.clearColor;}[self.view addSubview:b];}
 NSArray*commands=@[@"Menu",@"▲",@"▼",@"Help"];for(int i=0;i<4;i++){UIButton*b=[UIButton buttonWithType:UIButtonTypeSystem];b.tag=900+i;[b setTitle:commands[i] forState:UIControlStateNormal];b.tintColor=[UIColor colorWithRed:.12 green:.29 blue:.39 alpha:1];b.titleLabel.font=[UIFont systemFontOfSize:13 weight:UIFontWeightSemibold];[b addTarget:self action:@selector(toolbar:) forControlEvents:UIControlEventTouchUpInside];if(@available(iOS 26.0,*)){UIButtonConfiguration*c=[UIButtonConfiguration glassButtonConfiguration];c.title=commands[i];b.configuration=c;}[self.view addSubview:b];}
}
- (void)viewDidLayoutSubviews {
 [super viewDidLayoutSubviews];CGRect b=UIEdgeInsetsInsetRect(self.view.bounds,self.view.safeAreaInsets);BOOL wide=b.size.width>b.size.height;
 CGFloat x=b.origin.x+16,y=b.origin.y,w=b.size.width-32;
 [self.view viewWithTag:980].frame=CGRectMake(x-6,y+2,w+12,70);
 [self.view viewWithTag:981].frame=CGRectMake(x,y+12,48,48);
 [self.view viewWithTag:982].frame=CGRectMake(x+60,y+10,w-60,30);
 [self.view viewWithTag:983].frame=CGRectMake(x+61,y+41,w-60,16);
 CGFloat size=wide?MIN(b.size.height-100,w-210):MIN(w-20,b.size.height-300);size=MAX(120,MIN(520,size));
 CGFloat sx=wide?x+200+(w-200-size)/2:x+(w-size)/2,sy=y+90;
 screenView.frame=CGRectMake(sx,sy,size,size);[self.view viewWithTag:984].frame=CGRectInset(screenView.frame,-10,-10);
 CGFloat nx=wide?x:x,ny=wide?y+150:sy+size+84,nw=wide?176:w;
 for(int i=0;i<6;i++){int cols=wide?2:3;[self.view viewWithTag:910+i].frame=CGRectMake(nx+(i%cols)*(nw+6)/cols,ny+(i/cols)*48,(nw-(cols-1)*6)/cols,44);}
 CGFloat statY=wide?y+88:sy+size+24;[self.view viewWithTag:985].frame=CGRectMake(x,statY,wide?180:w,20);[self.view viewWithTag:986].frame=CGRectMake(x,statY+24,wide?180:w,20);
 CGFloat cmdY=wide?y+b.size.height-36:ny+100;for(int i=0;i<4;i++)[self.view viewWithTag:900+i].frame=CGRectMake(x+i*(wide?180:w)/4,cmdY,(wide?180:w)/4,32);
}
- (void)navigate:(UIButton*)sender {if(dialogDepth||self.presentedViewController||!active||active->menu!=1000)return;int items[]={1100,1102,1103,1110,1108,1106};EventType e={.eType=menuEvent};e.data.menu.itemID=items[sender.tag-910];enqueue(e);}
- (void)toolbar:(UIButton*)sender {if(self.presentedViewController)return;if(sender.tag==900){[self showMenu];return;}if(sender.tag==903){EventType e={.eType=menuEvent};e.data.menu.itemID=MenuHelpHelpCurrentScreen;enqueue(e);return;}EventType e={.eType=keyDownEvent};e.data.keyDown.chr=sender.tag==901?chrPageUp:chrPageDown;enqueue(e);}
- (void)showMenu {if(dialogDepth||!active)return;UIAlertController*m=[UIAlertController alertControllerWithTitle:@"Space Trader" message:nil preferredStyle:UIAlertControllerStyleActionSheet];NSArray *items= active->menu==1000?@[@[@"Buy cargo",@1100],@[@"Sell cargo",@1101],@[@"Shipyard",@1102],@[@"Buy equipment",@1103],@[@"Sell equipment",@1104],@[@"Personnel roster",@1105],@[@"Bank",@1106],@[@"System information",@1107],@[@"Commander status",@1108],@[@"Galactic chart",@1109],@[@"Short-range chart",@1110],@[@"Game…",@(-1)],@[@"Help…",@(-2)]]:@[@[@"Game…",@(-1)],@[@"Help…",@(-2)]];
 for(NSArray*item in items){[m addAction:[UIAlertAction actionWithTitle:item[0] style:UIAlertActionStyleDefault handler:^(UIAlertAction*a){int n=[item[1]intValue];if(n<0){dispatch_after(dispatch_time(DISPATCH_TIME_NOW,250*NSEC_PER_MSEC),dispatch_get_main_queue(),^{[self submenu:n];});}else{EventType e={.eType=menuEvent};e.data.menu.itemID=n;enqueue(e);}}]];}[m addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil]];m.popoverPresentationController.sourceView=[self.view viewWithTag:900];[self presentViewController:m animated:YES completion:nil];}
- (void)submenu:(int)group {NSArray*items=group==-1?@[@[@"New game",@1000],@[@"Save snapshot",@1010],@[@"Switch game",@1001],@[@"Options",@1004],@[@"Shortcuts",@1005],@[@"High scores",@1007],@[@"Clear high scores",@1008],@[@"Retire",@1002]]:@[@[@"Current screen",@1200],@[@"How to play",@1203],@[@"Trading",@1204],@[@"Traveling",@1205],@[@"Ship equipment",@1206],@[@"Skills",@1207],@[@"First steps",@1208],@[@"Acknowledgements",@1209],@[@"About original game",@1211],@[@"iPhone port credits",@(-3)]];UIAlertController*m=[UIAlertController alertControllerWithTitle:group==-1?@"Game":@"Help" message:nil preferredStyle:UIAlertControllerStyleActionSheet];for(NSArray*item in items)[m addAction:[UIAlertAction actionWithTitle:item[0] style:UIAlertActionStyleDefault handler:^(UIAlertAction*a){int n=[item[1]intValue];if(n==-3){dispatch_after(dispatch_time(DISPATCH_TIME_NOW,250*NSEC_PER_MSEC),dispatch_get_main_queue(),^{UIAlertController*c=[UIAlertController alertControllerWithTitle:@"Space Trader Classic" message:@"Original game © Pieter Spronck, with Sam Anderson and Samuel Goldstein. Original pictures © Alexander Lawrence. iPhone compatibility layer built for Wireproof, 2026. Original C gameplay and extracted Palm resources; native arm64, no emulator or ROM. GPL source and licenses accompany the IPA. Resource format references: PumpkinOS (GPLv3)." preferredStyle:UIAlertControllerStyleAlert];[c addAction:[UIAlertAction actionWithTitle:@"Done" style:UIAlertActionStyleCancel handler:nil]];[self presentViewController:c animated:YES completion:nil];});}else {EventType e={.eType=menuEvent};e.data.menu.itemID=n;enqueue(e);}}]];[m addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil]];m.popoverPresentationController.sourceView=[self.view viewWithTag:900];[self presentViewController:m animated:YES completion:nil];}
- (void)tap:(UITapGestureRecognizer*)gesture {if(!active||self.presentedViewController)return;CGPoint p=[gesture locationInView:screenView];p.x=p.x*160/screenView.bounds.size.width;p.y=p.y*160/screenView.bounds.size.height;FormPtr f=active;for(int i=f->count-1;i>=0;i--){Object*o=&f->objects[i];CGRect r=CGRectMake(f->x+o->x-1,f->y+o->y-1,o->w+2,o->h+2);if(o->hidden||!CGRectContainsPoint(r,p))continue;if(o->type==0){UIAlertController*a=[UIAlertController alertControllerWithTitle:@"Enter value" message:nil preferredStyle:UIAlertControllerStyleAlert];[a addTextFieldWithConfigurationHandler:^(UITextField*t){t.text=str(o->handle?:o->text);t.autocorrectionType=UITextAutocorrectionTypeNo;t.keyboardType=(f->id==NewCommanderForm||f->id==FindSystemForm)?UIKeyboardTypeDefault:UIKeyboardTypeNumbersAndPunctuation;}];[a addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil]];[a addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction*x){NSString*s=a.textFields.firstObject.text;if(o->handle){extern size_t handleSize(Handle);copy(o->handle,s,handleSize(o->handle));}else copy(o->text,s,sizeof(o->text));drawObject(f,o);flush();}]];[self presentViewController:a animated:YES completion:nil];return;}if(o->type==1){if(o->style==3){NSDictionary*fd=resources[@"forms"][[NSString stringWithFormat:@"%d",f->id]];int listID=0;for(NSDictionary*q in fd[@"objects"])if([q[@"type"]intValue]==10&&[q[@"id"]intValue]==o->id)listID=[q[@"list"]intValue];for(NSDictionary*q in fd[@"objects"])if([q[@"type"]intValue]==2&&[q[@"id"]intValue]==listID){Object*list=FrmGetObjectPtr(f,FrmGetObjectIndex(f,listID));UIAlertController*a=[UIAlertController alertControllerWithTitle:@"Choose shortcut" message:nil preferredStyle:UIAlertControllerStyleActionSheet];NSArray*items=q[@"items"];for(int k=0;k<items.count;k++)[a addAction:[UIAlertAction actionWithTitle:items[k] style:UIAlertActionStyleDefault handler:^(UIAlertAction*z){list->value=k;CtlSetLabel(o,[items[k] UTF8String]);flush();}]];[a addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil]];a.popoverPresentationController.sourceView=screenView;a.popoverPresentationController.sourceRect=CGRectMake(p.x*screenView.bounds.size.width/160,p.y*screenView.bounds.size.height/160,1,1);[self presentViewController:a animated:YES completion:nil];return;}}if(o->style==2){o->value=!o->value;drawObject(f,o);}EventType e={.eType=ctlSelectEvent};e.data.ctlSelect.controlID=o->id;e.data.ctlSelect.on=o->value;enqueue(e);return;}}
 EventType e={.eType=penDownEvent,.penDown=true,.screenX=p.x,.screenY=p.y};enqueue(e);}
@end
static void updateDashboard(void){
 BOOL playing=NameCommander[0]&&active&&active->menu==1000;
 UILabel*status=(UILabel*)[controller.view viewWithTag:985];UILabel*detail=(UILabel*)[controller.view viewWithTag:986];
 status.text=playing?[NSString stringWithFormat:@"%@  •  %ld cr.",str(NameCommander),(long)Credits]:@"A galaxy of opportunity awaits";
 detail.text=playing?[NSString stringWithFormat:@"Day %d  •  Fuel %d pc  •  Hull %ld%%",(int)Days,(int)Ship.Fuel,(long)(100*Ship.Hull/MAX(1,GetHullStrength()))]:@"Classic rules • Original galaxy";
 int selected=active?active->id:0;
 for(int i=0;i<6;i++){UIButton*b=(UIButton*)[controller.view viewWithTag:910+i];b.enabled=playing&&!dialogDepth;b.alpha=b.enabled?1:.45;BOOL chosen=(i==0&&(selected==BuyCargoForm||selected==SellCargoForm))||(i==1&&selected==ShipYardForm)||(i==3&&selected==WarpForm);if(@available(iOS 26.0,*)){UIButtonConfiguration*c=chosen?[UIButtonConfiguration prominentGlassButtonConfiguration]:[UIButtonConfiguration glassButtonConfiguration];c.title=[b titleForState:UIControlStateNormal];c.contentInsets=NSDirectionalEdgeInsetsMake(6,4,6,4);c.titleTextAttributesTransformer=^NSDictionary*(NSDictionary*a){NSMutableDictionary*d=[a mutableCopy];d[NSFontAttributeName]=[UIFont systemFontOfSize:12 weight:UIFontWeightSemibold];return d;};c.baseForegroundColor=chosen?UIColor.whiteColor:UIColor.labelColor;c.baseBackgroundColor=[UIColor colorWithRed:.15 green:.43 blue:.49 alpha:1];b.configuration=c;b.backgroundColor=UIColor.clearColor;}else b.backgroundColor=chosen?[UIColor colorWithRed:.68 green:.83 blue:.88 alpha:1]:[UIColor colorWithWhite:1 alpha:.8];}
}
size_t handleSize(Handle h){return [handleSizes[[NSValue valueWithPointer:h]] unsignedLongValue];}
@interface STClassicAppDelegate:UIResponder<UIApplicationDelegate>
@end
@interface STClassicSceneDelegate:UIResponder<UIWindowSceneDelegate>
@property(nonatomic,strong)UIWindow*window;
@end
#include "SimulatorTests.inc"
@implementation STClassicAppDelegate
- (BOOL)application:(UIApplication*)app didFinishLaunchingWithOptions:(NSDictionary*)options {return YES;}
@end
@implementation STClassicSceneDelegate
- (void)scene:(UIScene*)scene willConnectToSession:(UISceneSession*)session options:(UISceneConnectionOptions*)options {
if (![scene isKindOfClass:UIWindowScene.class]) return;
resources=[NSJSONSerialization JSONObjectWithData:[NSData dataWithContentsOfFile:[NSBundle.mainBundle pathForResource:@"resources" ofType:@"json"]] options:0 error:nil];events=[NSMutableArray new];forms=[NSMutableDictionary new];images=[NSMutableDictionary new];handleSizes=[NSMutableDictionary new];saveDirectory=[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES).firstObject stringByAppendingPathComponent:@"SpaceTraderClassic-v1"];[[NSFileManager defaultManager]createDirectoryAtPath:saveDirectory withIntermediateDirectories:YES attributes:nil error:nil];CGColorSpaceRef cs=CGColorSpaceCreateDeviceRGB();canvas=CGBitmapContextCreate(NULL,640,640,8,640*4,cs,kCGImageAlphaPremultipliedLast);CGColorSpaceRelease(cs);CGContextTranslateCTM(canvas,0,640);CGContextScaleCTM(canvas,4,-4);CGContextSetInterpolationQuality(canvas,kCGInterpolationNone);fill(CGRectMake(0,0,160,160),UIColor.whiteColor);self.window=[[UIWindow alloc]initWithWindowScene:(UIWindowScene*)scene];controller=[GameController new];self.window.rootViewController=controller;[self.window makeKeyAndVisible];srandom((unsigned)time(NULL));beginSimulatorTests();[NSTimer scheduledTimerWithTimeInterval:.1 repeats:NO block:^(NSTimer*t){extern long MerchantPilotMain(Word,Ptr,Word);MerchantPilotMain(0,NULL,0);}];
}
- (void)sceneDidEnterBackground:(UIScene*)scene {if(NameCommander[0]&&CurForm!=MainForm&&CurForm!=NewCommanderForm)SaveGame(0);}
@end
int main(int argc,char**argv){@autoreleasepool{return UIApplicationMain(argc,argv,nil,NSStringFromClass(STClassicAppDelegate.class));}}
