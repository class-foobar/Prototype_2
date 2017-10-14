import AZflib
import GUI
import GAPI
from GTYPES import *
strid = GUI.GetSTRID()
loc = GUI.GetFLoc(strid)
wnd = GUI.GetArg(strid,0)
floc = GUI.GetName(wnd)
GUI.Once(wnd,strid)
bptr = 0
while not GUI.IsDestroyed(wnd):
    while GUI.Memory_Get(wnd,"isattaching"):
        pass
    tex = GUI.GetTextureFromWindow(wnd,GUI.Memory_Get(wnd,"subwindow"),0,0,1,1,0)
    bptr = GUI.ProjectTexture(GUI.Memory_Get(wnd,"subwindow"),tex,0,0,1,1,0)
    GUI.WaitFor(wnd,"t66",strid)
GAPI.ChangeBoolPtr(bptr,False)