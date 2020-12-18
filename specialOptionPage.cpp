
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



#include "specialOptionPage.hpp"
#include "specialOptionPageHelp.hpp"

specialOptionPage::specialOptionPage (QWidget *parent, OPTIONS *op):
  QWizardPage (parent)
{
  options = op;


  setTitle (tr ("Special Case Options"));


  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/cpfFilterWatermark.png"));


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  QGroupBox *iduBox = new QGroupBox (tr ("Invalidate CZMIL Deep Channel data with multiple valid shallow returns"), this);
  QHBoxLayout *iduBoxLayout = new QHBoxLayout;
  iduBox->setLayout (iduBoxLayout);

  QLabel *deepUnderNumLabel = new QLabel (tr ("Number of required valid shallow returns"));
  deepUnderNumLabel->setToolTip (tr ("Change the number of valid shallow returns required (1-3)"));
  deepUnderNumLabel->setWhatsThis (deepUnderNumText);
  iduBoxLayout->addWidget (deepUnderNumLabel);

  deepUnderNum = new QSpinBox (this);
  deepUnderNum->setRange (0, 5);
  deepUnderNum->setSingleStep (1);
  deepUnderNum->setValue (options->deep_under_num);
  deepUnderNum->setToolTip (tr ("Change the number of valid shallow returns required (0-5)"));
  deepUnderNum->setWhatsThis (deepUnderNumText);
  iduBoxLayout->addWidget (deepUnderNum);


  vbox->addWidget (iduBox);


  QGroupBox *apfBox = new QGroupBox (tr ("After-pulse filter"), this);
  apfBox->setToolTip (tr ("Shallow channel, last land return, after-pulse filter"));
  apfBox->setWhatsThis (apfBoxText);
  QHBoxLayout *apfBoxLayout = new QHBoxLayout;
  apfBox->setLayout (apfBoxLayout);

  QLabel *apfCountLabel = new QLabel (tr ("Maximum return count"));
  apfCountLabel->setToolTip (tr ("Change the maximum number of returns allowed"));
  apfCountLabel->setWhatsThis (apfCountText);
  apfBoxLayout->addWidget (apfCountLabel);

  apfCount = new QComboBox (this);
  apfCount->setToolTip (tr ("Change the maximum number of returns allowed"));
  apfCount->setEditable (false);
  apfCount->addItem ("0 (Disable after-pulse filter)");
  apfCount->addItem ("2");
  apfCount->addItem ("3");
  apfCount->addItem ("4");
  apfCount->addItem ("5");
  apfCount->setCurrentIndex (options->apf_count);
  apfBoxLayout->addWidget (apfCount);


  QLabel *apfThresholdLabel = new QLabel (tr ("Reflectance threshold"));
  apfThresholdLabel->setToolTip (tr ("Change the reflectance threshold"));
  apfThresholdLabel->setWhatsThis (apfThresholdText);
  apfBoxLayout->addWidget (apfThresholdLabel);

  apfThreshold = new QDoubleSpinBox (this);
  apfThreshold->setRange (0.001, 0.025);
  apfThreshold->setDecimals (3);
  apfThreshold->setSingleStep (.001);
  apfThreshold->setValue (options->apf_threshold);
  apfThreshold->setToolTip (tr ("Change the reflectance threshold"));
  apfThreshold->setWhatsThis (apfThresholdText);
  apfBoxLayout->addWidget (apfThreshold);


  QLabel *apfMinimumLabel = new QLabel (tr ("Reflectance minimum"));
  apfMinimumLabel->setToolTip (tr ("Change the reflectance minimum"));
  apfMinimumLabel->setWhatsThis (apfMinimumText);
  apfBoxLayout->addWidget (apfMinimumLabel);

  apfMinimum = new QDoubleSpinBox (this);
  apfMinimum->setRange (0.0001, 0.0100);
  apfMinimum->setDecimals (4);
  apfMinimum->setSingleStep (.0001);
  apfMinimum->setValue (options->apf_minimum);
  apfMinimum->setToolTip (tr ("Change the reflectance minimum"));
  apfMinimum->setWhatsThis (apfMinimumText);
  apfBoxLayout->addWidget (apfMinimum);


  vbox->addWidget (apfBox);


  QHBoxLayout *maskBoxLayout = new QHBoxLayout;


  QGroupBox *mBox = new QGroupBox (this);
  QVBoxLayout *mBoxLayout = new QVBoxLayout;
  mBox->setLayout (mBoxLayout);


  maskBoxLayout->addWidget (mBox);


  land_mask = new QCheckBox (tr ("Invalidate land over water (SWBD and/or mask file)"), this);
  land_mask->setChecked (options->land_over_water_mask);
  land_mask->setToolTip (tr ("Check this box if you wish to invalidate all returns that were processed as land  (all types) that are over "
                             "water according to SWBD or an optional mask file"));
  land_mask->setWhatsThis (tr ("Check this box if you wish to invalidate all returns that were processed as land (all types) that are over "
                               "water according to SWBD or an optional mask file"));
  mBoxLayout->addWidget (land_mask);


  water_mask = new QCheckBox (tr ("Invalidate water over land (SWBD and/or mask file)"), this);
  water_mask->setChecked (options->water_over_land_mask);
  water_mask->setToolTip (tr ("Check this box if you wish to invalidate all manually invalid returns that were processed as water (all types) that are over "
                              "land according to SWBD or an optional mask file"));
  water_mask->setWhatsThis (tr ("Check this box if you wish to invalidate all manually invalid returns that were processed as water (all types) that are over "
                                "land according to SWBD or an optional mask file"));
  mBoxLayout->addWidget (water_mask);


  QGroupBox *mask_file_box = new QGroupBox (this);
  QHBoxLayout *mask_file_boxLayout = new QHBoxLayout;
  mask_file_box->setLayout (mask_file_boxLayout);

  QLabel *mask_file_label = new QLabel (tr ("Mask File"), this);
  mask_file_boxLayout->addWidget (mask_file_label, 1);

  mask_file_edit = new QLineEdit (this);
  mask_file_edit->setReadOnly (true);
  mask_file_boxLayout->addWidget (mask_file_edit, 10);

  mask_file_browse = new QPushButton (tr ("Browse..."), this);
  mask_file_boxLayout->addWidget (mask_file_browse, 1);

  mask_file_label->setWhatsThis (mask_fileText);
  mask_file_box->setToolTip (tr ("Select an optional mask file created by the shape_mask program"));
  mask_file_edit->setWhatsThis (mask_fileText);
  mask_file_edit->setToolTip (tr ("Select an optional mask file created by the shape_mask program"));
  mask_file_browse->setWhatsThis (mask_fileBrowseText);
  mask_file_browse->setToolTip (tr ("Select an optional mask file created by the shape_mask program"));

  connect (mask_file_browse, SIGNAL (clicked ()), this, SLOT (slotMaskFileBrowse ()));


  maskBoxLayout->addWidget (mask_file_box);


  vbox->addLayout (maskBoxLayout);


  QGroupBox *fomBox = new QGroupBox (tr ("Invalidate first of many valid returns"), this);
  QVBoxLayout *fomBoxLayout = new QVBoxLayout;
  fomBox->setLayout (fomBoxLayout);

  manyTable = new QTableWidget (NUM_OPTECH_CLASSES, 12, this);
  manyTable->setAlternatingRowColors (true);

  QTableWidgetItem *tItem = new QTableWidgetItem (tr ("Data Type"));
  manyTable->setHorizontalHeaderItem (0, tItem);


  QTableWidgetItem *clrItem = new QTableWidgetItem (tr ("Clear"));
  clrItem->setToolTip (tr ("Clear all settings for this data type"));
  clrItem->setWhatsThis (tr ("Clear all settings for this data type"));
  manyTable->setHorizontalHeaderItem (1, clrItem);

  QTableWidgetItem *dupItem = new QTableWidgetItem (tr ("Copy"));
  dupItem->setToolTip (tr ("Copy all settings from this data type to all other data types"));
  dupItem->setWhatsThis (tr ("Copy all settings from this data type to all other data types"));
  manyTable->setHorizontalHeaderItem (2, dupItem);


  clrGrp = new QButtonGroup (this);
  clrGrp->setExclusive (false);
  connect (clrGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotClrClicked (int)));

  dupGrp = new QButtonGroup (this);
  dupGrp->setExclusive (false);
  connect (dupGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotDupClicked (int)));


  //  Stretch the first header column so the table will fit nicely in the page.

