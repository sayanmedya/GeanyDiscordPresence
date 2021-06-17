printf "Installing Discord Presence ...\n"

# copy libdiscord-rpc(https://github.com/discord/discord-rpc/releases) into /usr/lib
cp lib/libdiscord-rpc.so /usr/lib/libdiscord-rpc.so

# build the plugin
gcc -c discord.c plugin.c -L./lib -ldiscord-rpc -fPIC `pkg-config --cflags geany`
gcc plugin.o discord.o -o discord_presence.so -L./lib -ldiscord-rpc -shared `pkg-config --libs geany`

# find the path of geany's libdir
prefix=$(pkg-config --variable=libdir geany)

# move the plugin into geany's libdir
cp discord_presence.so $prefix/geany/discord_presence.so

# cleanup
rm plugin.o discord.o discord_presence.so

printf "Installation complete!\n"
printf "Enable the plugin in Tools -> Plugin Manager\n"