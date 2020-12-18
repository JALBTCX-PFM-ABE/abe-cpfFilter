
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



#include "typeOptionPage.hpp"

typeOptionPage::typeOptionPage (QWidget *parent, OPTIONS *op):
  QWizardPage (parent)
{
  options = op;


  setTitle (tr ("Data Type Options"));

  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/cpfFilterWatermark.png"));

  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  valGrp = new QButtonGroup (this);
  valGrp->setExclusive (true);


  QGroupBox *invBox = new QGroupBox (this);
  QHBoxLayout *invBoxLayout = new QHBoxLayout;
  invBox->setLayout (invBoxLayout);


  QString validText = tr ("Check this box to set the cpfFilter Data Type Options filtering mode to <b>Validate</b>.  If this is checked, "
                          "all options on this page will be used to validate manually invalid data of the specified channel and/or data type "
                          "within the specified ranges.<br><br>"
                          "<b>IMPORTANT NOTE: The <i>Validate</i> option only works for the <i>Data Type Options</i>.  It does not "
                          "work with the <i>Special Case Options</i>.  If <i>Validate</i> is selected, the options on the <i>Special "
                          "Case Options</i> page will be ignored.</b><br><br>");


  QString invalidText = tr ("Check this box to set the cpfFilter Data Type Options filtering mode to <b>Invalidate</b>.  If this is checked, "
                            "all options on this page and the following page will be used to invalidate valid data that meet the specified "
                            "criteria.");


  valid = new QCheckBox (tr ("Validate"), this);
  valid->setChecked (!options->inv_flag);
  valid->setToolTip (tr ("Check this box to validate data"));
  valid->setWhatsThis (validText);
  valGrp->addButton (valid, 0);
  invBoxLayout->addWidget (valid);

  invalid = new QCheckBox (tr ("Invalidate"), this);
  invalid->setChecked (options->inv_flag);
  invalid->setToolTip (tr ("Check this box to invalidate data"));
  invalid->setWhatsThis (invalidText);
  valGrp->addButton (invalid, 1);
  invBoxLayout->addWidget (invalid);

  vbox->addWidget (invBox);


  typeTable = new QTableWidget (NUM_OPTECH_CLASSES, 14, this);
  typeTable->setAlternatingRowColors (true);

  QTableWidgetItem *tItem = new QTableWidgetItem (tr ("Data Type"));
  tItem->setToolTip (tr ("These are the processing modes"));
  tItem->setWhatsThis (tr ("These checkboxes allow you to select the data types of the data to be filtered."));
  typeTable->setHorizontalHeaderItem (0, tItem);

  QTableWidgetItem *clrItem = new QTableWidgetItem (tr ("Clear"));
  clrItem->setToolTip (tr ("Clear all settings for this data type"));
  clrItem->setWhatsThis (tr ("Clear all settings for this data type"));
  typeTable->setHorizontalHeaderItem (1, clrItem);

  QTableWidgetItem *dupItem = new QTableWidgetItem (tr ("Copy"));
  dupItem->setToolTip (tr ("Copy all settings from this data type to all other data types"));
  dupItem->setWhatsThis (tr ("Copy all settings from this data type to all other data types"));
  typeTable->setHorizontalHeaderItem (2, dupItem);


  clrGrp = new QButtonGroup (this);
  clrGrp->setExclusive (false);
  connect (clrGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotClrClicked (int)));

  dupGrp = new QButtonGroup (this);
  dupGrp->setExclusive (false);
  connect (dupGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotDupClicked (int)));


  //  Stretch the first header column so the table will fit nicely in the page.

#if QT_VERSION >= 0x050000
  typeTable->horizontalHeader ()->setSectionResizeMode (0, QHeaderView::Stretch);
#else
  typeTable->horizontalHeader ()->setResizeMode (0, QHeaderView::Stretch); 
