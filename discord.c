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
    /* Updates discord presence */
	DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    char name[1024], full_name[1024], buffer[1096], tpe[20], dir[1024], d_buffer[1096];
    sprintf(full_name, "%s", DOC_FILENAME(doc));
    int l = strlen(full_name), ni, fni;
    for (fni = l - 1, ni = 0;fni >= 0;fni--) {
        if (full_name[fni] == '/')
            break;
        name[ni++] = full_name[fni];
    }
    name[ni] = '\0';
    int k = strlen(name);
    for (int i = 0;i < k/2;i++) {
       char temp = name[i];
       name[i] = name[k - 1 - i];
       name[k - 1 - i] = temp;
    }
    sprintf(buffer, "Editing %s", name);
    discordPresence.details = buffer;
    if (fni < 0) {}
    else if (fni == 0)
        discordPresence.state = "In /";
    else {
        int di = 0;
        fni--;
        for (ni = 0;fni >= 0;fni--) {
            if (full_name[fni] == '/')
                break;
            dir[di++] = full_name[fni];
        }
        dir[di] = '\0';
        k = strlen(dir);
        for (int i = 0;i < k/2;i++) {
            char temp = dir[i];
            dir[i] = dir[k - 1 - i];
            dir[k - 1 - i] = temp;
        }
        sprintf(d_buffer, "In %s", dir);
        discordPresence.state = d_buffer;
    }
    discordPresence.startTimestamp = time(0);
    sprintf(tpe, "%s", filetypes_get_display_name(doc->file_type));
    if (strcmp(tpe, "C++") == 0) {
        discordPresence.largeImageKey = "cpp";
        discordPresence.largeImageText = "C++";
    }
    else if (strcmp(tpe, "C") == 0) {
        discordPresence.largeImageKey = "c";
        discordPresence.largeImageText = "C";
    }
    else if (strcmp(tpe, "Python") == 0) {
        discordPresence.largeImageKey = "python";
        discordPresence.largeImageText = "Python";
    }
    else if (strcmp(tpe, "Java") == 0) {
        discordPresence.largeImageKey = "java";
        discordPresence.largeImageText = "Java";
    }
    else {
        discordPresence.largeImageKey = "none";
        discordPresence.largeImageText = "File";
    }
    discordPresence.smallImageKey = "geany_logo";
    discordPresence.smallImageText = "Geany";
	Discord_UpdatePresence(&discordPresence);
}