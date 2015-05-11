/*
 * Monkey-Moore - A simple and powerful relative search tool
 * Copyright (C) 2007 Ricardo J. Ricken (Darkl0rd)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef MONKEY_FRAME_HPP
#define MONKEY_FRAME_HPP

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "constants.hpp"
#include "byteswap.hpp"
#include "monkey_moore.hpp"
#include "monkey_prefs.hpp"

#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <vector>
#include <utility>
#include <mutex>

// typedefs to prevent lenghty code
typedef std::tuple<wxFileOffset, MonkeyMoore<uint8_t>::equivalency_type, wxString> result_type8;
typedef std::tuple<wxFileOffset, MonkeyMoore<uint16_t>::equivalency_type, wxString> result_type16;

struct SearchParameters;

/**
* Implements the main window frame. Creates and maintains the user interface,
* interacting and responding to the user's actions.
*/
class MonkeyFrame : public wxFrame
{
public:
   MonkeyFrame (const wxString &title, MonkeyPrefs &mprefs, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
   virtual ~MonkeyFrame ();

   void OnBrowse (wxCommandEvent &event);
   void OnSearchType (wxCommandEvent &event);
   void OnSearchMode (wxCommandEvent &event);
   void OnSearch (wxCommandEvent &event);
   void OnAdvanced (wxCommandEvent &event);
   void OnCharsetList (wxCommandEvent &event);
   void OnCharsetChosen (wxCommandEvent &event);
   void OnByteOrder (wxCommandEvent &event);
   template <typename _DataType> void OnAllResults (wxCommandEvent &event);
   void OnResizeResultsListColumn(wxListEvent &event);
   void OnListItemActivated (wxListEvent &event);
   void OnListItemRightClick (wxListEvent &event);
   void OnListDeleteAll (wxListEvent &event);
   void OnCopyAddress (wxCommandEvent &event);
   void OnManageCharSeqs (wxCommandEvent &event);
   template <typename _DataType> void OnCreateTbl (wxCommandEvent &event);
   template <typename _DataType> void OnClear (wxCommandEvent &event);
   void OnOptions (wxCommandEvent &event);
   void OnAbout (wxCommandEvent &event);
   void OnCancel (wxCommandEvent &event);
   void OnTextEnter (wxCommandEvent &event);
   void OnClose (wxCloseEvent &event);
   void OnUpdateUI (wxUpdateUIEvent &event);
   void OnSize (wxSizeEvent &event);
   void OnShow (wxShowEvent &event);

   template <typename _DataType> void OnThreadUpdate (wxThreadEvent &event);
   template <typename _DataType> void OnThreadCompleted (wxThreadEvent &event);
   template <typename _DataType> void OnThreadAborted (wxThreadEvent &event);

   /**
   * Sets the progress bar percentage.
   * @param percentage a value in the 0-100 range
   */
   inline void SetCurrentProgress (const int percentage) const {
      GetWindow<wxGauge>(MonkeyMoore_Progress)->SetValue(percentage);
   }

   /**
   * Finds out whether the search was aborted or not.
   * @return If true, it was aborted.
   */
   inline bool IsSearchAborted () {
      std::lock_guard<std::mutex> lock(abortMutex);
      return search_was_aborted;
   }

   /**
   * Shows a simple message box.
   * @param message content
   * @param caption optional caption
   */
   inline void ShowWarning (const wxString &message, const wxString &caption = MM_ERRORCAPTION) {
      wxMessageBox(message, caption, wxOK | wxICON_INFORMATION, this);
   }

private:
   void ShowProgressBar (const bool show = true);
   bool CheckKeyword (const wxString &kw, const wxChar wc, const wxString &cp);
   void AdjustResultColumns (bool sizeToContents = false);
   
   template <typename _DataType>
      bool StartSearchThread (SearchParameters &p);

   template <typename _DataType>
      void ShowResults (bool showAll = true);

   /**
   * Get a pointer to the widget with the specified ID.
   * @tparam _Type Type of the widget (must be a wxWindow or a child from it)
   * @param[in] id Widget ID
   * @return A statically-casted pointer to the specified widget.
   */
   template <typename _Type>
      inline _Type *GetWindow (long id) const { return static_cast<_Type *>(FindWindowById(id)); }

   /**
   * Calls the GetValue member function on a widget.
   * @tparam _RetType The type returned by the GetValue call
   * @tparam _CtrlType The type of the widget
   * @param[in] id Widget ID
   * @return The value returned by a call of GetValue on the specified widget.
   */
   template <typename _RetType, typename _CtrlType>
      inline _RetType GetValue (long id) const { return static_cast<_CtrlType *>(FindWindowById(id))->GetValue(); }

   /**
   * Checks if the wxCheckBox specified is checked or not.
   * @param id The ID of the wxCheckBox
   * @return True if it is checked, false otherwise.
   */
   inline bool IsChecked (long id) const { return static_cast<wxCheckBox *>(FindWindowById(id))->IsChecked(); }

   /**
   * Get a reference to a vector containing the last search results.
   * It returns the correct type/data depending on the type specified.
   * @tparam _Datatype (must be either u8 or u16)
   * @return A vector containing the last results of _Type
   */
   template <typename _DataType>
      std::vector<std::tuple<wxFileOffset,
         typename MonkeyMoore<_DataType>::equivalency_type, wxString>> &lastResults();

   int progressBoxHeight;                     /**< Height of the progress box in pixels */

   bool searchmode_8bits;                     /**< 8-bit search mode is selected?       */
   bool byteorder_little;                     /**< Is byte order set to little endian?  */
   bool advanced_shown;                       /**< Is the advanced box shown?           */
   bool search_done;                          /**< Is the search done?                  */
   bool search_in_progress;                   /**< Is the search in progress?           */
   bool search_was_aborted;                   /**< Was the search aborted?              */

   wxImageList images;                        /**< Images used in the UI                */
   std::mutex abortMutex;                     /**< Thread synchronization object        */
   wxStopWatch chronometer;                   /**< Times the search                     */
   MonkeyPrefs &prefs;                        /**< Settings and preferences             */

   std::vector<result_type8> last_results8;   /**< Results from the last 8-bit search   */
   std::vector<result_type16> last_results16; /**< Results from the last 16-bit search  */

   DECLARE_EVENT_TABLE();
};

#endif //~MONKEY_FRAME_HPP
