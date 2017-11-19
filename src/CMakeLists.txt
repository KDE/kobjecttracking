add_subdirectory(declarative)

add_library(KF5ObjectTracking objecttracking.cpp objecttimetracker.cpp objectwatcher.cpp objectdebug.cpp)
add_library(KF5::ObjectTracking ALIAS KF5ObjectTracking)

target_link_libraries(KF5ObjectTracking PUBLIC Qt5::Core)

set_target_properties(KF5ObjectTracking PROPERTIES
   VERSION ${KOBJECTTRACKING_VERSION_STRING}
   SOVERSION ${KOBJECTTRACKING_SOVERSION}
   EXPORT_NAME ObjectTracking
)

generate_export_header(KF5ObjectTracking BASE_NAME KObjectTracking EXPORT_FILE_NAME kobjecttracking_export.h)

ecm_generate_headers(KObjectTracking_CamelCase_HEADERS
  HEADER_NAMES
    ObjectTracking
#     ObjectDebug

  REQUIRED_HEADERS KObjectTracking_HEADERS
  PREFIX KObjectTracking
)

target_include_directories(KF5ObjectTracking PUBLIC "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}>")

install(TARGETS KF5ObjectTracking EXPORT KF5ObjectTrackingTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${KObjectTracking_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KObjectTracking COMPONENT Devel)
install(FILES
    ${KObjectTracking_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kobjecttracking_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/kobjecttracking COMPONENT Devel)

if(BUILD_QCH)
    ecm_add_qch(
        KF5ObjectTracking_QCH
        NAME KObjectTracking
        BASE_NAME KF5ObjectTracking
        VERSION ${KF5_VERSION}
        ORG_DOMAIN org.kde
        SOURCES ${KObjectTracking_HEADERS}
        BLANK_MACROS
            KOBJECTTRACKING_EXPORT
            KOBJECTTRACKING_DEPRECATED
            KOBJECTTRACKING_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KObjectTracking LIB_NAME KF5ObjectTracking FILENAME_VAR COREPRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KObjectTracking)
install(FILES ${COREPRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

