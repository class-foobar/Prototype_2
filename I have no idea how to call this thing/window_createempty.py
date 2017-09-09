import sys
import GUI
import AZflib
def main(strid, posx,posy,sizex,sizey, flags = (1 << 3)):
    wnd = GUI.GetArg(strid,0)
    GUI.NewWindow(0,"BASEWINDOW",posx,posy,sizex,sizey,flags,"NULL")