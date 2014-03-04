/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qSlicerTraineeDataRecorderModule_h
#define __qSlicerTraineeDataRecorderModule_h

// SlicerQt includes
#include "qSlicerLoadableModule.h"

#include "qSlicerTraineeDataRecorderModuleExport.h"

class qSlicerTraineeDataRecorderModulePrivate;

/// \ingroup Slicer_QtModules_TraineeDataRecorder
class Q_SLICER_QTMODULES_TRAINEEDATARECORDER_EXPORT qSlicerTraineeDataRecorderModule :
  public qSlicerLoadableModule
{
  Q_OBJECT
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicerTraineeDataRecorderModule(QObject *parent=0);
  virtual ~qSlicerTraineeDataRecorderModule();

  qSlicerGetTitleMacro(QTMODULE_TITLE);
  
  /// Help to use the module
  virtual QString helpText()const;

  /// Return acknowledgements
  virtual QString acknowledgementText()const;
  virtual QStringList contributors()const;
  
  /// Return a custom icon for the module
  virtual QIcon icon()const;

  /// Return the category for the module
  virtual QString category()const;
  virtual QStringList categories()const;

protected:

  /// Initialize the module. Register the volumes reader/writer
  virtual void setup();

  /// Create and return the widget representation associated to this module
  virtual qSlicerAbstractModuleRepresentation * createWidgetRepresentation();

  /// Create and return the logic associated to this module
  virtual vtkMRMLAbstractLogic* createLogic();

protected:
  QScopedPointer<qSlicerTraineeDataRecorderModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerTraineeDataRecorderModule);
  Q_DISABLE_COPY(qSlicerTraineeDataRecorderModule);

};

#endif
