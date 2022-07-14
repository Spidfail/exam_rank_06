/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 20:01:19 by exam              #+#    #+#             */
/*   Updated: 2022/07/13 21:14:58 by exam             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>

int sockfd;
size_t id_max;
fd_set	pool_set;
fd_set	write_set;
fd_set	read_set;


typedef struct list {
	int fd, id, new;
	struct list *next;
} t_lst;

t_lst	*list;

void	fatal() {
	char	str[] = "Fatal error\n";
	write(2, str, strlen(str));
	exit(1);
}

void	fatal_args() {
	char	str[] ="Wrong number of arguments\n";
	write(2, str, strlen(str));
	exit(1);
}

int		get_max_fd() {
	int		fd = 0;
	for (t_lst *tmp = list ; tmp ; tmp = tmp->next) {
		if (tmp->fd > fd)
			fd = tmp->fd;
	}
	if (fd == 0)
		return sockfd;
	return fd;
}

t_lst	*get_client(int fd) {
	for (t_lst *tmp = list ; tmp ; tmp = tmp->next) {
		if (tmp->fd == fd)
			return tmp;
	}
	return NULL;
}

void		send_all(char *str, ssize_t len, int fd_sender) {
	for (t_lst *tmp = list ; tmp ; tmp = tmp->next) {
		if (FD_ISSET(tmp->fd, &write_set) && tmp->fd != fd_sender)
			if (send(tmp->fd, str, len, 0) < 0)
				fatal();
	}
}

void	add_list(t_lst *new_cli) {
	if (list == NULL) {
		list = new_cli;
		return ;
	}
	for (t_lst *tmp = list ; tmp ; tmp = tmp->next) {
		if (tmp->next == NULL) {
			tmp->next = new_cli;
			break;
		}
	}
}

void	add_client() {
	char	header[100];
	struct sockaddr_in 	cli;
	socklen_t	len = sizeof(cli);
	t_lst	*new_cli = calloc(1, sizeof(t_lst));

	bzero(&cli, sizeof(cli)); 
	bzero(header, 100); 
	if (new_cli == NULL)
		fatal();
	new_cli->fd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (new_cli->fd < 0)
		fatal();
	new_cli->id = id_max;
	new_cli->next = NULL;
	new_cli->new = 1;
	id_max++;
	FD_SET(new_cli->fd, &pool_set);
	add_list(new_cli);
	sprintf(header, "server: client %d just arrived\n", new_cli->id);
	send_all(header, strlen(header), new_cli->fd);
}

void	remove_client(int fd) {
	t_lst	*buff = NULL;
	char	header[100];
	bzero(header, 100);
	if (list->fd == fd) {
		buff = list;
		list = list->next;
	}
	else {
		for (t_lst *tmp = list ; tmp->next ; tmp = tmp->next) {
			if (tmp->next->fd == fd) {
				buff = tmp->next;
				tmp->next = buff->next;
				break ;
			}
		}
	}
	sprintf(header, "server: client %d just left\n", buff->id);
	FD_CLR(buff->fd, &pool_set);
	close(buff->fd);
	send_all(header, strlen(header), fd);
	free(buff);
}

int		msg(int fd) {
	char	c = 0;
	char	header[100];
	ssize_t	rtn = 0;
	t_lst	*client = get_client(fd);

	bzero(header, 100);
	sprintf(header, "client %d: ", client->id);
	rtn = recv(fd, &c, 1, 0);
	if (rtn <= 0)
		return EXIT_FAILURE;
	if (client->new)
		send_all(header, strlen(header), fd);
	send_all(&c, 1, fd);
	client->new = (c == '\n');
	return EXIT_SUCCESS;
}

int		main(int ac, char **av) {
	struct sockaddr_in servaddr; 
	sockfd = 0;
	id_max = 0;
	list = NULL;
	uint16_t	port = 0;

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
		fatal();
	if (ac != 2)
		fatal_args();
	port = atoi(av[1]);
	bzero(&servaddr, sizeof(servaddr)); 
	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(port); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
		fatal();	
	if (listen(sockfd, 100000) != 0) 
		fatal();
	FD_ZERO(&pool_set);
	FD_ZERO(&write_set);
	FD_ZERO(&read_set);
	FD_SET(sockfd, &pool_set);
	while (1) {
		int	nfds = get_max_fd();
		FD_COPY(&pool_set, &read_set);
		FD_COPY(&pool_set, &write_set);
		if (select(nfds + 1, &read_set, &write_set, NULL, 0) < 0) {
			continue ;
		}
		for (int i = 0 ; i < nfds + 1 ; ++i) {
			if (FD_ISSET(i, &read_set)) {
				if (i == sockfd) {
					add_client();
					break ;
				}
				else
					if (msg(i) == EXIT_FAILURE)
						remove_client(i);
			}
		}
	}
}
