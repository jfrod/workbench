
/*LICENSE_START*/
/*
 * Copyright 2013 Washington University,
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

#define __CUSTOM_VIEW_DIALOG_DECLARE__
#include "CustomViewDialog.h"
#undef __CUSTOM_VIEW_DIALOG_DECLARE__

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QInputDialog>
#include <QLabel>
#include <QListWidgetItem>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include "BrainBrowserWindow.h"
#include "BrainBrowserWindowComboBox.h"
#include "BrowserTabContent.h"
#include "CaretPreferences.h"
#include "EventBrowserWindowGraphicsRedrawn.h"
#include "EventGraphicsUpdateOneWindow.h"
#include "EventManager.h"
#include "GuiManager.h"
#include "Matrix4x4.h"
#include "Model.h"
#include "ModelTransform.h"
#include "ModelYokingGroup.h"
#include "SessionManager.h"
#include "WuQDataEntryDialog.h"
#include "WuQListWidget.h"
#include "WuQMessageBox.h"
#include "WuQtUtilities.h"
#include "WuQWidgetObjectGroup.h"

using namespace caret;


    
/**
 * \class caret::CustomViewDialog 
 * \brief Dialog for creation of custom views (orientations).
 * \ingroup GuiQt
 */

/**
 * Constructor.
 */
CustomViewDialog::CustomViewDialog(QWidget* parent)
: WuQDialogNonModal("Custom Orientation",
                    parent)
{
    m_blockDialogUpdate = true;
    
    /*
     * Remove apply button by using an empty name
     */
    setApplyButtonText("");

    /*
     * Create the controls
     */
    QWidget* customViewWidget = createCustomViewWidget();
    QWidget* transformWidget = createTransformsWidget();
    m_copyWidget = createCopyWidget();
    
    /*
     * Layout for dialog
     */
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->setSpacing(layout->spacing() / 2);
    layout->addWidget(transformWidget, 0, Qt::AlignVCenter);
    layout->addWidget(m_copyWidget, 0, Qt::AlignVCenter);
    layout->addWidget(customViewWidget, 0, Qt::AlignVCenter);
    setCentralWidget(widget);
    
    /*
     * No auto default button processing (Qt highlights button)
     */
    disableAutoDefaultForAllPushButtons();
 
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_BROWSER_WINDOW_GRAPHICS_HAVE_BEEN_REDRAWN);
    
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    m_blockDialogUpdate = false;
    
//    loadCustomViewListWidget();
}

/**
 * Destructor.
 */
CustomViewDialog::~CustomViewDialog()
{
    EventManager::get()->removeAllEventsFromListener(this);
}

/**
 * @return The Custom View widget.
 */
QWidget*
CustomViewDialog::createCustomViewWidget()
{
    /*
     * View selection list widget
     */
    m_customViewListWidget = new WuQListWidget();
    m_customViewListWidget->setFixedHeight(100);
    m_customViewListWidget->setFixedWidth(140);
    m_customViewListWidget->setSelectionMode(QListWidget::SingleSelection);
    
    QObject::connect(m_customViewListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(customViewSelected()));
    QObject::connect(m_customViewListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(customViewSelectedAndApplied()));

    m_newCustomViewPushButton = new QPushButton("New...");
    WuQtUtilities::setWordWrappedToolTip(m_newCustomViewPushButton,
                                         "Create a new Custom View by entering its name.  The view will use the current transformation values.");
    QObject::connect(m_newCustomViewPushButton, SIGNAL(clicked()),
                     this, SLOT(newCustomViewPushButtonClicked()));
    
    m_deleteCustomViewPushButton = new QPushButton("Delete...");
    WuQtUtilities::setWordWrappedToolTip(m_deleteCustomViewPushButton,
                                         "Delete the selected Custom View.");
    QObject::connect(m_deleteCustomViewPushButton, SIGNAL(clicked()),
                     this, SLOT(deleteCustomViewPushButtonClicked()));
    
    WuQtUtilities::matchWidgetWidths(m_newCustomViewPushButton,
                                     m_deleteCustomViewPushButton);
    
    QGroupBox* groupBox = new QGroupBox("Custom Orientation");
    QVBoxLayout* layout = new QVBoxLayout(groupBox);
    WuQtUtilities::setLayoutMargins(layout, 4, 2);
    layout->addWidget(m_customViewListWidget, 100, Qt::AlignHCenter);
    layout->addWidget(m_newCustomViewPushButton, 0, Qt::AlignHCenter);
    layout->addWidget(m_deleteCustomViewPushButton, 0, Qt::AlignHCenter);
    layout->addStretch();
    
    groupBox->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);

    return groupBox;
}

