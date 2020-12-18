
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



#include "cpfFilter.hpp"
#include "cpfFilterHelp.hpp"


static double settings_version = 1.0;


QListWidget      *checkList;
RUN_PROGRESS     progress;


cpfFilter::cpfFilter (int32_t *argc, char **argv, QWidget *parent)
  : QWizard (parent, 0)
{
  QResource::registerResource ("/icons.rcc");


  //  Set the main icon

  setWindowIcon (QIcon (":/icons/cpfFilterWatermark.png"));


  //  Get the user's defaults if available

  envin (&options);


  // Set the application font

  QApplication::setFont (options.font);


  strcpy (options.mask_name, "");


  setWizardStyle (QWizard::ClassicStyle);


  setOption (HaveHelpButton, true);
  setOption (ExtendedWatermarkPixmap, false);

  connect (this, SIGNAL (helpRequested ()), this, SLOT (slotHelpClicked ()));


  //  Set the window size and location from the defaults

  this->resize (options.window_width, options.window_height);
  this->move (options.window_x, options.window_y);


  setPage (0, new startPage (argc, argv, this, &options));

  setPage (1, new typeOptionPage (this, &options));

  setPage (2, spec_opt_pg = new specialOptionPage (this, &options));

  setPage (3, new inputPage (this, &inputFiles, &options));

  setPage (4, new runPage (this, &progress, &checkList));


  setButtonText (QWizard::CustomButton1, tr ("&Run"));
  setOption (QWizard::HaveCustomButton1, true);
  button (QWizard::CustomButton1)->setToolTip (tr ("Start filtering the files"));
  button (QWizard::CustomButton1)->setWhatsThis (runText);
  connect (this, SIGNAL (customButtonClicked (int)), this, SLOT (slotCustomButtonClicked (int)));


  setStartId (0);
}


cpfFilter::~cpfFilter ()
{
}



