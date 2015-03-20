#-------------------------------------------------
#
# Project created by QtCreator 2012-06-09T19:47:42
#
#-------------------------------------------------


QT += core xml quick multimedia sensors


TARGET = Atlas
TEMPLATE = app
TRANSLATIONS = t1_cs.ts

QMAKE_RESOURCE_FLAGS += -threshold 0 -compress 9
QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -pipe

QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3 -pipe

#enable sailfih
#CONFIG+=sailfish

win32 {
    QT += widgets

    RC_FILE = etc/atlas.rc
}

!android {
    !sailfish{
        linux{
            QT += widgets
            LIBS += -ludev
            SOURCES += gamepad/gamepadlinux.cpp \
                        gamepad/gamepaddevice.cpp
            HEADERS += gamepad/gamepadlinux.h \
                    gamepad/gamepaddevice.h
        }
    }
}

android{
    QT += androidextras
    SOURCES += gamepad/gamepadandroid.cpp
    HEADERS += gamepad/gamepadandroid.h
}

sailfish {
    DEFINES += Q_OS_SAILFISH
    QMAKE_CXXFLAGS -= -std=c++11
    QMAKE_CXXFLAGS += -std=c++0x
    # Other Sailfish OS specific code here
}


unix{
    #VARIABLES
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    BINDIR = $$PREFIX/bin
    DATADIR =$$PREFIX/share

#  DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

    #MAKE INSTALL

    INSTALLS += target

    target.path =$$BINDIR
    data.path = $$DATADIR/atlas/data
    data.files = data/*

    INSTALLS += data
}




#QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
#QMAKE_CFLAGS += -fsanitize=address -fno-omit-frame-pointer
#QMAKE_LFLAGS += -fsanitize=address

#x11extras
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -O2 -mtune=native
#QMAKE_CFLAGS_RELEASE -= -O2 -mtune=native

#best with gcc
#QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -flto
#QMAKE_CFLAGS_RELEASE += -O3 -march=native -flto


#http://blog.qt.digia.com/blog/2013/04/17/using-gccs-4-8-0-address-sanitizer-with-qt/?utm_source=rss&utm_medium=rss&utm_campaign=using-gccs-4-8-0-address-sanitizer-with-qt
#QMAKE_CXXFLAGS_DEBUG += -fsanitize=address -fno-omit-frame-pointer
#QMAKE_CFLAGS_DEBUG += -march=native
#QMAKE_LFLAGS_DEBUG += -march=native

#    lib/platform/SDL/context.cpp \
#    lib/platform/SDL/context.h \

SOURCES += main.cpp\
    editor/shape.cpp \
    editor/tool/terrain/action/terrainaction.cpp \
    editor/tool/terrain/action/terrainactionbrush.cpp \
    editor/tool/terrain/action/terrainactiondecrease.cpp \
    editor/tool/terrain/action/terrainactioneraser.cpp \
    editor/tool/terrain/action/terrainactionincrease.cpp \
    editor/tool/terrain/action/terrainactionsculptor.cpp \
    editor/tool/terrain/action/terrainactionsmooth.cpp \
    editor/tool/vegetation/vegetationtool.cpp \
    welcome.cpp \
    editor/driver.cpp \
    editor/tool/terrain/terrainundo.cpp \
    editor/player.cpp \
    assets/assets.cpp \
    editor/tool/vegetation/dialoggrasstexture.cpp \
    editor/tool/terrain/terrainshadow.cpp \
    canvas.cpp \
    workspace.cpp \
    dialog.cpp \
    dialog/help.cpp \
    dialog/about.cpp \
    editor/tool/water/watertool.cpp \
    editor/undostack.cpp \
    editor/undocommand.cpp \
    lib/math/box.cpp \
    lib/math/cube.cpp \
    lib/math/math.cpp \
    lib/math/matrix3.cpp \
    lib/math/matrix4.cpp \
    lib/math/plane.cpp \
    lib/math/quaternion.cpp \
    lib/math/sphere.cpp \
    lib/math/triangle.cpp \
    lib/tools/camera.cpp \
    lib/tools/diamondsquare.cpp \
    lib/tools/frustum.cpp \
    lib/tools/image.cpp \
    lib/tools/inifile.cpp \
    lib/tools/mesh.cpp \
    lib/tools/mesh3ds.cpp \
    lib/tools/meshfbx.cpp \
    lib/tools/meshloader.cpp \
    lib/tools/meshobj.cpp \
    lib/tools/moveable.cpp \
    lib/tools/perlinnoise.cpp \
    lib/tools/shell.cpp \
    lib/tools/sound.cpp \
    lib/tools/soundsystem.cpp \
    lib/module/grass/grassbase.cpp \
    lib/module/grass/grassgraphics.cpp \
    lib/module/grass/grassregion.cpp \
    lib/module/grass/grassrender.cpp \
    lib/module/model/model.cpp \
    lib/module/model/modelbase.cpp \
    lib/module/model/modelgraphics.cpp \
    lib/module/model/modelmesh.cpp \
    lib/module/model/modelrender.cpp \
    lib/module/ocean/oceanbase.cpp \
    lib/module/ocean/oceangraphics.cpp \
    lib/module/ocean/oceanrender.cpp \
    lib/module/skybox/skyboxgraphics.cpp \
    lib/module/terrain/terrainbase.cpp \
    lib/module/terrain/terraincompress.cpp \
    lib/module/terrain/terraingraphics.cpp \
    lib/module/terrain/terrainmaker.cpp \
    lib/module/terrain/terrainregion.cpp \
    lib/module/terrain/terrainrender.cpp \
    lib/module/terrain/terrainsurface.cpp \
    lib/module/terrain/transvoxel.cpp \
    lib/module/water/waterbase.cpp \
    lib/module/water/watergraphics.cpp \
    lib/module/water/waterregion.cpp \
    lib/module/modulebase.cpp \
    lib/module/modulerender.cpp \
    lib/platform/platformcontext.cpp \
    lib/platform/Qt/context.cpp \
    projectloader.cpp \
    lib/module/terrain/terrainvoxel.cpp \
    dialog/newprojectdialog.cpp \
    dialog/environmentdialog.cpp \
    dialog/optionsdialog.cpp \
    dialog/projectdialog.cpp \
    editor/tool/terrain/terrainmaterialdialog.cpp \
    editor/tool/terrain/terrainimportdialog.cpp \
    editor/tool/terrain/terrainproceduraldialog.cpp \
    assets/assettexturedialog.cpp \
    assets/assetmeshdialog.cpp \
    project.cpp \
    projectio.cpp \
    lib/module/terrain/terrainio.cpp \
    lib/module/model/modelio.cpp \
    lib/module/grass/grassio.cpp \
    lib/module/water/waterio.cpp \
    lib/module/moduleio.cpp \
    editor/imageprovider.cpp \
    lib/module/terrain/terrainmaterial.cpp \
    lib/module/skybox/skyboxio.cpp \
    window.cpp \
    atlas.cpp \
    lib/module/ocean/oceanio.cpp \
    lib/tools/elapsedcpu.cpp \
    lib/graphics/graphics.cpp \
    lib/graphics/indexbuffer.cpp \
    lib/graphics/opengl.cpp \
    lib/graphics/shader.cpp \
    lib/graphics/testopengl.cpp \
    lib/graphics/texture.cpp \
    lib/graphics/texture3d.cpp \
    lib/graphics/texturearray.cpp \
    lib/graphics/texturecube.cpp \
    lib/graphics/vertexarray.cpp \
    lib/graphics/vertexbuffrer.cpp \
    lib/module/terrain/terrainrenderforward.cpp \
    lib/module/skybox/skyboxrender.cpp \
    lib/module/editor/editorrender.cpp \
    lib/module/editor/editorbase.cpp \
    lib/module/skybox/skyboxrenderforward.cpp \
    lib/module/model/modelrenderforward.cpp \
    lib/module/ocean/oceanrenderforward.cpp \
    lib/module/editor/editorrenderforward.cpp \
    lib/math/vector2.cpp \
    lib/math/vector3.cpp \
    lib/math/vector4.cpp \
    globalsettings.cpp \
    editor/controlsettings.cpp \
    editor/instrument.cpp \
    editor/tool/toolbase.cpp \
    lib/graphics/colorrgb.cpp \
    lib/graphics/colorrgba.cpp \
    lib/tools/file.cpp \
    lib/octreenode.cpp \
    lib/mapobject.cpp \
    lib/map.cpp \
    lib/environment/lightspot.cpp \
    lib/environment/lightpoint.cpp \
    lib/environment/lightmanager.cpp \
    lib/environment/lightdirectional.cpp \
    lib/environment/lightambient.cpp \
    lib/environment/light.cpp \
    lib/environment/fog.cpp \
    lib/environment/environmentio.cpp \
    lib/environment/environment.cpp \
    lib/environment/atmosphere.cpp \
    lib/render/worldrenderforward.cpp \
    lib/render/worldrender.cpp \
    lib/render/shadowbuffer.cpp \
    lib/render/scenerendererforward.cpp \
    lib/render/scenerendererdeferred.cpp \
    lib/render/scenerender.cpp \
    lib/render/lightdirectionalrender.cpp \
    lib/render/lightambientrender.cpp \
    lib/render/gbuffer.cpp \
    lib/render/fogrender.cpp \
    lib/worldio.cpp \
    lib/worldgraphics.cpp \
    lib/worldbase.cpp \
    lib/world.cpp \
    gamepad/gamepadevent.cpp \
    lib/math/cone.cpp \
    lib/math/cylinder.cpp \
    lib/module/editor/editorgraphics.cpp \
    lib/math/mathdefine.cpp \
    gamepad/gamepad.cpp \
    gamepad/gamepadprototype.cpp \
    editor/toolbox.cpp \
    editor/tool/model/meshdatamodel.cpp \
    editor/tool/model/modeldatamodel.cpp \
    editor/tool/model/modeltool.cpp \
    editor/tool/tool.cpp \
    editor/tool/terrain/terraintool.cpp \
    editor/tool/toolcontrol.cpp \
    editor/tool/model/modelcontrol.cpp \
    editor/tool/terrain/terraincontrol.cpp \
    lib/render/vr/cardboardvrparams.cpp \
    lib/render/vr/vrdistortionmesh.cpp \
    lib/render/vr/vrframebuffer.cpp \
    lib/render/vr/vrparams.cpp \
    lib/render/vr/vrrender.cpp \
    lib/render/vr/vrsimplerender.cpp \
    lib/math/parsenumber.cpp \
    lib/module/editor/editorpositioning.cpp \
    editor/tool/model/modeleditdialog.cpp


HEADERS  += \
    editor/shape.h \
    editor/tool/terrain/action/terrainaction.h \
    editor/tool/terrain/action/terrainactionbrush.h \
    editor/tool/terrain/action/terrainactiondecrease.h \
    editor/tool/terrain/action/terrainactioneraser.h \
    editor/tool/terrain/action/terrainactionincrease.h \
    editor/tool/terrain/action/terrainactionsculptor.h \
    editor/tool/terrain/action/terrainactionsmooth.h \
    editor/tool/vegetation/vegetationtool.h \
    welcome.h \
    editor/driver.h \
    editor/tool/terrain/terrainundo.h \
    editor/player.h \
    assets/assets.h \
    editor/tool/vegetation/dialoggrasstexture.h \
    editor/tool/terrain/terrainshadow.h \
    canvas.h \
    workspace.h \
    dialog.h \
    dialog/help.h \
    dialog/about.h \
    editor/tool/water/watertool.h \
    editor/undostack.h \
    editor/undocommand.h \
    lib/math/box.h \
    lib/math/cube.h \
    lib/math/math.h \
    lib/math/matrix3.h \
    lib/math/matrix4.h \
    lib/math/plane.h \
    lib/math/quaternion.h \
    lib/math/sphere.h \
    lib/math/triangle.h \
    lib/tools/camera.h \
    lib/tools/diamondsquare.h \
    lib/tools/envi.h \
    lib/tools/frustum.h \
    lib/tools/image.h \
    lib/tools/inifile.h \
    lib/tools/mesh.h \
    lib/tools/mesh3ds.h \
    lib/tools/meshfbx.h \
    lib/tools/meshloader.h \
    lib/tools/meshobj.h \
    lib/tools/moveable.h \
    lib/tools/perlinnoise.h \
    lib/tools/shell.h \
    lib/tools/sound.h \
    lib/tools/soundsystem.h \
    lib/module/grass/grassbase.h \
    lib/module/grass/grassgraphics.h \
    lib/module/grass/grassregion.h \
    lib/module/grass/grassrender.h \
    lib/module/model/model.h \
    lib/module/model/modelbase.h \
    lib/module/model/modelgraphics.h \
    lib/module/model/modelmesh.h \
    lib/module/model/modelrender.h \
    lib/module/ocean/oceanbase.h \
    lib/module/ocean/oceangraphics.h \
    lib/module/ocean/oceanrender.h \
    lib/module/skybox/skyboxgraphics.h \
    lib/module/terrain/terrainbase.h \
    lib/module/terrain/terraincompress.h \
    lib/module/terrain/terraingraphics.h \
    lib/module/terrain/terrainmaker.h \
    lib/module/terrain/terrainregion.h \
    lib/module/terrain/terrainrender.h \
    lib/module/terrain/terrainsurface.h \
    lib/module/water/waterbase.h \
    lib/module/water/watergraphics.h \
    lib/module/water/waterregion.h \
    lib/module/modulebase.h \
    lib/module/modulerender.h \
    lib/platform/platformcontext.h \
    lib/platform/Qt/context.h \
    lib/graphics/vertexarray.h \
    lib/graphics/opengldebug.h \
    projectloader.h \
    lib/module/terrain/terrainvoxel.h \
    dialog/newprojectdialog.h \
    dialog/environmentdialog.h \
    dialog/optionsdialog.h \
    dialog/projectdialog.h \
    editor/tool/terrain/terrainproceduraldialog.h \
    editor/tool/terrain/terrainmaterialdialog.h \
    editor/tool/terrain/terrainimportdialog.h \
    assets/assettexturedialog.h \
    assets/assetmeshdialog.h \
    project.h \
    projectio.h \
    lib/module/terrain/terrainio.h \
    lib/module/model/modelio.h \
    lib/module/grass/grassio.h \
    lib/module/water/waterio.h \
    lib/module/moduleio.h \
    editor/imageprovider.h \
    lib/module/terrain/terrainmaterial.h \
    lib/module/skybox/skyboxio.h \
    window.h \
    atlas.h \
    lib/module/ocean/oceanio.h \
    lib/tools/elapsedcpu.h \
    lib/graphics/graphics.h \
    lib/graphics/indexbuffer.h \
    lib/graphics/opengl.h \
    lib/graphics/opengldebug.h \
    lib/graphics/opengldefine.h \
    lib/graphics/shader.h \
    lib/graphics/testopengl.h \
    lib/graphics/texture.h \
    lib/graphics/texture3d.h \
    lib/graphics/texturearray.h \
    lib/graphics/texturecube.h \
    lib/graphics/vertexarray.h \
    lib/graphics/vertexbuffrer.h \
    lib/module/terrain/terrainrenderforward.h \
    lib/module/skybox/skyboxrender.h \
    lib/module/editor/editorrender.h \
    lib/module/editor/editorbase.h \
    lib/module/skybox/skyboxrenderforward.h \
    lib/module/model/modelrenderforward.h \
    lib/module/ocean/oceanrenderforward.h \
    lib/module/editor/editorrenderforward.h \
    lib/math/vector4.h \
    lib/math/vector2.h \
    lib/math/vector3.h \
    globalsettings.h \
    editor/controlsettings.h \
    editor/instrument.h \
    editor/tool/toolbase.h \
    lib/graphics/colorrgb.h \
    lib/graphics/colorrgba.h \
    lib/tools/file.h \
    lib/octreenode.h \
    lib/mapobject.h \
    lib/map.h \
    lib/environment/lightspot.h \
    lib/environment/lightpoint.h \
    lib/environment/lightmanager.h \
    lib/environment/lightdirectional.h \
    lib/environment/lightambient.h \
    lib/environment/light.h \
    lib/environment/fog.h \
    lib/environment/environmentio.h \
    lib/environment/environment.h \
    lib/environment/atmosphere.h \
    lib/render/worldrenderforward.h \
    lib/render/worldrender.h \
    lib/render/shadowbuffer.h \
    lib/render/scenerendererforward.h \
    lib/render/scenerendererdeferred.h \
    lib/render/scenerender.h \
    lib/render/lightdirectionalrender.h \
    lib/render/lightambientrender.h \
    lib/render/gbuffer.h \
    lib/render/fogrender.h \
    lib/worldio.h \
    lib/worldgraphics.h \
    lib/worldbase.h \
    lib/world.h \
    gamepad/gamepad.h \
    gamepad/gamepadevent.h \
    lib/math/cone.h \
    lib/math/cylinder.h \
    lib/module/editor/editorgraphics.h \
    lib/math/mathdefine.h \
    gamepad/gamepadprototype.h \
    lib/graphics/openglfunctions.h \
    editor/toolbox.h \
    editor/tool/model/meshdatamodel.h \
    editor/tool/model/modeldatamodel.h \
    editor/tool/model/modeltool.h \
    editor/tool/tool.h \
    editor/tool/terrain/terraintool.h \
    editor/tool/toolcontrol.h \
    editor/tool/model/modelcontrol.h \
    editor/tool/terrain/terraincontrol.h \
    lib/render/vr/cardboardvrparams.h \
    lib/render/vr/vrdistortionmesh.h \
    lib/render/vr/vrframebuffer.h \
    lib/render/vr/vrparams.h \
    lib/render/vr/vrrender.h \
    lib/render/vr/vrsimplerender.h \
    lib/math/parsenumber.h \
    lib/module/editor/editorpositioning.h \
    editor/tool/model/modeleditdialog.h


OTHER_FILES += \
    t1_cs.ts \
    android/version.xml \
    android/AndroidManifest.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-es/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-ro/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    qml/Workspace.qml \
    qml/Welcome.qml \
    qml/TerrainTool.qml \
    qml/ModelTool.qml \
    qml/VegetationTool.qml \
    qml/atlas.qml \
    qml/TextureSelector.qml \
    qml/ModelSelect.qml \
    qml/TerrainImportDialog.qml \
    qml/TerrainProceduralDialog.qml \
    qml/TerrainMaterialDialog.qml \
    qml/help.qml \
    qml/about.qml \
    qml/tools/dialogGrassTexture.qml \
    qml/tools/ModelSelect.qml \
    qml/tools/ModelTool.qml \
    qml/tools/TerrainTool.qml \
    qml/tools/TextureSelector.qml \
    qml/tools/VegetationTool.qml \
    TODO.txt \
    qml/tools/WaterTool.qml \
    qml/JoyStick.qml \
    qml/AtlasWindow \
    qml/ProjectLoading.qml\
    qml/dialog/options/OptionsPage.qml \
    qml/dialog/options/pageAssets.qml \
    qml/dialog/options/pageEnvironment.qml \
    qml/dialog/options/pageTools.qml \
    qml/dialogDecoration.qml \
    qml/dialog/NewProjectDialog.qml \
    qml/dialog/AboutDialog.qml \
    qml/dialog/HelpDialog.qml \
    qml/dialog/OptionsDialog.qml \
    qml/dialog/EnvironmentDialog.qml \
    qml/dialog/ProjectDialog.qml \
    qml/tools/TerrainMaterialDialog.qml \
    qml/tools/TerrainProceduralDialog.qml \
    qml/tools/TerrainImportDialog.qml \
    qml/AssetTextureDialog.qml \
    qml/AssetMeshDialog.qml \
    lib/shader/LightDirectional.fsh \
    lib/shader/LightDirectional.vsh \
    lib/shader/Grass.fsh \
    lib/shader/Grass.vsh \
    lib/shader/Model.fsh \
    lib/shader/Model.vsh \
    lib/shader/PostProcessing.vsh \
    lib/shader/PostProcessing.fsh \
    lib/shader/Terrain.fsh \
    lib/shader/Terrain.vsh \
    lib/shader/Shadow.vsh \
    lib/shader/Shadow.fsh \
    lib/shader/Skybox.vsh \
    lib/shader/Skybox.fsh \
    lib/shader/Direct.fsh \
    lib/shader/Direct.vsh \
    lib/shader/LightAmbient.vsh \
    lib/shader/LightAmbient.fsh \
    lib/shader/TerrainForward.fsh \
    lib/shader/TerrainForward.vsh \
    lib/shader/SkyboxForward.fsh \
    lib/shader/SkyboxForward.vsh \
    lib/shader/ModelForward.fsh \
    lib/shader/ModelForward.vsh \
    lib/shader/Ocean.fsh \
    lib/shader/Ocean.vsh \
    lib/shader/OceanForward.fsh \
    lib/shader/OceanForward.vsh \
    lib/shader/Fog.fsh \
    lib/shader/Fog.vsh \
    lib/shader/VrRender.fsh \
    lib/shader/VrRender.vsh \
    qml/dialog/options/pageControl.qml \
    qml/dialog/options/pageModules.qml \
    android/src/com/zarubond/atlas/AtlasActivity.java \
    android/src/com/zarubond/atlas/GamePad.java \
    Changelog.txt

RESOURCES += \
    Resources.qrc

#evil_hack_to_fool_lupdate {

#SOURCES += \
 #   qml/Workspace.qml \
  #  qml/Welcome.qml \
  #  qml/TerrainTool.qml \
  #  qml/ModelTool.qml \
  #  qml/VegetationTool.qml \
  #  qml/atlas.qml \
  #  qml/TextureSelector.qml \
  #  qml/ModelSelect.qml \
  #  qml/dialogNewProject.qml \
  #  qml/dialogOptions.qml \
  #  qml/dialogSun.qml \
 #   qml/dialogGrassTexture.qml \
 #   qml/dialogTerrainImport.qml \
 #   qml/dialogTerrainProcedural.qml \
 #   qml/dialogTerrainMaterial.qml \
 #   qml/help.qml \
 #   qml/about.qml \
 #   qml/tools/dialogGrassTexture.qml \
 #   qml/tools/ModelSelect.qml \
 #   qml/tools/ModelTool.qml \
 #   qml/tools/TerrainTool.qml \
 #   qml/tools/TextureSelector.qml \
 #   qml/tools/VegetationTool.qml \
 #   qml/dialogDecoration.qml \
 #   qml/JoyStick.qml \
 #   qml/AtlasWindow
#}


#deployment.files=project/*

#android { #define an android block
 #   deployment.path=/assets #all assets must go to "/assets" folder of your android package
#}

#INSTALLS += deployment

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
