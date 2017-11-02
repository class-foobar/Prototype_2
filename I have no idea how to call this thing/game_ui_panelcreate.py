import AZflib
import GUI
import GAPI
loc = GUI.GetFLoc(strid)
file = AZflib.Open(loc + "/data/UI/gameui.data")
infoparent = AZflib.GetVar(file,"OTHER@INFOWND@PARENT","STR")
choiceparent = AZflib.GetVar(file,"OTHER@CHOICEWND@PARENT","STR")
projectparent = AZflib.GetVar(file,"OTHER@PROJECTWND@PARENT","STR")
infoi = AZflib.GetVar(file,"OTHER@INFOWND@N","INT")
choicei = AZflib.GetVar(file,"OTHER@CHOICEWND@N","INT")
projecti = AZflib.GetVar(file,"OTHER@PROJECTWND@N","INT")
infot = GUI.Memory_Get(GUI.GetWndByName(infoparent),"subwindow" + repr(infoi))
choiceit = GUI.Memory_Get(GUI.GetWndByName(choiceparent),"subwindow" + repr(infoi))
projectt = GUI.Memory_Get(GUI.GetWndByName(projectparent),"subwindow" + repr(infoi))
flags = (1<<3)
info = GUI.NewWindow(infot,"SCROLLABLE",0,0,1,1,flags,"selectinfo")
choice = GUI.NewWindow(choiceit,"SCROLLABLE",0,0,1,1,flags,"schoicewnd")
proj = GUI.NewWindow(projectt,"SCROLLABLE",0,0,1,1,flags,"selectprojection")