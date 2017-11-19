add_library(KF5ObjectTrackingDeclarative objecttrackingplugin.cpp)
target_link_libraries(KF5ObjectTrackingDeclarative Qt5::Qml KF5::ObjectTracking)

set_target_properties(KF5ObjectTrackingDeclarative PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/org/kde/ObjectTracking)

install(TARGETS KF5ObjectTrackingDeclarative DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/ObjectTracking)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/ObjectTracking)
