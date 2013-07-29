
/*LICENSE_START*/
/*
 * Copyright 2012 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome 
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR  
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/

#include <iostream>

#define __CIFTI_CONNECTIVITY_MATRIX_VIEW_CONTROLLER_DECLARE__
#include "CiftiConnectivityMatrixViewController.h"
#undef __CIFTI_CONNECTIVITY_MATRIX_VIEW_CONTROLLER_DECLARE__

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QSignalMapper>

#include "Brain.h"
#include "CiftiFiberOrientationFile.h"
#include "CiftiFiberTrajectoryFile.h"
#include "CiftiMappableConnectivityMatrixDataFile.h"
#include "CursorDisplayScoped.h"
#include "EventManager.h"
#include "EventGraphicsUpdateAllWindows.h"
#include "EventSurfaceColoringInvalidate.h"
#include "EventUserInterfaceUpdate.h"
#include "FiberTrajectoryMapProperties.h"
#include "GuiManager.h"
#include "WuQMessageBox.h"
#include "WuQtUtilities.h"

using namespace caret;

static const char* FILE_POINTER_PROPERTY_NAME = "filePointer";

/**
 * \class caret::CiftiConnectivityMatrixViewController 
 * \brief View-Controller connectivity files
 * \ingroup GuiQt
 */
/**
 * Constructor.
 */
CiftiConnectivityMatrixViewController::CiftiConnectivityMatrixViewController(const Qt::Orientation /*orientation*/,
                                                                             QWidget* parent)
: QWidget(parent)
{
    m_gridLayout = new QGridLayout();
    WuQtUtilities::setLayoutSpacingAndMargins(m_gridLayout, 2, 2);
    m_gridLayout->setColumnStretch(COLUMN_ENABLE_CHECKBOX, 0);
    m_gridLayout->setColumnStretch(COLUMN_COPY_BUTTON, 0);
    m_gridLayout->setColumnStretch(COLUMN_NAME_LINE_EDIT, 100);
    m_gridLayout->setColumnStretch(COLUMN_ORIENTATION_FILE_COMBO_BOX, 100);
    const int titleRow = m_gridLayout->rowCount();
    m_gridLayout->addWidget(new QLabel("On"),
                            titleRow, COLUMN_ENABLE_CHECKBOX);
    m_gridLayout->addWidget(new QLabel("Copy"),
                            titleRow, COLUMN_COPY_BUTTON);
    m_gridLayout->addWidget(new QLabel("Connectivity File"),
                            titleRow, COLUMN_NAME_LINE_EDIT);
    m_gridLayout->addWidget(new QLabel("Fiber Orientation File"),
                            titleRow, COLUMN_ORIENTATION_FILE_COMBO_BOX);
    
    m_signalMapperFileEnableCheckBox = new QSignalMapper(this);
    QObject::connect(m_signalMapperFileEnableCheckBox, SIGNAL(mapped(int)),
                     this, SLOT(enabledCheckBoxClicked(int)));
    
    m_signalMapperFileCopyToolButton = new QSignalMapper(this);
    QObject::connect(m_signalMapperFileCopyToolButton, SIGNAL(mapped(int)),
                     this, SLOT(copyToolButtonClicked(int)));
    
    m_signalMapperFiberOrientationFileComboBox = new QSignalMapper(this);
    QObject::connect(m_signalMapperFiberOrientationFileComboBox, SIGNAL(mapped(int)),
                     this, SLOT(fiberOrientationFileComboBoxActivated(int)));
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(m_gridLayout);
    layout->addStretch();
    
    s_allCiftiConnectivityMatrixViewControllers.insert(this);

    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_USER_INTERFACE_UPDATE);
}

/**
 * Destructor.
 */
CiftiConnectivityMatrixViewController::~CiftiConnectivityMatrixViewController()
{
    s_allCiftiConnectivityMatrixViewControllers.erase(this);
    
    EventManager::get()->removeAllEventsFromListener(this);
}

/**
 * Update this view controller.
 */