#endif


  QTableWidgetItem *cItem[9];


  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      int32_t num = i + 1;
      if (i > 0) num += 29;
      czmil_get_short_proc_mode_string (num, short_proc_mode_name[i]);
      czmil_get_proc_mode_string (num, proc_mode_name[i]);


      typeText[i] = tr ("Checking this box will cause all %1 data to be (in)validated based on the values in the associated <b><i>Above</i></b> "
                        "and <b><i>Below</i></b> fields and the selected channel fields.").arg (proc_mode_name[i]);

      type[i] = new QCheckBox (QString (short_proc_mode_name[i]), this);
      type[i]->setChecked (options->data_type[i]);
      type[i]->setToolTip (tr ("Check this box if you wish to (in)validate %1 data").arg (proc_mode_name[i]));
      type[i]->setWhatsThis (typeText[i]);
      typeTable->setCellWidget (i, 0, type[i]);


      clr[i] = new QPushButton (this);
      clr[i]->setToolTip (tr ("Clears all settings for this data type"));
      clr[i]->setWhatsThis (tr ("Clears all settings for this data type"));
      typeTable->setCellWidget (i, 1, clr[i]);
      clrGrp->addButton (clr[i], i);


      dup[i] = new QPushButton (this);
      dup[i]->setToolTip (tr ("Copies all settings for this data type to all other data types"));
      dup[i]->setWhatsThis (tr ("Copies all settings for this data type to all other data types"));
      typeTable->setCellWidget (i, 2, dup[i]);
      dupGrp->addButton (dup[i], i);


      for (int32_t j = 0 ; j < 9 ; j++)
        {
          char chan_name[128];
          czmil_get_channel_string (j, chan_name);

          QString num;
          num.setNum (j + 1);


          cItem[j] = new QTableWidgetItem (num);
          cItem[j]->setToolTip (chan_name);
          cItem[j]->setWhatsThis (tr ("These checkboxes allow you to enable filtering for %1.").arg (chan_name));
          typeTable->setHorizontalHeaderItem (j + 3, cItem[j]);


          chan[i][j] = new QCheckBox (this);
          chan[i][j]->setChecked (options->type_chan[i][j]);
          chan[i][j]->setToolTip (tr ("Check this box if you wish to (in)validate %1 data for %2").arg (proc_mode_name[i]).arg (chan_name));
          chan[i][j]->setWhatsThis (tr ("Check this box if you wish to (in)validate %1 data for %2").arg (proc_mode_name[i]).arg (chan_name));

          typeTable->setCellWidget (i, j + 3, chan[i][j]);
        }


      above_valueText[i] = tr ("Set this value to select the elevation above which all %1 data will be (in)validated if this data type's select "
                               "box is checked.  To (in)validate all data of this data type select the minimum value for this field (-10000.0) "
                               "and the maximum value for the <b><i>Below</i></b> field (10000.0).<br><br>"
                               "<b>IMPORTANT NOTE: If the value in this field is less than the value in the <i>Below</i> field, data of this data "
                               "type <i>between</i> the two values will be (in)validated.</b>").arg (proc_mode_name[i]);


      QTableWidgetItem *aItem = new QTableWidgetItem (tr ("Above"));
      aItem->setToolTip (tr ("Filter data above this value"));
      aItem->setWhatsThis (above_valueText[i]);
      typeTable->setHorizontalHeaderItem (12, aItem);


      above_value[i] = new QDoubleSpinBox (this);
      above_value[i]->setDecimals (2);
      above_value[i]->setRange (-10000.00, 10000.00);
      above_value[i]->setSingleStep (10.0);
      above_value[i]->setValue (options->above_value[i]);
      above_value[i]->setWrapping (false);
      above_value[i]->setToolTip (tr ("Elevation above which (in)validation of this channel will be performed"));
      above_value[i]->setWhatsThis (above_valueText[i]);

      typeTable->setCellWidget (i, 12, above_value[i]);


      below_valueText[i] = tr ("Set this value to select the elevation below which all %1 data will be (in)validated if this data type's select "
                               "box is checked.  To (in)validate all data of this data type select the maximum value for this field (10000.0) "
                               "and the minimum value for the <b><i>Above</i></b> field (-10000.0).<br><br>"
                               "<b>IMPORTANT NOTE: If the value in this field is greater than the value in the <i>Above</i> field, data of this data "
                               "type <i>between</i> the two values will be (in)validated.</b>").arg (proc_mode_name[i]);


      QTableWidgetItem *bItem = new QTableWidgetItem (tr ("Below"));
      bItem->setToolTip (tr ("Filter data below this value"));
      bItem->setWhatsThis (below_valueText[i]);
      typeTable->setHorizontalHeaderItem (13, bItem);


      below_value[i] = new QDoubleSpinBox (this);
      below_value[i]->setDecimals (2);
      below_value[i]->setRange (-10000.00, 10000.00);
      below_value[i]->setSingleStep (10.0);
      below_value[i]->setValue (options->below_value[i]);
      below_value[i]->setWrapping (false);
      below_value[i]->setToolTip (tr ("Elevation below which (in)validation of this channel will be performed"));
      below_value[i]->setWhatsThis (below_valueText[i]);


      typeTable->setCellWidget (i, 13, below_value[i]);
    }

  vbox->addWidget (typeTable);

  typeTable->resizeColumnsToContents ();
  typeTable->resizeRowsToContents ();


  registerField ("invalidate", invalid);

  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      registerField (QString ("type %1 flag").arg (i), type[i]);

      for (int32_t j = 0 ; j < 9 ; j++)
        {
          registerField (QString ("type %1 chan %2 flag").arg (i).arg (j), chan[i][j]);
        }

      registerField (QString ("type %1 above").arg (i), above_value[i], "value");
      registerField (QString ("type %1 below").arg (i), below_value[i], "value");
    }
}



void typeOptionPage::slotDupClicked (int id)
{
  options->data_type[id] = type[id]->isChecked ();

  for (int32_t j = 0 ; j < 9 ; j++) options->type_chan[id][j] = chan[id][j]->isChecked ();

  options->above_value[id] = above_value[id]->value ();
  options->below_value[id] = below_value[id]->value ();

  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      options->data_type[i] = options->data_type[id];
      type[i]->setChecked (options->data_type[id]);
      options->above_value[i] = options->above_value[id];
      above_value[i]->setValue (options->above_value[i]);
      options->below_value[i] = options->below_value[id];
      below_value[i]->setValue (options->below_value[i]);

      for (int32_t j = 0 ; j < 9 ; j++)
        {
          options->type_chan[i][j] = options->type_chan[id][j];
          chan[i][j]->setChecked (options->type_chan[i][j]);
        }
    }
}



void typeOptionPage::slotClrClicked (int id)
{
  options->data_type[id] = NVFalse;
  type[id]->setChecked (options->data_type[id]);
  options->above_value[id] = -10000.0;
  above_value[id]->setValue (options->above_value[id]);
  options->below_value[id] = 10000.0;
  below_value[id]->setValue (options->below_value[id]);

  for (int32_t j = 0 ; j < 9 ; j++)
    {
      options->type_chan[id][j] = NVFalse;
      chan[id][j]->setChecked (options->type_chan[id][j]);
    }
}
