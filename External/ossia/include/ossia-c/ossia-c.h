#ifndef OSSIA_H
#define OSSIA_H
#include <stddef.h>

/** @defgroup CAPI C API
 * C bindings of libossia. Covers networking.
 *
 * @{
 */
/**
 * @file ossia-c.h
 */
/**
 * ### API core concepts and ideas:
 *
 * ### Multithreading notes:
 *
 * Most functions are safe when called with different devices.
 * When working on the same device, however, one has to take care to lock.
 * e.g. the following cases are unsafe:
 *
 * |          Thread 1                |               Thread 2              |
 * |----------------------------------|-------------------------------------|
 * | `ossia_node_find(my_dev, "/foo");` | `ossia_node_create(my_dev, "/bar");`  |
 * |----------------------------------|-------------------------------------|
 * | `ossia_device_free(my_dev);`       | `ossia_device_get_root_node(my_dev);` |
 * |----------------------------------|-------------------------------------|
 *
 * While the following cases are safe:
 *
 * |          Thread 1                    |               Thread 2              |
 * |--------------------------------------|-------------------------------------|
 * | `ossia_node_find(my_dev, "/foo");`   | `ossia_node_create(other, "/bar");` |
 * |--------------------------------------|-------------------------------------|
 * | `ossia_parameter_push_i(addr, 123);` | `ossia_parameter_get_value(addr);`  |
 * |--------------------------------------|-------------------------------------|
 *
 * * Unsafe: Cannot be safely called from multiple threads.
 * * Safe: Can be safely called from multiple threads, may block.
 * * MT-Safe: Can be safely called from multiple threads, won't block.
 * * Data-safe: Can be safely called from multiple threads with the same device.
 *              For instance, `ossia_parameter_set_value` and `ossia_parameter_get_value`.
 *
 * Only functions marked as data-safe can be called in parallel on the same device,
 * and no function with weaker guarantees shall be called on the device at the same time
 * in another thread.
 */


#if defined(_MSC_VER)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef BOOL
#undef INT
#undef FLOAT
#undef CHAR
#endif

