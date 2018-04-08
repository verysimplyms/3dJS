#include "includes.h"
#include "duktape.h"

static PrintConsole top, sub;

inline string JS()
{
	string js;
	js += "var system=[];";
	js += "var console=[];";
	js += "var screen=[];";
	js += "var input=[];";
	js += "var applet=[];";
	js += "system.exitApp=function(){__exit();};";
	js += "system.isNew3DS=function(){return __isnew();};";
	js += "system.getTime=function(){var tme=[];tme.Seconds=__tmsc();tme.Minutes=__tmmn();tme.Hour=__tmhr();tme.dayName=__tmdys();tme.dayNumber=__tmdyn();tme.monthName=__tmmns();tme.monthNumber=__tmmnn();tme.Year=__tmyr();return tme;};";
	js += "system.reboot=function(){__reboot();};";
	js += "system.powerOff=function(){__pwroff();};";
	js += "system.getBattery=function(){return __curbat();};";
	js += "system.isCharging=function(){return __batchg();};";
	js += "system.launchApplet=function(Applet){__applt(Applet);};";
	js += "console.log=function(){for(var i=0;i!=arguments.length;i++){__print(arguments[i]);}__prln();};";
	js += "console.read=function(){return __read();};";
	js += "console.setScreen=function(Screen){__setscr(Screen);};";
	js += "console.clear=function(){__clear();};";
	js += "screen.Top=1;";
	js += "screen.Bottom=0;";
	js += "input.None=0;";
	js += "input.A=1;";
	js += "input.B=2;";
	js += "input.Select=4;";
	js += "input.Start=8;";
	js += "input.DRight=16;";
	js += "input.DLeft=32;";
	js += "input.DUp=64;";
	js += "input.DDown=128;";
	js += "input.R=256;";
	js += "input.L=512;";
	js += "input.X=1024;";
	js += "input.Y=2048;";
	js += "input.ZL=16384;";
	js += "input.ZR=32768;";
	js += "input.Touch=1048576;";
	js += "input.CSRight=16777216;";
	js += "input.CSLeft=33554432;";
	js += "input.CSUp=67108864;";
	js += "input.CSDown=134217728;";
	js += "input.CPRight=268435456;";
	js += "input.CPLeft=536870912;";
	js += "input.CPUp=1073741824;";
	js += "input.CPDown=2147483648;";
	js += "input.checkUp=function(){return __ipup();};";
	js += "input.checkDown=function(){return __ipdown();};";
	js += "input.checkHeld=function(){return __ipheld();};";
	js += "input.isPressed=function(){return input.checkHeld()!=input.None;};";
	js += "input.getTouch=function(){var tch=[];tch.X=__tchsx();tch.Y=__tchsy();return tch;};";
	js += "applet.HomeMenu = 0x101;";
	js += "applet.AlternateMenu = 0x103;";
	js += "applet.Camera = 0x110;";
	js += "applet.FriendsList = 0x112;";
	js += "applet.GameNotes = 0x113;";
	js += "applet.WebBrowser = 0x114;";
	js += "applet.InstructionManual = 0x115;";
	js += "applet.Notifications = 0x116;";
	js += "applet.Miiverse = 0x117;";
	js += "applet.MiiversePosting = 0x118;";
	js += "applet.AmiiboSettings = 0x119;";
	js += "applet.Application = 0x300;";
	js += "applet.eShop = 0x301;";
	js += "applet.SoftwareKeyboard = 0x401;";
	js += "applet.MiiSelector = 0x402;";
	js += "applet.PhotoSelector = 0x404;";
	js += "applet.SoundSelector = 0x405;";
	js += "applet.ErrorDisplay = 0x406;";
	js += "applet.eShopMint = 0x407;";
	js += "applet.CirclePadCalibrator = 0x408;";
	js += "applet.Notepad = 0x409;";
	js += "undefined;";
	return js;
}