void cpfFilter::initializePage (int id)
{
  button (QWizard::HelpButton)->setIcon (QIcon (":/icons/contextHelp.png"));
  button (QWizard::CustomButton1)->setEnabled (false);


  switch (id)
    {
    case 0:
      break;

    case 1:
      break;

    case 2:
      options.inv_flag = field ("invalidate").toBool ();
      spec_opt_pg->setEnabled (options.inv_flag);
      break;

    case 3:
      break;

    case 4:

      for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
        {
          options.data_type[i] = field (QString ("type %1 flag").arg (i)).toBool ();

          for (int32_t j = 0 ; j < 9 ; j++)
            {
              options.type_chan[i][j] = field (QString ("type %1 chan %2 flag").arg (i).arg (j)).toBool ();
            }

          options.above_value[i] = field (QString ("type %1 above").arg (i)).toFloat ();
          options.below_value[i] = field (QString ("type %1 below").arg (i)).toFloat ();
        }

      options.land_over_water_mask = field ("land_mask").toBool ();
      options.water_over_land_mask = field ("water_mask").toBool ();
      options.deep_under_num = field ("deepUnderNum").toInt ();
      options.apf_count = field ("apfCount").toInt ();
      options.apf_threshold = field ("apfThreshold").toDouble ();
      options.apf_minimum = field ("apfMinimum").toDouble ();

      for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
        {
          for (int32_t j = 0 ; j < 9 ; j++)
            {
              options.first_of_many[i][j] = field (QString ("type %1 chan %2 first of many value").arg (i).arg (j)).toInt ();
            }
        }

      if (check_swbd_mask (3) && strlen (options.mask_name) < 3)
        {
          options.land_over_water_mask = NVFalse;
          options.water_over_land_mask = NVFalse;
        }

      options.shape_mask = NVFalse;
      if (strlen (options.mask_name) > 3) options.shape_mask = NVTrue;

      button (QWizard::CustomButton1)->setEnabled (true);


      //  Use frame geometry to get the absolute x and y.

      QRect tmp = this->frameGeometry ();
      options.window_x = tmp.x ();
      options.window_y = tmp.y ();


      //  Use geometry to get the width and height.

      tmp = this->geometry ();
      options.window_width = tmp.width ();
      options.window_height = tmp.height ();


      //  Save the options.

      envout (&options);


      QString string;

      checkList->clear ();


      uint8_t filter = NVFalse;


      if (options.inv_flag)
        {
          string = tr ("\nAll valid data will be invalidated when it meets the following conditions:");
          checkList->addItem (string);
        }
      else
        {
          string = tr ("\nAll manually invalid data will be validated when it meets the following conditions:");
          checkList->addItem (string);
        }


      checkList->addItem (tr ("\nData Type Options\n"));


      for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
        {
          uint8_t type_flag = NVFalse;

          char proc_mode_name[128];
          int32_t num = i + 1;
          if (i > 0) num += 29;
          czmil_get_proc_mode_string (num, proc_mode_name);

          for (int32_t j = 0 ; j < 9 ; j++)
            {
              if (options.type_chan[i][j])
                {
                  if (options.data_type[i])
                    {
                      type_flag = NVTrue;
                      break;
                    }
                }
            }

          if (type_flag)
            {
              string = QString (proc_mode_name) + " data";
              checkList->addItem (string);
            }

          for (int32_t j = 0 ; j < 9 ; j++)
            {
              if (options.data_type[i] && options.type_chan[i][j])
                {
                  filter = NVTrue;

                  char chan_name[128];
                  czmil_get_channel_string (j, chan_name);

                  string = QString ("\t%1").arg (chan_name);
                  checkList->addItem (string);
                }
            }

          if (type_flag)
            {
              if (options.above_value[i] < options.below_value[i])
                {
                  between_flag[i] = NVTrue;

                  string = QString ("\tBetween %1 and %2").arg (options.below_value[i]).arg (options.above_value[i]);
                  checkList->addItem (string);
                }
              else
                {
                  between_flag[i] = NVFalse;

                  string = QString ("\tAbove %1 and below %2").arg (options.above_value[i]).arg (options.below_value[i]);
                  checkList->addItem (string);
                }
            }
        }


      //  We're not going to honor the Special Case Options unless we're invalidating.

      if (options.inv_flag)
        {
          checkList->addItem (tr ("\n\nSpecial Case Options\n"));

          if (options.land_over_water_mask)
            {
              filter = NVTrue;
              if (options.shape_mask)
                {
                  string = tr ("Land returns over %1 water areas will be filtered.").arg (options.mask_name);
                }
              else
                {
                  string = tr ("Land returns over SWBD water areas will be filtered.");
                }
              checkList->addItem (string);
            }

          if (options.water_over_land_mask)
            {
              filter = NVTrue;
              if (options.shape_mask)
                {
                  string = tr ("Water returns over %1 land areas will be filtered.").arg (options.mask_name);
                }
              else
                {
                  string = tr ("Water returns and shallow water returns over SWBD land areas will be filtered.");
                }
              checkList->addItem (string);
            }

          if (options.deep_under_num)
            {
              filter = NVTrue;
              string = tr ("Invalidate CZMIL Deep Channel data when %1 valid shallow returns are present").arg (options.deep_under_num);
              checkList->addItem (string);
            }

          if (options.apf_count)
            {
              filter = NVTrue;
              string = tr ("Invalidate valid shallow channel land returns where there are between 2 and %1 valid returns,\n"
                           "at least one valid return reflectance, other than the last valid return, exceeds %L2,\n"
                           "and valid returns after the initial threshold valid return have a reflectance less than %L3").arg (options.apf_count + 1).arg 
                (options.apf_threshold, 0, 'f', 3).arg (options.apf_minimum, 0, 'f', 4);

              checkList->addItem (string);
            }

          for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
            {
              uint8_t type_flag = NVFalse;

              char proc_mode_name[128];
              int32_t num = i + 1;
              if (i > 0) num += 29;
              czmil_get_proc_mode_string (num, proc_mode_name);

              for (int32_t j = 0 ; j < 9 ; j++)
                {
                  if (options.first_of_many[i][j])
                    {
                      type_flag = NVTrue;
                      break;
                    }
                }

              if (type_flag)
                {
                  string = QString (proc_mode_name) + " data";
                  checkList->addItem (string);

                  for (int32_t j = 0 ; j < 9 ; j++)
                    {
                      if (options.first_of_many[i][j])
                        {
                          filter = NVTrue;

                          char chan_name[128];
                          czmil_get_channel_string (j, chan_name);

                          string = QString ("\tFirst return of at least %1 %2 returns").arg (options.first_of_many[i][j]).arg (chan_name);
                          checkList->addItem (string);
                        }
                    }
                }
            }
        }

      if (!filter)
        {
          QListWidgetItem *cur =
            new QListWidgetItem (tr ("\n\nWhiskey Tango Foxtrot Over?!\n\nYou have apparently set this program to do NO filtering.\n\n"
                                     "You might want to go back and check your settings.\n\n"));
          checkList->addItem (cur);
          checkList->setCurrentItem (cur);
          checkList->scrollToItem (cur);
 
          button (QWizard::CustomButton1)->setEnabled (false);
        }
      else
        {
          QListWidgetItem *cur = new QListWidgetItem ("");

          checkList->addItem (cur);
          checkList->setCurrentItem (cur);
          checkList->scrollToItem (cur); 
        }
      break;
    }
}