#include <ossia_export.h>
#if defined(__cplusplus)
extern "C"
{
#endif
struct ossia_protocol;
struct ossia_device;
struct ossia_domain;
struct ossia_value;
struct ossia_logger;

typedef struct ossia_protocol* ossia_protocol_t;
typedef struct ossia_device* ossia_device_t;
typedef struct ossia_domain* ossia_domain_t;
typedef struct ossia_value* ossia_value_t;
typedef struct ossia_logger* ossia_logger_t;
typedef void* ossia_node_t;
typedef void* ossia_parameter_t;

typedef void (*ossia_node_callback_t)(void* ctx, ossia_node_t);
struct ossia_node_callback_index;
typedef struct ossia_node_callback_index* ossia_node_callback_idx_t;

typedef void (*ossia_parameter_callback_t)(void* ctx, ossia_parameter_t);
struct ossia_parameter_callback_index;
typedef struct ossia_parameter_callback_index* ossia_parameter_callback_idx_t;

typedef void (*ossia_value_callback_t)(void* ctx, ossia_value_t);
struct ossia_value_callback_index;
typedef struct ossia_value_callback_index* ossia_value_callback_idx_t;

struct ossia_vec2f
{ float val[2]; };
struct ossia_vec3f
{ float val[3]; };
struct ossia_vec4f
{ float val[4]; };

/****************/
/*** Protocol ***/
/****************/

/** @defgroup CProto Protocol
 * @brief Instantiation of various protocols such as OSC, Minuit, OSCQuery...
 * @see ossia::net::protocol_base
 *  @{
 */

/**
 * @brief Instantiate a multiplexer protocol
 * @return An instance of a multiplexing protocol
 *
 * @see ossia::net::multiplex_protocol
 * @note Multithread guarantees: MT-safe
 */
OSSIA_EXPORT
ossia_protocol_t ossia_protocol_multiplex_create();

/**
 * @brief Adds a protocol to the set of protocols managed by the multiplexer
 *
 * @see ossia::net::multiplex_protocol
 * @note Multithread guarantees: MT-safe
 */
OSSIA_EXPORT
void ossia_protocol_multiplex_expose_to(
    ossia_protocol_t local,
    ossia_protocol_t other);

/**
 * @brief Instantiate a raw OSC protocol
 * @param ip Where to connect
 * @param remote_port Port on which this software will send messages to.
 * @param local_port Port on which the remote software will send messages
 *
 * @see ossia::net::osc_protocol
 * @note Multithread guarantees: MT-safe
 */
OSSIA_EXPORT
ossia_protocol_t ossia_protocol_osc_create(
    const char* ip,
    int remote_port,
    int local_port);

/**
 * @brief Instantiate a Minuit protocol
 * @param local_name Under which name the local software appears
 * @param ip Where to connect
 * @param remote_port Port on which this object will send messages to.
 * @param local_port Port on which the remote software will send messages
 *
 * @see ossia::net::minuit_protocol
 * @note Multithread guarantees: MT-safe
 */
OSSIA_EXPORT
ossia_protocol_t ossia_protocol_minuit_create(
    const char* local_name,
    const char* ip,
    int remote_port,
    int local_port);

/**
 * @brief Instantiate an OSCQuery server.
 * @param osc_port Local OSC port
 * @param ws_port Local websocket port
 *
 * @see ossia::oscquery::oscquery_server
 * @note Multithread guarantees: MT-safe
 */
OSSIA_EXPORT
ossia_protocol_t ossia_protocol_oscquery_server_create(
    int osc_port,
    int ws_port);

/**
 * @brief Instantiate an OSCQuery client.
 * @param host something like ws://127.0.0.1:5678
 *
 * @see ossia::oscquery::oscquery_mirror
 * @note Multithread guarantees: MT-safe
 */
OSSIA_EXPORT
ossia_protocol_t ossia_protocol_oscquery_mirror_create(
    const char* host);

/**
 * @brief Free the memory associated to a protocol.
 * @note Multithread guarantees: MT-safe
 */
OSSIA_EXPORT
void ossia_protocol_free(
    ossia_protocol_t);

/** @}*/

/**************/
/*** Device ***/
/**************/

/** @defgroup CDev Device
 * @brief Device tree management.
 * @see ossia::net::device_base
 *  @{
 */

/**
 * @brief Create a device.
 *
 * ossia_device_create takes ownership
 * of the protocol ; it must not be used afterwards.
 *
 * @see ossia::net::device_base
 * @note Multithread guarantees: Safe
 */
OSSIA_EXPORT
ossia_device_t ossia_device_create(
    ossia_protocol_t protocol,
    const char* name);

/**
 * @brief Free a device instance
 * @note Multithread guarantees: Safe
 */
OSSIA_EXPORT
void ossia_device_free(
    ossia_device_t device);

/**
 * @brief Free the global device table.
 *
 * The devices are automatically registered in a table.
 * This function frees and removes all the registered devices.
 *
 * @note Multithread guarantees: Safe
 */
OSSIA_EXPORT
void ossia_device_reset_static();

/**
 * @brief Request the nodes of a server if the protocol handles it.
 * @param device The device which must be updated.
 *
 * @see ossia::net::protocol_base::update
 * @note Multithread guarantees: MT-Safe
 */
OSSIA_EXPORT
int ossia_device_update_namespace(
    ossia_device_t device);

/**
 * @brief Get the root node of a device
 *
 * @note Never null.
 * @return The node corresponding to the '/' address.
 * @see ossia::net::device_base::get_root_node
 *
 * @note Multithread guarantees: Data-Safe
 */
OSSIA_EXPORT
ossia_node_t ossia_device_get_root_node(
    ossia_device_t device);

/**
 * @brief Get the name of a device.
 *
 * @note The caller has the ownership of the returned string.
 * It should be freed with free()
 *
 * @note Multithread guarantees: MT-Safe
 */
OSSIA_EXPORT
const char* ossia_device_get_name(
    ossia_device_t device);

/**
 * @brief Add a callback called when a node is created in a device.
 * @param device Device on which the callback must be added.
 * @param callback Function to be called.
 * @param ctx Will be passed to the callback.
 * @return An identifier to be able to remove the callback on a later date.
 *
 * @see ossia::net::device_base::on_node_created
 * @note Multithread guarantees: MT-Safe.
 *       The callback is called from the thread where the modification occured.
 */
OSSIA_EXPORT
ossia_node_callback_idx_t ossia_device_add_node_created_callback(
    ossia_device_t device,
    ossia_node_callback_t callback,
    void* ctx);

/**
 * @brief Remove a callback added with ossia_device_add_node_created_callback
 * @param device Device on which the callback must be removed.
 * @param index Index of the callback to remove.
 *
 * @see ossia::net::device_base::on_node_created
 * @note Multithread guarantees: MT-Safe.
 * @note The callback index must not be used afterwards
 */
OSSIA_EXPORT
void ossia_device_remove_node_created_callback(
    ossia_device_t device,
    ossia_node_callback_idx_t index);

/**
 * @brief Add a callback called when a node is removed in a device.
 * @param device Device on which the callback must be added.
 * @param callback Function to be called.
 * @param ctx Will be passed to the callback.
 * @return An identifier to be able to remove the callback on a later date.
 *
 * @see ossia::net::device_base::on_node_removing
 * @note Multithread guarantees: MT-Safe.
 *       The callback is called from the thread where the modification occured.
 */
OSSIA_EXPORT
ossia_node_callback_idx_t ossia_device_add_node_removing_callback(
    ossia_device_t device,
    ossia_node_callback_t callback,
    void* ctx);
/**
 * @brief Remove a callback added with ossia_device_add_node_removing_callback
 * @param device Device on which the callback must be removed.
 * @param index Index of the callback to remove.
 *
 * @see ossia::net::device_base::on_node_created
 * @note Multithread guarantees: MT-Safe.
 * @note The callback index must not be used afterwards
 */
OSSIA_EXPORT
void ossia_device_remove_node_removing_callback(
    ossia_device_t device,
    ossia_node_callback_idx_t index);

/**
 * @brief Add a callback called when a parameter is removed in a device.
 * @param device Device on which the callback must be added.
 * @param callback Function to be called.
 * @param ctx Will be passed to the callback.
 * @return An identifier to be able to remove the callback on a later date.
 *
 * @see ossia::net::device_base::on_parameter_removing
 * @note Multithread guarantees: MT-Safe.
 *       The callback is called from the thread where the modification occured.
 */
OSSIA_EXPORT
ossia_parameter_callback_idx_t ossia_device_add_parameter_deleting_callback(
    ossia_device_t device,
    ossia_parameter_callback_t callback,
    void* ctx);
/**
 * @brief Remove a callback added with ossia_device_add_parameter_deleting_callback
 * @param device Device on which the callback must be removed.
 * @param index Index of the callback to remove.
 *
 * @see ossia::net::device_base::on_parameter_removing
 * @note Multithread guarantees: MT-Safe.
 * @note The callback index must not be used afterwards
 */
OSSIA_EXPORT
void ossia_device_remove_parameter_deleting_callback(
    ossia_device_t device,
    ossia_parameter_callback_idx_t index);

/** @}*/


/*****************/
/*** Parameter ***/
/*****************/

/** @defgroup CParam Parameter
 * @brief Parameter creation, and attributes.
 * @see ossia::net::parameter_base
 *  @{
 */

/**
 * @see ossia::val_type
 */
typedef enum
{
  FLOAT_T,
  INT_T,
  VEC2F_T,
  VEC3F_T,
  VEC4F_T,
  IMPULSE_T,
  BOOL_T,
  STRING_T,
  LIST_T,
  CHAR_T
} ossia_type;

/**
 * @see ossia::access_mode
 */
typedef enum
{
  BI,
  GET,
  SET
} ossia_access_mode;

/**
 * @see ossia::bounding_mode
 */
typedef enum
{
  FREE,
  CLIP,
  WRAP,
  FOLD,
  LOW,
  HIGH
} ossia_bounding_mode;

/**
 * @brief Return the node a parameter is part of.
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_node_t ossia_parameter_get_node(
    ossia_parameter_t param);

/**
 * @see ossia::net::set_access_mode
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_set_access_mode(
    ossia_parameter_t param,
    ossia_access_mode am);
/**
 * @see ossia::net::get_access_mode
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_access_mode ossia_parameter_get_access_mode(
    ossia_parameter_t param);

/**
 * @see ossia::net::set_bounding_mode
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_set_bounding_mode(
    ossia_parameter_t param,
    ossia_bounding_mode bm);
/**
 * @see ossia::net::get_bounding_mode
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_bounding_mode ossia_parameter_get_bounding_mode(
    ossia_parameter_t param);

/**
 * @see ossia::net::set_domain
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_set_domain(
    ossia_parameter_t param,
    ossia_domain_t domain);
/**
 * @see ossia::net::get_domain
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_domain_t ossia_parameter_get_domain(
    ossia_parameter_t param);

/**
 * @see ossia::net::set_unit
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_set_unit(
    ossia_parameter_t param,
    const char* unit);
/**
 * @see ossia::net::get_unit
 * @note Multithread guarantees: Data-Safe.
 * @note No need to free
 */
OSSIA_EXPORT
const char* ossia_parameter_get_unit(
    ossia_parameter_t param);

/**
 * @see ossia::net::set_muted
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_set_muted(
    ossia_parameter_t param,
    int muted);
/**
 * @see ossia::net::get_muted
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
int ossia_parameter_get_muted(
    ossia_parameter_t param);

/**
 * @see ossia::net::set_disabled
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_set_disabled(
    ossia_parameter_t param,
    int disabled);
/**
 * @see ossia::net::get_disabled
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
int ossia_parameter_get_disabled(
    ossia_parameter_t param);

/**
 * @see ossia::net::set_critical
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_set_critical(
    ossia_parameter_t param,
    int critical);
/**
 * @see ossia::net::get_critical
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
int ossia_parameter_get_critical(
    ossia_parameter_t param);

/**
 * @see ossia::net::set_repetition_filter
 * @note Multithread guarantees: Data-Safe.
 *
 * repetition_filter is a boolean value.
 */
OSSIA_EXPORT
void ossia_parameter_set_repetition_filter(
    ossia_parameter_t param,
    int repetition_filter);
/**
 * @see ossia::net::get_repetition_filter
 * @note Multithread guarantees: Data-Safe.
 * @note No need to free
 */
OSSIA_EXPORT
int ossia_parameter_get_repetition_filter(
    ossia_parameter_t param);

/**
 * @see ossia::net::parameter_base::set_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_set_value(
    ossia_parameter_t param,
    ossia_value_t value);
/**
 * @see ossia::net::parameter_base::value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_value_t ossia_parameter_get_value(
    ossia_parameter_t param);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_value(
    ossia_parameter_t param,
    ossia_value_t value);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_impulse(
    ossia_parameter_t param);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_i(
    ossia_parameter_t param,
    int value);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 *
 * b is a boolean, 0 for false, 1 for true.
 */
OSSIA_EXPORT
void ossia_parameter_push_b(
    ossia_parameter_t param,
    int b);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_f(
    ossia_parameter_t param,
    float value);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_2f(
    ossia_parameter_t param,
    float v1, float v2);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_3f(
    ossia_parameter_t param,
    float v1, float v2, float v3);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_4f(
    ossia_parameter_t param,
    float v1, float v2, float v3, float v4);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_c(
    ossia_parameter_t param,
    char value);
/**
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_s(
    ossia_parameter_t param,
    const char* value);

/**
 * @brief Push an array of integers.
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_in(
    ossia_parameter_t param,
    const int* value,
    size_t sz);
/**
 * @brief Push an array of floats.
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_fn(
    ossia_parameter_t param,
    const float* value,
    size_t sz);
/**
 * @brief Push a string with known length.
 * @see ossia::net::parameter_base::push_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_push_cn(
    ossia_parameter_t param,
    const char* value,
    size_t sz);
/**
 * @see ossia::net::parameter_base::fetch_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_value_t ossia_parameter_fetch_value(
    ossia_parameter_t param);

/**
 * @brief Enable or disable remote updates for a given address
 * @note Multithread guarantees: Safe.
 */
OSSIA_EXPORT
void ossia_parameter_set_listening(
    ossia_parameter_t param,
    int listening);

/**
 * @brief Add a callback called when the value of a parameter changes.
 * @param address Address on which the callback must be added.
 * @param callback Function to be called.
 * @param ctx Will be passed to the callback.
 * @return An identifier to be able to remove the callback on a later date.
 *
 * @see ossia::net::parameter_base::add_callback
 * @note Multithread guarantees: MT-Safe.
 *       The callback is called from the thread where the value was changed.
 */
OSSIA_EXPORT
ossia_value_callback_idx_t ossia_parameter_add_callback(
    ossia_parameter_t param,
    ossia_value_callback_t callback,
    void* ctx);

/**
 * @brief Add a callback called when the value of a parameter changes.
 * @param address Address on which the callback must be added.
 * @param callback Function to be called.
 * @param ctx Will be passed to the callback.
 *
 * @see ossia::net::parameter_base::add_callback
 * @note Multithread guarantees: MT-Safe.
 *       The callback is called from the thread where the value was changed.
 */
OSSIA_EXPORT
void ossia_parameter_push_callback(
    ossia_parameter_t param,
    ossia_value_callback_t callback,
    void* ctx);

/**
 * @brief Remove a callback added with ossia_parameter_add_callback
 * @param address Address on which the callback must be removed
 * @param index Identifier of the callback
 *
 * @note The callback index must not be used afterwards
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_remove_callback(
    ossia_parameter_t param,
    ossia_value_callback_idx_t index);

/**
 * @brief Free a callback index
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_parameter_free_callback_idx(
    ossia_value_callback_idx_t);

/** @}*/


/************/
/*** Node ***/
/************/

/** @defgroup CNode Node
 * @brief Node creation, removal, finding.
 * @see ossia::net::node_base
 *  @{
 */
/**
 * @brief Find a child node
 * @param root The root node where the search starts from.
 * @param name An address such as "/foo/bar"
 * @return The node, or NULL if not found.
 *
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::net::find_node
 */
OSSIA_EXPORT ossia_node_t ossia_node_find(
    ossia_node_t root,
    const char* path);

/**
 * @brief Find nodes matching a wildcard pattern
 * @param root The root node where the search starts from.
 * @param name An address such as /foo.[0-5]/{bar,baz}*
 * @param data Where the found nodes will be stored.
 *             Set to NULL if none were found.
 * @param size Pointer to the number of found nodes.
 *
 * Usage:
 * @code
 * ossia_node_t* data;
 * size_t sz;
 * ossia_node_find_pattern(root, "/foo/bar.*", &data, &sz);
 * ...
 * ossia_node_array_free(data);
 * @endcode
 *
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::net::find_nodes
 */
OSSIA_EXPORT void ossia_node_find_pattern(
    ossia_node_t root,
    const char* pattern,
    ossia_node_t** data,
    size_t* size);

/**
 * @brief Create a child node
 * @param root The root node where the search starts from.
 * @param name An address such as "/foo/bar"
 * @return The node, or NULL if not found.
 *
 * Node creation follows the instance rules:
 * @code
 * ossia_node_create(root, "/foo/bar");
 * ossia_node_create(root, "/foo/bar");
 * ossia_node_create(root, "/foo/bar");
 * @endcode
 *
 * will create `/foo/bar`, `/foo/bar.1`, `/foo/bar.2`. *
 *
 * @note Multithread guarantees: MT-Safe.
 * @see ossia::net::create_node
 */
OSSIA_EXPORT ossia_node_t ossia_node_create(
    ossia_node_t root,
    const char* path);

/**
 * @brief Create nodes matching a brace expansion pattern
 * @param root The root node where the creation starts from.
 * @param name An address such as /foo.[0-5]/{bar,baz}
 * @param data Where the created nodes will be stored.
 *             Set to NULL if none were created.
 * @param size Pointer to the number of created nodes.
 *
 * Usage:
 * @code
 * ossia_node_t* data;
 * size_t sz;
 * ossia_node_create_pattern(root, "/foo.[0-5]/{bar,baz}", &data, &sz);
 * ...
 * ossia_node_array_free(data);
 * @endcode
 *
 * @note Multithread guarantees: MT-Safe.
 * @see ossia::net::create_nodes
 */
OSSIA_EXPORT void ossia_node_create_pattern(
    ossia_node_t root,
    const char* pattern,
    ossia_node_t** data,
    size_t* size);

/**
 * @brief Free an array of nodes allocated with ossia_node_find_pattern or ossia_node_create_pattern.
 *
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT void ossia_node_array_free(
    ossia_node_t*);

/**
 * @brief Create a direct child node
 * @param node The parent
 * @param name The name of the node, e.g. "foo"
 *
 * @note Multithread guarantees: MT-Safe.
 * @see ossia::net::node_base::add_child.
 * @see ossia_node_create
 */
OSSIA_EXPORT
ossia_node_t ossia_node_add_child(
    ossia_node_t node,
    const char * name);

/**
 * @brief Removes a direct child node.
 * @param node The parent.
 * @param name The name of the node, e.g. "foo"
 *
 * @note Multithread guarantees: MT-Safe.
 * @see ossia::net::node_base::remove_child
 */
OSSIA_EXPORT
void ossia_node_remove_child(
    ossia_node_t node,
    ossia_node_t name);

/**
 * @brief Get the name of a node
 *
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::net::node_base::get_name
 */
OSSIA_EXPORT
const char* ossia_node_get_name(
    ossia_node_t node);

/**
 * @brief Get the root device of a node
 *
 * @note Don't free the device obtained from here
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::net::node_base::get_device
 */
OSSIA_EXPORT
ossia_device_t ossia_node_get_device(
    ossia_node_t node);

/**
 * @brief Get the number of children of a node.
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::net::node_base::children
 */
OSSIA_EXPORT
int ossia_node_child_size(
    ossia_node_t node);

/**
 * @brief Get a specific direct child of a node.
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::net::node_base::children
 */
OSSIA_EXPORT
ossia_node_t ossia_node_get_child(
    ossia_node_t node,
    int child_n);

/**
 * @brief Find a direct child of a node by name.
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::net::node_base::find_child
 */
OSSIA_EXPORT
ossia_node_t ossia_node_find_child(
    ossia_node_t node,
    const char* name);

/**
 * @brief Create a parameter in a node.
 * @param type Type of the address
 * @note Multithread guarantees: MT-Safe.
 * @see ossia::net::node_base::create_parameter
 */
OSSIA_EXPORT
ossia_parameter_t ossia_node_create_parameter(
    ossia_node_t node,
    ossia_type type);

/**
 * @brief Get the address contained in a node if any.
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::net::node_base::get_parameter
 */
OSSIA_EXPORT
ossia_parameter_t ossia_node_get_parameter(
    ossia_node_t node);

/**
 * @brief Remove the address contained in a node if any.
 * @note Multithread guarantees: MT-Safe.
 * @see ossia::net::node_base::remove_parameter
 */
OSSIA_EXPORT
void ossia_node_remove_parameter(
    ossia_node_t node);

/**
 * @brief Add a callback called when a node is about to be removed.
 * @param node Node on which the callback is added.
 * @param callback Function to be called.
 * @param ctx Will be passed to the callback.
 * @return An identifier to be able to remove the callback on a later date.
 *
 * @see ossia::net::node_base::about_to_be_deleted
 * @note Multithread guarantees: MT-Safe.
 *       The callback is called from the thread where the node is being deleted
 */
OSSIA_EXPORT
ossia_node_callback_idx_t ossia_node_add_deleting_callback(
    ossia_node_t node,
    ossia_node_callback_t callback,
    void* ctx);
/**
 * @brief Remove a callback added with ossia_node_add_deleting_callback
 * @param node Node on which the callback must be removed.
 * @param index Index of the callback to remove.
 *
 * @see ossia::net::node_base::about_to_be_deleted
 * @note Multithread guarantees: MT-Safe.
 * @note The callback index must not be used afterwards
 */
OSSIA_EXPORT
void ossia_node_remove_deleting_callback(
    ossia_node_t node,
    ossia_node_callback_idx_t index);


/**
 * @see ossia::net::set_description
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_set_description(
    ossia_node_t node,
    const char* description);
/**
 * @see ossia::net::get_description, ossia_string_free
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
const char* ossia_node_get_description(
    ossia_node_t node);

/**
 * @see ossia::net::set_extended_type
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_set_extended_type(
    ossia_node_t node,
    const char* extended_type);
/**
 * @see ossia::net::get_extended_type, ossia_string_free
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
const char* ossia_node_get_extended_type(
    ossia_node_t node);


/**
 * @see ossia::net::set_tags
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_set_tags(
    ossia_node_t node,
    const char** tags,
    size_t sz);
/**
 * @see ossia::net::get_tags, ossia_tags_free
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_node_get_tags(
    ossia_node_t node,
    char*** tags,
    size_t* sz);

/**
 * @brief Free tags allocated with ossia_node_get_tags
 */
OSSIA_EXPORT
void ossia_tags_free(char** tags, size_t sz);

/**
 * @see ossia::net::set_hidden
 * @note Multithread guarantees: MT-Safe.
 *
 * hidden is a boolean value.
 */
OSSIA_EXPORT
void ossia_node_set_hidden(
    ossia_node_t node,
    int hidden);
/**
 * @see ossia::net::get_hidden
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
int ossia_node_get_hidden(
    ossia_node_t node);

/**
 * @see ossia::net::set_refresh_rate
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_set_refresh_rate(
    ossia_node_t node,
    int refresh_rate);
/**
 * @see ossia::net::set_refresh_rate
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_unset_refresh_rate(
    ossia_node_t node);
/**
 * @see ossia::net::get_refresh_rate
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
int ossia_node_get_refresh_rate(
    ossia_node_t node,
    int* ok = NULL);

/**
 * @see ossia::net::set_priority
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_set_priority(
    ossia_node_t node,
    float priority);
/**
 * @see ossia::net::set_priority
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_unset_priority(
    ossia_node_t node);
/**
 * @see ossia::net::get_priority
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
float ossia_node_get_priority(
    ossia_node_t node,
    int* ok = NULL);

/**
 * @see ossia::net::set_value_step_size
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_set_value_step_size(
    ossia_node_t node,
    double value_step_size);
/**
 * @see ossia::net::set_value_step_size
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_unset_value_step_size(
    ossia_node_t node);
/**
 * @see ossia::net::get_value_step_size
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
double ossia_node_get_value_step_size(
    ossia_node_t node,
    int* ok = NULL);


/**
 * @see ossia::net::set_instance_bounds
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_set_instance_bounds(
    ossia_node_t node,
    int min, int max);
/**
 * @see ossia::net::set_instance_bounds
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_unset_instance_bounds(
    ossia_node_t node);
/**
 * @see ossia::net::get_instance_bounds
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_node_get_instance_bounds(
    ossia_node_t node,
    int* min, int* max,
    int* ok = NULL);

/**
 * @see ossia::net::set_default_value
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_node_set_default_value(
    ossia_node_t node,
    ossia_value_t default_value);
/**
 * @see ossia::net::get_default_value
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_value_t ossia_node_get_default_value(
    ossia_node_t node);

/** @}*/

/**************/
/*** Domain ***/
/**************/

/** @defgroup CDom Domain
 * @brief Parameter min / max
 * @see ossia::net::domain
 *  @{
 */
/**
 * @brief Create a domain from two values.
 * @note Multithread guarantees: Data-Safe.
 * @note The values will be copied inside the domain.
 * @see ossia::make_domain
 */
OSSIA_EXPORT
ossia_domain_t ossia_domain_make_min_max(
    ossia_value_t min,
    ossia_value_t max);

/**
 * @brief Get the min of a domain
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::get_min
 */
OSSIA_EXPORT
ossia_value_t ossia_domain_get_min(
    ossia_domain_t domain);
/**
 * @brief Set the min of a domain
 * @note Multithread guarantees: MT-Safe.
 * @see ossia::set_min
 */
OSSIA_EXPORT
void ossia_domain_set_min(
    ossia_domain_t domain,
    ossia_value_t value);
/**
 * @brief Get the max of a domain
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::get_max
 */
OSSIA_EXPORT
ossia_value_t ossia_domain_get_max(
    ossia_domain_t domain);
/**
 * @brief Set the max of a domain
 * @note Multithread guarantees: MT-Safe.
 * @see ossia::set_max
 */
OSSIA_EXPORT
void ossia_domain_set_max(
    ossia_domain_t domain,
    ossia_value_t value);

/**
 * @brief Get the values of a domain
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_domain_get_values(
    ossia_domain_t domain,
    ossia_value_t** values,
    size_t* n);
/**
 * @brief Set the values of a domain
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_domain_set_values(
    ossia_domain_t domain,
    const ossia_value_t* values,
    size_t n);


/**
 * @brief Free a domain
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_domain_free(
    ossia_domain_t domain);

/** @}*/

/*************/
/*** Value ***/
/*************/
/** @defgroup CVal Value
 * @brief Tools to create various kinds of values.
 *
 * The ossia_value_to_... family of functions expect that the actual
 * value type is the one requested; anything else is undefined behaviour.
 * Hence, use this only when you are sure because this is unsafe.
 *
 * In contrast, the ossia_value_convert_... functions will try to do
 * a meaningful conversion to get the type requested, which is safe, but
 * much slower.
 *
 * In short:
 *
 * @code
 * ossia_value_t v = ossia_value_create_string("1.2");
 * const char* = ossia_value_to_float(v); // result undefined
 * float f1 = ossia_value_to_float(v); // result undefined
 * float f2 = ossia_value_convert_float(v); // ok, f2 == 1.2
 * @endcode
 *
 * @see ossia::value
 *  @{
 */
/**
 * @brief Create a value of type Impulse
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_impulse();
/**
 * @brief Create a value of type int
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_int(int value);
/**
 * @brief Create a value of type float
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_float(float value);
/**
 * @brief Create a value of type vec2f
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_2f(float v1, float v2);
/**
 * @brief Create a value of type vec3f
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_3f(float v1, float v2, float v3);
/**
 * @brief Create a value of type vec4f
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_4f(float v1, float v2, float v3, float v4);
/**
 * @brief Create a value of type bool
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_bool(int value);
/**
 * @brief Create a value of type char
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_char(char value);
/**
 * @brief Create a value of type string
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_string(const char* value);
/**
 * @brief Create a value of type string
 * @note Multithread guarantees: Data-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_byte_array(const char* value, size_t size);
/**
 * @brief Create a value of type list
 * @note Multithread guarantees: Data-Safe.
 * @note The passed array will be copied
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_list(const ossia_value_t* values, size_t size);
/**
 * @brief Create a value of type list with only ints
 * @note Multithread guarantees: Data-Safe.
 * @note The passed array will be copied
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_in(const int* values, size_t size);
/**
 * @brief Create a value of type list with only floats
 * @note Multithread guarantees: Data-Safe.
 * @note The passed array will be copied
 * @see ossia::value
 */
OSSIA_EXPORT
ossia_value_t ossia_value_create_fn(const float* values, size_t size);

/**
 * @brief Free the value.
 * @note Multithread guarantees: MT-Safe.
 * @see ossia::value
 */
OSSIA_EXPORT
void ossia_value_free(ossia_value_t value);

/**
 * @brief Get the type of a value
 * @note Multithread guarantees: Data-Safe.
 * @see ossia_type
 */
OSSIA_EXPORT
ossia_type ossia_value_get_type(ossia_value_t type);

/**
 * @brief Get the value if it is an int.
 * @return Undefined if ossia_value_get_type(val) != INT_T
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
int ossia_value_to_int(ossia_value_t val);
/**
 * @brief Get the value if it is an float.
 * @return Undefined if ossia_value_get_type(val) != FLOAT_T
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
float ossia_value_to_float(ossia_value_t val);
/**
 * @brief Get the value if it is a vec2f.
 * @return Undefined if ossia_value_get_type(val) != VEC2F_T
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_vec2f ossia_value_to_2f(ossia_value_t val);
/**
 * @brief Get the value if it is a vec3f.
 * @return Undefined if ossia_value_get_type(val) != VEC3F_T
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_vec3f ossia_value_to_3f(ossia_value_t val);
/**
 * @brief Get the value if it is a vec4f.
 * @return Undefined if ossia_value_get_type(val) != VEC4F_T
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
ossia_vec4f ossia_value_to_4f(ossia_value_t val);
/**
 * @brief Get the value if it is an bool.
 * @return Undefined if ossia_value_get_type(val) != BOOL_T
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
int ossia_value_to_bool(ossia_value_t val);
/**
 * @brief Get the value if it is a char.
 * @return Undefined if ossia_value_get_type(val) != CHAR_T
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
char ossia_value_to_char(ossia_value_t val);
/**
 * @brief Get the value if it is a byte array.
 * @return Undefined if ossia_value_get_type(val) != STRING_T
 * @note Multithread guarantees: Data-Safe.
 *
 * Usage:
 * @code
 * char* str;
 * size_t b;
 * ossia_value_to_byte_array(val, &str, &b);
 * @endcode
 *
 * @see ossia_string_free
 */
OSSIA_EXPORT
void ossia_value_to_byte_array(ossia_value_t val, char** str, size_t* sz);
/**
 * @brief Get the value if it is a null-terminated string.
 * @return Undefined if ossia_value_get_type(val) != STRING_T
 * @note Multithread guarantees: Data-Safe.
 *
 * @see ossia_string_free
 */
OSSIA_EXPORT
const char* ossia_value_to_string(ossia_value_t val);
/**
 * @brief Get the value if it is a list.
 * @return Undefined if ossia_value_get_type(val) != LIST_T
 * @note Multithread guarantees: Data-Safe.
 * @see ossia_value_free_list
 */
OSSIA_EXPORT
void ossia_value_to_list(ossia_value_t val_in, ossia_value_t** out, size_t* size);
/**
 * @brief Free an array of values
 */
OSSIA_EXPORT
void ossia_value_free_list(ossia_value_t* out);

/**
 * @brief Get the value if it is a list of floats.
 * @return Undefined if ossia_value_get_type(val) != LIST_T or any of the values are not FLOAT_T.
 * @note Multithread guarantees: Data-Safe.
 * @see ossia_value_free_list
 */
OSSIA_EXPORT
void ossia_value_to_fn(ossia_value_t val_in, float** out, size_t* size);
/**
 * @brief Free an array of float
 */
OSSIA_EXPORT
void ossia_value_free_fn(float* out);

/**
 * @brief Get the value if it is a list of ints.
 * @return Undefined if ossia_value_get_type(val) != LIST_T or any of the values are not INT_T.
 * @note Multithread guarantees: Data-Safe.
 * @see ossia_value_free_list
 */
OSSIA_EXPORT
void ossia_value_to_in(ossia_value_t val_in, int** out, size_t* size);
/**
 * @brief Free an array of int
 */
OSSIA_EXPORT
void ossia_value_free_in(int* out);

/**
 * @brief Get the value as an int
 *
 * This function will do a best-effort conversion.
 *
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
int ossia_value_convert_int(ossia_value_t val);
/**
 * @brief Get the value as a float
 *
 * This function will do a best-effort conversion.
 *
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
float ossia_value_convert_float(ossia_value_t val);
/**
 * @brief Get the value as a bool
 *
 * This function will do a best-effort conversion.
 *
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
int ossia_value_convert_bool(ossia_value_t val);
/**
 * @brief Get the value as a char
 *
 * This function will do a best-effort conversion.
 *
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
char ossia_value_convert_char(ossia_value_t val);
/**
 * @brief Get the value as a byte array
 *
 * This function will do a best-effort conversion.
 *
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_value_convert_byte_array(ossia_value_t val, char** str, size_t* sz);
/**
 * @brief Get the value as a list
 *
 * This function will do a best-effort conversion.
 *
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_value_convert_list(ossia_value_t val_in, ossia_value_t** out, size_t* size);


OSSIA_EXPORT
void ossia_string_free(char*);

/** @}*/

/***************/
/*** Logging ***/
/***************/
/** @defgroup CLog Logging
 * @brief Log various messages in JSON to a websocket server, and send heartbeats.
 *  @{
 */
/** Log levels available **/
enum log_level { trace, debug, info, warn, error, critical, off };

/**
 * @brief Create a logger instance
 * @param websocket host: ws://127.0.0.1:1337
 * @param app: name of the local app
 *
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
ossia_logger_t ossia_logger_create(const char* host, const char* app);
/**
 * @brief Start sending logger heartbeats
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_logger_init_heartbeat(ossia_logger_t log, int pid, const char* cmdline);
/**
 * @brief Change the current log level.
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_logger_set_level(ossia_logger_t log, log_level lvl);
/**
 * @brief Log a message.
 * @note Multithread guarantees: Data-Safe.
 */
OSSIA_EXPORT
void ossia_log(ossia_logger_t log, log_level lvl, const char* message);

/**
 * @brief Free a logger instance.
 * @note Multithread guarantees: MT-Safe.
 */
OSSIA_EXPORT
void ossia_logger_free(ossia_logger_t log);

/** @}*/

/*************************/
/*** Message-queue API ***/
/*************************/

/** @defgroup CMQ Message queue
 * @brief When threads cannot be used from the client side.
 *
 * Usage:
 *
 * \code
 * ossia_mq_t mq = ossia_mq_create(device);
 * ...
 * ossia::parameter_t param;
 * ossia_value_t val;
 * while(ossia_mq_pop(mq, &param, &val)) {
 *   // Do things with param & val
 * }
 * ossia_mq_free(mq);
 * \endcode
 *
 *  @{
 */
typedef void* ossia_mq_t;

/**
 * @brief Create a message queue for a given device
 *
 * The queue is single-produce single-consumer.
 * The values are produced by received network messages, in
 * a network thread.
 *
 * This can be used to safely get the values in another thread.
 */
OSSIA_EXPORT
ossia_mq_t ossia_mq_create(ossia_device_t);

/**
 * @brief Register a parameter into a message queue
 */
OSSIA_EXPORT
void ossia_mq_register(ossia_mq_t, ossia_parameter_t);

/**
 * @brief Unregister a parameter from a message queue
 */
OSSIA_EXPORT
void ossia_mq_unregister(ossia_mq_t, ossia_parameter_t);

/**
 * @brief Get the head of the message queue if any
 * @return 1 if the queue could be read successfully
 */
OSSIA_EXPORT
int ossia_mq_pop(ossia_mq_t mq, ossia_parameter_t* param, ossia_value_t* val);

/**
 * @brief Remove a message queue
 */
OSSIA_EXPORT
void ossia_mq_free(ossia_mq_t);

/** @}*/

#if defined(__cplusplus)
}
#endif

/** @}*/

#endif