/**
 * Called when a custom view is selected.
 */
void
CustomViewDialog::customViewSelected()
{
    
}

/**
 * Called when a custom view name is double-clicked.
 */
void
CustomViewDialog::customViewSelectedAndApplied()
{
    copyToTransformPushButtonClicked();
}

///**
// * @return The selected user view or NULL if no user view selected.
// */
//UserView*
//CustomViewDialog::getSelectedUserView()
//{
//    UserView* userView = NULL;
//    QListWidgetItem* lwi = m_customViewListWidget->currentItem();
//    if (lwi != NULL) {
//        userView = reinterpret_cast<UserView*>(qVariantValue<quintptr>(lwi->data(Qt::UserRole)));
//    }
//    return userView;
//}
//
/**
 * @return Name of selected custom view..
 */
AString
CustomViewDialog::getSelectedCustomViewName()
{
    AString viewName;
    
    QListWidgetItem* lwi = m_customViewListWidget->currentItem();
    if (lwi != NULL) {
        viewName = lwi->text();
    }
    
    return viewName;
}


/**
 * Load content of custom view list widget.
 * 
 * @param selectedName
 *    If not empty, this name will be selected.
 */
void
CustomViewDialog::loadCustomViewListWidget(const AString& selectedName)
{
    /*
     * Cannot use getSelectedUserView() since the item returned may be invalid
     * if items have been removed.
     */
    QString previousViewName = getSelectedCustomViewName();
    if (selectedName.isEmpty() == false) {
        previousViewName = selectedName;
    }
    
    int defaultSelectedIndex = m_customViewListWidget->currentRow();
    
    m_customViewListWidget->clear();
    
    CaretPreferences* prefs = getCaretPreferences();
    prefs->readCustomViews();
    const std::vector<std::pair<AString,AString> > customViewNamesAndComments = prefs->getCustomViewNamesAndComments();
    const int32_t numViews = static_cast<int32_t>(customViewNamesAndComments.size());
    
    for (int32_t i = 0; i < numViews; i++) {
        const AString viewName = customViewNamesAndComments[i].first;
        const AString comment  = customViewNamesAndComments[i].second;
        
        if (viewName == previousViewName) {
            defaultSelectedIndex = i;
        }
        
        QListWidgetItem* lwi = new QListWidgetItem(viewName);
        if (comment.isEmpty() == false) {
            lwi->setToolTip(WuQtUtilities::createWordWrappedToolTipText(comment));
        }
        
        m_customViewListWidget->addItem(lwi);
    }
    
    if (defaultSelectedIndex >= numViews) {
        defaultSelectedIndex = numViews - 1;
    }
    if (defaultSelectedIndex < 0) {
        defaultSelectedIndex = 0;
    }
    
    if ((defaultSelectedIndex >= 0)
        && (defaultSelectedIndex < m_customViewListWidget->count())) {
        m_customViewListWidget->setCurrentRow(defaultSelectedIndex);
        customViewSelected();
        
        m_customViewListWidget->scrollToItem(m_customViewListWidget->currentItem());
    }
    
    const bool haveViews = (numViews > 0);
    m_copyWidget->setEnabled(haveViews);
    
    const bool haveSelectedView = (getSelectedCustomViewName().isEmpty() == false);
    m_deleteCustomViewPushButton->setEnabled(haveSelectedView);
}

