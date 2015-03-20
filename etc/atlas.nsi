

;--------------------------------
; Interface
 
  !include "MUI2.nsh"
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE License.txt
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
;Uninstaller
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
  
  !insertmacro MUI_LANGUAGE "English"
  
Name "Atlas"
# define name of installer
OutFile "Atlas.exe"
 
# define installation directory
InstallDir $PROGRAMFILES\Atlas

DirText "This will install Atlas editor on your computer. Choose a directory"
 
# For removing Start Menu shortcut in Windows 7
RequestExecutionLevel user

 
# start default section
Section
 
    # set the installation directory as the destination for the following actions
    SetOutPath $INSTDIR

    File "bin\Atlas.exe"    
    File "bin\icuuc52.dll"
    File "bin\icuin52.dll" 
    File "bin\icudt52.dll"
    File "bin\libwinpthread-1.dll"  
    File "bin\libgcc_s_dw2-1.dll"
    File "bin\libstdc++-6.dll"
    File "bin\Qt5Gui.dll"   
    File "bin\Qt5Qml.dll"     
    File "bin\Qt5Svg.dll"
    File "bin\Qt5Multimedia.dll"  
    File "bin\Qt5Quick.dll"
    File "bin\Qt5Widgets.dll"    
    File "bin\Qt5Core.dll"
    File "bin\Qt5Network.dll"     
    File "bin\Qt5Sensors.dll" 
    File "bin\Qt5Xml.dll"

    SetOutPath "$INSTDIR\platforms"
    File "bin\platforms\qminimal.dll"
    File "bin\platforms\qwindows.dll"
    
    SetOutPath "$INSTDIR\imageformats"
    File /a /r "bin\imageformats\"
    
    SetOutPath "$INSTDIR\QtGraphicalEffects"
    File /a /r "bin\QtGraphicalEffects\"
    
    SetOutPath "$INSTDIR\QtQuick"
    File /a /r "bin\QtQuick\"
    
    SetOutPath "$INSTDIR\QtQuick.2"
    File /a /r "bin\QtQuick.2\"
    
    
    # create the uninstaller
    WriteUninstaller "$INSTDIR\uninstall.exe"
 
    # create a shortcut named "new shortcut" in the start menu programs directory
    # point the new shortcut at the program uninstaller
    CreateDirectory "$SMPROGRAMS\Zarubond"
    CreateShortCut "$SMPROGRAMS\Zarubond\Atlas.lnk" "$INSTDIR\Atlas.exe"
    CreateShortCut "$SMPROGRAMS\Zarubond\Uninstall Atlas.lnk" "$INSTDIR\uninstall.exe"
SectionEnd
 
# uninstaller section start
Section "uninstall"
 
    # first, delete the uninstaller
    Delete "$INSTDIR\uninstall.exe" ;delete self
    Delete "$INSTDIR\Atlas.exe"
    Delete "$INSTDIR\icuuc52.dll"
    Delete "$INSTDIR\icuin52.dll" 
    Delete "$INSTDIR\icudt52.dll"
    Delete "$INSTDIR\libwinpthread-1.dll"  
    Delete "$INSTDIR\libgcc_s_dw2-1.dll"
    Delete "$INSTDIR\libstdc++-6.dll"
    Delete "$INSTDIR\Qt5Gui.dll"   
    Delete "$INSTDIR\Qt5Qml.dll"     
    Delete "$INSTDIR\Qt5Svg.dll"
    Delete "$INSTDIR\Qt5Multimedia.dll"  
    Delete "$INSTDIR\Qt5Quick.dll"
    Delete "$INSTDIR\Qt5Widgets.dll"    
    Delete "$INSTDIR\Qt5Core.dll"
    Delete "$INSTDIR\Qt5Network.dll"     
    Delete "$INSTDIR\Qt5Sensors.dll" 
    Delete "$INSTDIR\Qt5Xml.dll"
    Delete "$INSTDIR\platforms\qminimal.dll"
    Delete "$INSTDIR\platforms\qwindows.dll"
    RMDir "$INSTDIR\platforms"
    RMDir /r "$INSTDIR\imageformats"
    RMDir /r "$INSTDIR\QtGraphicalEffects"
    RMDir /r "$INSTDIR\QtQuick"
    RMDir /r "$INSTDIR\QtQuick.2"
    RMDir $INSTDIR
     
    # second, remove the link from the start menu
    Delete "$SMPROGRAMS\Zarubond\Atlas.lnk"
    Delete "$SMPROGRAMS\Zarubond\Uninstall Atlas.lnk"
    RMDIR "$SMPROGRAMS\Zarubond"
 
# uninstaller section end
SectionEnd