#if QT_VERSION >= 0x050000
  manyTable->horizontalHeader ()->setSectionResizeMode (0, QHeaderView::Stretch);
#else
  manyTable->horizontalHeader ()->setResizeMode (0, QHeaderView::Stretch); 
#endif


  QTableWidgetItem *cItem[9];


  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      int32_t num = i + 1;
      if (i > 0) num += 29;
      czmil_get_short_proc_mode_string (num, short_proc_mode_name[i]);
      czmil_get_proc_mode_string (num, proc_mode_name[i]);


      type[i] = new QLabel (QString (short_proc_mode_name[i]), this);
      manyTable->setCellWidget (i, 0, type[i]);


      clr[i] = new QPushButton (this);
      clr[i]->setToolTip (tr ("Clears all settings for this data type"));
      clr[i]->setWhatsThis (tr ("Clears all settings for this data type"));
      manyTable->setCellWidget (i, 1, clr[i]);
      clrGrp->addButton (clr[i], i);


      dup[i] = new QPushButton (this);
      dup[i]->setToolTip (tr ("Copies all settings for this data type to all other data types"));
      dup[i]->setWhatsThis (tr ("Copies all settings for this data type to all other data types"));
      manyTable->setCellWidget (i, 2, dup[i]);
      dupGrp->addButton (dup[i], i);


      for (int32_t j = 0 ; j < 9 ; j++)
        {
          char chan_name[128];
          czmil_get_channel_string (j, chan_name);

          QString num;
          num.setNum (j + 1);


          cItem[j] = new QTableWidgetItem (num);
          cItem[j]->setToolTip (chan_name);
          cItem[j]->setWhatsThis (tr ("These spinboxes allow you to enable filtering for %1.").arg (chan_name));
          manyTable->setHorizontalHeaderItem (j + 3, cItem[j]);


          chan[i][j] = new QSpinBox (this);
          chan[i][j]->setRange (0, 5);
          chan[i][j]->setSingleStep (1);
          chan[i][j]->setValue (options->first_of_many[i][j]);
          chan[i][j]->setToolTip (tr ("Set this value to non-zero (1-5) if you wish to invalidate %1 data for %2").arg (proc_mode_name[i]).arg (chan_name));
          chan[i][j]->setWhatsThis (tr ("Check this box if you wish to invalidate %1 data for %2").arg (proc_mode_name[i]).arg (chan_name));

          manyTable->setCellWidget (i, j + 3, chan[i][j]);
        }
    }


  fomBoxLayout->addWidget (manyTable);

  manyTable->resizeColumnsToContents ();
  manyTable->resizeRowsToContents ();


  vbox->addWidget (fomBox);


  registerField ("deepUnderNum", deepUnderNum);
  registerField ("land_mask", land_mask);
  registerField ("water_mask", water_mask);
  registerField ("apfCount", apfCount);
  registerField ("apfThreshold", apfThreshold, "value", "valueChanged");
  registerField ("apfMinimum", apfMinimum, "value", "valueChanged");

  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      for (int32_t j = 0 ; j < 9 ; j++)
        {
          registerField (QString ("type %1 chan %2 first of many value").arg (i).arg (j), chan[i][j]);
        }
    }
}