/**
 * @return Create and return the copy buttons widget.
 */
QWidget*
CustomViewDialog::createCopyWidget()
{
    QPushButton* copyToCustomViewPushButton = new QPushButton("Copy -->");
    WuQtUtilities::setWordWrappedToolTip(copyToCustomViewPushButton,
                                         "Copy the Transform's values into the selected Custom Orientation.");
    QObject::connect(copyToCustomViewPushButton, SIGNAL(clicked()),
                     this, SLOT(copyToCustomViewPushButtonClicked()));
    
    QPushButton* copyToTransformPushButton = new QPushButton("<-- Load");
    WuQtUtilities::setWordWrappedToolTip(copyToTransformPushButton,
                                         "Load the Custom Orientation's transform values into the Transform.");
    QObject::connect(copyToTransformPushButton, SIGNAL(clicked()),
                     this, SLOT(copyToTransformPushButtonClicked()));
    
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->addWidget(copyToCustomViewPushButton);
    layout->addWidget(copyToTransformPushButton);
    
    widget->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);
    
    return widget;
}

/**
 * Called when Copy To Custom View push buttton is clicked.
 */
void
CustomViewDialog::copyToCustomViewPushButtonClicked()
{
    CaretPreferences* prefs = getCaretPreferences();
    
    ModelTransform modelTransform;
    if (prefs->getCustomView(getSelectedCustomViewName(), modelTransform)) {
        moveTransformToCustomView(modelTransform);
        prefs->addOrReplaceCustomView(modelTransform);
    }
}

/**
 * Move the transform values to the given user view.
 *
 * @param userView
 *     User View into which transform values are moved.
 */
void
CustomViewDialog::moveTransformToCustomView(ModelTransform& modelTransform)
{
    double panX, panY, rotX, rotY, rotZ, zoom;
    getTransformationControlValues(panX,
                                   panY,
                                   rotX,
                                   rotY,
                                   rotZ,
                                   zoom);
    
    Matrix4x4 rotationMatrix;
    rotationMatrix.setRotation(rotX,
                               rotY,
                               rotZ);
    float rotationMatrixArray[4][4];
    rotationMatrix.getMatrix(rotationMatrixArray);
    
    modelTransform.setPanXyRotationMatrixAndZoom(panX,
                                                 panY,
                                                 rotationMatrixArray,
                                                 zoom);
}


/**
 * Called when Move To Transform push buttton is clicked.
 */
void
CustomViewDialog::copyToTransformPushButtonClicked()
{
    CaretPreferences* prefs = getCaretPreferences();
    
    const AString customViewName = getSelectedCustomViewName();
    
    ModelTransform modelTransform;
    if (prefs->getCustomView(customViewName, modelTransform)) {
        float panX, panY, rotationMatrixArray[4][4], zoom;
        modelTransform.getPanXyRotationMatrixAndZoom(panX,
                                                panY,
                                                rotationMatrixArray,
                                                zoom);
        
        Matrix4x4 rotationMatrix;
        rotationMatrix.setMatrix(rotationMatrixArray);
        
        double rotX, rotY, rotZ;
        rotationMatrix.getRotation(rotX, rotY, rotZ);
        
        setTransformationControlValues(panX, panY, rotX, rotY, rotZ, zoom);
        
        transformValueChanged();
        
    }
}

/**
 * @return The Transform widget.
 */
