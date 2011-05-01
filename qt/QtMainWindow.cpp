
#include <QtGui>

#include "QtMainWindow.h"

namespace QtSudoku
{

QtMainWindow::QtMainWindow( QAbstractItemView *view,
                            QWidget *parent,
                            Qt::WindowFlags flags )
    : QMainWindow( parent, flags ),
      _view( view )
{
    setCentralWidget( _view );

    createActions();
    createMenus();
    createStatusBar();

    readSettings();

    setUnifiedTitleAndToolBarOnMac( true );
}

QAction* QtMainWindow::GetNewAction()
{
    return _newAction;
}

QAction* QtMainWindow::GetLoadAction()
{
    return _loadAction;
}

QAction* QtMainWindow::GetExitAction()
{
    return _exitAction;
}

QAction* QtMainWindow::GetUndoAction()
{
    return _undoAction;
}

QAction* QtMainWindow::GetRedoAction()
{
    return _redoAction;
}

QAction* QtMainWindow::GetMarkHintsAction()
{
    return _markHintsAction;
}

QAction* QtMainWindow::GetSolveAction()
{
    return _solveAction;
}

void QtMainWindow::closeEvent( QCloseEvent *event )
{
    // we can warn the user about closing
    if ( true )//askSaveSettings() )
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void QtMainWindow::about()
{
    QMessageBox::about( this,
                        tr("About Sudoku"),
                        tr("This is a sample Sudoku game. The underlying "
                           "Code uses design pattern in order to keep the "
                           "system easy to maintain and extend.  It also "
                           "demonstrates the use of Qt for the GUI built "
                           "on top of the Sudoku engine.\n"
                           "All code written by Matt Durak") );
}

void QtMainWindow::createActions()
{
    _newAction = new QAction( tr("&New"), this );
    _newAction->setShortcuts( QKeySequence::New );
    _newAction->setStatusTip( tr("Create a new (blank) puzzle") );

    _loadAction = new QAction( tr("&Load..."), this );
    _loadAction->setShortcuts( QKeySequence::Open );
    _loadAction->setStatusTip( tr("Load a puzzle from a file") );

    _exitAction = new QAction( tr("E&xit"), this );
    _exitAction->setShortcuts( QKeySequence::Quit );
    _exitAction->setStatusTip( tr("Quit the game") );
    connect( _exitAction, SIGNAL( triggered() ),  this, SLOT( close() ) );

    _undoAction = new QAction( tr("&Undo"), this );
    _undoAction->setShortcuts( QKeySequence::Undo );
    _undoAction->setStatusTip( tr("Undo previous move") );
    _undoAction->setEnabled( false );
    // connect to _undoAction, SLOT(setEnabled( bool ))

    _redoAction = new QAction( tr("&Redo"), this );
    _redoAction->setShortcuts( QKeySequence::Redo );
    _redoAction->setStatusTip( tr("Redo last undone move") );
    _redoAction->setEnabled( false );
    // connect to _redoAction, SLOT(setEnabled( bool ))

    _markHintsAction = new QAction( tr("&Mark Hints"), this );
    _markHintsAction->setStatusTip( tr("Show marks for all valid contents") );
    _markHintsAction->setEnabled( false );

    _solveAction = new QAction( tr("Show &Solution"), this );
    _solveAction->setStatusTip( tr("This will solve a puzzle for you") );
    _solveAction->setEnabled( false );

    _aboutAction = new QAction( tr("&About"), this );
    _aboutAction->setStatusTip( tr("About this application") );
    connect( _aboutAction, SIGNAL(triggered()), this, SLOT(about()) );

    _aboutQtAction = new QAction( tr("About &Qt"), this );
    _aboutQtAction->setStatusTip( tr("Show the Qt library's About box") );
    connect( _aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
}

void QtMainWindow::createMenus()
{
    _fileMenu = menuBar()->addMenu( tr("&File") );
    _fileMenu->addAction( _newAction );
    _fileMenu->addAction( _loadAction );
    _fileMenu->addSeparator();
    _fileMenu->addAction( _exitAction);

    _editMenu = menuBar()->addMenu( tr("&Edit")) ;
    _editMenu->addAction( _undoAction );
    _editMenu->addAction( _redoAction );
    _editMenu->addSeparator();
    _editMenu->addAction( _markHintsAction );
    _editMenu->addAction( _solveAction );

    menuBar()->addSeparator();

    _helpMenu = menuBar()->addMenu( tr("&Help") );
    _helpMenu->addAction( _aboutAction );
    _helpMenu->addAction( _aboutQtAction );
}

void QtMainWindow::createStatusBar()
{
    statusBar()->showMessage( tr("Ready") );
}

void QtMainWindow::readSettings()
{
    QSettings settings( "Matt Durak", "Sudoku" );
    QPoint pos = settings.value( "pos", QPoint( 200, 200 ) ).toPoint();
    QSize size = settings.value( "size", QSize( 800, 600 ) ).toSize();
    resize(size);
    move(pos);
}

void QtMainWindow::writeSettings()
{
    QSettings settings( "Matt Durak", "Sudoku" );
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool QtMainWindow::askSaveSettings()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(
        this,
        tr("Sudoku"),
        tr("You are about to quit.\n"
           "Do you want to save your window layout?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
    if ( ret == QMessageBox::Save )
    {
        writeSettings();
    }
    else if ( ret == QMessageBox::Cancel )
    {
        return false;
    }
    return true;
}

}
