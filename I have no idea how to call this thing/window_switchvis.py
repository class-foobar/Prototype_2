import sys
import GUI
import AZflib
import importlib
def main(strid):
    strid = GUI.GetSTRID()
    loc = GUI.GetFLoc(strid)
    wnd = GUI.GetArg(strid,0)
    floc = GUI.GetName(wnd)
    file = AZflib.Open(loc + "/data/UI/gameui.data")
    wndname = AZflib.GetVar(file,"STR",floc + "@DATA@" + "PANEL")
    GUI.SwitchWinVis(GUI.GetWndByName(wndname))

    