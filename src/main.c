#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

int main(int argc, char **argv, char **envp)
{
    int fd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (fd < 0) {
        perror("init socket");
        return EXIT_FAILURE;
    }
    struct sockaddr_nl me;
    memset(&me, 0, sizeof(me));
    me.nl_family = AF_NETLINK;
    me.nl_pid = (__u32) getpid();
    me.nl_groups = RTMGRP_IPV4_ROUTE;

    if (bind(fd, (const struct sockaddr *) &me, sizeof(me)) < 0) {
        perror("Bind");
        close(fd);
        return EXIT_FAILURE;
    }

    char buf[UINT16_MAX];
    struct msghdr msg;
    struct sockaddr_nl sock;
    memset(&msg, 0, sizeof(msg));
    memset(&sock, 0, sizeof(sock));

    msg.msg_name = &sock;
    msg.msg_namelen = sizeof(sock);

    struct iovec vec;
    vec.iov_base = &buf;
    vec.iov_len = UINT16_MAX;

    msg.msg_iov = &vec;
    msg.msg_iovlen = 1;

    struct nlmsghdr *nlmsg = vec.iov_base;



    ssize_t res = recvmsg(fd, &msg, 0);
    if (res < 0) {
        perror("Recv msg");
        close(fd);
        return EXIT_FAILURE;
    }
    struct rtmsg *rtmsg1 = NLMSG_DATA(nlmsg);
    switch (nlmsg->nlmsg_type) {
        case (RTM_NEWROUTE):   puts("RTM_NEWROUTE");
            break;
        case (RTM_DELROUTE): puts("RTM_DELROUTE");
            break;
        default:
            printf("Other\n");
            break;

    }



    close(fd);
    return EXIT_SUCCESS;
}