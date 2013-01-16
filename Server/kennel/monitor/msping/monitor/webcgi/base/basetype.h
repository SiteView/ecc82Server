#ifndef _SV_TREE_VIEW_NODE_TYPE_H
#define _SV_TREE_VIEW_NODE_TYPE_H
enum TreeNodeType { Tree_SEGROUP, Tree_SE,Tree_GROUP,Tree_DEVICE ,Tree_MONITOR, Tree_NOCHILD };

enum dyn_monitor_state
{
    dyn_no_data = 0x00,
    dyn_normal = 0x01,
    dyn_warnning = 0x02,
    dyn_error = 0x03,
    dyn_disable = 0x04,
    dyn_bad = 0x05
};
const int AlAWYSMONITOR = 0x03;
#endif
