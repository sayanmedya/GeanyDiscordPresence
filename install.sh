printf "Installing GeanyDiscordPresence ...\n"

# copy libdiscord-rpc(https://github.com/discord/discord-rpc/releases) into /usr/lib
if cp lib/libdiscord-rpc.so /usr/lib/libdiscord-rpc.so; then
	printf "Copied libdiscord-rpc(https://github.com/discord/discord-rpc/releases) into /usr/lib\n"
else
	printf "\033[31mError:\033[0m Copying libdiscord-rpc failed. Try: sudo ./install.sh\n"
	exit
fi
# build the plugin
if gcc -c discord.c plugin.c -L./lib -ldiscord-rpc -fPIC `pkg-config --cflags geany`; then
	printf "Compilation successful.\n"
else
	printf "\033[31mError:\033[0m Compilation failed.\n"
	exit
fi 

if gcc plugin.o discord.o -o discord_presence.so -L./lib -ldiscord-rpc -shared `pkg-config --libs geany`; then
	printf "Build successful.\n"
else
	printf "\033[31mError:\033[0m Build failed.\n"
	exit
fi 

# find the path of geany's libdir
if prefix=$(pkg-config --variable=libdir geany); then
	printf "Found Geany's libdir path: "
	printf $prefix
	printf "\n"
else
	printf "\033[31mError:\033[0m Could not find Geany's libdir path.\n"
	exit
fi

# move the plugin into geany's libdir
if cp discord_presence.so $prefix/geany/discord_presence.so; then
	printf "Copied the extension into Geany's libdir path.\n"
else
	printf "\033[31mError:\033[0m Could not copy the extension into Geany's libdir path.\n"
	exit
fi

# cleanup
rm plugin.o discord.o discord_presence.so

printf "\033[32mInstallation complete!\033[0m\n"
printf "Enable the plugin in Tools -> Plugin Manager.\n"