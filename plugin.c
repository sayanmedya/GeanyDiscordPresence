#include <geanyplugin.h>
#include "include/discord_rpc.h"
#include "discord.h"

static void on_document_activate(GObject *obj, GeanyDocument *doc, gpointer user_data)
{
    /* Update presence */
    updPresence(doc);
}

static gboolean drpc_init(GeanyPlugin *plugin, gpointer pdata)
{
    /* Initiate discord rpc */
    discordInit();
    plugin_signal_connect(plugin, NULL, "document-activate", 
                            TRUE, G_CALLBACK(on_document_activate), NULL); // listen for active document changes
    printf("Discord Presence Plugin: Starting Plugin\n");
    return TRUE;
}

static void drpc_cleanup(GeanyPlugin *plugin, gpointer pdata)
{
    /* Cleanup discord rpc */
    Discord_Shutdown();
    printf("Discord Presence Plugin: Stopping Plugin\n");
}


G_MODULE_EXPORT
void geany_load_module(GeanyPlugin *plugin)
{
    /* Set metadata */
    plugin->info->name = "Discord Presence";
    plugin->info->description = "Update your discord status with a rich presence.";
    plugin->info->version = "0.1";
    plugin->info->author = "Sayan Medya <sayanmedya@gmail.com>";

    /* Set functions */
    plugin->funcs->init = drpc_init;
    plugin->funcs->cleanup = drpc_cleanup;

    /* Register! */
    GEANY_PLUGIN_REGISTER(plugin, 225);
}