void 
CiftiConnectivityMatrixViewController::updateViewController()
{
    Brain* brain = GuiManager::get()->getBrain();
    
    std::vector<CaretMappableDataFile*> files;
    
    std::vector<CiftiFiberTrajectoryFile*> trajFiles;
    brain->getConnectivityFiberTrajectoryFiles(trajFiles);
    for (std::vector<CiftiFiberTrajectoryFile*>::iterator trajIter = trajFiles.begin();
         trajIter != trajFiles.end();
         trajIter++) {
        files.push_back(*trajIter);
    }
    
    std::vector<CiftiMappableConnectivityMatrixDataFile*> matrixFiles;
    brain->getAllCiftiConnectivityMatrixFiles(matrixFiles);
    for (std::vector<CiftiMappableConnectivityMatrixDataFile*>::iterator matrixIter = matrixFiles.begin();
         matrixIter != matrixFiles.end();
         matrixIter++) {
        files.push_back(*matrixIter);
    }
    
    
    const int32_t numFiles = static_cast<int32_t>(files.size());

    for (int32_t i = 0; i < numFiles; i++) {
        QCheckBox* checkBox = NULL;
        QLineEdit* lineEdit = NULL;
        QToolButton* copyToolButton = NULL;
        QComboBox* comboBox = NULL;
        
        if (i < static_cast<int32_t>(m_fileEnableCheckBoxes.size())) {
            checkBox = m_fileEnableCheckBoxes[i];
            lineEdit = m_fileNameLineEdits[i];
            copyToolButton = m_fileCopyToolButtons[i];
            comboBox = m_fiberOrientationFileComboBoxes[i];
        }
        else {
            checkBox = new QCheckBox("");
            m_fileEnableCheckBoxes.push_back(checkBox);
            
            lineEdit = new QLineEdit();
            m_fileNameLineEdits.push_back(lineEdit);
            
            copyToolButton = new QToolButton();
            copyToolButton->setText("Copy");
            copyToolButton->setToolTip("Copy loaded row data to a new CIFTI Scalar File");
            m_fileCopyToolButtons.push_back(copyToolButton);
            
            comboBox = new QComboBox();
            m_fiberOrientationFileComboBoxes.push_back(comboBox);
            
            QObject::connect(copyToolButton, SIGNAL(clicked()),
                             m_signalMapperFileCopyToolButton, SLOT(map()));
            m_signalMapperFileCopyToolButton->setMapping(copyToolButton, i);
            
            QObject::connect(checkBox, SIGNAL(clicked(bool)),
                             m_signalMapperFileEnableCheckBox, SLOT(map()));
            m_signalMapperFileEnableCheckBox->setMapping(checkBox, i);
            
            QObject::connect(comboBox, SIGNAL(activated(int)),
                             m_signalMapperFiberOrientationFileComboBox, SLOT(map()));
            m_signalMapperFiberOrientationFileComboBox->setMapping(comboBox, i);
            
            const int row = m_gridLayout->rowCount();
            m_gridLayout->addWidget(checkBox,
                                    row, COLUMN_ENABLE_CHECKBOX);
            m_gridLayout->addWidget(copyToolButton,
                                    row, COLUMN_COPY_BUTTON);
            m_gridLayout->addWidget(lineEdit,
                                    row, COLUMN_NAME_LINE_EDIT);
            m_gridLayout->addWidget(comboBox,
                                    row, COLUMN_ORIENTATION_FILE_COMBO_BOX);
        }
        
        const CiftiMappableConnectivityMatrixDataFile* matrixFile = dynamic_cast<const CiftiMappableConnectivityMatrixDataFile*>(files[i]);
        const CiftiFiberTrajectoryFile* trajFile = dynamic_cast<const CiftiFiberTrajectoryFile*>(files[i]);
        
        bool checkStatus = false;
        if (matrixFile != NULL) {
            checkStatus = matrixFile->isMapDataLoadingEnabled(0);
        }
        else if (trajFile != NULL) {
            checkStatus = trajFile->isDataLoadingEnabled();
        }
        else {
            CaretAssertMessage(0, "Has a new file type been added?");
        }
        
        checkBox->setChecked(checkStatus);
        checkBox->setProperty(FILE_POINTER_PROPERTY_NAME,
                              qVariantFromValue((void*)files[i]));
        lineEdit->setText(files[i]->getFileName());
    }


    const int32_t numItems = static_cast<int32_t>(m_fileEnableCheckBoxes.size());
    for (int32_t i = 0; i < numItems; i++) {
        const bool showIt = (i < numFiles);
        
        m_fileEnableCheckBoxes[i]->setVisible(showIt);
        m_fileCopyToolButtons[i]->setVisible(showIt);
        m_fileNameLineEdits[i]->setVisible(showIt);
        m_fiberOrientationFileComboBoxes[i]->setVisible(showIt);
        m_fiberOrientationFileComboBoxes[i]->setEnabled(showIt);
    }
    
    updateFiberOrientationComboBoxes();
}

/**
 * Update the fiber orientation combo boxes.
 */