inline duk_ret_t Duk_eval(duk_context *ctx, void *udata)
{
	(void) udata;
	duk_eval(ctx);
	return 1;
}

inline duk_ret_t Duk_str(duk_context *ctx, void *udata)
{
	(void) udata;
	duk_to_string(ctx, -1);
	return 1;
}

inline gfxScreen_t detscreen(int scr)
{
	return (scr == 0) ? GFX_BOTTOM : GFX_TOP;
}

inline duk_ret_t JS___exit(duk_context *ctx)
{
	exit(0);
	return 0;
}

inline duk_ret_t JS___clear(duk_context *ctx)
{
	cout << "\x1b[2J";
	return 0;
}

inline duk_ret_t JS___setscr(duk_context *ctx)
{
	int scr = duk_to_number(ctx, 0);
	(scr == 0) ? consoleSelect(&top) : consoleSelect(&sub);
	return 0;
}

inline duk_ret_t JS___prln(duk_context *ctx)
{
	cout << endl;
	return 0;
}

inline duk_ret_t JS___print(duk_context *ctx)
{
	int argv = duk_get_top(ctx);
	for(int i = 0; i < argv; i++)
	{
		cout << duk_to_string(ctx, i);
	}
	return 0;
}

inline duk_ret_t JS___read(duk_context *ctx)
{
	SwkbdState swkbd;
	char readed[256];
	swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, -1);
	swkbdInputText(&swkbd, readed, sizeof(readed));
	duk_push_string(ctx, readed);
	return 1;
}

inline duk_ret_t JS___ipheld(duk_context *ctx)
{
	hidScanInput();
	int inp = hidKeysHeld();
	duk_push_number(ctx, inp);
	return 1;
}

inline duk_ret_t JS___ipup(duk_context *ctx)
{
	hidScanInput();
	int inp = hidKeysUp();
	duk_push_number(ctx, inp);
	return 1;
}

inline duk_ret_t JS___ipdown(duk_context *ctx)
{
	hidScanInput();
	int inp = hidKeysDown();
	duk_push_number(ctx, inp);
	return 1;
}

inline duk_ret_t JS___isnew(duk_context *ctx)
{
	bool res;
	APT_CheckNew3DS(&res);
	duk_push_boolean(ctx, res);
	return 1;
}

const char* months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const char* weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const int daysAtStartOfMonthLUT[12] = {0 % 7, 31 % 7, 59 % 7, 90 % 7, 120 % 7, 151 % 7, 181 % 7, 212 % 7, 243 % 7, 273 % 7, 304 % 7, 334 % 7 };

static inline bool isLeapYear(int year)
{
	return (year%4) == 0 && !((year%100) == 0 && (year%400) != 0);
}

static inline int getDayOfWeek(int day, int month, int year)
{
	day += 2*(3-((year/100)%4));
	year %= 100;
	day += year + (year/4);
	day += daysAtStartOfMonthLUT[month] - (isLeapYear(year) && (month <= 1));
	return day % 7;
}

inline duk_ret_t JS___tmhr(duk_context *ctx)
{
	time_t tms = time(NULL);
	struct tm* tmst = gmtime((const time_t*)&tms);
	int hr = tmst->tm_hour;
	duk_push_number(ctx, hr);
	return 1;
}

inline duk_ret_t JS___tmmn(duk_context *ctx)
{
	time_t tms = time(NULL);
	struct tm* tmst = gmtime((const time_t*)&tms);
	int mn = tmst->tm_min;
	duk_push_number(ctx, mn);
	return 1;
}

inline duk_ret_t JS___tmsc(duk_context *ctx)
{
	time_t tms = time(NULL);
	struct tm* tmst = gmtime((const time_t*)&tms);
	int sc = tmst->tm_sec;
	duk_push_number(ctx, sc);
	return 1;
}

