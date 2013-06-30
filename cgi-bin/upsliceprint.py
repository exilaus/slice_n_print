#!/usr/bin/env python
import cgi, os,sys
import subprocess
import cgitb; cgitb.enable()
import ConfigParser

class Unbuffered:
   def __init__(self, stream):
       self.stream = stream
   def write(self, data):
       self.stream.write(data)
       self.stream.flush()
   def __getattr__(self, attr):
       return getattr(self.stream, attr)



#---transfert file
try: # Windows needs stdio set for binary mode.
    import msvcrt
    msvcrt.setmode (0, os.O_BINARY) # stdin  = 0
    msvcrt.setmode (1, os.O_BINARY) # stdout = 1
except ImportError:
    pass

form = cgi.FieldStorage()

# A nested FieldStorage instance holds the file
fileitem = form['file']

# Test if the file was uploaded
if fileitem.filename:
   
   # strip leading path from file name to avoid directory traversal attacks
   fn = os.path.basename(fileitem.filename)
   open( './stl/' + fn, 'wb').write(fileitem.file.read())
   message = fn + '" was uploaded successfully'
   
else:
   message = 'No file was uploaded'
sys.stdout=Unbuffered(sys.stdout)
print "Content-Type: text/html;charset=utf-8\r\n\r\n"    
print        
print "<html>"
print "<head>"
print "<title>Slice&Print</title>"
print "<link rel=""stylesheet"" type=""text/css"" href=""..\style.css"">"
print "<body>"
print "<h2>Slice & Print</h2>"
print "<center>"
print "<p>result upload: %s</p>" % (message) 


#------------load cura setting ini or by form
preset = form.getvalue('preset')
if preset <> "I" :
    config = ConfigParser.ConfigParser()
    config.read('%s.ini' % preset)
    MAXX = int(config.get('printer','MAXX'))
    MAXY = int(config.get('printer','MAXY'))
    MAXZ = int(config.get('printer','MAXZ'))
    layerThickness = config.get('cura','layerThickness')
    initialLayerThickness = config.get('cura','initialLayerThickness')
    filamentDiameter = config.get('cura','filamentDiameter')
    filamentFlow = config.get('cura','filamentFlow')
    extrusionWidth = config.get('cura','extrusionWidth')
    supportLineWidth = config.get('cura','extrusionWidth') 
    insetCount = config.get('cura','insetCount')
    downSkinCount = config.get('cura','downSkinCount')
    upSkinCount = config.get('cura','upSkinCount')
    sparseInfillLineDistance = config.get('cura','sparseInfillLineDistance') 
    infillOverlap = config.get('cura','infillOverlap')
    skirtDistance = config.get('cura','skirtDistance')
    skirtLineCount = config.get('cura','skirtLineCount')
    initialSpeedupLayers = config.get('cura','initialSpeedupLayers')
    initialLayerSpeed = config.get('cura','initialLayerSpeed')
    printSpeed= config.get('cura','printSpeed')
    infillSpeed= config.get('cura','infillSpeed')
    moveSpeed= config.get('cura','moveSpeed')
    fanOnLayerNr= config.get('cura','fanOnLayerNr')
    supportAngle= config.get('cura','supportAngle')
    supportEverywhere= config.get('cura','supportEverywhere')
    retractionAmount= config.get('cura','retractionAmount')
    retractionSpeed= config.get('cura','retractionSpeed')
    retractionAmountExtruderSwitch= config.get('cura','retractionAmountExtruderSwitch')
    multiVolumeOverlap= config.get('cura','multiVolumeOverlap')
    objectPositionx= config.get('cura','objectPosition.X')
    objectPositiony= config.get('cura','objectPosition.Y')
    objectSink= config.get('cura','objectSink')
    raftMargin= config.get('cura','raftMargin')
    raftLineSpacing= config.get('cura','raftLineSpacing')
    raftBaseThickness= config.get('cura','raftBaseThickness')
    raftBaseLinewidth= config.get('cura','raftBaseLinewidth')
    raftInterfaceThickness= config.get('cura','raftInterfaceThickness')
    raftInterfaceLinewidth= config.get('cura','raftInterfaceLinewidth')
    minimalLayerTime= config.get('cura','minimalLayerTime')
    minimalFeedrate= config.get('cura','minimalFeedrate')
    coolHeadLift= config.get('cura','coolHeadLift')
    fanSpeedMin= config.get('cura','fanSpeedMin')
    fanSpeedMax= config.get('cura','fanSpeedMax')
    fixHorrible= config.get('cura','fixHorrible')
