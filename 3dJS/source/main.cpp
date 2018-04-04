#include <3ds.h>
#include "duktape.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#define Version "0.1"
#define VersionLarge "beta 1"

static duk_ret_t JS_print(duk_context *ctx)
{
	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) - 1);
	printf("%s\n", duk_to_string(ctx, -1));
	return 0;
}

static duk_ret_t Duk_eval(duk_context *ctx, void *udata)
{
	(void) udata;
	duk_eval(ctx);
	return 1;
}

static duk_ret_t Duk_str(duk_context *ctx, void *udata)
{
	(void) udata;
	duk_to_string(ctx, -1);
	return 1;
}

static void mainmsg()
{
	cout << CONSOLE_YELLOW << endl << "-----|" << CONSOLE_RESET << " 3DS JavaScript interpreter " << CONSOLE_YELLOW << "|-----" << endl << endl << endl << " -" << CONSOLE_RESET << " Press A to enter JavaScript code.\n   Code result will be displayed on the\n   top screen." << endl << endl << CONSOLE_YELLOW << " -" << CONSOLE_RESET << " Press B to load code from a file.\n   Code result will be displayed on the\n   top screen." << endl << endl << CONSOLE_YELLOW << " -" << CONSOLE_RESET << " Press Start to restart this app." << endl << endl << endl << "Developed by " << CONSOLE_BLUE << "X" << CONSOLE_CYAN << "or" << CONSOLE_BLUE << "T" << CONSOLE_CYAN << "roll" << CONSOLE_RESET << "." << endl << endl << "Version " << CONSOLE_MAGENTA << Version << CONSOLE_RESET << " / " << CONSOLE_MAGENTA  << VersionLarge << CONSOLE_RESET << ".";
}

int main()
{
	gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);
	mainmsg();
	duk_context *ctx = duk_create_heap_default();
	duk_push_c_function(ctx, JS_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");
	
	while(aptMainLoop())
	{
		int k = hidKeysDown();
		if(k & KEY_A)
		{
			consoleInit(GFX_TOP, NULL);
			consoleClear();
			consoleInit(GFX_BOTTOM, NULL);
			cout << endl << endl << CONSOLE_CYAN << " Type your JavaScript code." << CONSOLE_RESET;
			static SwkbdState swkbd;
			static char jscode[128];
			SwkbdButton button = SWKBD_BUTTON_NONE;
			swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
			swkbdSetInitialText(&swkbd, jscode);
			swkbdSetHintText(&swkbd, "JavaScript code to be interpreted");
			button = swkbdInputText(&swkbd, jscode, sizeof(jscode));
			consoleClear();
			if(string(jscode) != "")
			{
				cout << endl << endl << CONSOLE_RESET << "Typed JavaScript source code:" << endl << endl << endl << CONSOLE_YELLOW << "----------------------------------------" << endl << endl << CONSOLE_RESET << jscode << endl << endl << endl << CONSOLE_YELLOW << "----------------------------------------" << endl << endl << CONSOLE_RESET << "Executing code...";
				consoleInit(GFX_TOP, NULL);
				duk_push_string(ctx, jscode);
				duk_safe_call(ctx, Duk_eval, NULL, 1, 1);
				duk_safe_call(ctx, Duk_str, NULL, 1, 1);
				const char* res = duk_get_string(ctx, -1);
				if(res) cout << endl << res;
				else cout << endl << endl << "<Code finished with no errors>";
				duk_pop(ctx);
			}
			else
			{
				consoleClear();
				consoleInit(GFX_BOTTOM, NULL);
				consoleClear();
				mainmsg();
			}
		}
		else if(k & KEY_START)
		{
			consoleClear();
			consoleInit(GFX_BOTTOM, NULL);
			consoleClear();
			mainmsg();
		}
		else if(k & KEY_B)
		{
			consoleInit(GFX_TOP, NULL);
			consoleClear();
			consoleInit(GFX_BOTTOM, NULL);
			cout << endl << endl << CONSOLE_CYAN << " Type the name of any file at SD card." << CONSOLE_RESET;
			static SwkbdState swkbd;
			static char jsfile[128];
			string jscode = "";
			SwkbdButton button = SWKBD_BUTTON_NONE;
			swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
			swkbdSetInitialText(&swkbd, jsfile);
			swkbdSetHintText(&swkbd, "JavaScript source file to load");
			button = swkbdInputText(&swkbd, jsfile, sizeof(jsfile));
			if(string(jsfile) != "")
			{
				ifstream jsf("sdmc://" + string(jsfile));
				if(jsf.good())
				{
					consoleClear();
					cout << endl << endl << "Source file: sdmc://" + string(jsfile) << endl << endl << CONSOLE_YELLOW <<  "----------------------------------------" << CONSOLE_RESET << endl << endl;
					string line;
					while(getline(jsf, line))
					{
						jscode += line;
						cout << line << endl;
					}
					cout << endl << endl << endl << CONSOLE_YELLOW << "----------------------------------------" << endl << endl << CONSOLE_RESET << "Executing code...";
					consoleInit(GFX_TOP, NULL);
					duk_push_string(ctx, jscode.c_str());
					duk_safe_call(ctx, Duk_eval, NULL, 1, 1);
					duk_safe_call(ctx, Duk_str, NULL, 1, 1);
					const char* res = duk_get_string(ctx, -1);
					cout << endl << endl << res;
				}
				else
				{
					consoleClear();
					cout << endl << endl << CONSOLE_MAGENTA << "Given file does not exist." << endl << endl << "Press Start to restart or B to retry" << endl << "with another file." << CONSOLE_RESET;
				}
			}
			else
			{
				consoleClear();
				consoleInit(GFX_BOTTOM, NULL);
				consoleClear();
				mainmsg();
			}
		}
		hidScanInput();
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}
	duk_destroy_heap(ctx);
	gfxExit();
	return 0;
}