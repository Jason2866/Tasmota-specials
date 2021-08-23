#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_objmask_map) {
    { be_const_key(init, 2), be_const_func(lvbe_objmask_create) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(dot_p, -1), be_const_var(0) },
    { be_const_key(member, 0), be_const_func(lvx_member) },
};

static be_define_const_map(
    be_class_lv_objmask_map,
    4
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_objmask,
    1,
    (bclass *)&be_class_lv_obj,
    lv_objmask
);