QWidget*
CustomViewDialog::createTransformsWidget()
{
    const int spinBoxWidth = 90;
    
    /*
     * Window number
     */
    QLabel* windowLabel = new QLabel("Workbench Window: ");
    m_browserWindowComboBox = new BrainBrowserWindowComboBox(this);
    m_browserWindowComboBox->getWidget()->setFixedWidth(spinBoxWidth);
    QObject::connect(m_browserWindowComboBox, SIGNAL(browserWindowSelected(BrainBrowserWindow*)),
                     this, SLOT(browserWindowComboBoxValueChanged(BrainBrowserWindow*)));
    
    /*
     * Panning
     */
    const double panStep = 1.0;
    QLabel* panLabel = new QLabel("Pan (X,Y):");
    m_xPanDoubleSpinBox = new QDoubleSpinBox;
    m_xPanDoubleSpinBox->setMinimum(-100000.0);
    m_xPanDoubleSpinBox->setMaximum( 100000.0);
    m_xPanDoubleSpinBox->setSingleStep(panStep);
    m_xPanDoubleSpinBox->setDecimals(2);
    m_xPanDoubleSpinBox->setFixedWidth(spinBoxWidth);
    QObject::connect(m_xPanDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(transformValueChanged()));
    m_yPanDoubleSpinBox = new QDoubleSpinBox;
    m_yPanDoubleSpinBox->setMinimum(-100000.0);
    m_yPanDoubleSpinBox->setMaximum( 100000.0);
    m_yPanDoubleSpinBox->setSingleStep(panStep);
    m_yPanDoubleSpinBox->setDecimals(2);
    m_yPanDoubleSpinBox->setFixedWidth(spinBoxWidth);
    QObject::connect(m_yPanDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(transformValueChanged()));
    
    /*
     * Rotation
     */
    const double rotationMinimum = -360.0;
    const double rotationMaximum =  360.0;
    const double rotateStep = 1.0;
    QLabel* rotateLabel = new QLabel("Rotate (X,Y,Z): ");
    m_xRotateDoubleSpinBox = new QDoubleSpinBox;
    m_xRotateDoubleSpinBox->setWrapping(true);
    m_xRotateDoubleSpinBox->setMinimum(rotationMinimum);
    m_xRotateDoubleSpinBox->setMaximum(rotationMaximum);
    m_xRotateDoubleSpinBox->setSingleStep(rotateStep);
    m_xRotateDoubleSpinBox->setDecimals(2);
    m_xRotateDoubleSpinBox->setFixedWidth(spinBoxWidth);
    QObject::connect(m_xRotateDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(transformValueChanged()));
    m_yRotateDoubleSpinBox = new QDoubleSpinBox;
    m_yRotateDoubleSpinBox->setWrapping(true);
    m_yRotateDoubleSpinBox->setMinimum(rotationMinimum);
    m_yRotateDoubleSpinBox->setMaximum(rotationMaximum);
    m_yRotateDoubleSpinBox->setSingleStep(rotateStep);
    m_yRotateDoubleSpinBox->setDecimals(2);
    m_yRotateDoubleSpinBox->setFixedWidth(spinBoxWidth);
    QObject::connect(m_yRotateDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(transformValueChanged()));
    m_zRotateDoubleSpinBox = new QDoubleSpinBox;
    m_zRotateDoubleSpinBox->setWrapping(true);
    m_zRotateDoubleSpinBox->setMinimum(rotationMinimum);
    m_zRotateDoubleSpinBox->setMaximum(rotationMaximum);
    m_zRotateDoubleSpinBox->setSingleStep(rotateStep);
    m_zRotateDoubleSpinBox->setDecimals(2);
    m_zRotateDoubleSpinBox->setFixedWidth(spinBoxWidth);
    QObject::connect(m_zRotateDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(transformValueChanged()));
    
    /*
     * Zoom
     */
    const double zoomStep = 0.1;
    QLabel* zoomLabel = new QLabel("Zoom: ");
    m_zoomDoubleSpinBox = new QDoubleSpinBox;
    m_zoomDoubleSpinBox->setMinimum(0.001);
    m_zoomDoubleSpinBox->setMaximum(10000.0);
    m_zoomDoubleSpinBox->setSingleStep(zoomStep);
    m_zoomDoubleSpinBox->setDecimals(3);
    m_zoomDoubleSpinBox->setFixedWidth(spinBoxWidth);
    QObject::connect(m_zoomDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(transformValueChanged()));
    
    m_transformWidgetGroup = new WuQWidgetObjectGroup(this);
    m_transformWidgetGroup->add(m_xPanDoubleSpinBox);
    m_transformWidgetGroup->add(m_yPanDoubleSpinBox);
    m_transformWidgetGroup->add(m_xRotateDoubleSpinBox);
    m_transformWidgetGroup->add(m_yRotateDoubleSpinBox);
    m_transformWidgetGroup->add(m_zRotateDoubleSpinBox);
    m_transformWidgetGroup->add(m_zoomDoubleSpinBox);
    
    /*------------------------------------------------------------------------*/
    /*
     * Layout widgets
     */
    /*
     * Columns for grid layout
     */
    int column = 0;
    const int COLUMN_LABEL  = column++;
    const int COLUMN_X      = column++;
    const int COLUMN_Y      = column++;
    const int COLUMN_Z      = column++;
    const int COLUMN_COUNT  = column++;
    
    QGroupBox* groupBox = new QGroupBox("Transform");
    QGridLayout* gridLayout = new QGridLayout(groupBox);
    WuQtUtilities::setLayoutMargins(gridLayout, 4, 4);
    int row = 0;
    gridLayout->addWidget(windowLabel,
                          row,
                          COLUMN_LABEL);
    gridLayout->addWidget(m_browserWindowComboBox->getWidget(),
                          row,
                          COLUMN_X);
    row++;
    
    gridLayout->addWidget(WuQtUtilities::createHorizontalLineWidget(),
                          row,
                          COLUMN_LABEL,
                          1,
                          COLUMN_COUNT);
    gridLayout->setRowMinimumHeight(row,
                                    10.0);
    row++;
    
    gridLayout->addWidget(panLabel,
                          row,
                          COLUMN_LABEL);
    gridLayout->addWidget(m_xPanDoubleSpinBox,
                          row,
                          COLUMN_X);
    gridLayout->addWidget(m_yPanDoubleSpinBox,
                          row,
                          COLUMN_Y);
    row++;
    
    gridLayout->addWidget(rotateLabel,
                          row,
                          COLUMN_LABEL);
    gridLayout->addWidget(m_xRotateDoubleSpinBox,
                          row,
                          COLUMN_X);
    gridLayout->addWidget(m_yRotateDoubleSpinBox,
                          row,
                          COLUMN_Y);
    gridLayout->addWidget(m_zRotateDoubleSpinBox,
                          row,
                          COLUMN_Z);
    row++;
    
    gridLayout->addWidget(zoomLabel,
                          row,
                          COLUMN_LABEL);
    gridLayout->addWidget(m_zoomDoubleSpinBox,
                          row,
                          COLUMN_X);
    row++;
    
    groupBox->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);
    
    return groupBox;
}

