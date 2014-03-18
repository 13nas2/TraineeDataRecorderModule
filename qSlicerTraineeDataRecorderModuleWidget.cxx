

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

  d->setupUi(this);

  // If adding widgets, embed widgets here: eg: //d->TransformBufferWidget = qSlicerTransformBufferWidget::New( d->logic() );
  
  this->Superclass::setup();

  // GUI refresh: updates every 10ms
  QTimer *t = new QTimer( this );
  connect( t,  SIGNAL( timeout() ), this, SLOT( updateWidget() ) );
  t->start(10);

  //connect button
  connect( d->saveButton, SIGNAL( clicked() ), this, SLOT( onSaveButtonClicked() ) ); 
  connect(d->pushButtonCreate, SIGNAL( clicked() ), this, SLOT( onCreateButtonClicked() ) ); 
  connect(d->pushButtonLogin, SIGNAL( clicked() ), this, SLOT( onLoginButtonClicked() ) ); 
  connect(d->OkButton, SIGNAL( clicked() ), this, SLOT(onOKButtonClicked()));
  
  d->groupBox->hide();
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
}

void qSlicerTraineeDataRecorderModuleWidget
::onCreateButtonClicked()
{
	Q_D( qSlicerTraineeDataRecorderModuleWidget );

	//retrieve database
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	database.setDatabaseName("trainees.db");
	if(database.open())
	{
		//if database opens successfully, check whether there is a users table.
		//if not, then create one.
		QStringList tables = database.tables(QSql::Tables);
		if(!tables.contains("users"))
		{
			QSqlQuery query;
			query.exec("CREATE table users(username varchar(30) primary key, password varchar(30));");
			QMessageBox::information(0, "Creating Table users	", "Executed create query");
		}

		//when user enters information and clicks "Create new user"
		//check that the lineEdit fields are not null
		if(( d->lineEditUsername->text() != "") && (d->lineEditPassword->text() != ""))
		{
			//check that user does not already exist in the database
			QSqlQuery checkUserQuery(database);
			checkUserQuery.prepare("Select * from users where username = ?");
			checkUserQuery.bindValue(0, d->lineEditUsername->text());
			checkUserQuery.exec();

			if(checkUserQuery.next()) //found the user  /*//MAYBE: ask the user to enter the password again to login?...*/
			{
				QMessageBox::information(0, "Cannot create user", "This user already exists. Please use login button instead");
			}
			else
			{
				//add username and password to table
				QSqlQuery query;
				query.prepare("INSERT INTO users (username, password) " "VALUES (:username, :password)");
				query.bindValue(":username", d->lineEditUsername->text());
				query.bindValue(":password", d->lineEditPassword->text());
				query.exec();

				QString s = "You have created new user:  ";
				s = s.append( d->lineEditUsername->text());
				QMessageBox::information(0, "Created User", s);
					
				// after user created, clear username and password fields to allow user to login
				d->lineEditUsername->setText("");
				d->lineEditPassword->setText("");
			}
		}
	}
	else{
		QMessageBox::critical(0, QObject::tr("Database Error"),database.lastError().text());
	}
	database.close();
}


