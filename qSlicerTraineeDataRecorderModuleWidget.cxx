

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
#include <iostream>
// Qt includes

// SlicerQt includes
#include "qSlicerTraineeDataRecorderModuleWidget.h"
#include "ui_qSlicerTraineeDataRecorderModule.h"
#include "qSlicerIO.h"
#include "qSlicerIOManager.h"
#include "qSlicerApplication.h"
#include <QtGui>

// MRMLWidgets includes
#include <qMRMLUtils.h>

#include "vtkSlicerTraineeDataRecorderLogic.h"
#include "vtkMRMLLinearTransformNode.h"

#include "qMRMLNodeComboBox.h"
#include "vtkMRMLViewNode.h"
#include "vtkSlicerTraineeDataRecorderLogic.h"
#include "vtkMRMLTransformBufferNode.h"

//#include <sqlite3.h>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_TraineeDataRecorder
class qSlicerTraineeDataRecorderModuleWidgetPrivate: public Ui_qSlicerTraineeDataRecorderModule
{
  Q_DECLARE_PUBLIC( qSlicerTraineeDataRecorderModuleWidget ); 

protected:
  qSlicerTraineeDataRecorderModuleWidget* const q_ptr;
public:
  qSlicerTraineeDataRecorderModuleWidgetPrivate( qSlicerTraineeDataRecorderModuleWidget& object );
  ~qSlicerTraineeDataRecorderModuleWidgetPrivate();

  vtkSlicerTraineeDataRecorderLogic* logic() const;

  // Add embedded widgets here
  qSlicerTransformBufferWidget* TransformBufferWidget;
  qSlicerRecorderControlsWidget* RecorderControlsWidget;
  qSlicerMessagesWidget* MessagesWidget;
};

//-----------------------------------------------------------------------------
// qSlicerTraineeDataRecorderModuleWidgetPrivate methods



qSlicerTraineeDataRecorderModuleWidgetPrivate::qSlicerTraineeDataRecorderModuleWidgetPrivate( qSlicerTraineeDataRecorderModuleWidget& object ) : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------

qSlicerTraineeDataRecorderModuleWidgetPrivate::~qSlicerTraineeDataRecorderModuleWidgetPrivate()
{
}


vtkSlicerTraineeDataRecorderLogic* qSlicerTraineeDataRecorderModuleWidgetPrivate::logic() const
{
  Q_Q( const qSlicerTraineeDataRecorderModuleWidget );
  return vtkSlicerTraineeDataRecorderLogic::SafeDownCast( q->logic() );
}


//-----------------------------------------------------------------------------
// qSlicerTraineeDataRecorderModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerTraineeDataRecorderModuleWidget::qSlicerTraineeDataRecorderModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerTraineeDataRecorderModuleWidgetPrivate( *this ) )
{
}


qSlicerTraineeDataRecorderModuleWidget::~qSlicerTraineeDataRecorderModuleWidget()
{
}

void qSlicerTraineeDataRecorderModuleWidget::setup()
{
  Q_D(qSlicerTraineeDataRecorderModuleWidget);

  d->TotalTimeLabel=NULL;
  d->NumTransformsLabel=NULL;
  d->NumTransformsLabel=NULL;

  d->setupUi(this);
  // Embed widgets here
  d->TransformBufferWidget = qSlicerTransformBufferWidget::New( d->logic() );
  d->BufferGroupBox->layout()->addWidget( d->TransformBufferWidget );
  d->RecorderControlsWidget = qSlicerRecorderControlsWidget::New( d->TransformBufferWidget );
  d->ControlsGroupBox->layout()->addWidget( d->RecorderControlsWidget );
  d->MessagesWidget = qSlicerMessagesWidget::New( d->TransformBufferWidget );
  d->MessagesGroupBox->layout()->addWidget( d->MessagesWidget ); 
  this->Superclass::setup();

  this->BufferStatus = d->TransformBufferWidget->BufferStatus;
  this->BufferTransformsStatus = d->TransformBufferWidget->BufferTransformsStatus;
  this->BufferMessagesStatus = d->TransformBufferWidget->BufferMessagesStatus;
  
  // GUI refresh: updates every 10ms
  QTimer *t = new QTimer( this );
  connect( t,  SIGNAL( timeout() ), this, SLOT( updateWidget() ) );
  t->start(10);

  //connect button
  connect( d->saveButton, SIGNAL( clicked() ), this, SLOT( onSaveButtonClicked() ) ); 
  connect(d->pushButtonCreate, SIGNAL( clicked() ), this, SLOT( onCreateButtonClicked() ) ); 
  connect(d->pushButtonLogin, SIGNAL( clicked() ), this, SLOT( onLoginButtonClicked() ) ); 
  //
}


