import sys
import GUI
import AZflib
import importlib
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
inputvec = ["LBUTTONUP","RBUTTONUP","LBUTTONDOWN","RBUTTONDOWN","MBUTTONUP","MBUTTONDOWN","MOUSEMOVE"]
for v in inputvec:
    b = AZflib.FindVar(file,floc + "@DATA@" + v)
    if b:
       GUI.Memory_Set(wnd,v,"STR",AZflib.GetVar(file,"STR",floc + "@DATA@" + v))
GUI.Exit(strid)