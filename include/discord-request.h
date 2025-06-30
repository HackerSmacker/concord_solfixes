/**
 * @file discord-request.h
 * @ingroup DiscordInternalREST
 * @author Cogmasters
 * @brief Generic macros for initializing a @ref discord_attributes
 */

#ifndef DISCORD_REQUEST_H
#define DISCORD_REQUEST_H

typedef void (*cast_done_typed)(struct discord *,
                                struct discord_response *,
                                const void *);
typedef void (*cast_init)(void *);
typedef void (*cast_cleanup)(void *);
typedef size_t (*cast_from_json)(const char *, size_t, void *);

/**
 * @brief Helper for setting attributes for a specs-generated return struct
 *
 * @param[out] attr @ref discord_attributes handler to be initialized
 * @param[in] type datatype of the struct
 * @param[in] recv dispatch attributes
 * @param[in] _reason reason for request (if available)
 */
#define DISCORD_ATTR_INIT(attr, type, recv, _reason)                          \
    do {                                                                      \
        attr.response.size = sizeof(struct type);                             \
        attr.response.init = (cast_init)type##_init;                          \
        attr.response.from_json = (cast_from_json)type##_from_json;           \
        attr.response.cleanup = (cast_cleanup)type##_cleanup;                 \
        attr.reason = _reason;                                                \
        if (recv) {                                                           \
            DISCORD_RECV_DEFAULT_FIELDS_COPY((attr).dispatch, *recv);         \
            attr.dispatch.has_type = true;                                    \
            attr.dispatch.done.typed = (cast_done_typed)recv->done;           \
            attr.dispatch.sync = recv->sync;                                  \
        }                                                                     \
    } while (0)

/**
 * @brief Helper for setting attributes for attruests that doensn't expect a
 *      response object
 *
 * @param[out] attr @ref discord_attributes handler to be initialized
 * @param[in] recv dispatch attributes
 * @param[in] _reason reason for request (if available)
 */
#define DISCORD_ATTR_BLANK_INIT(attr, recv, _reason)                          \
    do {                                                                      \
        attr.reason = _reason;                                                \
        if (recv) {                                                           \
            DISCORD_RECV_DEFAULT_FIELDS_COPY(attr.dispatch, *recv);           \
            attr.dispatch.has_type = false;                                   \
            attr.dispatch.done.typeless = recv->done;                         \
            attr.dispatch.sync = (void *)recv->sync;                          \
        }                                                                     \
    } while (0)

/**
 * @brief Helper for initializing attachments ids
 *
 * @param[in,out] attchs a @ref discord_attachment list to have its IDs
 *      initialized
 */
#define DISCORD_ATTACHMENTS_IDS_INIT(attchs)                                  \
    do {                                                                      \
        for (u64snowflake i = 0; i < discord_length(attchs); ++i) {           \
            attchs[i].id = i;                                                 \
        }                                                                     \
    } while (0)

#endif /* DISCORD_REQUEST_H */
