#-------------------------------------------------
#
# Project created by QtCreator 2012-06-09T19:47:42
#
#-------------------------------------------------

QT       += core gui opengl xml
TARGET = Atlas
TEMPLATE = app
#x11extras
#QMAKE_CXXFLAGS_RELEASE -= -O3
#QMAKE_CXXFLAGS_RELEASE -= -march=native

#QMAKE_CXXFLAGS += -0fast -march=native
#QMAKE_CXXFLAGS -= -02
#http://blog.qt.digia.com/blog/2013/04/17/using-gccs-4-8-0-address-sanitizer-with-qt/?utm_source=rss&utm_medium=rss&utm_campaign=using-gccs-4-8-0-address-sanitizer-with-qt
#QMAKE_CXXFLAGS_DEBUG += -fsanitize=address -fno-omit-frame-pointer
#QMAKE_CFLAGS_DEBUG += -march=native
#QMAKE_LFLAGS_DEBUG += -march=native

SOURCES += main.cpp\
        atlas.cpp \
    tools/camera.cpp \
    tools/shader.cpp \
    tools/opengl.cpp \
    tools/texture.cpp \
    tools/texturecube.cpp \
    world/octreenode.cpp \
    world/mapobject.cpp \
    world/map.cpp \
    world/complement/light.cpp \
    tools/inifile.cpp \
    tools/math/vertex4.cpp \
    tools/math/vertex3.cpp \
    tools/math/triangle.cpp \
    tools/math/sphere.cpp \
    tools/math/matrix4.cpp \
    tools/math/matrix3.cpp \
    tools/math/math.cpp \
    tools/math/cube.cpp \
    editor/editor.cpp \
    postprocess.cpp \
    tools/frustum.cpp \
    tools/math/plane.cpp \
    tools/simplexnoise.cpp \
    editor/shape.cpp \
    editor/tool/editortool.cpp \
    editor/tool/action/terrainaction.cpp \
    editor/tool/terraintool.cpp \
    editor/tool/modeltool.cpp \
    editor/tool/action/terrainactionbrush.cpp \
    editor/tool/action/terrainactiondecrease.cpp \
    editor/tool/action/terrainactioneraser.cpp \
    editor/tool/action/terrainactionincrease.cpp \
    editor/tool/action/terrainactionsculptor.cpp \
    editor/tool/action/terrainactionsmooth.cpp \
    editor/tool/vegetationtool.cpp \
    welcome.cpp \
    dialog/dialognewproject.cpp \
    editor/tool/dialog/dialogterrainprocedural.cpp \
    session.cpp \
    editor/tool/dialog/dialogterrainimport.cpp \
    project/project.cpp \
    editor/navigator.cpp \
    project/projecthandler.cpp \
    dialog/dialogsun.cpp \
    world/complement/lightmanager.cpp \
    editor/driver.cpp \
    editor/tool/terrainundo.cpp \
    tools/shell.cpp \
    world/module/water/waterbase.cpp \
    world/module/grass/grassrender.cpp \
    world/module/partmanager.cpp \
    world/module/partdrawer.cpp \
    world/module/model/modelrender.cpp \
    world/module/model/model.cpp \
    world/module/terrain/transvoxel.cpp \
    world/module/terrain/terrainsurface.cpp \
    world/module/terrain/terrainrender.cpp \
    world/module/terrain/terrainregion.cpp \
    world/module/terrain/terraingraphics.cpp \
    world/module/terrain/terrainbase.cpp \
    world/mapdrawer.cpp \
    project/terrainio.cpp \
    project/partio.cpp \
    project/modelio.cpp \
    project/grassio.cpp \
    world/module/grass/grassgraphics.cpp \
    world/module/terrain/terraincompress.cpp \
    tools/vertexbuffrer.cpp \
    tools/indexbuffer.cpp \
    tools/graphics.cpp \
    world/module/model/modelgraphics.cpp \
    tools/texture3d.cpp \
    tools/texturearray.cpp \
    project/skyboxio.cpp \
    world/module/skybox/skybox.cpp \
    world/module/skybox/skyboxgraphics.cpp \
    world/module/water/watergraphics.cpp \
    canvas.cpp \
    editor/tool/dialog/dialogterrainmaterial.cpp \
    dialog/configdialog.cpp \
    tools/moveable.cpp \
    tools/mesh.cpp \
    tools/mesh3ds.cpp \
    tools/meshobj.cpp \
    tools/meshloader.cpp \
    tools/sound.cpp \
    tools/soundsystem.cpp \
    tools/math/quaternion.cpp \
    tools/math/box.cpp \
    editor/player.cpp \
    world/module/model/modelmesh.cpp \
    world/module/model/modelbase.cpp \
    world/complement/atmosphere.cpp \
    world/complement/fog.cpp \
    world/module/grass/grassgarden.cpp \
    world/module/grass/grassbase.cpp \
    tools/perlinnoise.cpp \
    assets/assets.cpp \
    assets/texturedialog.cpp \
    assets/meshdialog.cpp \
    tools/meshfbx.cpp \
    tools/diamondsquare.cpp \
    settings.cpp \
    world/module/skybox/skyboxrender.cpp \
    editor/tool/dialog/dialoggrasstexture.cpp


