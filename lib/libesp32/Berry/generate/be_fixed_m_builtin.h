#include "be_constobj.h"

static be_define_const_map_slots(m_builtin_map) {
    { be_const_key(number, 16), be_const_int(7) },
    { be_const_key(map, -1), be_const_int(19) },
    { be_const_key(classname, 22), be_const_int(5) },
    { be_const_key(bytes, 2), be_const_int(21) },
    { be_const_key(int, 12), be_const_int(9) },
    { be_const_key(module, -1), be_const_int(11) },
    { be_const_key(print, 20), be_const_int(1) },
    { be_const_key(issubclass, -1), be_const_int(14) },
    { be_const_key(assert, -1), be_const_int(0) },
    { be_const_key(list, -1), be_const_int(18) },
    { be_const_key(__iterator__, -1), be_const_int(16) },
    { be_const_key(real, -1), be_const_int(10) },
    { be_const_key(super, 21), be_const_int(3) },
    { be_const_key(isinstance, 8), be_const_int(15) },
    { be_const_key(classof, 4), be_const_int(6) },
    { be_const_key(input, -1), be_const_int(2) },
    { be_const_key(call, 19), be_const_int(22) },
    { be_const_key(compile, -1), be_const_int(13) },
    { be_const_key(open, -1), be_const_int(17) },
    { be_const_key(size, -1), be_const_int(12) },
    { be_const_key(range, -1), be_const_int(20) },
    { be_const_key(str, -1), be_const_int(8) },
    { be_const_key(type, -1), be_const_int(4) },
};

static be_define_const_map(
    m_builtin_map,
    23
);

static const bvalue __vlist_array[] = {
    be_const_func(l_assert),
    be_const_func(l_print),
    be_const_func(l_input),
    be_const_func(l_super),
    be_const_func(l_type),
    be_const_func(l_classname),
    be_const_func(l_classof),
    be_const_func(l_number),
    be_const_func(l_str),
    be_const_func(l_int),
    be_const_func(l_real),
    be_const_func(l_module),
    be_const_func(l_size),
    be_const_func(l_compile),
    be_const_func(l_issubclass),
    be_const_func(l_isinstance),
    be_const_func(l_iterator),
    be_const_func(be_nfunc_open),
    be_const_class(be_class_list),
    be_const_class(be_class_map),
    be_const_class(be_class_range),
    be_const_class(be_class_bytes),
    be_const_func(l_call),
};

static be_define_const_vector(
    m_builtin_vector,
    __vlist_array,
    23
);