void qSlicerTraineeDataRecorderModuleWidget
::onLoginButtonClicked()
{
	/*user enters login information and clicks "login"
	  check that the lineEdit fields are not empty
	  if user/password pair exists in the database, user is now logged in - display a message either way.
    */

	// after user created or logged in, hide authentication box.
	Q_D( qSlicerTraineeDataRecorderModuleWidget );

	//retrieve database
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	database.setDatabaseName("trainees.db");
	if(database.open())
	{
		//if database opens successfully, check whether there is a users table.
		//if not, then create one.
		QStringList tables = database.tables(QSql::Tables);
		if(!tables.contains("users"))
		{
			QMessageBox::information(0, "Error", "database is still empty :(");	
		}
		else
		{
			if(( d->lineEditUsername->text() != "") && (d->lineEditPassword->text() != ""))
			{
				//check that user does not already exist in the database
				QSqlQuery checkUserQuery(database);
				checkUserQuery.prepare("Select * from users where username = ? and password = ?");
				checkUserQuery.bindValue(0, d->lineEditUsername->text());
				checkUserQuery.bindValue(1, d->lineEditPassword->text());
				checkUserQuery.exec();

				if(checkUserQuery.next())//found username/password pair.
				{
					QMessageBox::information(0, "Login successful", "You are now logged in.");
					d->groupBoxAuthentication->hide();
					d->groupBox->show();
				}
				else{
					QMessageBox::information(0, "Login failed", "Either the username or password is incorrect.");
					d->lineEditUsername->setText("");
					d->lineEditPassword->setText("");
				}
			}
		}
	}
	else{
		QMessageBox::critical(0, QObject::tr("Database Error"),database.lastError().text());
	}
	database.close();
}

void qSlicerTraineeDataRecorderModuleWidget
::onOKButtonClicked()
{
	Q_D( qSlicerTraineeDataRecorderModuleWidget );
	d->groupBox->show();

}

void qSlicerTraineeDataRecorderModuleWidget
::onSaveButtonClicked()
{
	Q_D( qSlicerTraineeDataRecorderModuleWidget );

	//mrml scene's root directory is C:/Users/Nisrin/Desktop
	//lwhen user clicks SAVE the mrml scene is automatically saved to a given location.

	//save the studentID in the database, 
	//retrieve database
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	database.setDatabaseName("trainees.db");
	if(database.open())
	{
		//if database opens successfully, check whether there is a students/trainee table.
		//if not, then create one.
		QStringList tables = database.tables(QSql::Tables);
		if(!tables.contains("students"))
		{
			QSqlQuery query;
			query.exec("CREATE table students(id int primary key, name varchar(30));");
			QMessageBox::information(0, "Creating Table students	", "Executed create query");
		}

		//when user clicks save check that studentID field is not null.
		if(( d->lineEditStudentID->text() != ""))
		{
			//check that user does not already exist in the database
			QSqlQuery checkStudentQuery(database);
			checkStudentQuery.prepare("Select * from students where id = ?");
			checkStudentQuery.bindValue(0, d->lineEditStudentID->text());
			checkStudentQuery.exec();

			if(checkStudentQuery.next())
			{
				//found student
				QMessageBox::information(0, "Student Exists", "Don't need to enter any information, this student exists in the database");
			}
			else{

				//ask user for student's name and other information. *** TO DO***
				QSqlQuery query;
				query.prepare("INSERT INTO students(id, firstname) " "VALUES (:id, :firstname)");
				query.bindValue(":id", d->lineEditStudentID->text());
				query.bindValue(":firstname", d->lineEditFirstName->text());
				query.exec();

				QString s = "You have created new student:  ";
				s = s.append( d->lineEditFirstName->text());
				QMessageBox::information(0, "Created Student", s);	
			}
		}
	}
	else{
		QMessageBox::critical(0, QObject::tr("Database Error"),database.lastError().text());
	}
	database.close();
	
	QDir directory = "/";
	//select a directory using file dialog 
    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"), directory.path());
   
	/*if ( path.isNull() == false )
    {
        d->MessageLabel->setText(path);
		this->updateWidget();
    }
	*/

	//write to the given directory
	ofstream myfile (path.toStdString().append("/example.txt").c_str());
	if (myfile.is_open())
	{
		myfile << "User logged in: " <<  d->lineEditUsername->text().toStdString().c_str();
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
}


void qSlicerTraineeDataRecorderModuleWidget
::onSaveSceneButtonClicked()
{
	Q_D( qSlicerTraineeDataRecorderModuleWidget );
	QMessageBox::information(0, "Saving", "Saving Scene to Default Location");
}