HEADERS  += atlas.h \
    tools/camera.h \
    tools/shader.h \
    tools/opengl.h \
    tools/texture.h \
    tools/texturecube.h \
    world/octreenode.h \
    world/mapobject.h \
    world/map.h \
    world/complement/light.h \
    tools/inifile.h \
    tools/math/vertex4.h \
    tools/math/vertex3.h \
    tools/math/triangle.h \
    tools/math/sphere.h \
    tools/math/matrix4.h \
    tools/math/matrix3.h \
    tools/math/math.h \
    tools/math/cube.h \
    editor/editor.h \
    postprocess.h \
    tools/frustum.h \
    tools/math/plane.h \
    tools/simplexnoise.h \
    editor/shape.h \
    editor/tool/editortool.h \
    editor/tool/action/terrainaction.h \
    editor/tool/terraintool.h \
    editor/tool/modeltool.h \
    editor/tool/action/terrainactionbrush.h \
    editor/tool/action/terrainactiondecrease.h \
    editor/tool/action/terrainactioneraser.h \
    editor/tool/action/terrainactionincrease.h \
    editor/tool/action/terrainactionsculptor.h \
    editor/tool/action/terrainactionsmooth.h \
    editor/tool/vegetationtool.h \
    welcome.h \
    editor/tool/dialog/dialogterrainprocedural.h \
    dialog/dialognewproject.h \
    session.h \
    editor/tool/dialog/dialogterrainimport.h \
    project/project.h \
    editor/navigator.h \
    project/projecthandler.h \
    dialog/dialogsun.h \
    world/complement/lightmanager.h \
    editor/driver.h \
    editor/tool/terrainundo.h \
    tools/shell.h \
    world/component.h \
    world/module/water/waterbase.h \
    world/module/grass/grassrender.h \
    world/module/grass/grassio.h \
    world/module/partmanager.h \
    world/module/partdrawer.h \
    world/module/model/modelrender.h \
    world/module/model/modelio.h \
    world/module/model/model.h \
    world/module/terrain/terrainsurface.h \
    world/module/terrain/terrainrender.h \
    world/module/terrain/terrainregion.h \
    world/module/terrain/terrainio.h \
    world/module/terrain/terraingraphics.h \
    world/module/terrain/terrainbase.h \
    world/module/partio.h \
    world/world.h \
    world/mapdrawer.h \
    project/worldhandler.h \
    project/world/worldhandler.h \
    project/world/terrainio.h \
    project/world/modelio.h \
    project/world/grassio.h \
    project/world/partio.h \
    project/terrainio.h \
    project/partio.h \
    project/modelio.h \
    project/grassio.h \
    world/module/grass/grassgraphics.h \
    world/module/terrain/terraincompress.h \
    tools/vertexbuffrer.h \
    tools/indexbuffer.h \
    tools/graphics.h \
    world/module/model/modelgraphics.h \
    tools/texture3d.h \
    tools/texturearray.h \
    project/skyboxio.h \
    world/module/skybox/skybox.h \
    world/module/skybox/skyboxgraphics.h \
    world/module/water/watergraphics.h \
    canvas.h \
    editor/tool/dialog/dialogterrainmaterial.h \
    dialog/configdialog.h \
    tools/moveable.h \
    tools/mesh3ds.h \
    tools/mesh.h \
    tools/meshobj.h \
    tools/meshloader.h \
    tools/sound.h \
    tools/soundsystem.h \
    tools/math/quaternion.h \
    tools/math/box.h \
    editor/player.h \
    world/module/model/modelmesh.h \
    world/module/model/modelbase.h \
    world/complement/atmosphere.h \
    world/complement/fog.h \
    world/module/grass/grassgarden.h \
    world/module/grass/grassbase.h \
    tools/perlinnoise.h \
    assets/assets.h \
    assets/texturedialog.h \
    assets/meshdialog.h \
    tools/meshfbx.h \
    tools/diamondsquare.h \
    settings.h \
    world/module/skybox/skyboxrender.h \
    editor/tool/dialog/dialoggrasstexture.h

FORMS    += atlas.ui \
    editor/tool/terraintool.ui \
    editor/tool/modeltool.ui \
    editor/tool/vegetationtool.ui \
    welcome.ui \
    dialog/dialognewproject.ui \
    editor/tool/dialog/dialogterraintexture.ui \
    editor/tool/dialog/dialogterrainprocedural.ui \
    editor/tool/dialog/dialogterrainimport.ui \
    dialog/dialogsun.ui \
    dialog/configdialog.ui \
    assets/texturedialog.ui \
    assets/meshdialog.ui \
    editor/tool/dialog/dialoggrasstexture.ui

OTHER_FILES += \
    shader/Terrain.fsh \
    shader/Terrain.vsh \
    shader/Skybox.fsh \
    shader/Skybox.vsh \
    shader/PostProcessing.fsh \
    shader/PostProcessing.vsh \
    shader/Model.fsh \
    shader/Model.vsh \
    shader/Grass.fsh \
    shader/Grass.vsh \
    shader/Shadow.fsh \
    shader/Shadow.vsh \
    shader/Water.frag \
    shader/Water.vert \ 
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
    android/res/values/strings.xml

RESOURCES += \
    Resources.qrc

#deployment.files=project/*

#android { #define an android block
 #   deployment.path=/assets #all assets must go to "/assets" folder of your android package
#}

#INSTALLS += deployment
