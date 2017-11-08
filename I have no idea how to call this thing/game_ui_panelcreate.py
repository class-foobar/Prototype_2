import AZflib
import GUI
import GAPI
def main(strid,map):
    loc = map["loc"]
    wnd = map["wnd"]
    file = AZflib.Open(loc + "/data/UI/gameui.data")
    infoparent = AZflib.GetVar(file,"STR","OTHER@INFOWND@PARENT")
    choiceparent = AZflib.GetVar(file,"STR","OTHER@CHOICEWND@PARENT")
    projectparent = AZflib.GetVar(file,"STR","OTHER@PROJECTWND@PARENT")
    infoi = AZflib.GetVar(file,"INT","OTHER@INFOWND@N")
    choicei = AZflib.GetVar(file,"INT","OTHER@CHOICEWND@N")
    projecti = AZflib.GetVar(file,"INT","OTHER@PROJECTWND@N")
    infot = GUI.Memory_Get(GUI.GetWndByName(infoparent),"subwindow" + repr(infoi))
    choiceit = GUI.Memory_Get(GUI.GetWndByName(choiceparent),"subwindow" + repr(infoi))
    projectt = GUI.Memory_Get(GUI.GetWndByName(projectparent),"subwindow" + repr(infoi))
    flags = (1<<3)
    info = GUI.NewWindow(infot,"SCROLLABLE",0.0,0.0,1.0,1.0,flags,"selectinfo")
    choice = GUI.NewWindow(choiceit,"SCROLLABLE",0.0,0.0,1.0,1.0,flags,"schoicewnd")
    proj = GUI.NewWindow(projectt,"SCROLLABLE",0.0,0.0,1.0,1.0,flags,"selectprojection")