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

// Qt includes
#include <QtPlugin>

// TraineeDataRecorder Logic includes
#include <vtkSlicerTraineeDataRecorderLogic.h>

// TraineeDataRecorder includes
#include "qSlicerTraineeDataRecorderModule.h"
#include "qSlicerTraineeDataRecorderModuleWidget.h"
#include "qSlicerTraineeDataRecorderIO.h"

// Slicer includes
#include "qSlicerNodeWriter.h"
#include "qSlicerCoreIOManager.h"
#include "qSlicerCoreApplication.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerTraineeDataRecorderModule, qSlicerTraineeDataRecorderModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_TraineeDataRecorder
class qSlicerTraineeDataRecorderModulePrivate
{
public:
  qSlicerTraineeDataRecorderModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerTraineeDataRecorderModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerTraineeDataRecorderModulePrivate::qSlicerTraineeDataRecorderModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerTraineeDataRecorderModule methods

//-----------------------------------------------------------------------------
qSlicerTraineeDataRecorderModule::qSlicerTraineeDataRecorderModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerTraineeDataRecorderModulePrivate)
{
}

//-----------------------------------------------------------------------------
QString qSlicerTraineeDataRecorderModule::category()const
{
  // return "Developer Tools";
  return "";
}

//-----------------------------------------------------------------------------

QStringList qSlicerTraineeDataRecorderModule::categories() const
{
  return QStringList() << "Perk Tutor";
}
//-----------------------------------------------------------------------------
qSlicerTraineeDataRecorderModule::~qSlicerTraineeDataRecorderModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerTraineeDataRecorderModule::helpText()const
{
  return "The purpose of the Trainee Data Recorder module is to record and save tool trajectories associated with needle-based interventions. For help on how to use this module visit: <a href='http://www.perktutor.org/'>PerkTutor</a>.";
}

//-----------------------------------------------------------------------------
QString qSlicerTraineeDataRecorderModule::acknowledgementText()const
{
  return "This work was was funded by Cancer Care Ontario and the Ontario Consortium for Adaptive Interventions in Radiation Oncology (OCAIRO)";
}


//-----------------------------------------------------------------------------
QStringList qSlicerTraineeDataRecorderModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Nisrin Abou-Seido (Queen's University)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerTraineeDataRecorderModule::icon()const
{
  return QIcon(":/Icons/TraineeDataRecorder.png");
}

//-----------------------------------------------------------------------------
void qSlicerTraineeDataRecorderModule::setup()
{
  this->Superclass::setup();

  qSlicerCoreApplication* app = qSlicerCoreApplication::application();
  vtkSlicerTraineeDataRecorderLogic* TraineeDataRecorderLogic = vtkSlicerTraineeDataRecorderLogic::SafeDownCast( this->logic() );
  
  // Register the IO
  app->coreIOManager()->registerIO( new qSlicerTraineeDataRecorderIO( TraineeDataRecorderLogic, this ) );
  app->coreIOManager()->registerIO( new qSlicerNodeWriter( "TraineeDataRecorder", QString( "TransformBuffer" ), QStringList() << "vtkMRMLTransformBufferNode", this ) );
 
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerTraineeDataRecorderModule::createWidgetRepresentation()
{
  return new qSlicerTraineeDataRecorderModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerTraineeDataRecorderModule::createLogic()
{
  return vtkSlicerTraineeDataRecorderLogic::New();
}