inline duk_ret_t JS___tmdys(duk_context *ctx)
{
	time_t tms = time(NULL);
	struct tm* tmst = gmtime((const time_t*)&tms);
	int idy = tmst->tm_mday;
	int imt = tmst->tm_mon;
	int iyr = tmst->tm_year + 1900;
	const char* daystr = weekDays[getDayOfWeek(idy, imt, iyr)];
	duk_push_string(ctx, daystr);
	return 1;
}

inline duk_ret_t JS___tmdyn(duk_context *ctx)
{
	time_t tms = time(NULL);
	struct tm* tmst = gmtime((const time_t*)&tms);
	int day = tmst->tm_mday;
	duk_push_number(ctx, day);
	return 1;
}

inline duk_ret_t JS___tmmns(duk_context *ctx)
{
	time_t tms = time(NULL);
	struct tm* tmst = gmtime((const time_t*)&tms);
	int mon = tmst->tm_mon;
	const char* monstr = months[mon];
	duk_push_string(ctx, monstr);
	return 1;
}

inline duk_ret_t JS___tmmnn(duk_context *ctx)
{
	time_t tms = time(NULL);
	struct tm* tmst = gmtime((const time_t*)&tms);
	int mon = tmst->tm_mon;
	duk_push_number(ctx, mon);
	return 1;
}

inline duk_ret_t JS___tmyr(duk_context *ctx)
{
	time_t tms = time(NULL);
	struct tm* tmst = gmtime((const time_t*)&tms);
	int yr = tmst->tm_year + 1900;
	duk_push_number(ctx, yr);
	return 1;
}

inline duk_ret_t JS___tchsx(duk_context *ctx)
{
	touchPosition touch;
	hidTouchRead(&touch);
	duk_push_number(ctx, touch.px);
	return 1;
}

inline duk_ret_t JS___tchsy(duk_context *ctx)
{
	touchPosition touch;
	hidTouchRead(&touch);
	duk_push_number(ctx, touch.py);
	return 1;
}

inline duk_ret_t JS___reboot(duk_context *ctx)
{
	Handle serviceHandle = 0;
	srvGetServiceHandle(&serviceHandle, "ptm:sysm");
	u32 *commandBuffer = getThreadCommandBuffer();
	commandBuffer[0] = 0x040700C0;
	commandBuffer[1] = 0x00000000;
	commandBuffer[2] = 0x00000000;
	commandBuffer[3] = 0x00000000;
	svcSendSyncRequest(serviceHandle);
	svcCloseHandle(serviceHandle);
	return 0;
}

inline duk_ret_t JS___pwroff(duk_context *ctx)
{
	Handle serviceHandle = 0;
	srvGetServiceHandle(&serviceHandle, "ptm:sysm");
	u32 *commandBuffer = getThreadCommandBuffer();
	commandBuffer[0] = 0x040700C0;
	commandBuffer[1] = 0x00000000;
	commandBuffer[2] = 0x00000000;
	commandBuffer[3] = 0x00000000;
	svcSendSyncRequest(serviceHandle);
	svcCloseHandle(serviceHandle);
	exit(0);
	return 0;
}

inline duk_ret_t JS___curbat(duk_context *ctx)
{
	u8 data = 0;
	PTMU_GetBatteryLevel(&data);
	duk_push_number(ctx, (int)data * 20);
	return 1;
}

inline duk_ret_t JS___batchg(duk_context *ctx)
{
	u8 data = 0;
	PTMU_GetBatteryChargeState(&data);
	bool chg = (data == 1) ? true : false;
	duk_push_boolean(ctx, chg);
	return 1;
}

inline duk_ret_t JS___applt(duk_context *ctx)
{
	gfxFlushBuffers();
	gfxSwapBuffers();
	gspWaitForVBlank();
	int id = duk_to_number(ctx, 0);
	u32 buff[0x400 / 4];
	memset(buff, 0, sizeof(buff));
	if(!aptLaunchLibraryApplet((NS_APPID)id, buff, sizeof(buff), 0)) exit(0);
	return 0;
}