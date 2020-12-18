
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



#ifndef CPFFILTERDEF_H
#define CPFFILTERDEF_H

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif


#include "nvutility.h"
#include "nvutility.hpp"

#include "czmil.h"

#define     NUM_OPTECH_CLASSES       11


typedef struct
{
  int32_t       window_x;
  int32_t       window_y;
  int32_t       window_width;
  int32_t       window_height;
  QString       input_dir;
  QString       mask_dir;
  char          mask_name[1024];
  uint8_t       shape_mask;

  uint8_t       inv_flag;                                  //!<  Set to 1 to invalidate valid data or 0 to validate manually invalid data.


  //  The following are data types, channels, or conditions for invalidating or validating (depending on the setting of inv_flag, above).

  uint8_t       data_type[NUM_OPTECH_CLASSES];              /*!<  Set to (in)validate this data type in associated type_chan channel for values above and
                                                                 below the above_value and below_value respectively.  */

  uint8_t       type_chan[NUM_OPTECH_CLASSES][9];           /*!<  Set to (in)validate channel data for the associated data_type between above_value
                                                                 and below_value.  */

  float         above_value[NUM_OPTECH_CLASSES];            /*!<  The values above which points are (in)validated for the specified data_types and
                                                                 type_chans.  Data types are:
                                                                 [0] = land
                                                                 [1] = water
                                                                 [2] = shallow water
                                                                 [3] = average depth
                                                                 [4] = turbid water
                                                                 [5] = Robust Surface Detection missed surface return
                                                                 [6] = Robust Surface Detection multiple water surface detection
                                                                 [7] = Robust Surface Detection multiple water surface detection (deep channel)
                                                           */
  float         below_value[NUM_OPTECH_CLASSES];            /*!<  The values below which points are (in)validated for the specified data_types and
                                                                 type_chans.  Data types are the same as those for above_value.  */

  uint8_t       land_over_water_mask;                      /*!<  Set to invalidate points processed as land if they are over water according to SWBD and/or
                                                                 an optional mask file created by the shape_mask program.  */
  uint8_t       water_over_land_mask;                      /*!<  Set to invalidate points processed as water (of any kind) if they are over land according
                                                                 to SWBD and/or an optional mask file created by the shape_mask program.  */
  int32_t       deep_under_num;                            /*!<  Number of valid shallow returns that must be exeeded to invalidate the deep return.  If
                                                                 this is set to 0 then the deep channel will not be filtered.  */
  int32_t       first_of_many[NUM_OPTECH_CLASSES][9];       /*!<  Number of valid returns of the same data type on the same channel that are required to
                                                                 invalidate the first return.  If this is set to 0 then no first return filtering will
                                                                 be done.  Data types are the same as those for above_value.  */
  int32_t       apf_count;                                 //!<  Maximum number of returns for the shallow channel, last land return, after-pulse filter
  double        apf_threshold;                             //!<  Reflectance threshold for the shallow channel, last land return, after-pulse filter
  double        apf_minimum;                               //!<  Reflectance minimum for the shallow channel, last land return, after-pulse filter
  QFont         font;                                      //!<  Font used for all ABE GUI applications
} OPTIONS;



typedef struct
{
  QGroupBox           *fbox;
  QProgressBar        *fbar;
  QGroupBox           *rbox;
  QProgressBar        *rbar;
} RUN_PROGRESS;



#endif
