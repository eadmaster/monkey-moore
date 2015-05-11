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

#ifndef MONKEY_APP_HPP
#define MONKEY_APP_HPP

#include <wx/wxprec.h>

#ifdef __BORLANDC__
   #pragma hdrstop
#endif

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "monkey_prefs.hpp"

/**
* Starts Monkey-Moore's user interface execution.
*/
class MonkeyApp : public wxApp
{
public:
   /**
   * This method is called during program initialization. It chooses the
   * correct language based on the current operational system's language,
   * picking it from a catalog, when available. It also creates and shows
   * the main windows frame.
   * @return True if initializated with success, false otherwise.
   */
   virtual bool OnInit();
   virtual int OnExit();

   inline wxString getSequencesFileName() const
   { return m_seqsFile; }

private:
   bool LoadConfiguration ();

   wxLocale m_loc;          // defines our current locale
   wxString m_cfgFile;      // complete path to configuration file
   wxString m_seqsFile; // custom character sequences file path
   MonkeyPrefs m_prefs;     // application settings and preferences
};

DECLARE_APP(MonkeyApp)

#endif //~MONKEY_APP_HPP
