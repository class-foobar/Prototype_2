import sys
import GUI
import AZflib
import importlib
strid = GUI.GetSTRID()
loc = GUI.GetFLoc(strid)
wnd = GUI.GetArg(strid,0)
floc = GUI.GetName(wnd)
if floc != "NULL":
    file = AZflib.Open(loc + "/data/UI/gameui.data")
    GUI.Memory_Set(wnd,"BOOL","isprojecting",False)
    if AZflib.FindVar(file,floc + "@DATA@" + "SplitH"):
        num = AZflib.GetVar(file,"INT",floc + "@DATA@" + "SplitH")
        i = 0;
        size = 0.0
        size = float(GUI.GetSY(wnd)) / float(num)
        propsize = 1.0 / float(num)
        while i < num:
            nwnd = GUI.NewWindow(wnd,"EMPTY",0,propsize * i, 1,propsize,(1 << 3) | (2 << 0),"NULL")
        
            i += 1
    if AZflib.FindVar(file,floc + "@DATA@" + "hide"):
        if AZflib.GetVar(file,"BOOL",floc + "@DATA@" + "hide") == True:
            GUI.HideWindow(wnd)
    AZflib.Release(file)
