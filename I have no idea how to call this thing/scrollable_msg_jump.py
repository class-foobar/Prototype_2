import AZflib
import GUI 
import GAPI
from GTYPES import *
strid = GUI.GetSTRID()
loc = GUI.GetFLoc(strid)
wnd = GUI.GetArg(strid,0)
floc = GUI.GetName(wnd)
swnd = GUI.Memory_Get(wnd,"subwnd");
size = GUI.Memory_Get(wnd,"scrollablesize")
llpos = GUI.Memory_Get(wnd,"scrollablepos")
totalsize = GUI.Memory_Get(wnd,"scrollsize")
llrealsize = GUI.GetRealSizeY(wnd,1)
llwidth = GUI.GetRealSizeX(wnd,1)
relposX = GUI.GetX(wnd,1)-GUI.GetX(wnd,0)
curpos = int2(GUI.CursporX(),GUI.CursporY())
pos = int2(GUI.GetRealPosX(wnd,1), GUI.GetRealPosY(wnd,1))
relcpos = curpos+pos
GUI.SetShapePos(wnd,relposX,relposY,2)