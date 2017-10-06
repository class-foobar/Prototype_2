import sys
import GUI
import AZflib
def main(strid):
    wnd = GUI.GetArg(strid,0)
    flags = (1 << 3)
    GUI.NewWindow(0,"BASEWINDOW",0.2,0.2,0.2,0.2,flags,"NULL")