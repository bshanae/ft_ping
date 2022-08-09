project(ft_ping)

include_directories(
	main/include)

add_executable(ft_ping
	main/src/main.c
	main/src/utilities.c
	main/src/ipv4.c)