void cpfFilter::cleanupPage (int id)
{
  switch (id)
    {
    case 0:
      break;

    case 1:
      break;

    case 2:
      break;

    case 3:
      break;

    case 4:
      break;

    case 5:
      break;
    }
}



void cpfFilter::slotHelpClicked ()
{
  QWhatsThis::enterWhatsThisMode ();
}



//  This is where the fun stuff happens.

void 
cpfFilter::slotCustomButtonClicked (int id __attribute__ ((unused)))
{
  char string[1024];
  int32_t cpf_hnd = -1;
  QString tmp;
  QListWidgetItem *cur;
  CZMIL_CPF_Header cpf_header;
  CZMIL_CPF_Data cpf_record;


  QApplication::setOverrideCursor (Qt::WaitCursor);


  button (QWizard::FinishButton)->setEnabled (false);
  button (QWizard::BackButton)->setEnabled (false);
  button (QWizard::CustomButton1)->setEnabled (false);


  //  Get the files from the QTextEdit box on the inputPage.

  QTextCursor inputCursor = inputFiles->textCursor ();

  inputCursor.setPosition (0);


  QStringList sort_files;

  sort_files.clear ();


  //  First we need to put the files in the sort list

  do
    {
      sort_files << inputCursor.block ().text ();
    } while (inputCursor.movePosition (QTextCursor::NextBlock));


  //  Sort so we can remove dupes.

  sort_files.sort ();


  //  Remove dupes and place into input_files.

  QString name, prev_name = "";
  input_files.clear ();

  for (int32_t i = 0 ; i < sort_files.size () ; i++)
    {
      name = sort_files.at (i);

      if (name != prev_name)
        {
          input_files.append (name);
          prev_name = name;
        }
    }


  int32_t input_file_count = input_files.size ();


  progress.rbar->setRange (0, input_file_count);

  for (int32_t j = 0 ; j < input_file_count ; j++)
    {
      QString name = input_files.at (j);


      progress.rbar->setValue (j);

      progress.fbar->reset ();
      tmp = tr ("Filtering file %1 of %2 - %3").arg(j + 1).arg(input_file_count).arg(QFileInfo (name).fileName ());
      progress.fbox->setTitle (tmp);
      qApp->processEvents ();


      //  Open the CPF file.

      strcpy (string, name.toLatin1 ());
      if ((cpf_hnd = czmil_open_cpf_file (string, &cpf_header, CZMIL_UPDATE) < 0))
        {
          perror (czmil_strerror ());
        }
      else
        {
          progress.fbar->setRange (0, cpf_header.number_of_records);

          int32_t mod_count = 0;
          int32_t mod = NVFalse;
          for (int32_t i = 0 ; i < cpf_header.number_of_records ; i++)
            {
              progress.fbar->setValue (i);


              //  Read the CPF record.

              if (czmil_read_cpf_record (cpf_hnd, i, &cpf_record) != CZMIL_SUCCESS)
                {
                  perror (czmil_strerror ());
                  break;
                }


              //  Invalidate section...

              if (options.inv_flag)
                {
                  //  Data Type section...

                  for (int32_t dt = 0 ; dt < NUM_OPTECH_CLASSES ; dt++)
                    {
                      if (options.data_type[dt])
                        {
                          int32_t cls = dt + 1;
                          if (dt) cls += 29;


                          //  Channel loop...

                          for (int32_t k = 0 ; k < 9 ; k++)
                            {
                              if (options.type_chan[dt][k])
                                {
                                  //  Returns loop...

                                  for (int32_t m = 0 ; m < cpf_record.returns[k] ; m++)
                                    {
                                      //  Check the data type...

                                      if (cpf_record.optech_classification[k] == cls)
                                        {
                                          //  No point in invalidating already invalid records.

                                          if (!(cpf_record.channel[k][m].status & CZMIL_RETURN_INVAL))
                                            {
                                              //  Check for the "between" flag.

                                              if (between_flag[dt])
                                                {
                                                  if (cpf_record.channel[k][m].elevation > options.above_value[dt] &&
                                                      cpf_record.channel[k][m].elevation < options.below_value[dt])
                                                    {
                                                      cpf_record.channel[k][m].status = CZMIL_RETURN_MANUALLY_INVAL;

                                                      mod = NVTrue;
                                                      mod_count++;
                                                    }
                                                }
                                              else
                                                {
                                                  if (cpf_record.channel[k][m].elevation > options.above_value[dt] ||
                                                      cpf_record.channel[k][m].elevation < options.below_value[dt])
                                                    {
                                                      cpf_record.channel[k][m].status = CZMIL_RETURN_MANUALLY_INVAL;

                                                      mod = NVTrue;
                                                      mod_count++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }



                  //  Special Case section...


                  //  Deep under valid shallow...

                  if (options.deep_under_num)
                    {
                      int32_t shallow_count = 0;


                      //  Channel loop...

                      for (int32_t k = 0 ; k < 7 ; k++)
                        {
                          //  Returns loop...

                          for (int32_t m = 0 ; m < cpf_record.returns[k] ; m++)
                            {
                              //  Count valid, non-water-surface, shallow returns.

                              if (!(cpf_record.channel[k][m].status & CZMIL_RETURN_INVAL) &&
                                  (cpf_record.channel[k][m].ip_rank || cpf_record.optech_classification[k] == CZMIL_OPTECH_CLASS_HYBRID)) shallow_count++;
                            }
                        }


                      //  If we met or exceeded our shallow count...

                      if (shallow_count >= options.deep_under_num)
                        {
                          for (int32_t m = 0 ; m < cpf_record.returns[CZMIL_DEEP_CHANNEL] ; m++)
                            {
                              //  No point in invalidating already invalid records.

                              if (!(cpf_record.channel[CZMIL_DEEP_CHANNEL][m].status & CZMIL_RETURN_INVAL))
                                {
                                  cpf_record.channel[CZMIL_DEEP_CHANNEL][m].status = CZMIL_RETURN_MANUALLY_INVAL;

                                  mod = NVTrue;
                                  mod_count++;
                                }
                            }
                        }
                    }


                  //  Shallow channel after-pulses...

                  if (options.apf_count)
                    {
                      //  Shallow channel loop...

                      for (int32_t k = 0 ; k < 7 ; k++)
                        {
                          //  Only do this for land returns.

                          if (cpf_record.optech_classification[k] < CZMIL_OPTECH_CLASS_WATER)
                            {
                              //  Set the filter flag for this channel.

                              uint8_t apf_filter = NVFalse;


                              //  Note that, unless options.apf_count is zero, the value in options.apf_count is an index into a QComboBox
                              //  so we have to add 1 to the value to get the actual count.

                              int32_t actual_apf_count = options.apf_count + 1;
                              int32_t last_valid_return = 0;
                              int32_t initial_threshold_return = 0;
                              int32_t valid_return_count = 0;


                              //  Returns loop...

                              for (int32_t m = 0 ; m < cpf_record.returns[k] ; m++)
                                {
                                  //  Check for validity

                                  if (!(cpf_record.channel[k][m].status & CZMIL_RETURN_INVAL))
                                    {
                                      //  Save the last valid return number (it keeps getting reset until we run out of valid returns).

                                      last_valid_return = m;


                                      //  Save the valid return count.

                                      valid_return_count++;


                                      //  Check for the reflectance threshold (we need at least one to exceed the threshold).

                                      if (cpf_record.channel[k][m].reflectance > options.apf_threshold)
                                        {
                                          apf_filter = NVTrue;
                                          if (!initial_threshold_return) initial_threshold_return = m;
                                        }
                                    }
                                }


                              //  If we have more than the required number of valid returns or less than 2 valid returns, we won't check the 
                              //  last valid return.

                              if (valid_return_count <= actual_apf_count && valid_return_count >= 2)
                                {
                                  //  If we have met the proper conditions, filter valid returns following the initial, threshold exceeding, valid return.

                                  if (apf_filter)
                                    {
                                      for (int32_t n = initial_threshold_return + 1 ; n <= last_valid_return ; n++)
                                        {
                                          //  If the return reflectance is less than the minimum value, invalidate it.

                                          if (cpf_record.channel[k][n].reflectance < options.apf_minimum)
                                            {
                                              cpf_record.channel[k][n].status = CZMIL_RETURN_MANUALLY_INVAL;
                                              mod = NVTrue;
                                              mod_count++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }


                  //  First of many...

                  for (int32_t dt = 0 ; dt < NUM_OPTECH_CLASSES ; dt++)
                    {
                      int32_t cls = dt + 1;
                      if (dt) cls += 29;


                      //  Channel loop...

                      for (int32_t k = 0 ; k < 9 ; k++)
                        {
                          //  Check the data type...

                          if (cpf_record.optech_classification[k] == cls)
                            {
                              if (options.first_of_many[dt][k])
                                {
                                  int8_t first_point = -1;
                                  int32_t many_count = 0;


                                  //  Returns loop...

                                  for (int32_t m = 0 ; m < cpf_record.returns[k] ; m++)
                                    {
                                      //  Don't use water surface returns

                                      if (cpf_record.channel[k][m].ip_rank || cpf_record.optech_classification[k] == CZMIL_OPTECH_CLASS_HYBRID)
                                        {
                                          if (first_point < 0)
                                            {
                                              first_point = m;
                                            }
                                          else
                                            {
                                              //  Count valid returns after the first non-water-surface return

                                              if (!(cpf_record.channel[k][m].status & CZMIL_RETURN_INVAL)) many_count++;
                                            }
                                        }
                                    }

                                  if (many_count >= options.first_of_many[dt][k])
                                    {
                                      //  No point in invalidating already invalidated return

                                      if (!(cpf_record.channel[k][first_point].status & CZMIL_RETURN_INVAL))
                                        {
                                          cpf_record.channel[k][first_point].status = CZMIL_RETURN_MANUALLY_INVAL;

                                          mod = NVTrue;
                                          mod_count++;
                                        }
                                    }
                                }
                            }
                        }
                    }


                  //  Invalidate land over water...

                  if (options.land_over_water_mask)
                    {
                      //  Channel loop...

                      for (int32_t k = 0 ; k < 9 ; k++)
                        {
                          //  Returns loop...

                          for (int32_t m = 0 ; m < cpf_record.returns[k] ; m++)
                            {
                              if (cpf_record.optech_classification[k] < 32 && cpf_record.optech_classification[k] > 0)
                                {
                                  //  No point in invalidating (or checking) already invalid records.

                                  if (!(cpf_record.channel[k][m].status & CZMIL_RETURN_INVAL))
                                    {
                                      int32_t ret;


                                      //  If we're using a mask file created by shape_mask check it first.

                                      if (options.shape_mask)
                                        {
                                          ret = shape_mask_is_land (options.mask_name, cpf_record.channel[k][m].latitude,
                                                                    cpf_record.channel[k][m].longitude);
                                          if (ret == -9)
                                            {
                                              ret = swbd_is_land (cpf_record.channel[k][m].latitude, cpf_record.channel[k][m].longitude, 1);
                                            }
                                          else
                                            {
                                              if (ret < 0)
                                                {
                                                  QMessageBox::warning (this, "cpfFilter", tr ("Error %1 reading shape mask file %2").arg (ret).arg (options.mask_name));
                                                  exit (-1);
                                                }
                                            }
                                        }
                                      else
                                        {
                                          ret = swbd_is_land (cpf_record.channel[k][m].latitude, cpf_record.channel[k][m].longitude, 1);
                                        }


                                      if (ret == 0)
                                        {
                                          cpf_record.channel[k][m].status = CZMIL_RETURN_MANUALLY_INVAL;
                                          mod = NVTrue;
                                          mod_count++;
                                        }
                                    }
                                }
                            }
                        }
                    }


                  //  Invalidate water over land...

                  if (options.water_over_land_mask)
                    {
                      //  Channel loop...

                      for (int32_t k = 0 ; k < 9 ; k++)
                        {
                          //  Returns loop...

                          for (int32_t m = 0 ; m < cpf_record.returns[k] ; m++)
                            {
                              if (cpf_record.optech_classification[k] > 31)
                                {
                                  //  No point in invalidating (or checking) already invalid records.

                                  if (!(cpf_record.channel[k][m].status & CZMIL_RETURN_INVAL))
                                    {
                                      int32_t ret;


                                      //  If we're using a mask file created by shape_mask check it first.

                                      if (options.shape_mask)
                                        {
                                          ret = shape_mask_is_land (options.mask_name, cpf_record.channel[k][m].latitude,
                                                                    cpf_record.channel[k][m].longitude);
                                          if (ret == -9)
                                            {
                                              ret = swbd_is_land (cpf_record.channel[k][m].latitude, cpf_record.channel[k][m].longitude, 1);
                                            }
                                          else
                                            {
                                              if (ret < 0)
                                                {
                                                  QMessageBox::warning (this, "cpfFilter", tr ("Error %1 reading shape mask file %2").arg (ret).arg (options.mask_name));
                                                  exit (-1);
                                                }
                                            }
                                        }
                                      else
                                        {
                                          ret = swbd_is_land (cpf_record.channel[k][m].latitude, cpf_record.channel[k][m].longitude, 1);
                                        }

                                      if (ret == 1)
                                        {
                                          cpf_record.channel[k][m].status = CZMIL_RETURN_MANUALLY_INVAL;
                                          mod = NVTrue;
                                          mod_count++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }


              //  Validate section...

              else
                {
                  //  Data Type section...

                  for (int32_t dt = 0 ; dt < NUM_OPTECH_CLASSES ; dt++)
                    {
                      if (options.data_type[dt])
                        {
                          int32_t cls = dt + 1;
                          if (dt) cls += 29;


                          //  Channel loop...

                          for (int32_t k = 0 ; k < 9 ; k++)
                            {
                              if (options.type_chan[dt][k])
                                {
                                  //  Returns loop...

                                  for (int32_t m = 0 ; m < cpf_record.returns[k] ; m++)
                                    {
                                      // Check the data type...
                                      
                                      if (cpf_record.optech_classification[k] == cls)
                                        {
                                          //  No point in validating already valid records.  We also don't want to validate filter invalid records.

                                          if (cpf_record.channel[k][m].status & CZMIL_RETURN_MANUALLY_INVAL)
                                            {
                                              //  Check for the "between" flag.

                                              if (between_flag[dt])
                                                {
                                                  if (cpf_record.channel[k][m].elevation > options.above_value[dt] &&
                                                      cpf_record.channel[k][m].elevation < options.below_value[dt])
                                                    {
                                                      cpf_record.channel[k][m].status &= ~CZMIL_RETURN_MANUALLY_INVAL;

                                                      mod = NVTrue;
                                                      mod_count++;
                                                    }
                                                }
                                              else
                                                {
                                                  if (cpf_record.channel[k][m].elevation > options.above_value[dt] ||
                                                      cpf_record.channel[k][m].elevation < options.below_value[dt])
                                                    {
                                                      cpf_record.channel[k][m].status &= ~CZMIL_RETURN_MANUALLY_INVAL;

                                                      mod = NVTrue;
                                                      mod_count++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }


              if (mod)
                {
                  //  Write the CPF record.

                  if (czmil_update_cpf_return_status (cpf_hnd, i, &cpf_record) != CZMIL_SUCCESS)
                    {
                      perror (czmil_strerror ());
                      break;
                    }
                }
            }

          czmil_close_cpf_file (cpf_hnd);

          progress.fbar->setValue (cpf_header.number_of_records);
          qApp->processEvents ();

          checkList->addItem (" ");
          cur = new QListWidgetItem (tr ("%1 filtering finished, %2 returns modified").arg (tmp).arg (mod_count));
          checkList->addItem (cur);
          checkList->setCurrentItem (cur);
          checkList->scrollToItem (cur);
        }
    }


  //  If we used a shape mask file, free the memory by calling shape_mask_is_land with latitude > 180.

  if (options.shape_mask) shape_mask_is_land (options.mask_name, 999.0, 999.0);


  //  If we used the SWBD files, free the memory by calling swbd_is_land with args over 360.

  if (options.land_over_water_mask || options.water_over_land_mask) swbd_is_land (360.0, 360.0, 1);


  progress.rbar->setValue (input_file_count);
  qApp->processEvents ();

  button (QWizard::FinishButton)->setEnabled (true);
  button (QWizard::CancelButton)->setEnabled (false);


  QApplication::restoreOverrideCursor ();
  qApp->processEvents ();


  checkList->addItem (" ");
  cur = new QListWidgetItem (tr ("Filtering complete, press Finish to exit."));

  checkList->addItem (cur);
  checkList->setCurrentItem (cur);
  checkList->scrollToItem (cur);
}



//  Get the users defaults.

void cpfFilter::envin (OPTIONS *options)
{
  //  We need to get the font from the global settings.

#ifdef NVWIN3X
  QString ini_file2 = QString (getenv ("USERPROFILE")) + "/ABE.config/" + "globalABE.ini";
#else
  QString ini_file2 = QString (getenv ("HOME")) + "/ABE.config/" + "globalABE.ini";
#endif

  options->font = QApplication::font ();

  QSettings settings2 (ini_file2, QSettings::IniFormat);
  settings2.beginGroup ("globalABE");


  QString defaultFont = options->font.toString ();
  QString fontString = settings2.value (QString ("ABE map GUI font"), defaultFont).toString ();
  options->font.fromString (fontString);


  settings2.endGroup ();


  double saved_version = 1.0;


  // Set defaults so that if keys don't exist the parameters are defined

  options->inv_flag = NVTrue;

  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      options->data_type[i] = NVTrue;
      options->above_value[i] = -10000.0;
      options->below_value[i] = 10000.0;

      for (int32_t j = 0 ; j < 9 ; j++)
        {
          options->type_chan[i][j] = NVTrue;
          options->first_of_many[i][j] = 0;
        }
    }

  options->land_over_water_mask = NVFalse;
  options->water_over_land_mask = NVFalse;
  options->deep_under_num = 0;

  options->apf_count = 0;
  options->apf_threshold = 0.007;
  options->apf_minimum = 0.0003;

  options->input_dir = ".";
  options->mask_dir = ".";
  options->window_x = 0;
  options->window_y = 0;
  options->window_width = 800;
  options->window_height = 400;


  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/cpfFilter.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/cpfFilter.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("cpfFilter");

  saved_version = settings.value ("settings version", saved_version).toDouble ();


  //  If the settings version has changed we need to leave the values at the new defaults since they may have changed.

  if (settings_version != saved_version) return;


  options->inv_flag = settings.value ("invalidate flag", options->inv_flag).toBool ();

  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      char proc_mode_name[128];
      int32_t num = i + 1;
      if (i > 0) num += 29;
      czmil_get_proc_mode_string (num, proc_mode_name);

      options->data_type[i] = settings.value (QString (proc_mode_name) + " flag", options->data_type[i]).toBool ();

      for (int32_t j = 0 ; j < 9 ; j++)
        {
          char chan_name[128];
          czmil_get_channel_string (j, chan_name);

          options->type_chan[i][j] = settings.value (QString (proc_mode_name) + " " + QString (chan_name) + " flag", options->type_chan[i][j]).toBool ();
          options->first_of_many[i][j] = settings.value (QString (proc_mode_name) + " " + QString (chan_name) + " first of many value",
            options->first_of_many[i][j]).toInt ();
        }

      options->above_value[i] = settings.value (QString (proc_mode_name) + " above value", options->above_value[i]).toFloat ();

      options->below_value[i] = settings.value (QString (proc_mode_name) + " below value", options->below_value[i]).toFloat ();
    }


  options->land_over_water_mask = settings.value (QString ("invalidate land over masked water areas"), options->land_over_water_mask).toBool ();

  options->water_over_land_mask = settings.value (QString ("invalidate water over masked land areas"), options->water_over_land_mask).toBool ();

  options->deep_under_num = settings.value (QString ("invalidate deep returns shallow count"), options->deep_under_num).toInt ();

  options->apf_count = settings.value (QString ("after pulse filter count"), options->apf_count).toInt ();
  options->apf_threshold = settings.value (QString ("after pulse filter threshold"), options->apf_threshold).toDouble ();
  options->apf_minimum = settings.value (QString ("after pulse filter minimum"), options->apf_minimum).toDouble ();

  options->input_dir = settings.value (QString ("input directory"), options->input_dir).toString ();
  options->mask_dir = settings.value (QString ("mask directory"), options->mask_dir).toString ();

  options->window_width = settings.value (QString ("width"), options->window_width).toInt ();
  options->window_height = settings.value (QString ("height"), options->window_height).toInt ();
  options->window_x = settings.value (QString ("x position"), options->window_x).toInt ();
  options->window_y = settings.value (QString ("y position"), options->window_y).toInt ();

  settings.endGroup ();
}




//  Save the users defaults.

void cpfFilter::envout (OPTIONS *options)
{
  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/cpfFilter.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/cpfFilter.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("cpfFilter");

  settings.setValue (QString ("settings version"), settings_version);


  settings.setValue ("invalidate flag", options->inv_flag);

  for (int32_t i = 0 ; i < NUM_OPTECH_CLASSES ; i++)
    {
      char proc_mode_name[128];
      int32_t num = i + 1;
      if (i > 0) num += 29;
      czmil_get_proc_mode_string (num, proc_mode_name);

      settings.setValue (QString (proc_mode_name) + " flag", options->data_type[i]);

      for (int32_t j = 0 ; j < 9 ; j++)
        {
          char chan_name[128];
          czmil_get_channel_string (j, chan_name);

          settings.setValue (QString (proc_mode_name) + " " + QString (chan_name) + " flag", options->type_chan[i][j]);
          settings.setValue (QString (proc_mode_name) + " " + QString (chan_name) + " first of many value", options->first_of_many[i][j]);
        }

      settings.setValue (QString (proc_mode_name) + " above value", options->above_value[i]);

      settings.setValue (QString (proc_mode_name) + " below value", options->below_value[i]);
    }


  settings.setValue (QString ("invalidate land over masked water areas"), options->land_over_water_mask);

  settings.setValue (QString ("invalidate water over masked land areas"), options->water_over_land_mask);

  settings.setValue (QString ("invalidate deep returns shallow count"), options->deep_under_num);


  settings.setValue (QString ("after pulse filter count"), options->apf_count);
  settings.setValue (QString ("after pulse filter threshold"), options->apf_threshold);
  settings.setValue (QString ("after pulse filter minimum"), options->apf_minimum);


  settings.setValue (QString ("input directory"), options->input_dir);
  settings.setValue (QString ("mask directory"), options->mask_dir);

  settings.setValue (QString ("width"), options->window_width);
  settings.setValue (QString ("height"), options->window_height);
  settings.setValue (QString ("x position"), options->window_x);
  settings.setValue (QString ("y position"), options->window_y);

  settings.endGroup ();
}
