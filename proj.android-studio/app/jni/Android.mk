LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../../Classes/AppDelegate.cpp \
../../../Classes/Controllers/ActionController.cpp \
../../../Classes/Controllers/BoardController.cpp \
../../../Classes/Controllers/GameController.cpp \
../../../Classes/Controllers/HiderBoardController.cpp \
../../../Classes/Controllers/HopplingBoardController.cpp \
../../../Classes/Controllers/LevelController.cpp \
../../../Classes/Controllers/PathBoardController.cpp \
../../../Classes/Controllers/PoolController.cpp \
../../../Classes/Controllers/PopsicleBoardController.cpp \
../../../Classes/Controllers/ScoreController.cpp \
../../../Classes/Controllers/SoundController.cpp \
../../../Classes/Controllers/SpawnController.cpp \
../../../Classes/Controllers/THopplingBoardController.cpp \
../../../Classes/Controllers/TopplingBoardController.cpp \
../../../Classes/Controllers/WaffleBoardController.cpp \
../../../Classes/Ext/ActionEx.cpp \
../../../Classes/Ext/Math.cpp \
../../../Classes/Ext/Popup.cpp \
../../../Classes/Ext/ProgressBar.cpp \
../../../Classes/General/Constants.cpp \
../../../Classes/General/Utils.cpp \
../../../Classes/Layers/BoardLayer.cpp \
../../../Classes/Layers/Dialogs.cpp \
../../../Classes/Models/BoardModels/BoardModel.cpp \
../../../Classes/Models/BoardModels/Cell.cpp \
../../../Classes/Models/BoardModels/FallPath.cpp \
../../../Classes/Models/BoardModels/HolesQueue.cpp \
../../../Classes/Models/BoardModels/Match.cpp \
../../../Classes/Models/BoardModels/PieceSwapper.cpp \
../../../Classes/Models/BoardModels/Score.cpp \
../../../Classes/Models/DataModels/BoardLayerData.cpp \
../../../Classes/Models/DataModels/CustomSpawnTableItem.cpp \
../../../Classes/Models/DataModels/Level.cpp \
../../../Classes/Models/DataModels/SeekerPriorityTable.cpp \
../../../Classes/Models/DataModels/SpawnTable.cpp \
../../../Classes/Models/DataModels/TileColors.cpp \
../../../Classes/Models/DataModels/UserData.cpp \
../../../Classes/Models/ShowObjects/AnimationShowObject.cpp \
../../../Classes/Models/ShowObjects/ShowObject.cpp \
../../../Classes/Models/ShowObjects/TileShowObject.cpp \
../../../Classes/Models/TilePools/LayeredMatchObjectPool.cpp \
../../../Classes/Models/TilePools/NodePool.cpp \
../../../Classes/Models/TilePools/TilePool.cpp \
../../../Classes/Models/TilePools/TileShowObjectPool.cpp \
../../../Classes/Models/Tiles/ChocolateChipObject.cpp \
../../../Classes/Models/Tiles/ColorPieObject.cpp \
../../../Classes/Models/Tiles/ConveyorBeltObject.cpp \
../../../Classes/Models/Tiles/CookieTile.cpp \
../../../Classes/Models/Tiles/DisplayCaseObject.cpp \
../../../Classes/Models/Tiles/FixTiles.cpp \
../../../Classes/Models/Tiles/HiderGroup.cpp \
../../../Classes/Models/Tiles/HiderSegmentObject.cpp \
../../../Classes/Models/Tiles/LargeTile.cpp \
../../../Classes/Models/Tiles/LavaCakeObject.cpp \
../../../Classes/Models/Tiles/LayeredCrackerTile.cpp \
../../../Classes/Models/Tiles/LayeredMatchObject.cpp \
../../../Classes/Models/Tiles/MovingTile.cpp \
../../../Classes/Models/Tiles/ObjectSpinnerObject.cpp \
../../../Classes/Models/Tiles/PopsicleObject.cpp \
../../../Classes/Models/Tiles/SpawnerObject.cpp \
../../../Classes/Models/Tiles/ThopplerTile.cpp \
../../../Classes/Models/Tiles/Tile.cpp \
../../../Classes/Native/AdsControl.cpp \
../../../Classes/Native/GGBridge.cpp \
../../../Classes/Scenes/GamePlayScene.cpp \
../../../Classes/Scenes/LevelMapScene.cpp \
../../../Classes/Scenes/MainMenuScene.cpp \
../../../Classes/Scenes/StartScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
