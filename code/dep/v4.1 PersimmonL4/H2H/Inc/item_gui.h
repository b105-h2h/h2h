/*
 * item_gui.h
 */

#ifndef ITEM_GUI_H_
#define ITEM_GUI_H_

typedef enum {GUI_OK, GUI_SIZE_ERROR}           gui_status_t;
typedef enum {GUI_ACTIVE, GUI_INACTIVE}         gui_active_t;
typedef enum {GUI_NO_BORDER, GUI_RECTANGLE}     gui_border_t;

typedef enum {LEFT, CENTER, RIGHT}              gui_h_align_t;
typedef enum {TOP, MID, BOTTOM}                 gui_v_align_t;

#endif /* ITEM_GUI_H_ */