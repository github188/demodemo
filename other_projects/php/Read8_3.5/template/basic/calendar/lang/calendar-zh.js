// ** Translated by 白水山言
// ** I18N

// Calendar EN language
// Author: Mihai Bazon, <mishoo@infoiasi.ro>
// Encoding: any
// Distributed under the same terms as the calendar itself.

// For translators: please use UTF-8 if possible.  We strongly believe that
// Unicode is the answer to a real internationalized world.  Also please
// include your contact information in the header, as can be seen above.

// full day names
Calendar._DN = new Array
("星期日",
 "星期一",
 "星期二",
 "星期三",
 "星期四",
 "星期五",
 "星期六",
 "星期日");

// Please note that the following array of short day names (and the same goes
// for short month names, _SMN) isn't absolutely necessary.  We give it here
// for exemplification on how one can customize the short day names, but if
// they are simply the first N letters of the full name you can simply say:
//
//   Calendar._SDN_len = N; // short day name length
//   Calendar._SMN_len = N; // short month name length
//
// If N = 3 then this is not needed either since we assume a value of 3 if not
// present, to be compatible with translation files that were written before
// this feature.

// short day names
Calendar._SDN = new Array
("日",
 "一",
 "二",
 "三",
 "四",
 "五",
 "六",
 "日");

// full month names
Calendar._MN = new Array
("一月",
 "二月",
 "三月",
 "四月",
 "五月",
 "六月",
 "七月",
 "八月",
 "九月",
 "十月",
 "十一月",
 "十二月");

// short month names
Calendar._SMN = new Array
("一月",
 "二月",
 "三月",
 "四月",
 "五月",
 "六月",
 "七月",
 "八月",
 "九月",
 "十月",
 "十一月",
 "十二月");

// tooltips
Calendar._TT = {};
Calendar._TT["INFO"] = "关于本日历";

Calendar._TT["ABOUT"] =
"DHTML 日期/时间 选择器\n" +
"(c) dynarch.com 2002-2003\n" + // 这里不需翻译 ;-)
"请访问 http://dynarch.com/mishoo/calendar.epl 检查最新版本\n" +
"基于 GNU LGPL 发布  详见 http://gnu.org/licenses/lgpl.html" +
"\n\n" +
"日期选择:\n" +
"- 使用 \xab, \xbb 选择年份\n" +
"- 使用 " + String.fromCharCode(0x2039) + ", " + String.fromCharCode(0x203a) + " 选择月份\n" +
"- 在上述按钮上长按鼠标可快速选择";
Calendar._TT["ABOUT_TIME"] = "\n\n" +
"时间选择:\n" +
"- Click on any of the time parts to increase it\n" +
"- or Shift-click to decrease it\n" +
"- or click and drag for faster selection.";

Calendar._TT["PREV_YEAR"] = "去年 (长按显示菜单)";
Calendar._TT["PREV_MONTH"] = "上个月 (长按显示菜单)";
Calendar._TT["GO_TODAY"] = "今天";
Calendar._TT["NEXT_MONTH"] = "下个月 (长按显示菜单)";
Calendar._TT["NEXT_YEAR"] = "明年 (长按显示菜单)";
Calendar._TT["SEL_DATE"] = "选择日期";
Calendar._TT["DRAG_TO_MOVE"] = "拖拽可移动";
Calendar._TT["PART_TODAY"] = " (今天)";

// the following is to inform that "%s" is to be the first day of week
// %s will be replaced with the day name.
Calendar._TT["DAY_FIRST"] = "设 %s 为一周的开始";

// This may be locale-dependent.  It specifies the week-end days, as an array
// of comma-separated numbers.  The numbers are from 0 to 6: 0 means Sunday, 1
// means Monday, etc.
Calendar._TT["WEEKEND"] = "6,0";

Calendar._TT["CLOSE"] = "关闭";
Calendar._TT["TODAY"] = "今天";
Calendar._TT["TIME_PART"] = "(Shift-)Click or drag to change value";

// date formats
Calendar._TT["DEF_DATE_FORMAT"] = "%Y-%m-%d";
Calendar._TT["TT_DATE_FORMAT"] = "%a, %b %e";

Calendar._TT["WK"] = "周";
Calendar._TT["TIME"] = "时间:";