void
CiftiConnectivityMatrixViewController::updateFiberOrientationComboBoxes()
{
    std::vector<CiftiFiberOrientationFile*> orientationFiles;
    GuiManager::get()->getBrain()->getConnectivityFiberOrientationFiles(orientationFiles);
    const int32_t numOrientationFiles = static_cast<int32_t>(orientationFiles.size());
    
    const int32_t numItems = static_cast<int32_t>(m_fiberOrientationFileComboBoxes.size());
    for (int32_t i = 0; i < numItems; i++) {
        QComboBox* comboBox = m_fiberOrientationFileComboBoxes[i];
        CiftiMappableConnectivityMatrixDataFile* matrixFile = NULL;
        CiftiFiberTrajectoryFile* trajFile = NULL;
        
        if (comboBox->isEnabled()) {
            getFileAtIndex(i,
                           matrixFile,
                           trajFile);
        }
        
        if (trajFile != NULL) {
            int32_t selectedIndex = 0;
            CiftiFiberOrientationFile* selectedOrientationFile = trajFile->getMatchingFiberOrientationFile();
            
            comboBox->clear();
            
            for (int32_t iOrient = 0; iOrient < numOrientationFiles; iOrient++) {
                CiftiFiberOrientationFile* orientFile = orientationFiles[iOrient];
                if (trajFile->isFiberOrientationFileCombatible(orientFile)) {
                    if (orientFile == selectedOrientationFile) {
                        selectedIndex = iOrient;
                    }
                    
                    comboBox->addItem(orientFile->getFileNameNoPath(),
                                      qVariantFromValue((void*)orientFile));
                }
            }
            
            if ((selectedIndex >= 0)
                && (selectedIndex < comboBox->count())) {
                comboBox->setCurrentIndex(selectedIndex);
            }
        }
        else {
            comboBox->clear();
        }
        
        const bool showComboBox = (trajFile != NULL);
        m_fiberOrientationFileComboBoxes[i]->setVisible(showComboBox);
        m_fiberOrientationFileComboBoxes[i]->setEnabled(showComboBox);
        std::cout << "Show Orientation File Combo Box: "
        << i
        << ": "
        << qPrintable(AString::fromBool(showComboBox))
        << std::endl;
    }
}

/**
 * Called when an enabled check box changes state.
 *
 * @param indx
 *    Index of checkbox that was clicked.
 */
void
CiftiConnectivityMatrixViewController::enabledCheckBoxClicked(int indx)
{
    CaretAssertVectorIndex(m_fileEnableCheckBoxes, indx);
    const bool newStatus = m_fileEnableCheckBoxes[indx]->isChecked();
    
    CiftiMappableConnectivityMatrixDataFile* matrixFile = NULL;
    CiftiFiberTrajectoryFile* trajFile = NULL;
    
    getFileAtIndex(indx,
                   matrixFile,
                   trajFile);
    
    if (matrixFile != NULL) {
        matrixFile->setMapDataLoadingEnabled(0,
                                             newStatus);
    }
    else if (trajFile != NULL) {
        trajFile->setDataLoadingEnabled(newStatus);
    }
    else {
        CaretAssertMessage(0, "Has a new file type been added?");
    }
    
    updateOtherCiftiConnectivityMatrixViewControllers();
}

/**
 * Get the file associated with the given index.  One of the output files
 * will be NULL and the other will be non-NULL.
 *
 * @param indx
 *    The index.
 * @param ciftiMatrixFileOut
 *    If there is a CIFTI matrix file at the given index, this will be non-NULL.
 * @param ciftiTrajFileOut
 *    If there is a CIFTI trajectory file at the given index, this will be non-NULL.
 */
void
CiftiConnectivityMatrixViewController::getFileAtIndex(const int32_t indx,
                                                      CiftiMappableConnectivityMatrixDataFile* &ciftiMatrixFileOut,
                                                      CiftiFiberTrajectoryFile* &ciftiTrajFileOut)
{
    CaretAssertVectorIndex(m_fileEnableCheckBoxes, indx);
    void* ptr = m_fileEnableCheckBoxes[indx]->property(FILE_POINTER_PROPERTY_NAME).value<void*>();
    CiftiMappableDataFile* mapFilePointer = (CiftiMappableDataFile*)ptr;
    
    ciftiMatrixFileOut = dynamic_cast<CiftiMappableConnectivityMatrixDataFile*>(mapFilePointer);
    ciftiTrajFileOut   = dynamic_cast<CiftiFiberTrajectoryFile*>(mapFilePointer);
    
    if (ciftiMatrixFileOut != NULL) {
        /* OK */
    }
    else if (ciftiTrajFileOut != NULL) {
        /* OK */
    }
    else {
        CaretAssertMessage(0,
                           "Has a new file type been added?");
    }
}

/**
 * Called when fiber orientation file combo box changed.
 *
 * @param indx
 *    Index of combo box that was changed.
 */
