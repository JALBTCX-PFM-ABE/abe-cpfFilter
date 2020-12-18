
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




#ifndef VERSION

#define     VERSION     "PFM Software - cpfFilter V1.30 - 12/14/18"

#endif

/*! <pre>


    Version 1.00
    Jan C. Depner (PFM Software)
    08/30/13

    First version.


    Version 1.01
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 1.02
    Jan C. Depner (PFM Software)
    12/12/13

    Added options to invalidate land returns, water returns, shallow water returns, land returns over water
    based on SWBD, and water/shallow water returns over land based on SWBD.  Land, water, and shallow water
    processing modes are 1, 2, and 3 respectively (or CZMIL_RETURN_PROC_LAND, CZMIL_RETURN_PROC_WATER, and
    CZMIL_RETURN_PROC_SHALLOW_WATER).


    Version 1.03
    Jan C. Depner (PFM Software)
    12/17/13

    Aaaaarrrrrgggghhhhh!  I had the land/water tests backwards for the SWBD "land over water" and vice versa
    tests.


    Version 1.04
    Jan C. Depner (PFM Software)
    12/30/13

    Switched to using the new 1 second SWBD land mask.


    Version 1.05
    Jan C. Depner (PFM Software)
    01/06/14

    Fixed an obvious screwup in inputPage.cpp where I was re-using the loop counters.
    Scoping in C++ saved my bacon but it was just too damn confusing.


    Version 1.06
    Jan C. Depner (PFM Software)
    01/15/14

    Aaaaarrrrgggghhhh!  Typo in a loop variable.


    Version 1.07
    Jan C. Depner (PFM Software)
    01/20/14

    Now using processing_mode instead of classification after changes to czmil API to handle
    processing mode being stored in classification prior to libCZMIL 2.0.


    Version 1.08
    Jan C. Depner (PFM Software)
    01/24/14

    - Remover pre-pulse filtering since Optech is now handling that in HydroFusion.
    - Added above/below elevation filtering.
    - Split the optionsPage into levelOptions, typeOptions, and channelOptions.


    Version 1.09
    Jan C. Depner (PFM Software)
    01/31/14

    - Fixed cut-n-paste error with filter_below options.


    Version 1.10
    Jan C. Depner (PFM Software)
    02/07/14

    - Fixed yet another cut-n-paste error with filter_below options.
    - Winter Olympics!!!


    Version 1.11
    Jan C. Depner (PFM Software)
    02/09/14

    - Changed the "No filter", "Invalidate", "Validate" radio buttons to "Invalidate" and "Validate" checkBoxes
      that emulate radio buttons except that both may be off.  This makes the UI a bit easier on the eye.


    Version 1.12
    Jan C. Depner (PFM Software)
    02/11/14

    - One more freaking patch for elevation filtering.


    Version 1.13
    Jan C. Depner (PFM Software)
    07/01/14

    - Replaced all of the old, borrowed icons with new, public domain icons.  Mostly from the Tango set
      but a few from flavour-extended and 32pxmania.


    Version 1.14
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 1.15
    Jan C. Depner (PFM Software)
    09/04/14

    Support for new CZMIL v2 fields.


    Version 1.16
    Jan C. Depner (PFM Software)
    11/30/14

    Added all the new optech_classification processing modes.  Also, added matching validate options for all
    invalidation fields.


    Version 1.17
    Jan C. Depner (PFM Software)
    12/18/14

    Added invalidate deep with multiple shallow returns option.


    Version 1.20
    Jan C. Depner (PFM Software)
    12/23/14

    Completely reworked the options.  It's now much more versatile than it was.  Also added "invalidate first of many" option.


    Version 1.21
    Jan C. Depner (PFM Software)
    01/02/15

    Fixed bug in "first of many" section.


    Version 1.22
    Jan C. Depner (PFM Software)
    01/07/15

    Fixed massive bug in data type section.  Also, made better copy and clear buttons.


    Version 1.23
    Jan C. Depner (PFM Software)
    04/01/16

    Added shallow channel, land return, after-pulse filter.


    Version 1.24
    Jan C. Depner (PFM Software)
    08/04/16

    Fixed logic bugs in checks for CZMIL_INVALID.


    Version 1.25
    Jan C. Depner (PFM Software)
    08/27/16

    - Now uses the same font as all other ABE GUI apps.  Font can only be changed in pfmView Preferences.


    Version 1.26
    Jan C. Depner (PFM Software)
    10/07/16

    - Lowered bottom limit for Reflectance Threshold to 0.001.


    Version 1.27
    Jan C. Depner (PFM Software)
    04/19/17

    - Now handles ip_rank = 0 returns for waveforms that were CZMIL_OPTECH_CLASS_HYBRID processed.


    Version 1.28
    Michael Novoa (Optimal GEO)
    07/07/17

    - Updated Data Type tables on 'Data Type Options' and 'Special Case Options' pages to include all currently defined CZMIL Optech classes (1 and 31 thru 40).
    - Fixed bug in 'Special Case Options' page where clicking the 'Clear' button on any row cleared the entire table.


    Version 1.29
    Jan C. Depner (PFM Software)
    09/26/17

    - A bunch of changes to support doing translations in the future.  There is a generic
      cpfFilter_xx.ts file that can be run through Qt's "linguist" to translate to another language.
      
    
    Version 1.30
    Michael Novoa (Optimal GEO)
    12/14/18
    
    - Fixed bug in validation code in which the Optech classification (i.e., "Data Type") was being ignored resulting in all
      classes always being restored regardless of user settings.

</pre>*/
