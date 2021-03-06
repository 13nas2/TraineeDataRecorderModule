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

// .NAME vtkSlicerTraineeDataRecorderLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerTraineeDataRecorderLogic_h
#define __vtkSlicerTraineeDataRecorderLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"
#include "vtkMRML.h"
#include "vtkMRMLNode.h"
#include "vtkMRMLScene.h"
#include "vtkObjectFactory.h"

#include "vtkSmartPointer.h"

// MRML includes
// class vtkMRMLIGTLConnectorNode;
class vtkMRMLViewNode;
#include "vtkMRMLTransformBufferNode.h"
// STD includes
#include <cstdlib>

#include "vtkSlicerTraineeDataRecorderModuleLogicExport.h"



/// \ingroup Slicer_QtModules_TraineeDataRecorder
class VTK_SLICER_TRAINEEDATARECORDER_MODULE_LOGIC_EXPORT vtkSlicerTraineeDataRecorderLogic :
  public vtkSlicerModuleLogic
{
public:
  vtkTypeMacro(vtkSlicerTraineeDataRecorderLogic,vtkSlicerModuleLogic);

  static vtkSlicerTraineeDataRecorderLogic *New();

  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkSlicerTraineeDataRecorderLogic();
  virtual ~vtkSlicerTraineeDataRecorderLogic();

  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void UpdateFromMRMLScene();
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node);
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node);
  
public:
  /// Initialize listening to MRML events
  void InitializeEventListeners();

  void ProcessMRMLNodesEvents( vtkObject* caller, unsigned long event, void* callData );
  
  double GetCurrentTimestamp();

  // Functions to control recording.
  void AddObservedTransformNode( vtkMRMLTransformBufferNode* bufferNode, vtkMRMLNode* node );
  void RemoveObservedTransformNode( vtkMRMLTransformBufferNode* bufferNode, vtkMRMLNode* node );
  bool IsObservedTransformNode( vtkMRMLTransformBufferNode* bufferNode, vtkMRMLNode* node );

  void SetRecording( vtkMRMLTransformBufferNode* bufferNode, bool isRecording );
  bool GetRecording( vtkMRMLTransformBufferNode* bufferNode );
  void ClearTransforms( vtkMRMLTransformBufferNode* bufferNode );

  void AddMessage( vtkMRMLTransformBufferNode* bufferNode, std::string name, double time );
  void RemoveMessage( vtkMRMLTransformBufferNode* bufferNode, int index );
  void ClearMessages( vtkMRMLTransformBufferNode* bufferNode );

  void ImportFromFile( vtkMRMLTransformBufferNode* bufferNode, std::string fileName );
  void ExportToFile( vtkMRMLTransformBufferNode* bufferNode, std::string fileName );

  void AddTransform( vtkMRMLTransformBufferNode* bufferNode, vtkMRMLTransformNode* transformNode );
  
private:

  vtkSlicerTraineeDataRecorderLogic(const vtkSlicerTraineeDataRecorderLogic&); // Not implemented
  void operator=(const vtkSlicerTraineeDataRecorderLogic&);               // Not implemented
  // Reference to the module MRML node.

  std::vector<vtkMRMLTransformBufferNode*> RecordingBuffers;
  double Clock0;

};

#endif