void
CiftiConnectivityMatrixViewController::fiberOrientationFileComboBoxActivated(int indx)
{
    CaretAssertVectorIndex(m_fiberOrientationFileComboBoxes, indx);
    
    CiftiMappableConnectivityMatrixDataFile* matrixFile = NULL;
    CiftiFiberTrajectoryFile* trajFile = NULL;
    
    getFileAtIndex(indx,
                   matrixFile,
                   trajFile);
    
    CaretAssertMessage(trajFile,
                       "Selected orientation file but trajectory file is invalid.");
    
    QComboBox* cb = m_fiberOrientationFileComboBoxes[indx];
    void* ptr = cb->itemData(indx,
                             Qt::UserRole).value<void*>();
    CaretAssert(ptr);
    CiftiFiberOrientationFile* orientFile = (CiftiFiberOrientationFile*)ptr;
    
    std::vector<CiftiFiberOrientationFile*> orientationFiles;
    GuiManager::get()->getBrain()->getConnectivityFiberOrientationFiles(orientationFiles);
    if (std::find(orientationFiles.begin(),
                  orientationFiles.end(),
                  orientFile) == orientationFiles.end()) {
        CaretAssertMessage(0,
                           "Selected fiber orientation file is no longer valid.");
        return;
    }
    
    trajFile->setMatchingFiberOrientationFile(orientFile);
    
    updateOtherCiftiConnectivityMatrixViewControllers();
    EventManager::get()->sendEvent(EventGraphicsUpdateAllWindows().getPointer());
}


/**
 * Called when copy tool button is clicked.
 *
 * @param indx
 *    Index of copy tool button that was clicked.
 */
void
CiftiConnectivityMatrixViewController::copyToolButtonClicked(int indx)
{
    CursorDisplayScoped cursor;
    cursor.showWaitCursor();
    
    CiftiMappableConnectivityMatrixDataFile* matrixFile = NULL;
    CiftiFiberTrajectoryFile* trajFile = NULL;
    
    getFileAtIndex(indx,
                   matrixFile,
                   trajFile);
    
    
    try {
        Brain* brain = GuiManager::get()->getBrain();
        
        if (matrixFile != NULL) {
            brain->convertCiftiMatrixFileToCiftiScalarFile(matrixFile);
        }
        else if (trajFile != NULL) {
            throw DataFileException("Conversion to CIFTI Traj Parcel not implemented.");
        }
        else {
            CaretAssertMessage(0,
                               "Has a new file type been added?");
        }
        
        EventManager::get()->sendEvent(EventUserInterfaceUpdate().getPointer());
    }
    catch (const DataFileException& dfe) {
        cursor.restoreCursor();
        WuQMessageBox::errorOk(m_fileCopyToolButtons[indx],
                               dfe.whatString());
    }
}

///**
// * Update graphics and GUI after
// */
//void 
//CiftiConnectivityMatrixViewController::updateUserInterfaceAndGraphicsWindow()
//{
//    updateOtherCiftiConnectivityMatrixViewControllers();
//    
//    EventManager::get()->sendEvent(EventSurfaceColoringInvalidate().getPointer());
//    EventManager::get()->sendEvent(EventGraphicsUpdateAllWindows().getPointer());
//}

/**
 * Update other connectivity view controllers other than 'this' instance
 * that contain the same connectivity file.
 */
void 
CiftiConnectivityMatrixViewController::updateOtherCiftiConnectivityMatrixViewControllers()
{
    for (std::set<CiftiConnectivityMatrixViewController*>::iterator iter = s_allCiftiConnectivityMatrixViewControllers.begin();
         iter != s_allCiftiConnectivityMatrixViewControllers.end();
         iter++) {
        CiftiConnectivityMatrixViewController* clvc = *iter;
        if (clvc != this) {
            clvc->updateViewController();
        }
    }
}

/**
 * Receive events from the event manager.
 *
 * @param event
 *   Event sent by event manager.
 */
void
CiftiConnectivityMatrixViewController::receiveEvent(Event* event)
{
    
    if (event->getEventType() == EventTypeEnum::EVENT_USER_INTERFACE_UPDATE) {
        EventUserInterfaceUpdate* uiEvent =
        dynamic_cast<EventUserInterfaceUpdate*>(event);
        CaretAssert(uiEvent);
        
//        if (uiEvent->isUpdateForWindow(this->browserWindowIndex)) {
            if (uiEvent->isConnectivityUpdate()
                || uiEvent->isToolBoxUpdate()) {
                this->updateViewController();
                uiEvent->setEventProcessed();
            }
//        }
    }
}



