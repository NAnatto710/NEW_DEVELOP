LGTCollisionTool UE5.7 build scripts

BuildPlugin.cmd
  Double-click this file while Unreal Editor is closed.
  It builds the test project directly and updates:
    <Project>\Plugins\LGTCollisionTool\Binaries\Win64
  No BuildPlugin package folder and no DLL copy step are used.

Engine location
  Default: C:\Program Files\Epic Games\UE_5.7
  If UE5.7 is elsewhere, set environment variable UE57_ROOT.

Expected layout
  <Project>\
    *.uproject
    Plugins\
      LGTCollisionTool\
        *.uplugin
        Source\
        Scripts\
        Binaries\
