/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_string("tasmota", 424643812, 7),
    /* K1   */  be_nested_string("wifi", 120087624, 4),
    /* K2   */  be_nested_string("find", -1108310694, 4),
    /* K3   */  be_nested_string("quality", -1697296346, 7),
    /* K4   */  be_nested_string("ip", 1261996636, 2),
    /* K5   */  be_nested_string("set_percentage", -1342944572, 14),
    /* K6   */  be_const_int(0),
    }),
    (be_nested_const_str("every_second", 2075451465, 12)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[23]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x58100003,  //  0004  LDCONST	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x8C0C0302,  //  0006  GETMET	R3	R1	K2
      0x58140004,  //  0007  LDCONST	R5	K4
      0x7C0C0400,  //  0008  CALL	R3	2
      0x4C100000,  //  0009  LDNIL	R4
      0x1C100604,  //  000A  EQ	R4	R3	R4
      0x78120003,  //  000B  JMPF	R4	#0010
      0x8C100105,  //  000C  GETMET	R4	R0	K5
      0x58180006,  //  000D  LDCONST	R6	K6
      0x7C100400,  //  000E  CALL	R4	2
      0x70020005,  //  000F  JMP		#0016
      0x4C100000,  //  0010  LDNIL	R4
      0x20100404,  //  0011  NE	R4	R2	R4
      0x78120002,  //  0012  JMPF	R4	#0016
      0x8C100105,  //  0013  GETMET	R4	R0	K5
      0x5C180400,  //  0014  MOVE	R6	R2
      0x7C100400,  //  0015  CALL	R4	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_string("init", 380752755, 4),
    /* K1   */  be_nested_string("tasmota", 424643812, 7),
    /* K2   */  be_nested_string("add_driver", 1654458371, 10),
    /* K3   */  be_nested_string("set_percentage", -1342944572, 14),
    /* K4   */  be_const_int(0),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[15]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0xB80E0200,  //  0007  GETNGBL	R3	K1
      0x8C0C0702,  //  0008  GETMET	R3	R3	K2
      0x5C140000,  //  0009  MOVE	R5	R0
      0x7C0C0400,  //  000A  CALL	R3	2
      0x8C0C0103,  //  000B  GETMET	R3	R0	K3
      0x58140004,  //  000C  LDCONST	R5	K4
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: del
********************************************************************/
be_local_closure(del,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("del", -816214454, 3),
    /* K1   */  be_nested_string("tasmota", 424643812, 7),
    /* K2   */  be_nested_string("remove_driver", 1030243768, 13),
    }),
    (be_nested_const_str("del", -816214454, 3)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[10]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0xB8060200,  //  0005  GETNGBL	R1	K1
      0x8C040302,  //  0006  GETMET	R1	R1	K2
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x7C040400,  //  0008  CALL	R1	2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_wifi_bars
********************************************************************/
extern const bclass be_class_lv_signal_bars;
be_local_class(lv_wifi_bars,
    0,
    &be_class_lv_signal_bars,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("every_second", 2075451465, 12, 1), be_const_closure(every_second_closure) },
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(init_closure) },
        { be_nested_key("del", -816214454, 3, -1), be_const_closure(del_closure) },
    })),
    (be_nested_const_str("lv_wifi_bars", 2109539196, 12))
);
/*******************************************************************/

void be_load_lv_wifi_bars_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_wifi_bars);
    be_setglobal(vm, "lv_wifi_bars");
    be_pop(vm, 1);
}

#endif // USE_LVGL