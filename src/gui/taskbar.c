/*
 *  UltraDefrag - a powerful defragmentation tool for Windows NT.
 *  Copyright (c) 2007-2012 Dmitri Arkhangelski (dmitriar@gmail.com).
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/**
 * @file taskbar.c
 * @brief Taskbar icon overlays.
 * @addtogroup Taskbar
 * @{
 */

#include "main.h"

HANDLE hTaskbarIconEvent = NULL;

/**
 * @brief Creates objects needed
 * for synchronization of taskbar
 * icon manipulations.
 */
void CreateTaskbarIconSynchObjects(void)
{
    hTaskbarIconEvent = CreateEvent(NULL,FALSE,TRUE,NULL);
    if(hTaskbarIconEvent == NULL){
        WgxDbgPrintLastError("CreateTaskbarIconSynchObjects: event creation failed");
        WgxDbgPrint("no taskbar icon overlays will be shown");
    }
}

/**
 * @brief Destroys objects needed
 * for synchronization of taskbar
 * icon manipulations.
 */
void DestroyTaskbarIconSynchObjects(void)
{
    if(hTaskbarIconEvent)
        CloseHandle(hTaskbarIconEvent);
}

/**
 * @brief Sets an overlay icon for
 * the application's taskbar icon.
 * @param[in] resource_id identifier
 * of the overlay icon resource.
 * Identifier equal to -1 causes icon removal.
 * @param[in] description_key key of
 * the description for overlay icon
 * to be retrieved from the i18n_table.
 */
void SetTaskbarIconOverlay(int resource_id, wchar_t *description_key)
{
    if(WaitForSingleObject(hLangPackEvent,INFINITE) != WAIT_OBJECT_0){
        WgxDbgPrintLastError("SetTaskbarIconOverlay: wait on hLangPackEvent failed");
    } else {
        (void)WgxSetTaskbarIconOverlay(hWindow,hInstance,resource_id,
            WgxGetResourceString(i18n_table,description_key));
        SetEvent(hLangPackEvent);
    }
}

/**
 * @brief Removes an overlay icon from
 * the application's taskbar icon.
 */
void RemoveTaskbarIconOverlay(void)
{
    (void)WgxRemoveTaskbarIconOverlay(hWindow);
}

/** @} */