/**
 * Called when window number combo box value changed.
 */
void
CustomViewDialog::browserWindowComboBoxValueChanged(BrainBrowserWindow* browserWindow)
{
    int32_t windowIndex = -1;
    if (browserWindow != NULL) {
        std::cout << "Selected " << browserWindow->getBrowserWindowIndex() << std::endl;
        windowIndex = browserWindow->getBrowserWindowIndex();
    }
    
    updateContent(windowIndex);
}

/**
 * Called when a transform value is changed.
 */
void
CustomViewDialog::transformValueChanged()
{
    double panX, panY, rotX, rotY, rotZ, zoom;
    getTransformationControlValues(panX,
                                   panY,
                                   rotX,
                                   rotY,
                                   rotZ,
                                   zoom);
    
    BrainBrowserWindow* bbw = m_browserWindowComboBox->getSelectedBrowserWindow();
    if (bbw != NULL) {
        BrowserTabContent* btc = bbw->getBrowserTabContent();
        const int32_t tabIndex = btc->getTabNumber();
        if (btc != NULL) {
            Model* model = btc->getModelControllerForTransformation();
            if (model != NULL) {
                Matrix4x4 rotationMatrix;
                rotationMatrix.setRotation(rotX, rotY, rotZ);
                float rotationMatrixArray[4][4];
                rotationMatrix.getMatrix(rotationMatrixArray);
                
                ModelTransform modelTransform;
                modelTransform.setPanXyRotationMatrixAndZoom(panX, panY, rotationMatrixArray, zoom);
                model->setTransformationsFromModelTransform(tabIndex,
                                                            modelTransform);
               updateGraphicsWindow();
            }
        }
    }
}

