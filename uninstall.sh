# remove libdiscord-rpc(https://github.com/discord/discord-rpc/releases)
rm /usr/lib/libdiscord-rpc.so

# find the path of geany's libdir
prefix=$(pkg-config --variable=libdir geany)

# remove the plugin from geany's libdir
rm $prefix/geany/discord_presence.so

printf "Uninstallation complete!\n"