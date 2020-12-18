
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

QString deepUnderNumText = 
  specialOptionPage::tr ("Setting this value to anything other than zero will cause all CZMIL Deep channel returns that have at least this "
                         "number of valid shallow returns (not counting water surface returns) in the same shot to be invalidated.  If this "
                         "field is set to zero then no filtering of CZMIL Deep channel data based on the number of shallow returns will be done.");

QString apfBoxText = 
  specialOptionPage::tr ("This is the shallow channel, last land return, after-pulse filter.  If the after-pulse filter maximum return count number is 2 "
                         "or greater then the after-pulse filter will be run using the after-pulse filter maximum return count, the after-pulse filter "
                         "reflectance threshold value, and the after-pulse filter reflectance minimum value.  The logic is as follows:<br><br>"
                         "<b>For land processed shallow channels with between 2 and <i>Maximum return count</i> valid returns, where at least one valid "
                         "return (other than the last valid return) reflectance exceeds the <i>Reflectance threshold</i> value, valid returns after the "
                         "initial, threshold exceeding, valid return that have a reflectance value less than the <i>Reflectance minimum</i> value will be "
                         "invalidated.</b>");

QString apfCountText = 
  specialOptionPage::tr ("This is the after-pulse filter maximum return count value.  If this is set to 0 the after-pulse filter will not be run. "
                         "If it is set to 2 or more then the after-pulse filter will be run using this value, the after-pulse filter "
                         "reflectance threshold value, and the after-pulse filter reflectance minimum value.  The logic is as follows:<br><br>"
                         "<b>For land processed shallow channels with between 2 and <i>Maximum return count</i> valid returns, where at least one valid "
                         "return (other than the last valid return) reflectance exceeds the <i>Reflectance threshold</i> value, valid returns after the "
                         "initial, threshold exceeding, valid return that have a reflectance value less than the <i>Reflectance minimum</i> value will be "
                         "invalidated.</b>");

QString apfThresholdText = 
  specialOptionPage::tr ("This is the after-pulse filter reflectance threshold value.  If the after-pulse maximum return count number is 2 or greater "
                         "then the after-pulse filter will be run using the after-pulse filter maximum return count, this value, and the after-pulse filter "
                         "reflectance minimum value.  The logic is as follows:<br><br>"
                         "<b>For land processed shallow channels with between 2 and <i>Maximum return count</i> valid returns, where at least one valid "
                         "return (other than the last valid return) reflectance exceeds the <i>Reflectance threshold</i> value, valid returns after the "
                         "initial, threshold exceeding, valid return that have a reflectance value less than the <i>Reflectance minimum</i> value will be "
                         "invalidated.</b>");

QString apfMinimumText = 
  specialOptionPage::tr ("This is the after-pulse filter minimum reflectance value.  If the after-pulse maximum return count number is 2 or greater then "
                         "the after-pulse filter will be run using the after-pulse filter maximum return count, the after-pulse filter reflectance "
                         "threshold value, and this value.  The logic is as follows:<br><br>"
                         "<b>For land processed shallow channels with between 2 and <i>Maximum return count</i> valid returns, where at least one valid "
                         "return (other than the last valid return) reflectance exceeds the <i>Reflectance threshold</i> value, valid returns after the "
                         "initial, threshold exceeding, valid return that have a reflectance value less than the <i>Reflectance minimum</i> value will be "
                         "invalidated.</b>");

QString mask_fileText = 
  specialOptionPage::tr ("Use the browse button to select an input mask file created by the shape_mask program.  You cannot "
                         "modify the text in the <b>Mask File</b> text window.  This file is optional and will only be used if "
                         "you select <b>Invalidate land over water (SWBD and/or mask file)</b> or "
                         "<b>Invalidate water over land (SWBD and/or mask file)</b> on the <b>Data Type Options</b> "
                         "page of the wizard.  When a mask file is selected it is checked to make sure that it is "
                         "a mask file created by the shape_mask program.");

QString mask_fileBrowseText = 
  specialOptionPage::tr ("Use the browse button to select an optional input mask file created by the shape_mask program.");