void qSlicerTraineeDataRecorderModuleWidget::enter()
{
  this->Superclass::enter();
  this->updateWidget();
}



void qSlicerTraineeDataRecorderModuleWidget
::updateWidget()
{
  Q_D( qSlicerTraineeDataRecorderModuleWidget );

  if ( this->BufferStatus == d->TransformBufferWidget->BufferStatus && this->BufferTransformsStatus == d->TransformBufferWidget->BufferTransformsStatus &&
    this->BufferMessagesStatus == d->TransformBufferWidget->BufferMessagesStatus )
  {
    return;
  }
  this->BufferStatus = d->TransformBufferWidget->BufferStatus;
  this->BufferTransformsStatus = d->TransformBufferWidget->BufferTransformsStatus;
  this->BufferMessagesStatus = d->TransformBufferWidget->BufferMessagesStatus;

  // The statistics should be reset to zeros if no buffer is selected
  if ( d->TransformBufferWidget->GetBufferNode() == NULL )
  {
    d->TotalTimeResultLabel->setText( "0.00" );
    d->NumTransformsResultLabel->setText( "0" );
    d->NumMessagesResultLabel->setText( "0" );
    return;
  }
  
  std::stringstream ss;

  ss.str( "" );
  ss.precision( 2 );
  ss << std::fixed << d->TransformBufferWidget->GetBufferNode()->GetTotalTime();
  d->TotalTimeResultLabel->setText( ss.str().c_str() );
  
  ss.str( "" );
  ss.precision( 0 );
  ss << std::fixed << d->TransformBufferWidget->GetBufferNode()->GetNumTransforms();;
  d->NumTransformsResultLabel->setText( ss.str().c_str() );
  
  ss.str( "" );
  ss.precision( 0 );
  ss << std::fixed << d->TransformBufferWidget->GetBufferNode()->GetNumMessages();;
  d->NumMessagesResultLabel->setText( ss.str().c_str() );
   
}


void qSlicerTraineeDataRecorderModuleWidget
::onCreateButtonClicked()
{
	Q_D( qSlicerTraineeDataRecorderModuleWidget );

	//when user enters information and clicks "Create new user"
	//check that the lineEdit fields are not null
	if(( d->lineEditUsername_2->text() != "") && (d->lineEditPassword->text() != ""))
	{
		//read database.
		

		//check that user does not already exist in the database

		//add username and password to table
		std::string s = "You are now logged in as new user:  ";
		std::string msg = s.append( d->lineEditUsername_2->text().toStdString());
		d->MessageLabel->setText(msg.c_str());
		
	}
	//MAYBE: ask the user to enter the password again...
	

	// after user created or logged in, hide authentication box.
	d->groupBoxAuthentication->hide();
}


void qSlicerTraineeDataRecorderModuleWidget
::onLoginButtonClicked()
{
	Q_D( qSlicerTraineeDataRecorderModuleWidget );

	//user enters login information and clicks "login"
	//check that the lineEdit fields are not null
	//if user exists in the database, check that the password is correct
	//display a message either way.

	// after user created or logged in, hide authentication box.
}


void qSlicerTraineeDataRecorderModuleWidget
::onSaveButtonClicked()
{
	Q_D( qSlicerTraineeDataRecorderModuleWidget );

	std::string s = "Hello ";
	std::string msg = s.append( d->lineEditName->text().toStdString());
	d->MessageLabel->setText(msg.c_str());

	d->MessageLabel->setText(this->mrmlScene()->GetRootDirectory()); // does this point to C:/Users/Nisrin?

	//mrml scene's root directory is C:/Users/Nisrin/Desktop
	//lets say we just want to save the scene to the desktop
	//when should this actually happen? 
	//After the trainee does some exercise in PerkTutor?

	/*
	WCHAR filename[MAX_PATH];
	WCHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path)) {
		d->MessageLabel->setText(filename);
	}*/

	QDir directory = "/";

	/* select a directory using file dialog */
    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"), directory.path());
    if ( path.isNull() == false )
    {
        d->MessageLabel->setText(path);
		this->updateWidget();
    }

	//write to the given directory
	ofstream myfile (path.toStdString().append("/example.txt").c_str());
	if (myfile.is_open())
	{
		myfile << "User logged in: " <<  d->lineEditName->text().toStdString().c_str();
		cout << "Saved username to file";
		myfile.close();
	}
	else cout << "Unable to open myfile";
	
	//write to sqlite database
	//sqlite3.create("users.db");
	
	//init
	//QString databaseFilename = "TraineeDatabase";
	//Q_Q(TraineeDatabase);
	//q->openDatabase(databaseFilename);

	//when the button is clicked, add another user to the database
	
	
	
	/*
	QProgressDialog dialog;
    dialog.setModal( false);
    dialog.setLabelText( "Hello!" );
    dialog.show();
	*/
}