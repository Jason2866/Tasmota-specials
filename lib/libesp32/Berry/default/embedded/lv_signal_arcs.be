#- LVGL lv_signal_bars and lv_wifi_bars
 -
--#

class lv_signal_arcs : lv_obj
  var ancestor_design     # previous design_cb
  var percentage          # value to display, range 0..100
  var p1, p2, area, line_dsc    # instances of objects kept to avoid re-instanciating at each call

  def init(parent, copy)
    # init parent object
    super(self).init(parent, copy)
    # keep a copy of 
    self.ancestor_design = self.get_design_cb()
    self.set_design_cb(self.my_design_cb)
    # own values
    self.percentage = 100
    # pre-allocate buffers
    self.p1 = lv_point()
    self.p2 = lv_point()
    self.area = lv_area()
    self.line_dsc = lv_draw_line_dsc()
  end

  def my_design_cb(clip_area, mode)
    import math
    def atleast1(x) if x >= 1 return x else return 1 end end
    # the model is that we have 4 bars and inter-bar (1/4 of width)
    var height = self.get_height()
    var width = self.get_width()

    var inter_bar = atleast1(height / 8)
    var bar = atleast1((height - inter_bar * 2) / 3)
    var bar_offset = bar / 2
    #print("inter_bar", inter_bar, "bar", bar, "bar_offset", bar_offset)
    if mode == lv.DESIGN_COVER_CHK
      #- Return false if the object is not covers the clip_area clip_area -#
      return self.ancestor_design.call(self, clip_area, mode)

    elif mode == lv.DESIGN_DRAW_MAIN
      #self.ancestor_design.call(self, clip_area, mode)  # commented since we don't draw a background
    
      # get coordinates of object
      self.get_coords(self.area)
      var x_ofs = self.area.x1
      var y_ofs = self.area.y1
      lv.draw_line_dsc_init(self.line_dsc)                        # initialize lv_draw_line_dsc structure
      self.init_draw_line_dsc(lv.OBJ_PART_MAIN, self.line_dsc)    # copy the current values

      self.line_dsc.round_start = 1
      self.line_dsc.round_end = 1
      self.line_dsc.width = (bar * 3 + 1) / 4
      var on_color = self.get_style_line_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT)
      var off_color = self.get_style_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT)

      # initial calculation, but does not take into account bounding box
      # var angle = int(math.deg(math.atan2(width / 2, height)))

      # better calculation
      var hypotenuse = height - bar           # center if at bar/2 from bottom and circle stops at bar/2 from top
      var adjacent = width / 2 - bar_offset   # stop at bar_offset from side
      var angle = int(90 - math.deg(math.acos(real(adjacent) / real(hypotenuse))))
      if (angle > 45) angle = 45 end

      # print("hypotenuse",hypotenuse,"adjacent",adjacent,"angle",angle)
      self.p1.x = x_ofs + width / 2
      self.p1.y = y_ofs + height - 1 - bar_offset

      self.line_dsc.color = self.percentage >= 25 ? on_color : off_color
      lv.draw_arc(self.p1.x, self.p1.y, 0 * (bar + inter_bar) + bar_offset, 0, 360, clip_area, self.line_dsc)
      self.line_dsc.color = self.percentage >= 50 ? on_color : off_color
      lv.draw_arc(self.p1.x, self.p1.y, 1 * (bar + inter_bar) + bar_offset - 1, 270 - angle, 270 + angle, clip_area, self.line_dsc)
      self.line_dsc.color = self.percentage >= 75 ? on_color : off_color
      lv.draw_arc(self.p1.x, self.p1.y, 2 * (bar + inter_bar) + bar_offset - 2, 270 - angle, 270 + angle, clip_area, self.line_dsc)

    #elif mode == lv.DESIGN_DRAW_POST    # commented since we don't want a frame around this object
      # self.ancestor_design.call(self, clip_area, mode)
    end
    return lv.DESIGN_RES_OK
  end

  def set_percentage(v)
    var old_bars = self.percentage / 4
    if v > 100 v = 100 end
    if v < 0   v = 0 end
    self.percentage = v
    if old_bars != v / 4
      self.invalidate()    # be frugal and avoid updating the widget if it's not needed
    end
  end

  def get_percentage()
    return self.percentage
  end
end

class lv_wifi_arcs: lv_signal_arcs
  def init(parent, copy)
    super(self).init(parent, copy)
    tasmota.add_driver(self)
    self.set_percentage(0)    # we generally start with 0, meaning not connected
  end

  def every_second()
    var wifi = tasmota.wifi()
    var quality = wifi.find("quality")
    var ip = wifi.find("ip")
    if ip == nil
      self.set_percentage(0)
    elif quality != nil
      self.set_percentage(quality)
    end
  end

  def del()
    super(self).del()
    tasmota.remove_driver(self)
  end
end

class lv_wifi_arcs_icon: lv_wifi_arcs
  def init(parent, copy)
    super(self).init(parent, copy)
    self.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(lv.COLOR_BLACK))
    if parent != nil
      var parent_height = parent.get_height()
      var pad_right = parent.get_style_pad_right(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT)
      self.set_height(parent_height)
      var w = (parent_height*4)/3
      self.set_width(w)	# 130%
      self.set_x(parent.get_width() - w - pad_right)
      parent.set_style_local_pad_right(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, pad_right + w + 1)
    end
  end
end