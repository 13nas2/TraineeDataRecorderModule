/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerMessagesWidget_h
#define __qSlicerMessagesWidget_h

// Qt includes
#include "qSlicerWidget.h"

// FooBar Widgets includes
#include "qSlicerTransformRecorderModuleWidgetsExport.h"
#include "ui_qSlicerMessagesWidget.h"

#include "vtkSlicerTransformRecorderLogic.h"
#include "vtkMRMLTransformRecorderNode.h"
#include "vtkMRMLLinearTransformNode.h"

class qSlicerMessagesWidgetPrivate;

/// \ingroup Slicer_QtModules_CreateModels
class Q_SLICER_MODULE_TRANSFORMRECORDER_WIDGETS_EXPORT 
qSlicerMessagesWidget : public qSlicerWidget
{
  Q_OBJECT
public:
  typedef qSlicerWidget Superclass;
  qSlicerMessagesWidget(QWidget *parent=0);
  virtual ~qSlicerMessagesWidget();

  static qSlicerMessagesWidget* New( vtkSlicerTransformRecorderLogic* newTRLogic );

  void SetLogic( vtkSlicerTransformRecorderLogic* newTRLogic );

protected slots:

  void onAddMessageButtonClicked();
  void onRemoveMessageButtonClicked();
  void onClearMessagesButtonClicked();

  virtual void setup();
  virtual void enter();

protected:
  QScopedPointer<qSlicerMessagesWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerMessagesWidget);
  Q_DISABLE_COPY(qSlicerMessagesWidget);

  vtkSlicerTransformRecorderLogic* trLogic;

  void resetTable();
};

#endif
