cmake_minimum_required(VERSION 3.20)

project(kwin_effect_blishhud_shader VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Force path to set CMAKE_INSTALL_PREFIX" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DQT_NO_DEBUG_OUTPUT")

set(QT_MIN_VERSION "6.4.0")
set(QT_MAJOR_VERSION 6)
set(KF_MIN_VERSION 6)

find_package(ECM "5.100" REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${ECM_MODULE_PATH}
        ${ECM_KDE_MODULE_DIR}
)

find_package(KF6 QUIET COMPONENTS ConfigWidgets)

if(${KF6_FOUND} EQUAL 0)
    set(QT_MIN_VERSION "5.15")
    set(QT_MAJOR_VERSION 5)
    set(KF_MIN_VERSION "5.78")
endif ()

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

# required frameworks by Core
find_package(KF${QT_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS ConfigWidgets)

if(${QT_MAJOR_VERSION} EQUAL 6)
    find_package(KF${QT_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS KCMUtils)
    find_package(KWin REQUIRED COMPONENTS kwineffects)
    message(STATUS "Found KWin Version: ${KWin_VERSION}")
else()
    find_package(KWinEffects REQUIRED COMPONENTS kwineffects kwinglutils)
    message(STATUS "Found KWin Version: ${KWinEffects_VERSION}")
endif ()

if(${KF_MIN_VERSION} EQUAL 6)
    set(KWIN_EFFECT_INCLUDE_FILE "/usr/include/kwin/effect/effect.h")
else ()
    set(KWIN_EFFECT_INCLUDE_FILE "/usr/include/kwineffects.h")
endif ()
execute_process(
        COMMAND sh -c "grep '#define KWIN_EFFECT_API_VERSION_MINOR' ${KWIN_EFFECT_INCLUDE_FILE} | awk '{print \$NF}'"
        OUTPUT_VARIABLE KWIN_EFFECT_API_VERSION_MINOR OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Found KWinEffect API Version: ${KWIN_EFFECT_API_VERSION_MINOR}")

find_package(epoxy REQUIRED)
find_package(XCB REQUIRED COMPONENTS XCB)
find_package(KWinDBusInterface CONFIG REQUIRED)

set(effect_SRCS
    blishhud_shader.cpp
    plugin.cpp
)

kcoreaddons_add_plugin(kwin_effect_blishhud_shader SOURCES ${effect_SRCS} INSTALL_NAMESPACE "kwin/effects/plugins/")

target_link_libraries(kwin_effect_blishhud_shader
    Qt${QT_MAJOR_VERSION}::DBus
    KF${QT_MAJOR_VERSION}::ConfigGui
)
if(${QT_MAJOR_VERSION} EQUAL 6)
    target_link_libraries(kwin_effect_blishhud_shader
            KWin::kwin
            Qt${QT_MAJOR_VERSION}::CorePrivate
    )
else()
    target_link_libraries(kwin_effect_blishhud_shader
            KWinEffects::kwineffects
            KWinEffects::kwinglutils
    )
endif()

install(FILES data/transparent_black_core.frag DESTINATION ${DATAPATH}share/kwin/shaders)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "FloFri")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "kwin-x11 (>= 4:5.27.10~) | kwin (>= 4:5.27.10~)")
include(CPack)
