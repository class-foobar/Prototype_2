import sys
import GUI
import AZflib
import importlib
strid = GUI.GetSTRID()
wnd = GUI.GetArg(strid, 0)
boxn = GUI.GetArg(strid, 1)
umsg = GUI.GetArg(strid, 2)
msg = GUI.GetArg(strid, 3)
btptr = GUI.GetArg(strid, 5)
class int2 :
	x = 0
	y = 0
class int4 :
	x = 0
	y = 0
	z = 0
	w = 0
class float2 :
	x = 0.0
	y = 0.0
def mov(wnd,strid):
    ispressed = True
    while ispressed:
        nmsg = GUI.WaitFor(wnd,"MOUSEMOVE LBUTTONUP",strid)
        if nmsg == "MOUSEMOVE":
            point = int2()
            point.x = GUI.CursorX()
            point.y = GUI.CursorY()
            GUI.SetX(wnd,point.x)
            GUI.SetY(wnd,point.y)
            GUI.UpdatePos(wnd,GUI.Memory_Get(wnd,"mouseXtop"),GUI.Memory_Get(wnd,"mouseYtop"))
            GUI.Memory_Set(wnd,"mouseXtop","INT",point.x)
            GUI.Memory_Set(wnd,"mouseYtop","INT",point.y)
        if nmsg == "LBUTTONUP":
            ispressed = False
point = int2()
point.x = GUI.GetArg(strid, 6)
point.y = GUI.GetArg(strid, 7)
if msg == "LBUTTONDOWN":
    GUI.Memory_Set(wnd,"istopmoving","BOOL",True)
    GUI.Memory_Set(wnd,"mouseXtop","INT",point.x)
    GUI.Memory_Set(wnd,"mouseYtop","INT",point.y)
    GUI.MouseMoveDetB(btptr,True)
if msg == "LBUTTONUP":
    GUI.Memory_Set(wnd,"istopmoving","BOOL",False)
    GUI.MouseMoveDetB(btptr,False)
if msg == "MOUSEMOVE" and GUI.Memory_Get(wnd,"istopmoving"):
    print ("the window is moving")
    mov(wnd,strid)
GUI.Exit(strid)