/**
 * Update the selected graphics window.
 */
void
CustomViewDialog::updateGraphicsWindow()
{
    BrainBrowserWindow* bbw = m_browserWindowComboBox->getSelectedBrowserWindow();
    if (bbw != NULL) {
        const int32_t windowIndex = bbw->getBrowserWindowIndex();
        
        m_blockDialogUpdate = true;
        EventManager::get()->sendEvent(EventGraphicsUpdateOneWindow(windowIndex).getPointer());
        m_blockDialogUpdate = false;
    }
}

/**
 * Gets called when the dialog gains focus.
 */
void
CustomViewDialog::focusGained()
{
    updateDialog();
}

/**
 * Update the dialog.
 */
void
CustomViewDialog::updateDialog()
{
    m_browserWindowComboBox->updateComboBox();
    updateContent(m_browserWindowComboBox->getSelectedBrowserWindowIndex());
}

/**
 * Update the dialog.
 */
void
CustomViewDialog::updateContent(const int32_t browserWindowIndexIn)
{
    /*
     * May get updates when graphics are redrawn by this dialog
     * and not doing this could result in infinite loop
     */
    if (m_blockDialogUpdate) {
        return;
    }
    
    /*
     * Update, set, and validate selected browser window
     */
    m_browserWindowComboBox->updateComboBox();
    m_browserWindowComboBox->setBrowserWindowByIndex(browserWindowIndexIn);
    const int32_t browserWindowIndex = m_browserWindowComboBox->getSelectedBrowserWindowIndex();
    
    BrainBrowserWindow* bbw = GuiManager::get()->getBrowserWindowByWindowIndex(browserWindowIndex);
    if (bbw != NULL) {
        BrowserTabContent* btc = bbw->getBrowserTabContent();
        const int32_t tabIndex = btc->getTabNumber();
        if (btc != NULL) {
            Model* model = btc->getModelControllerForTransformation();
            if (model != NULL) {
                const float* panning = model->getTranslation(tabIndex, Model::VIEWING_TRANSFORM_NORMAL);
                const Matrix4x4* rotationMatrix = model->getViewingRotationMatrix(tabIndex, Model::VIEWING_TRANSFORM_NORMAL);
                const float zooming = model->getScaling(tabIndex);
                
                double rotX, rotY, rotZ;
                rotationMatrix->getRotation(rotX, rotY, rotZ);
                
                setTransformationControlValues(panning[0],
                                               panning[1],
                                               rotX,
                                               rotY,
                                               rotZ,
                                               zooming);
            }
        }
        
        m_transformWidgetGroup->setEnabled(true);
    }
    else {
        m_transformWidgetGroup->setEnabled(false);
    }
    
    loadCustomViewListWidget();
}

/**
 * Get the transformation values.
 *
 * @param panX
 *    X pannning
 * @param panX
 *    X pannning
 * @param rotX
 *    X rotation
 * @param rotY
 *    Y rotation
 * @param rotZ
 *    Z rotation
 * @param zoom
 *    Zooming
 */