else:
    MAXX = int(form.getvalue('MAXX'))
    MAXY = int(form.getvalue('MAXY'))
    MAXZ = int(form.getvalue('MAXZ'))
    layerThickness = form.getvalue('layerThickness')
    initialLayerThickness = form.getvalue('initialLayerThickness')
    filamentDiameter = form.getvalue('filamentDiameter')
    filamentFlow = form.getvalue('filamentFlow')
    extrusionWidth = form.getvalue('extrusionWidth')
    supportLineWidth = form.getvalue('extrusionWidth')
    insetCount = form.getvalue('insetCount')
    downSkinCount = form.getvalue('downSkinCount')
    upSkinCount = form.getvalue('upSkinCount')
    sparseInfillLineDistance = form.getvalue('sparseInfillLineDistance')
    infillOverlap = form.getvalue('infillOverlap')
    skirtDistance = form.getvalue('skirtDistance')
    skirtLineCount = form.getvalue('skirtLineCount')
    initialSpeedupLayers = form.getvalue('initialSpeedupLayers')
    initialLayerSpeed = form.getvalue('initialLayerSpeed')
    printSpeed= form.getvalue('printSpeed')
    infillSpeed= form.getvalue('infillSpeed')
    moveSpeed= form.getvalue('moveSpeed')
    fanOnLayerNr= form.getvalue('fanOnLayerNr')
    supportAngle= form.getvalue('supportAngle')
    supportEverywhere= form.getvalue('supportEverywhere')
    retractionAmount= form.getvalue('retractionAmount')
    retractionSpeed= form.getvalue('retractionSpeed')
    retractionAmountExtruderSwitch= form.getvalue('retractionAmountExtruderSwitch')
    multiVolumeOverlap= form.getvalue('multiVolumeOverlap')
    objectPositionx= form.getvalue('objectPosition.X')
    objectPositiony= form.getvalue('objectPosition.Y')
    objectSink= form.getvalue('objectSink')
    raftMargin= form.getvalue('raftMargin')
    raftLineSpacing= form.getvalue('raftLineSpacing')
    raftBaseThickness= form.getvalue('raftBaseThickness')
    raftBaseLinewidth= form.getvalue('raftBaseLinewidth')
    raftInterfaceThickness= form.getvalue('raftInterfaceThickness')
    raftInterfaceLinewidth= form.getvalue('raftInterfaceLinewidth')
    minimalLayerTime= form.getvalue('minimalLayerTime')
    minimalFeedrate= form.getvalue('minimalFeedrate')
    coolHeadLift= form.getvalue('coolHeadLift')
    fanSpeedMin= form.getvalue('fanSpeedMin')
    fanSpeedMax= form.getvalue('fanSpeedMax')
    fixHorrible= form.getvalue('fixHorrible')
#-----------admesh correct and stats
print "<p></p><p></p>Check Object<p></p><p></p>"
subprocess.call("./admesh --normal-directions --tolerance=0.01 --exact ./stl/%s > stl-stats.txt" % fn,shell=True)
#-----------object are big object?
open_file=open('stl-stats.txt','r')
file_lines=open_file.readlines()
x = float(file_lines[17].strip())*1000
y = float(file_lines[18].strip())*1000
z = float(file_lines[19].strip())*1000

if MAXX/x<1 or MAXY/y<1 or MAXZ/z<1 :
   if MAXX/x > MAXY/y :
       if MAXY/y > MAXZ/z :
	      scale= MAXZ/z
       else:
	      scale= MAXY/y
   else:
       if MAXX/x > MAXZ/z:
	      scale= MAXZ/z
       else:
             scale= MAXX/x		
   subprocess.call("./admesh --normal-directions --tolerance=0.01 --exact --scale=%f ./stl/%s > stl-stats.txt" % (scale,fn),shell=True)
   print "<p>Object too much big reduced with factr scale: %s</p>" % (scale)
   open_file=open('stl-stats.txt','r')
   file_lines=open_file.readlines()
   print "Dimesion of object : <br>"
   print "Max X: %s<br>" % file_lines[17].strip()
   print "Max Y: %s<br>" % file_lines[18].strip()
   print "Max Z: %s<br>" % file_lines[19].strip()
   print "Volume: %s<br>" % file_lines[26].strip()
else:
   print "Dimesion of object : <br>"
   print "Max X: %s<br>" % file_lines[17].strip()
   print "Max Y: %s<br>" % file_lines[18].strip()
   print "Max Z: %s<br>" % file_lines[19].strip()
   print "Volume: %s<br>" % file_lines[26].strip()

#--------------Cura engine
print "<p></p><p></p>Start Slicing:<p></p><p></p>"
subprocess.call("./CuraEngine -v -s filamentDiameter=%s -s filamentFlow=%s -s initialLayerThickness=%s -s layerThickness=%s -s extrusionWidth=%s -s insetCount=%s -s downSkinCount=%s -s upSkinCount=%s -s initialSpeedupLayers=%s -s initialLayerSpeed=%s -s printSpeed=%s -s infillSpeed=%s -s moveSpeed=%s -s fanOnLayerNr=%s -s skirtDistance=%s -s skirtLineCount=%s -s sparseInfillLineDistance=%s -s infillOverlap=%s -s objectPosition.X=%s -s objectPosition.Y=%s -s objectSink=%s -s supportAngle=%s -s supportEverywhere=%s -s supportLineWidth=%s -s retractionAmount=%s -s retractionSpeed=%s -s retractionAmountExtruderSwitch=%s -s multiVolumeOverlap=%s -s minimalLayerTime=%s -s minimalFeedrate=%s -s coolHeadLift=%s -s fanSpeedMin=%s -s fanSpeedMax=%s -s raftMargin=%s -s raftLineSpacing=%s -s raftBaseThickness=%s -s raftBaseLinewidth=%s -s raftInterfaceThickness=%s -s raftInterfaceLinewidth=%s -s fixHorrible=%s -o ./gcode/%s.gcode ./stl/%s >>curalog.txt" % (filamentDiameter,filamentFlow,initialLayerThickness,layerThickness,extrusionWidth,insetCount,downSkinCount,upSkinCount,initialSpeedupLayers,initialLayerSpeed,printSpeed,infillSpeed,moveSpeed,fanOnLayerNr,skirtDistance,skirtLineCount,sparseInfillLineDistance,infillOverlap,objectPositionx,objectPositiony,objectSink,supportAngle,supportEverywhere,supportLineWidth,retractionAmount,retractionSpeed,retractionAmountExtruderSwitch,multiVolumeOverlap,minimalLayerTime,minimalFeedrate,coolHeadLift,fanSpeedMin,fanSpeedMax,raftMargin,raftLineSpacing,raftBaseThickness,raftBaseLinewidth,raftInterfaceThickness,raftInterfaceLinewidth,fixHorrible,fn,fn),shell=True) 
print "<br><b>Object sliced<b><br> and now need decide how software use for printing.........."

