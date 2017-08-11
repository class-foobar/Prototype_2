import sys
import GUI
args = sys.argv
strid = sys.argv[0]
wnd = GUI.GetArg(strid,0)
GUI.Memory_Set(wnd,"hasmover","BOOL", False)
strv = GUI.GetName(wnd) + "@SHAPE0@ICON"
if GUI.File_Find("gameui", strv):
    GUI.AddSpriteToShape(wnd,GUI.File_GetVar("gameui",strv,"STR"),0)
GUI.Exit(strid)