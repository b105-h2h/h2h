#include "menu.h"

int32_t menu_search_click(menu_t *menu, click_t *click, item_t *item)
{
  int i;
  uint32_t item_x_pos, item_y_pos;
  uint32_t item_width, item_height;

  /* Search for clicked item */
  for (i = 0; i < menu->item_num; i++)
  {
    item_x_pos = menu->items[i].item.area.pos.x_pos;
    item_y_pos = menu->items[i].item.area.pos.y_pos;

    item_width = menu->items[i].item.area.width;
    item_height = menu->items[i].item.area.height;

    /* If an item is found */
    if (((item_x_pos <= click->pos.x_pos) &&
                       (click->pos.x_pos <= item_x_pos + item_width)) &&
        ((item_y_pos <= click->pos.y_pos) &&
                       (click->pos.y_pos <= item_y_pos + item_height)))
    {
      /* If that item is active and triggers an action */
      if ((menu->items[i].item.area.is_active == GUI_ACTIVE) &&
          (menu->items[i].item.area.event != fsm_no_event))
      {
        /* Get item and return true */
        *item = menu->items[i].item;
        return 1;
      }
    }
  }

  /* If no item has been found */
  return 0;
}