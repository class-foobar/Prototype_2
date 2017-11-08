import AZflib
import GUI
import GAPI
from GTYPES import *
strid = GUI.GetSTRID()
loc = GUI.GetFLoc(strid)
wnd = GUI.GetArg(strid,0)
floc = GUI.GetName(wnd)
if floc != "NULL":
    file = AZflib.Open(loc + "/data/UI/gameui.data")
swnd = GUI.NewWindow(wnd,"EMPTY",0,0,1,1,WF.SUBFRAME|WF.SCALETO.V, "NULL")
GUI.Memory_Set(wnd,"PYOBJ", "subwnd",swnd)
GUI.Memory_Set(wnd,"INT", "scrollablesize",0)
GUI.Memory_Set(wnd,"INT", "scrollablepos",0)
GUI.Memory_Set(wnd,"BOOL", "isattaching",True)
GUI.SetAttachToF(wnd,"scrollable_onattachto.py")
GUI.SendMsg(wnd,0,winapi.MakeLParam(1,1),winapi.msg.WM.LBUTTONDOWN)