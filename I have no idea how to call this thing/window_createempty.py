import sys
import GUI
import AZflib
class int2:
    x = 0
    y = 0
class float2:
    x = 0.0
    y = 0.0
def main(strid, pos=float2(0.0,0.0),size=float2(0.1,0.1)):
    wnd = GUI.GetArg(strid,0)
    GUI.NewWindow(0,"BASEWINDOW",pos.x,pos.y,size.x,size.y,0,"NULL")

    
