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

#ifndef __qSlicerTraineeDataRecorderIO_h
#define __qSlicerTraineeDataRecorderIO_h

// SlicerQt includes
#include "qSlicerFileReader.h"
class qSlicerTraineeDataRecorderIOPrivate;

// Slicer includes
class vtkSlicerTraineeDataRecorderLogic;

//-----------------------------------------------------------------------------
class qSlicerTraineeDataRecorderIO
  : public qSlicerFileReader
{
  Q_OBJECT
public:
  typedef qSlicerFileReader Superclass;
  qSlicerTraineeDataRecorderIO( vtkSlicerTraineeDataRecorderLogic* newTraineeDataRecorderLogic = 0, QObject* parent = 0 );
  virtual ~qSlicerTraineeDataRecorderIO();

  void setTraineeDataRecorderLogic( vtkSlicerTraineeDataRecorderLogic* newTraineeDataRecorderLogic);
  vtkSlicerTraineeDataRecorderLogic* TraineeDataRecorderLogic() const;

  virtual QString description() const;
  virtual IOFileType fileType() const;
  virtual QStringList extensions() const;

  virtual bool load( const IOProperties& properties );
  
protected:
  QScopedPointer< qSlicerTraineeDataRecorderIOPrivate > d_ptr;

private:
  Q_DECLARE_PRIVATE( qSlicerTraineeDataRecorderIO );
  Q_DISABLE_COPY( qSlicerTraineeDataRecorderIO );
};

#endif
