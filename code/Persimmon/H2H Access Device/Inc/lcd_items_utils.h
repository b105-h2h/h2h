/*
 * items_utils.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef ITEMS_UTILS_H_
#define ITEMS_UTILS_H_

typedef enum {GUI_OK, GUI_SIZE_ERROR} gui_status_t;

typedef enum {ACTIVE, INACTIVE}                 gui_active_t;
typedef enum {GUI_CLICKED, GUI_NOT_CLICKED}     gui_clicked_t;
typedef enum {NO_BORDER, RECTANGLE}             gui_border_t;

typedef enum {LEFT, CENTER, RIGHT}    h_align_t;
typedef enum {TOP, MID, BOTTOM}       v_align_t;

#endif /* ITEMS_UTILS_H_ */