#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_NODE_ID = 0,
    FLOW_GLOBAL_VARIABLE_INFOR = 1
};

// Native global variables

extern const char *get_var_node_id();
extern void set_var_node_id(const char *value);
extern const char *get_var_infor();
extern void set_var_infor(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/