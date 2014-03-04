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

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes

// SlicerQt includes
#include "qSlicerTraineeDataRecorderIO.h"

// Logic includes
#include "vtkSlicerTraineeDataRecorderLogic.h"

// MRML includes

// VTK includes
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qSlicerTraineeDataRecorderIOPrivate
{
public:
  vtkSmartPointer<vtkSlicerTraineeDataRecorderLogic> TraineeDataRecorderLogic;
};

//-----------------------------------------------------------------------------
qSlicerTraineeDataRecorderIO::qSlicerTraineeDataRecorderIO( vtkSlicerTraineeDataRecorderLogic* newTraineeDataRecorderLogic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerTraineeDataRecorderIOPrivate)
{
  this->setTraineeDataRecorderLogic( newTraineeDataRecorderLogic );
}

//-----------------------------------------------------------------------------
qSlicerTraineeDataRecorderIO::~qSlicerTraineeDataRecorderIO()
{
}

//-----------------------------------------------------------------------------
void qSlicerTraineeDataRecorderIO::setTraineeDataRecorderLogic(vtkSlicerTraineeDataRecorderLogic* newTraineeDataRecorderLogic)
{
  Q_D(qSlicerTraineeDataRecorderIO);
  d->TraineeDataRecorderLogic = newTraineeDataRecorderLogic;
}

//-----------------------------------------------------------------------------
vtkSlicerTraineeDataRecorderLogic* qSlicerTraineeDataRecorderIO::TraineeDataRecorderLogic() const
{
  Q_D(const qSlicerTraineeDataRecorderIO);
  return d->TraineeDataRecorderLogic;
}

//-----------------------------------------------------------------------------
QString qSlicerTraineeDataRecorderIO::description() const
{
  return "Transform Buffer";
}

//-----------------------------------------------------------------------------
qSlicerIO::IOFileType qSlicerTraineeDataRecorderIO::fileType() const
{
  return QString("Transform Buffer");
}

//-----------------------------------------------------------------------------
QStringList qSlicerTraineeDataRecorderIO::extensions() const
{
  return QStringList() << "Transform Buffer (*.xml)";
}

//-----------------------------------------------------------------------------
bool qSlicerTraineeDataRecorderIO::load(const IOProperties& properties)
{
  Q_D(qSlicerTraineeDataRecorderIO);
  Q_ASSERT( properties.contains("fileName") );
  QString fileName = properties["fileName"].toString();
  
  vtkSmartPointer< vtkMRMLTransformBufferNode > importBufferNode;
  importBufferNode.TakeReference( vtkMRMLTransformBufferNode::SafeDownCast( this->mrmlScene()->CreateNodeByClass( "vtkMRMLTransformBufferNode" ) ) );
  importBufferNode->SetScene( this->mrmlScene() );
  this->mrmlScene()->AddNode( importBufferNode );
  d->TraineeDataRecorderLogic->ImportFromFile( importBufferNode, fileName.toStdString() );

  return true; // TODO: Check to see read was successful first
}
