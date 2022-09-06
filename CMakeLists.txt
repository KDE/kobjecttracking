cmake_minimum_required(VERSION 3.0)

set(KF5_VERSION "5.40.0") # handled by release scripts
project(ObjectTracking VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.40.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

set(REQUIRED_QT_VERSION 5.7.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Qml)

include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)

include(ECMAddQch)
include(ECMPoQmTools)

ecm_setup_version(PROJECT VARIABLE_PREFIX KOBJECTTRACKING
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kobjecttracking_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5ObjectTrackingConfigVersion.cmake"
                        SOVERSION 5)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()

add_subdirectory(src)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5ObjectTracking")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5ObjectTrackingCore_QCH KF5ObjectTrackingUi_QCH
        FILE KF5ObjectTrackingQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5ObjectTrackingQchTargets.cmake\")")
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5ObjectTrackingConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/KF5ObjectTrackingConfig.cmake"
                              INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/KF5ObjectTrackingConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/KF5ObjectTrackingConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"  COMPONENT Devel )

install(EXPORT KF5ObjectTrackingTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5ObjectTrackingTargets.cmake NAMESPACE KF5:: )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kobjecttracking_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel )

# contains list of debug categories, for kdebugsettings
# install(FILES kobjecttracking.categories DESTINATION ${KDE_INSTALL_CONFDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
