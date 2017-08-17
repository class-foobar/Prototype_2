import sys
import GUI
import AZflib
args = sys.argv
strid = sys.argv[0]
loc = sys.argv[1]
wnd = GUI.GetArg(strid,0)
GUI.Memory_Set(wnd,"hasmover","BOOL", False)
floc = GUI.GetName(wnd)
strv = floc + "@SHAPE0@ICON"
if GUI.File_Find("gameui", strv):
    GUI.AddSpriteToShape(wnd,GUI.File_GetVar("gameui",strv,"STR"),0)
INptr = GUI.GetCol(wnd,"IN")
OUTptr = GUI.GetCol(wnd,"OUT")
GUI.AddColPtr(wnd,INptr,"INcopy")
GUI.AddColPtr(wnd,OUTptr,"OUTcopy")
file = AZflib.Open(loc + "/data/UI/gameui.data")
b = AZflib.FindVar(strv + "@DATA@LBUTTONUP")
if b:
    GUI.Memory_Set(wnd,"LBUTTONUP","STR",AZflib.GetVar(file,"STR",strv + "@DATA@LBUTTONUP"))
GUI.Exit(strid)