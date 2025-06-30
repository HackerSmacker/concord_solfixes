/**
 * @file discord-response.h
 * @author Cogmasters
 * @brief Generic macros for initializing a @ref discord_response and return
 *      handles
 */

#ifndef DISCORD_RESPONSE_H
#define DISCORD_RESPONSE_H

/** @brief The response for the completed request */
struct discord_response {
    /** user arbitrary data provided at @ref discord_recv */
    void *data;
    /** kept concord's parameter provided at @ref discord_recv */
    const void *keep;
    /** request completion status @see @ref ConcordError */
    CCORDcode code;
    /** the JSON object in case of a @ref CCORD_OK, or the JSON error
     *      object in case of a @ref CCORD_DISCORD_JSON_CODE
     *
     * @see https://discord.com/developers/docs/reference#error-messages */
    struct ccord_szbuf_readonly json;
};

/******************************************************************************
 * Templates for generating type-safe return handles for async requests
 ******************************************************************************/

/**
 * @brief Macro containing common fields for `struct discord_recv*` datatypes
 * @note this exists for alignment purposes
 */
#define DISCORD_RECV_DEFAULT_FIELDS                                           \
    /** user arbitrary data to be passed to `done` or `fail` callbacks */     \
    void *data;                                                               \
    /** cleanup method to be called for `data`, once its no longer            \
    being referenced */                                                       \
    void (*cleanup)(struct discord * client, void *data);                     \
    /** Concord callback parameter the client wish to keep reference */       \
    const void *keep;                                                         \
    /** if `true` then request will be prioritized over already enqueued      \
        requests */                                                           \
    bool high_priority;                                                       \
    /** optional callback to be executed on a failed request */               \
    void (*fail)(struct discord * client, struct discord_response * resp)

#define _DISCORD_RECV_DEFAULT_FIELDS_SIZE                                     \
    sizeof(struct { DISCORD_RECV_DEFAULT_FIELDS; })

#define DISCORD_RECV_DEFAULT_FIELDS_COPY(dest, src)                           \
    memcpy(&(dest), &(src), _DISCORD_RECV_DEFAULT_FIELDS_SIZE)

#define _DISCORD_RECV_DECLARE(_type)                                          \
    /** @brief Request's return context */                                    \
    struct discord_recv_##_type {                                             \
        DISCORD_RECV_DEFAULT_FIELDS;                                          \
        /** optional callback to be executed on a successful request */       \
        void (*done)(struct discord * client,                                 \
                     struct discord_response *resp,                           \
                     const struct discord_##_type *recv);                     \
        /** if an address is provided, then request will block the thread and \
           perform on-spot.                                                   \
           On success the response object will be written to the address,     \
           unless enabled with @ref DISCORD_SYNC_FLAG */                      \
        struct discord_##_type *sync;                                         \
    }

/** @brief Request's return context */
struct discord_recv {
    DISCORD_RECV_DEFAULT_FIELDS;
    /** optional callback to be executed on a successful request */
    void (*done)(struct discord *client, struct discord_response *resp);
    /** if `true`, request will block the thread and perform on-spot */
    bool sync;
};

/** @brief flag for enabling `sync` mode without expecting a datatype return */
#define DISCORD_SYNC_FLAG ((void *)-1)

/** @addtogroup DiscordAPIOAuth2
 *  @{ */
_DISCORD_RECV_DECLARE(application);
_DISCORD_RECV_DECLARE(auth_response);
/** @} DiscordAPIOAuth2 */

/** @addtogroup DiscordAPIAuditLog
 *  @{ */
_DISCORD_RECV_DECLARE(audit_log);
/** @} DiscordAPIAuditLog */

/** @addtogroup DiscordAPIAutoModeration
 *  @{ */
_DISCORD_RECV_DECLARE(auto_moderation_rule);
/** @} DiscordAPIAutoModeration */

/** @addtogroup DiscordAPIChannel
 *  @{ */
_DISCORD_RECV_DECLARE(channel);
_DISCORD_RECV_DECLARE(message);
_DISCORD_RECV_DECLARE(followed_channel);
_DISCORD_RECV_DECLARE(thread_member);
_DISCORD_RECV_DECLARE(thread_response_body);
/** @} DiscordAPIChannel */

/** @addtogroup DiscordAPIEmoji
 *  @{ */
_DISCORD_RECV_DECLARE(emoji);
/** @} DiscordAPIEmoji */

/** @addtogroup DiscordAPIGuild
 *  @{ */
_DISCORD_RECV_DECLARE(guild);
_DISCORD_RECV_DECLARE(guild_preview);
_DISCORD_RECV_DECLARE(guild_member);
_DISCORD_RECV_DECLARE(guild_widget);
_DISCORD_RECV_DECLARE(guild_widget_settings);
_DISCORD_RECV_DECLARE(ban);
_DISCORD_RECV_DECLARE(role);
_DISCORD_RECV_DECLARE(welcome_screen);
_DISCORD_RECV_DECLARE(integration);
_DISCORD_RECV_DECLARE(prune_count);
/** @} DiscordAPIGuild */

/** @addtogroup DiscordAPIGuildScheduledEvent
 *  @{ */
_DISCORD_RECV_DECLARE(guild_scheduled_event);
_DISCORD_RECV_DECLARE(guild_scheduled_event_users);
/** @} DiscordAPIGuildScheduledEvent */

/** @addtogroup DiscordAPIGuildTemplate
 *  @{ */
_DISCORD_RECV_DECLARE(guild_template);
/** @} DiscordAPIGuildTemplate */

/** @addtogroup DiscordAPIInvite
 *  @{ */
_DISCORD_RECV_DECLARE(invite);
/** @} DiscordAPIInvite */

/** @addtogroup DiscordAPIStageInstance
 *  @{ */
_DISCORD_RECV_DECLARE(stage_instance);
/** @} DiscordAPIStageInstance */

/** @addtogroup DiscordAPISticker
 *  @{ */
_DISCORD_RECV_DECLARE(sticker);
_DISCORD_RECV_DECLARE(list_nitro_sticker_packs);
/** @} DiscordAPISticker */

/** @addtogroup DiscordAPIUser
 *  @{ */
_DISCORD_RECV_DECLARE(user);
_DISCORD_RECV_DECLARE(connection);
/** @} DiscordAPIUser */

/** @addtogroup DiscordAPIVoice
 *  @{ */
_DISCORD_RECV_DECLARE(voice_region);
/** @} DiscordAPIVoice */

/** @addtogroup DiscordAPIWebhook
 *  @{ */
_DISCORD_RECV_DECLARE(webhook);
/** @} DiscordAPIWebhook */

/** @addtogroup DiscordAPIInteractionsApplicationCommand
 * @ingroup DiscordAPIInteractions
 *  @{ */
_DISCORD_RECV_DECLARE(application_command);
_DISCORD_RECV_DECLARE(application_command_permission);
_DISCORD_RECV_DECLARE(guild_application_command_permission);
/** @} DiscordAPIInteractionsApplicationCommand */

/** @addtogroup DiscordAPIInteractionsReact
 * @ingroup DiscordAPIInteractions
 *  @{ */
_DISCORD_RECV_DECLARE(interaction_response);
/** @} DiscordAPIInteractionsReact */

#undef _DISCORD_RECV_DECLARE

#endif /* DISCORD_RESPONSE_H */
