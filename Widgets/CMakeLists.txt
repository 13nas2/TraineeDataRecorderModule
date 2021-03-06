project(qSlicer${MODULE_NAME}ModuleWidgets)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_SLICER_MODULE_${MODULE_NAME_UPPER}_WIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  )

set(${KIT}_SRCS
  qSlicerMessagesWidget.h
  qSlicerMessagesWidget.cxx
  qSlicerRecorderControlsWidget.h
  qSlicerRecorderControlsWidget.cxx
  qSlicerTransformBufferWidget.h
  qSlicerTransformBufferWidget.cxx
  )

set(${KIT}_MOC_SRCS
  qSlicerMessagesWidget.h
  qSlicerRecorderControlsWidget.h
  qSlicerTransformBufferWidget.h
  )

set(${KIT}_UI_SRCS
  Resources/UI/qSlicerMessagesWidget.ui
  Resources/UI/qSlicerRecorderControlsWidget.ui
  Resources/UI/qSlicerTransformBufferWidget.ui
  )

set(${KIT}_RESOURCES
  Resources/UI/qSlicerMessagesWidget.ui
  Resources/UI/qSlicerRecorderControlsWidget.ui
  Resources/UI/qSlicerTransformBufferWidget.ui
  )

set(${KIT}_TARGET_LIBRARIES
  vtkSlicer${MODULE_NAME}ModuleLogic
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleWidgets(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  UI_SRCS ${${KIT}_UI_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  RESOURCES ${${KIT}_RESOURCES}
  WRAP_PYTHONQT
  )

#-----------------------------------------------------------------------------


#-----------------------------------------------------------------------------
