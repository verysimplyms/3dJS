#include "includes.h"
#include "duktape.h"
#include "jscore.h"

static string Version = "Beta 2";
static bool menu;
static duk_context *ctx;
static const char *tmptext = "";

SwkbdState jskbd(const char* init, const char *hint)
{
	SwkbdState swkbd;
	swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
	swkbdSetHintText(&swkbd, hint);
	swkbdSetInitialText(&swkbd, init);
	return swkbd;
}

string StringReplace(string str, string from, string to)
{
	string ret = str;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
        ret = str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
	return ret;
}

const char* optioncolor(int selected, int required)
{
	return (selected == required) ? CONSOLE_RED : CONSOLE_RESET;
}

void clearformat()
{
	cout << CONSOLE_RESET << "\x1b[40m";
}

void clearall()
{
	consoleSelect(&sub);
	clearformat();
	consoleClear();
	consoleSelect(&top);
	clearformat();
	consoleClear();
}

/*

Rows and columns in screens: (colums, rows)

Top: 50, 30
Bottom: 40, 30

*/

void MainMenu(int selected)
{
	menu = true; 
	duk_destroy_heap(ctx);
	clearall();
	cout << endl << endl << CONSOLE_YELLOW << "---------------------|" << CONSOLE_GREEN << " 3dJS " << CONSOLE_YELLOW << "|---------------------" << CONSOLE_YELLOW << endl << "----------|" << CONSOLE_CYAN << " 3DS JavaScript interpreter " << CONSOLE_YELLOW << "|----------" << CONSOLE_RESET << endl << endl << endl << endl << optioncolor(selected, 0) << "       - " << CONSOLE_RESET << "Type and execute JavaScript code" << endl << endl << optioncolor(selected, 1) << "       - " << CONSOLE_RESET << "Load JavaScript code from a file" << endl << endl << optioncolor(selected, 2) << "       - " << CONSOLE_RESET << "About this interpreter" << endl << endl << endl << endl << endl << "   Move between options using " << CONSOLE_CYAN << "D-pad" << CONSOLE_RESET << " control." << endl << "   Open selected option with " << CONSOLE_CYAN << "A" << CONSOLE_RESET << " button." << endl << endl << endl << "   Developed by " << CONSOLE_BLUE << "X" << CONSOLE_CYAN << "or" << CONSOLE_BLUE << "T" << CONSOLE_CYAN << "roll" << CONSOLE_RESET << "." << endl << endl << endl << CONSOLE_RESET << "   \x1b[42m            " << endl << "\x1b[40m   " << "\x1b[42m" << "   " << Version << "   " << endl << "\x1b[40m   " << "\x1b[42m" << "            \x1b[40m";
	ctx = duk_create_heap_default();
	duk_push_c_function(ctx, JS___print, DUK_VARARGS);
	duk_put_global_string(ctx, "__print");
	duk_push_c_function(ctx, JS___prln, DUK_VARARGS);
	duk_put_global_string(ctx, "__prln");
	duk_push_c_function(ctx, JS___read, DUK_VARARGS);
	duk_put_global_string(ctx, "__read");
	duk_push_c_function(ctx, JS___clear, DUK_VARARGS);
	duk_put_global_string(ctx, "__clear");
	duk_push_c_function(ctx, JS___setscr, DUK_VARARGS);
	duk_put_global_string(ctx, "__setscr");
	duk_push_c_function(ctx, JS___ipheld, DUK_VARARGS);
	duk_put_global_string(ctx, "__ipheld");
	duk_push_c_function(ctx, JS___ipup, DUK_VARARGS);
	duk_put_global_string(ctx, "__ipup");
	duk_push_c_function(ctx, JS___ipdown, DUK_VARARGS);
	duk_put_global_string(ctx, "__ipdown");
	duk_push_c_function(ctx, JS___exit, DUK_VARARGS);
	duk_put_global_string(ctx, "__exit");
	duk_push_c_function(ctx, JS___isnew, DUK_VARARGS);
	duk_put_global_string(ctx, "__isnew");
	duk_push_c_function(ctx, JS___tmhr, DUK_VARARGS);
	duk_put_global_string(ctx, "__tmhr");
	duk_push_c_function(ctx, JS___tmmn, DUK_VARARGS);
	duk_put_global_string(ctx, "__tmmn");
	duk_push_c_function(ctx, JS___tmsc, DUK_VARARGS);
	duk_put_global_string(ctx, "__tmsc");
	duk_push_c_function(ctx, JS___tmdys, DUK_VARARGS);
	duk_put_global_string(ctx, "__tmdys");
	duk_push_c_function(ctx, JS___tmdyn, DUK_VARARGS);
	duk_put_global_string(ctx, "__tmdyn");
	duk_push_c_function(ctx, JS___tmmns, DUK_VARARGS);
	duk_put_global_string(ctx, "__tmmns");
	duk_push_c_function(ctx, JS___tmmnn, DUK_VARARGS);
	duk_put_global_string(ctx, "__tmmnn");
	duk_push_c_function(ctx, JS___tmyr, DUK_VARARGS);
	duk_put_global_string(ctx, "__tmyr");
	duk_push_c_function(ctx, JS___tchsx, DUK_VARARGS);
	duk_put_global_string(ctx, "__tchsx");
	duk_push_c_function(ctx, JS___tchsy, DUK_VARARGS);
	duk_put_global_string(ctx, "__tchsy");
	duk_push_c_function(ctx, JS___reboot, DUK_VARARGS);
	duk_put_global_string(ctx, "__reboot");
	duk_push_c_function(ctx, JS___pwroff, DUK_VARARGS);
	duk_put_global_string(ctx, "__pwroff");
	duk_push_c_function(ctx, JS___batchg, DUK_VARARGS);
	duk_put_global_string(ctx, "__batchg");
	duk_push_c_function(ctx, JS___curbat, DUK_VARARGS);
	duk_put_global_string(ctx, "__curbat");
	duk_push_c_function(ctx, JS___applt, DUK_VARARGS);
	duk_put_global_string(ctx, "__applt");
}

