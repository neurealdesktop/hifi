//
//  AssetUploadDialogFactory.cpp
//  interface/src/ui
//
//  Created by Stephen Birarda on 2015-08-26.
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "AssetUploadDialogFactory.h"

#include <AssetClient.h>
#include <AssetUpload.h>
#include <AssetUtils.h>

#include <QtCore/QDebug>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

AssetUploadDialogFactory& AssetUploadDialogFactory::getInstance() {
    static AssetUploadDialogFactory staticInstance;
    return staticInstance;
}

AssetUploadDialogFactory::AssetUploadDialogFactory() {
    
}

void AssetUploadDialogFactory::showDialog() {
    auto filename = QFileDialog::getOpenFileUrl(_dialogParent, "Select a file to upload");
    
    if (!filename.isEmpty()) {
        qDebug() << "Selected filename for upload to asset-server: " << filename;
        
        auto assetClient = DependencyManager::get<AssetClient>();
        auto upload = assetClient->createUpload(filename.path());
        
        if (upload) {
            // connect to the finished signal so we know when the AssetUpload is done
            QObject::connect(upload, &AssetUpload::finished, this, &AssetUploadDialogFactory::handleUploadFinished);
            
            // start the upload now
            upload->start();
        } else {
            // show a QMessageBox to say that there is no local asset server
            QString messageBoxText = QString("Could not upload \n\n%1\n\nbecause you are currently not connected" \
                                             " to a local asset-server.").arg(QFileInfo(filename.toString()).fileName());
            
            QMessageBox::information(_dialogParent, "Failed to Upload", messageBoxText);
        }
    }
}

void AssetUploadDialogFactory::handleUploadFinished(AssetUpload* upload, const QString& hash) {
    if (true) {
        // show message box for successful upload, with copiable text for ATP hash
        QDialog* hashCopyDialog = new QDialog(_dialogParent);
        
        // delete the dialog on close
        hashCopyDialog->setAttribute(Qt::WA_DeleteOnClose);
        
        // set the window title
        hashCopyDialog->setWindowTitle(tr("Successful Asset Upload"));
        
        // setup a layout for the contents of the dialog
        QVBoxLayout* boxLayout = new QVBoxLayout;
        
        // set the label text (this shows above the text box)
        QLabel* lineEditLabel = new QLabel;
        lineEditLabel->setText(QString("ATP URL for %1").arg(QFileInfo(upload->getFilename()).fileName()));
        
        // setup the line edit to hold the copiable text
        QLineEdit* lineEdit = new QLineEdit;
       
        QString atpURL = QString("%1://%2").arg(ATP_SCHEME).arg(hash);
        
        // set the ATP URL as the text value so it's copiable
        lineEdit->insert(atpURL);
        
        // figure out what size this line edit should be using font metrics
        QFontMetrics textMetrics { lineEdit->font() };
        
        // set the fixed width on the line edit
        // pad it by 10 to cover the border and some extra space on the right side (for clicking)
        static const int LINE_EDIT_RIGHT_PADDING { 10 };
        
        lineEdit->setFixedWidth(textMetrics.width(atpURL) + LINE_EDIT_RIGHT_PADDING );
        
        // left align the ATP URL line edit
        lineEdit->home(true);
        
        // add the label and line edit to the dialog
        boxLayout->addWidget(lineEditLabel);
        boxLayout->addWidget(lineEdit);
        
        // setup an OK button to close the dialog
        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
        connect(buttonBox, &QDialogButtonBox::accepted, hashCopyDialog, &QDialog::close);
        boxLayout->addWidget(buttonBox);
        
        // set the new layout on the dialog
        hashCopyDialog->setLayout(boxLayout);
        
        // show the new dialog
        hashCopyDialog->show();
    } else {
        //
    }
}
