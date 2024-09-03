cmake_minimum_required(VERSION 3.16)

set(BLISHHUDSHADER kwin_effect_blishhud_shader)

project(${BLISHHUDSHADER})

set(QT_MIN_VERSION "5.15.0")
set(KF5_MIN_VERSION "5.91")


cmake_policy(SET CMP0054 NEW)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include (FeatureSummary)

add_definitions (-Wall -Werror)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

find_package (ECM REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${ECM_MODULE_PATH}
    ${ECM_KDE_MODULE_DIR}
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_SOURCE_DIR}/cmake/Modules
)

include(KDEInstallDirs)
include(KDECMakeSettings)

feature_summary (WHAT ALL)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Gui
    Core
    DBus
    UiTools
    Widgets
    X11Extras
    OpenGL
    Network
    Xml
)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5X11Extras_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

# required frameworks by Core
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    Crash
    GlobalAccel
    I18n
    KIO
    Service
    Init
    Notifications
    Service
    WidgetsAddons
    WindowSystem
    GuiAddons
)

include(ECMSetupQtPluginMacroNames)
ecm_setup_qtplugin_macro_names(
    JSON_ARG2
    "KWIN_EFFECT_FACTORY"
    JSON_ARG3
    "KWIN_EFFECT_FACTORY_ENABLED"
    "KWIN_EFFECT_FACTORY_SUPPORTED"
    JSON_ARG4
    "KWIN_EFFECT_FACTORY_SUPPORTED_ENABLED"
    CONFIG_CODE_VARIABLE
    PACKAGE_SETUP_AUTOMOC_VARIABLES
)

execute_process(COMMAND kf5-config --install module OUTPUT_VARIABLE MODULEPATH OUTPUT_STRIP_TRAILING_WHITESPACE)

find_path(EFFECTS_H kwineffects.h PATH_SUFFIXES kf5)

if (EFFECTS_H)
    include_directories(${EFFECTS_H})
else (EFFECTS_H)
    message(STATUS "didnt find kwineffects.h, not building effects")
endif (EFFECTS_H)

find_library(KWIN_EFFECTS NAMES kwineffects PATH_SUFFIXES kf5)
find_library(KWIN_GLUTILS NAMES kwinglutils PATH_SUFFIXES kf5)
find_library(OPENGL NAMES GL)

if (NOT KWIN_EFFECTS)
    message(STATUS "didnt find kwineffects lib, not building effects")
endif (NOT KWIN_EFFECTS)

if (NOT KWIN_GLUTILS)
    message(STATUS "didnt find kwin glutils lib, not building effects")
endif (NOT KWIN_GLUTILS)

if (NOT OPENGL)
    message(STATUS "didnt find opengl, not building effects")
endif (NOT OPENGL)

if (NOT EFFECTS_H OR NOT KWIN_GLUTILS OR NOT KWIN_EFFECTS OR NOT OPENGL)
    message(FATAL_ERROR "cant continue")
endif (NOT EFFECTS_H OR NOT KWIN_GLUTILS OR NOT KWIN_EFFECTS OR NOT OPENGL)

macro(KWIN4_ADD_EFFECT_MODULE name)
    kcoreaddons_add_plugin(${name} SOURCES ${ARGN} INSTALL_NAMESPACE "kwin/effects/plugins")
endmacro()
#######################################
# Effect
find_package (Qt5 REQUIRED COMPONENTS
    Core
)

find_package (KF5 REQUIRED COMPONENTS
    Service
    WindowSystem
    GlobalAccel
    I18n
)

set(BLISHHUD_SHADER_SRCS plugin.cpp blishhud_shader.cpp)

kwin4_add_effect_module(${BLISHHUDSHADER} ${BLISHHUD_SHADER_SRCS})

target_link_libraries(${BLISHHUDSHADER}
    PUBLIC
        Qt5::Core
        Qt5::Gui
        ${KWIN_EFFECTS}
        ${KWIN_GLUTILS}
        epoxy
        GL

    PRIVATE
        KF5::ConfigCore
        KF5::ConfigGui
        KF5::CoreAddons
        KF5::ConfigWidgets
        KF5::GuiAddons
        KF5::WindowSystem
    
        KF5::ConfigWidgets
        KF5::GlobalAccel
        KF5::I18n
        KF5::XmlGui
        KF5::Service
        #KWinEffects::KWinEffects
        )


install(FILES data/transparent_black_core.frag DESTINATION ${DATAPATH}share/kwin/shaders)

#######################################
# Config
find_package (Qt5 REQUIRED COMPONENTS
    Core
    DBus
)

find_package (KF5 REQUIRED COMPONENTS
    ConfigWidgets
    GlobalAccel
    I18n
    Service
    XmlGui
)