void specialOptionPage::slotMaskFileBrowse ()
{
  QStringList         files, filters;
  QString             file;


  QFileDialog *fd = new QFileDialog (this, tr ("cpfFilter Open mask file"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->mask_dir);


  filters << tr ("Mask (*.msk)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("Mask (*.msk)"));

  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString mask_file_name = files.at (0);


      if (!mask_file_name.isEmpty())
        {
          strcpy (options->mask_name, mask_file_name.toLatin1 ());

          if (!check_shape_mask (options->mask_name))
            {
              QMessageBox::warning (this, tr ("Open mask file"),
                                    tr ("The file %1 is not a mask file or there was an error opening the file.\nThe error message returned was:\n\n%2").arg 
                                    (QDir::toNativeSeparators (mask_file_name)).arg (strerror (errno)));
              return;
            }

          mask_file_edit->setText (mask_file_name);
        }

      options->mask_dir = fd->directory ().absolutePath ();
    }
}



void specialOptionPage::slotDupClicked (int id)
{
  for (int32_t i = 0 ; i < 9 ; i++) 
    {
      options->first_of_many[id][i] = chan[id][i]->value ();
    }

  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      for (int32_t j = 0 ; j < 9 ; j++)
        {
          options->first_of_many[i][j] = options->first_of_many[id][j];
          chan[i][j]->setValue (options->first_of_many[i][j]);
        }
    }
}



void specialOptionPage::slotClrClicked (int id)
{
  for (int32_t i = 0 ; i < 9 ; i++) 
    {
      options->first_of_many[id][i] = 0;
      chan[id][i]->setValue (0);
    }
}



void specialOptionPage::setEnabled (uint8_t state)
{
  deepUnderNum->setEnabled (state);
  land_mask->setEnabled (state);
  water_mask->setEnabled (state);
  mask_file_browse->setEnabled (state);

  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      dup[i]->setEnabled (state);
      clr[i]->setEnabled (state);

      for (int32_t j = 0 ; j < 9 ; j++)
        {
          chan[i][j]->setEnabled (state);
        }
    }
}
