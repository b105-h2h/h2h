#include "touch_calib.h"

void touch_calib_3_point_calib(touch_calib_t *calib,
                               lcd_pos_t *l1, lcd_pos_t *l2, lcd_pos_t *l3,
                               touch_pos_t *t1, touch_pos_t *t2, touch_pos_t *t3)
{
  int32_t det;
  int32_t det_x1, det_x2, det_x3;
  int32_t det_y1, det_y2, det_y3;
  
  int32_t l_x1, l_y1,  l_x2, l_y2,  l_x3, l_y3;
  int32_t t_x1, t_y1,  t_x2, t_y2,  t_x3, t_y3;
  
  /* Get lcd values */
  l_x1 = (int32_t) l1->x_pos;
  l_y1 = (int32_t) l1->y_pos;
  l_x2 = (int32_t) l2->x_pos;
  l_y2 = (int32_t) l2->y_pos;
  l_x3 = (int32_t) l3->x_pos;
  l_y3 = (int32_t) l3->y_pos;
  
  /* Get touch values */
  t_x1 = (int32_t) t1->x_pos;
  t_y1 = (int32_t) t1->y_pos;
  t_x2 = (int32_t) t2->x_pos;
  t_y2 = (int32_t) t2->y_pos;
  t_x3 = (int32_t) t3->x_pos;
  t_y3 = (int32_t) t3->y_pos;
  
  det    = (t_x1 - t_x3) * (t_y2 - t_y3) - (t_x2 - t_x3) * (t_y1 - t_y3);
  
  det_x1 = (l_x1 - l_x3) * (t_y2 - t_y3) - (l_x2 - l_x3) * (t_y1 - t_y3);
  det_x2 = (t_x1 - t_x3) * (l_x2 - l_x3) - (t_x2 - t_x3) * (l_x1 - l_x3);
  det_x3 = l_x1 * (t_x2 * t_y3 - t_x3 * t_y2) -
           l_x2 * (t_x1 * t_y3 - t_x3 * t_y1) +
           l_x3 * (t_x1 * t_y2 - t_x2 * t_y1);

  det_y1 = (l_y1 - l_y3) * (t_y2 - t_y3) - (l_y2 - l_y3) * (t_y1 - t_y3);
  det_y2 = (t_x1 - t_x3) * (l_y2 - l_y3) - (t_x2 - t_x3) * (l_y1 - l_y3);
  det_y3 = l_y1 * (t_x2 * t_y3 - t_x3 * t_y2) -
           l_y2 * (t_x1 * t_y3 - t_x3 * t_y1) +
           l_y3 * (t_x1 * t_y2 - t_x2 * t_y1);
  
  /* Calculate calib values
   *
   * Q20.12 fixed point format
   */
  calib->alpha_x = (det_x1 << 12) / det;
  calib->beta_x  = (det_x2 << 12) / det;
  calib->delta_x = (det_x3 << 12) / det;

  calib->alpha_y = (det_y1 << 12) / det;
  calib->beta_y  = (det_y2 << 12) / det;
  calib->delta_y = (det_y3 << 12) / det;
}

lcd_pos_t touch_calib_get_lcd_pos(touch_calib_t *calib,
                                  touch_pos_t *touch_pos)
{
  lcd_pos_t lcd_pos;
  
  /* Calculate lcd position from touchpad position
   *
   * A 12 bit shift is needed because calibration
   * values are stored in Q20.12 fixed point format
   */
  lcd_pos.x_pos = (calib->alpha_x * touch_pos->x_pos +
                   calib->beta_x  * touch_pos->y_pos +
                   calib->delta_x) >> 12;
  lcd_pos.y_pos = (calib->alpha_y * touch_pos->x_pos +
                   calib->beta_y  * touch_pos->y_pos +
                   calib->delta_y) >> 12;
  
  return lcd_pos;
}