void
CustomViewDialog::getTransformationControlValues(double& panX,
                        double& panY,
                        double& rotX,
                        double& rotY,
                        double& rotZ,
                        double& zoom) const
{
    panX = m_xPanDoubleSpinBox->value();
    panY = m_yPanDoubleSpinBox->value();
    
    rotX = m_xRotateDoubleSpinBox->value();
    rotY = m_yRotateDoubleSpinBox->value();
    rotZ = m_zRotateDoubleSpinBox->value();
    
    zoom = m_zoomDoubleSpinBox->value();
}

/**
 * Set the transformation values.
 *
 * @param panX
 *    X pannning
 * @param panX
 *    X pannning
 * @param rotX
 *    X rotation
 * @param rotY
 *    Y rotation
 * @param rotZ
 *    Z rotation
 * @param zoom
 *    Zooming
 */
void
CustomViewDialog::setTransformationControlValues(const double panX,
                                    const double panY,
                                    const double rotX,
                                    const double rotY,
                                    const double rotZ,
                                    const double zoom) const
{
    m_transformWidgetGroup->blockAllSignals(true);
    
    m_xPanDoubleSpinBox->setValue(panX);
    m_yPanDoubleSpinBox->setValue(panY);
    
    m_xRotateDoubleSpinBox->setValue(rotX);
    m_yRotateDoubleSpinBox->setValue(rotY);
    m_zRotateDoubleSpinBox->setValue(rotZ);
    
    m_zoomDoubleSpinBox->setValue(zoom);
    
    m_transformWidgetGroup->blockAllSignals(false);
}

/**
 * @return The caret preferences.
 */
CaretPreferences*
CustomViewDialog::getCaretPreferences()
{
    CaretPreferences* prefs = SessionManager::get()->getCaretPreferences();
    return prefs;
}

///**
// * @return The user views (updated by rereading from preferences).
// */
//std::vector<UserView*>
//CustomViewDialog::getUserViews()
//{
//    CaretPreferences* cp = getCaretPreferences();
//    cp->readUserViews();
//    std::vector<UserView*> userViews = cp->getAllUserViews();
//    return userViews;
//}


/**
 * Called when new custom view push button clicked.
 */
void
CustomViewDialog::newCustomViewPushButtonClicked()
{
    CaretPreferences* prefs = getCaretPreferences();
    const std::vector<AString> existingCustomViewNames = prefs->getCustomViewNames();
    
    bool createViewFlag = false;
    AString newViewName;
    AString newViewComment;
    
    bool exitLoop = false;
    while (exitLoop == false) {
        WuQDataEntryDialog ded("New Custom View",
                               m_newCustomViewPushButton);
        
        QLineEdit* nameLineEdit = ded.addLineEditWidget("View Name");
        QTextEdit* commentTextEdit = ded.addTextEdit("Comment", "", false);
        
        nameLineEdit->setFocus();
        if (ded.exec() == WuQDataEntryDialog::Accepted) {
            newViewName = nameLineEdit->text().trimmed();

            
            
            if (newViewName.isEmpty() == false) {
                newViewComment = commentTextEdit->toPlainText().trimmed();
                
                /*
                 * If custom view exists with name entered by user,
                 * then warn the user.
                 */
                if (std::find(existingCustomViewNames.begin(),
                              existingCustomViewNames.end(),
                              newViewName) != existingCustomViewNames.end()) {
                    const QString msg = ("View named \""
                                         + newViewName
                                         + "\" already exits.  Replace?");
                    if (WuQMessageBox::warningYesNo(m_newCustomViewPushButton,
                                                    msg)) {
                        exitLoop = true;
                        createViewFlag = true;
                    }
                }
                else {
                    exitLoop = true;
                    createViewFlag = true;
                }
            }
            
        }
        else {
            exitLoop = true;
        }
    }
    if (createViewFlag && (newViewName.isEmpty() == false)) {
        ModelTransform  mt;
        mt.setName(newViewName);
        mt.setComment(newViewComment);
        moveTransformToCustomView(mt);
        prefs->addOrReplaceCustomView(mt);
        
        loadCustomViewListWidget(newViewName);
    }
}

