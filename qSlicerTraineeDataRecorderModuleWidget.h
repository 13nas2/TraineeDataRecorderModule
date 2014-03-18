
#ifndef __qSlicerTraineeDataRecorderModuleWidget_h
#define __qSlicerTraineeDataRecorderModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"
#include <QtGui>
#include "qSlicerTraineeDataRecorderModuleExport.h"
#include "qSlicerTransformBufferWidget.h"
#include "qSlicerMessagesWidget.h"
#include "qSlicerRecorderControlsWidget.h"


class qSlicerTraineeDataRecorderModuleWidgetPrivate;
class vtkMRMLNode;
class vtkMRMLTraineeDataRecorderNode;


/// \ingroup Slicer_QtModules_TraineeDataRecorder
class Q_SLICER_QTMODULES_TRAINEEDATARECORDER_EXPORT qSlicerTraineeDataRecorderModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerTraineeDataRecorderModuleWidget(QWidget *parent=0);
  virtual ~qSlicerTraineeDataRecorderModuleWidget();

  // This widget will keep track if the buffer is changed
  unsigned long BufferStatus;
  // These quantities might be repeated by different buffers, so we still need the above
  unsigned long BufferTransformsStatus;
  unsigned long BufferMessagesStatus;
  
protected:
  QScopedPointer<qSlicerTraineeDataRecorderModuleWidgetPrivate> d_ptr;
  
  virtual void setup();
  virtual void enter();

protected slots:

  void updateWidget();
  void onSaveButtonClicked();
  void onCreateButtonClicked();
  void onLoginButtonClicked(); 
  void onOKButtonClicked();
  void onSaveSceneButtonClicked();

private:
  Q_DECLARE_PRIVATE(qSlicerTraineeDataRecorderModuleWidget);
  Q_DISABLE_COPY(qSlicerTraineeDataRecorderModuleWidget);

  bool selectionsInitialized;
};

#endif
