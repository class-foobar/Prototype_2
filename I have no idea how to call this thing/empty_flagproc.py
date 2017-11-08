import sys
import GUI
import AZflib
import importlib
import os
from GTYPES import *
from inspect import signature
strid = GUI.GetSTRID()
loc = GUI.GetFLoc(strid)
wnd = GUI.GetArg(strid,0)
floc = GUI.GetName(wnd)
if floc != "NULL":
    file = AZflib.Open(loc + "/data/UI/gameui.data")
    GUI.Memory_Set(wnd,"isprojecting","BOOL",False)
    i = 0
    if AZflib.FindVar(file,GUI.GetName(wnd) + "@DATA@" + "SplitH"):
        num = AZflib.GetVar(file,"INT",GUI.GetName(wnd) + "@DATA@" + "SplitH")
        i = 0;
        size = 0.0
        size = float(GUI.GetSY(wnd)) / float(num)
        propsize = 1.0 / float(num)
        wndcopy = wnd
        floccopy = floc
        while i < num:
            nwnd = GUI.NewWindow(wnd,"EMPTY",0,propsize * i, 1,propsize,WF.SCALETO.V | WF.PROJECTION,"NULL")
            wnd = wndcopy
            floc = floccopy
            GUI.Memory_Set(wnd,"subwindow" + repr(i),nwnd)
            i += 1
    if AZflib.FindVar(file,GUI.GetName(wnd) + "@DATA@" + "hide"):
        if AZflib.GetVar(file,"BOOL",GUI.GetName(wnd) + "@DATA@" + "hide") == True:
            GUI.HideWindow(wnd)
    if AZflib.FindVar(file,GUI.GetName(wnd) + "@DATA@call"):
        os.chdir(sys.argv[0])
        pyf = AZflib.GetVar(file,"STR",floc+"@DATA@call")
        npyf = pyf.replace(".py","")
        mod = importlib.import_module(npyf)
        met = getattr(mod,"main")
        sig = signature(met)
        if len(sig.parameters) == 1:
            met(strid)
        else:
            met(strid,{"loc":loc,"wnd":wnd,"strid":strid})
    AZflib.Release(file)