/**
 * Called when save custom view push button clicked.
 */
void
CustomViewDialog::deleteCustomViewPushButtonClicked()
{
    const AString viewName = getSelectedCustomViewName();
    if (viewName.isEmpty() == false) {
        const QString msg = ("Delete view named \""
                             + viewName
                             + "\" ?");
        if (WuQMessageBox::warningYesNo(m_newCustomViewPushButton,
                                        msg)) {
            CaretPreferences* prefs = getCaretPreferences();
            prefs->removeCustomView(viewName);
            loadCustomViewListWidget();
        }
    }
/*
    bool ok = false;
    bool exitLoop = false;
    AString newViewName;
    while (exitLoop == false) {
        newViewName = QInputDialog::getText(m_deleteCustomViewPushButton,
                                            "",
                                            "Name of New View",
                                            QLineEdit::Normal,
                                            newViewName,
                                            &ok);
        if (ok) {
            bool overwriteFlag = false;
            CaretPreferences* prefs = SessionManager::get()->getCaretPreferences();
            const std::vector<UserView*> userViews = prefs->getAllUserViews();
            const int32_t numViews = static_cast<int32_t>(userViews.size());
            if (numViews > 0) {
                for (int32_t i = 0; i < numViews; i++) {
                    const QString viewName = userViews[i]->getName();
                    if (viewName == newViewName) {
                        overwriteFlag = true;
                    }
                }
            }
            if (overwriteFlag) {
                const QString msg = ("View named \""
                                     + newViewName
                                     + "\" already exits.  Replace?");
                if (WuQMessageBox::warningYesNo(m_deleteCustomViewPushButton,
                                                msg)) {
                    exitLoop = true;
                }
            }
            else {
                exitLoop = true;
            }
            
        }
        else {
            exitLoop = true;
        }
    }
    if (ok && (newViewName.isEmpty() == false)) {
        double panX, panY, rotX, rotY, rotZ, zoom;
        getTransformationControlValues(panX,
                                       panY,
                                       rotX,
                                       rotY,
                                       rotZ,
                                       zoom);
        const float panning[3] = { panX, panY, 0.0 };

        UserView uv;
        
        uv.setTranslation(panning);
        
        Matrix4x4 rotationMatrix;
        rotationMatrix.setRotation(rotX,
                                   rotY,
                                   rotZ);
        float rotationMatrixArray[4][4];
        rotationMatrix.getMatrix(rotationMatrixArray);
        uv.setRotation(rotationMatrixArray);
        
        uv.setScaling(zoom);
        
        uv.setName(newViewName);
        
        CaretPreferences* prefs = SessionManager::get()->getCaretPreferences();
        prefs->addUserView(uv);
    }
*/    
}

/**
 * Receive events from the event manager.
 *
 * @param event
 *   Event sent by event manager.
 */
void
CustomViewDialog::receiveEvent(Event* event)
{
    if (event->getEventType() == EventTypeEnum::EVENT_BROWSER_WINDOW_GRAPHICS_HAVE_BEEN_REDRAWN) {
        EventBrowserWindowGraphicsRedrawn* redrawnEvent =
        dynamic_cast<EventBrowserWindowGraphicsRedrawn*>(event);
        CaretAssert(redrawnEvent);
        
        redrawnEvent->setEventProcessed();

        const int32_t selectedBrowserWindowIndex = m_browserWindowComboBox->getSelectedBrowserWindowIndex();
        if (redrawnEvent->getBrowserWindowIndex() == selectedBrowserWindowIndex) {
            updateContent(selectedBrowserWindowIndex);
        }
    }
}