int main(int argc, const char **argv)
{
	int sl = 0;
	srand(time(NULL));
	nsInit();
	ptmSysmInit();
	ptmuInit();
	aptInit();
	newsInit();
	gfxInitDefault();
	consoleInit(GFX_TOP, &top);
	consoleInit(GFX_BOTTOM, &sub);
	consoleSelect(&top);
	MainMenu(sl);
	string jsfile, jscode;
	while(aptMainLoop())
	{
		hidScanInput();
		int k = hidKeysDown();
		if(k & KEY_DOWN)
		{
			if(sl == 2) sl = 0;
			else sl++;
			MainMenu(sl);
		}
		else if(k & KEY_UP)
		{
			if(sl == 0) sl = 2;
			else sl--;
			MainMenu(sl);
		}
		else if(k & KEY_A)
		{
			if(sl == 0)
			{
				clearall();
				cout << CONSOLE_CYAN << " Type the JS code to execute it." << CONSOLE_RESET;
				SwkbdState kbd = jskbd(tmptext, "JavaScript code to be interpreted");
				char cjscode[512];
				swkbdInputText(&kbd, cjscode, sizeof(cjscode));
				tmptext = cjscode;
				jscode = JS() + string(cjscode);
				consoleClear();
				if(!jscode.empty())
				{
					consoleSelect(&top);
					const char* code = jscode.c_str();
					duk_push_string(ctx, code);
					duk_safe_call(ctx, Duk_eval, NULL, 1, 1);
					duk_safe_call(ctx, Duk_str, NULL, 1, 1);
					string res = string(duk_get_string(ctx, -1));
					cout << endl << res << endl << endl << CONSOLE_MAGENTA << "-------------------------" << CONSOLE_RESET << endl << endl << "Code execution has finished." << endl << "Press" << CONSOLE_CYAN << " Start " << CONSOLE_RESET << "to return to" << CONSOLE_CYAN << " 3dJS" << CONSOLE_RESET << "\'s menu.";
					duk_pop(ctx);
				}
				else MainMenu(sl);
			}
			else if(sl == 1)
			{
				clearall();
				cout << CONSOLE_CYAN << " Type the name of the file at SD card." << CONSOLE_RESET;
				string sjscode = JS();
				SwkbdState kbd = jskbd(tmptext, "JavaScript source file to load");
				char cjsfile[512];
				swkbdInputText(&kbd, cjsfile, sizeof(cjsfile));
				tmptext = cjsfile;
				jsfile = StringReplace(string(cjsfile), "/", "\\");
				if(!jsfile.empty())
				{
					int chr;
					FILE * jsf;
					jsf = fopen(("sdmc://" + jsfile).c_str(), "r");
					if(jsf)
					{
						consoleClear();
						string line;
						while((chr = getc(jsf)) != EOF)
						{
							sjscode += (char)chr;
						}
						consoleSelect(&top);
						const char* code = sjscode.c_str();
						duk_push_string(ctx, code);
						duk_safe_call(ctx, Duk_eval, NULL, 1, 1);
						duk_safe_call(ctx, Duk_str, NULL, 1, 1);
						string res = string(duk_get_string(ctx, -1));
						cout << endl << res << endl << endl << CONSOLE_MAGENTA << "-------------------------" << CONSOLE_RESET << endl << endl << "Code execution has finished." << endl << "Press" << CONSOLE_CYAN << " Start " << CONSOLE_RESET << "to return to" << CONSOLE_CYAN << " 3dJS" << CONSOLE_RESET << "\'s menu.";
						duk_pop(ctx);
					}
					else
					{
						consoleSelect(&sub);
						consoleClear();
						consoleSelect(&top);
						consoleClear();
						cout << endl << endl << "Given file does not exist." << endl << "Press" << CONSOLE_CYAN << " Start " << CONSOLE_RESET << "to return to" << CONSOLE_CYAN << " 3dJS" << CONSOLE_RESET << "\'s menu.";
					}
					fclose(jsf);
				}
				else MainMenu(sl);
			}
			else if(sl == 2)
			{
				clearall();
				consoleSelect(&top);
				cout << endl << endl << CONSOLE_YELLOW << "-----------------| " << CONSOLE_GREEN << "Generic info" << CONSOLE_YELLOW << " |-----------------" << CONSOLE_RESET << endl << endl;
				cout << " This 3DS program uses " << CONSOLE_CYAN << "duktape" << CONSOLE_RESET << ", a JavaScript" << endl << endl;
				cout << " engine for C/C++, to handle and interpretate JS" << endl << endl;
				cout << " code on a 3DS system. I have added some" << endl << endl;
				cout << " constants and elements which make us able to use" << endl << endl;
				cout << " 3DS controls, screen printing, and which make" << endl << endl;
				cout << " us able to make full homebrew apps only using" << endl << endl;
				cout << " JavaScript and this app. Hope you enjoy it!" << endl << endl;
				cout << endl << CONSOLE_MAGENTA << "--------------------------------------------------" << CONSOLE_RESET << endl << endl << endl << "      Press" << CONSOLE_CYAN << " Start " << CONSOLE_RESET << "to return to" << CONSOLE_CYAN << " 3dJS" << CONSOLE_RESET << "\'s menu.     ";
				consoleSelect(&sub);
				cout << endl << endl << CONSOLE_YELLOW << "------------| " << CONSOLE_GREEN << "Future plans" << CONSOLE_YELLOW << " |------------" << CONSOLE_RESET << endl << endl;
				cout << " One of my objectives with this program" << endl << endl;
				cout << " is to extend the ability of making" << endl << endl;
				cout << " Nintendo homebrew apps for people who" << endl << endl;
				cout << " don't want to use C, C++ or Lua." << endl << endl;
				cout << " I'm planning to remake this idea into" << endl << endl;
				cout << " other platforms, such as Nintendo DS" << endl << endl;
				cout << " or even Nintendo Switch." << endl << endl;
				cout << endl << CONSOLE_MAGENTA << "----------------------------------------" << CONSOLE_RESET << endl << endl << endl << " Press" << CONSOLE_CYAN << " Start " << CONSOLE_RESET << "to return to" << CONSOLE_CYAN << " 3dJS" << CONSOLE_RESET << "\'s menu.";
			}
		}
		else if(k & KEY_START)
		{
			MainMenu(sl);
		}
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}
	nsExit();
	ptmSysmExit();
	ptmuExit();
	aptExit();
	newsExit();
	gfxExit();
	return 0;
}