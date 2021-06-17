// Handles discord-rpc related functions
// Adapted from https://github.com/discord/discord-rpc/blob/master/examples/send-presence/send-presence.c

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <geanyplugin.h>
#include "include/discord_rpc.h"
#include "discord.h"

static const char* APPLICATION_ID = "854749886583013387";

static void handleDiscordReady(const DiscordUser* connectedUser)
{
    printf("Discord Presence Plugin: connected to discord user %s#%s - %s\n",
           connectedUser->username,
           connectedUser->discriminator,
           connectedUser->userId);
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
    printf("Discord Presence Plugin: discord disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
    printf("Discord Presence Plugin: discord error (%d: %s)\n", errcode, message);
}

void discordInit()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

void updPresence(GeanyDocument *doc) {
    char buffer[1024];
	DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    sprintf(buffer, "Editing %s", DOC_FILENAME(doc));
	discordPresence.details = buffer;
    discordPresence.startTimestamp = time(0);
    discordPresence.largeImageKey = "cpp";
    discordPresence.largeImageText = "C++";
    discordPresence.smallImageKey = "geany_logo";
    discordPresence.smallImageText = "Geany";
	Discord_UpdatePresence(&discordPresence);
}