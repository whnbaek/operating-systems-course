#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <linux/taskstats.h>

#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

#define BUFFER_SIZE 8192
#define TASKSTATS_GENL_NAME "TASKSTATS"

// Define custom NLA macros
#if 0
#define NLA_DATA(nla) ((void *)((char *)(nla) + NLA_HDRLEN))
#define NLA_LEN(nla) ((nla)->nla_len - NLA_HDRLEN)
#define NLA_OK(nla, len) ((len) >= (int)sizeof(struct nlattr) && (nla)->nla_len >= sizeof(struct nlattr) && (nla)->nla_len <= (len))
#define NLA_NEXT(nla, len) ((len) -= NLMSG_ALIGN((nla)->nla_len), (struct nlattr *)((char *)(nla) + NLMSG_ALIGN((nla)->nla_len)))
#endif

#define NLA_DATA nla_data
#define NLA_LEN nla_len
#define NLA_OK nla_ok
#define NLA_NEXT nla_next


int get_family_id(int sock_fd) {
    struct {
        struct nlmsghdr nlh;
        struct genlmsghdr gnlh;
        char attr_buffer[BUFFER_SIZE];
    } request;

    struct sockaddr_nl dest_addr;
    struct nlmsghdr *response_nlh;
    char buffer[BUFFER_SIZE];
    struct iovec iov;
    struct msghdr msg;
    int len;

    // Prepare request
    memset(&request, 0, sizeof(request));
    request.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct genlmsghdr));
    request.nlh.nlmsg_type = GENL_ID_CTRL;
    request.nlh.nlmsg_flags = NLM_F_REQUEST;
    request.gnlh.cmd = CTRL_CMD_GETFAMILY;

    // Set attributes
    struct nlattr *attr = (struct nlattr *)((char *)&request + NLMSG_ALIGN(NLMSG_SPACE(sizeof(struct genlmsghdr))));
    attr->nla_type = CTRL_ATTR_FAMILY_NAME;
    attr->nla_len = NLA_HDRLEN + strlen(TASKSTATS_GENL_NAME) + 1;
    strcpy((char *)NLA_DATA(attr), TASKSTATS_GENL_NAME);

    request.nlh.nlmsg_len += NLMSG_ALIGN(attr->nla_len);

    // Prepare destination address
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;

    // Prepare message
    iov.iov_base = &request;
    iov.iov_len = request.nlh.nlmsg_len;
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = &dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // Send request
    if (sendmsg(sock_fd, &msg, 0) < 0) {
        perror("sendmsg");
        return -1;
    }

    // Receive response
    memset(buffer, 0, BUFFER_SIZE);
    len = recv(sock_fd, buffer, BUFFER_SIZE, 0);
    if (len < 0) {
        perror("recv");
        return -1;
    }

    response_nlh = (struct nlmsghdr *)buffer;
    for (; NLMSG_OK(response_nlh, len); response_nlh = NLMSG_NEXT(response_nlh, len)) {
        if (response_nlh->nlmsg_type == NLMSG_DONE) {
            break;
        }

        if (response_nlh->nlmsg_type == NLMSG_ERROR) {
            fprintf(stderr, "Error in netlink response\n");
            return -1;
        }

        struct genlmsghdr *gnlh = NLMSG_DATA(response_nlh);
        struct nlattr *attr = (struct nlattr *)((char *)gnlh + GENL_HDRLEN);
        for (; NLA_OK(attr, len); attr = NLA_NEXT(attr, &len)) {
            if (attr->nla_type == CTRL_ATTR_FAMILY_ID) {
                return *(int *)NLA_DATA(attr);
            }
        }
    }

    return -1;
}

void request_task_stats(int sock_fd, int family_id, pid_t target_pid) {
    struct sockaddr_nl dest_addr;
    struct nlmsghdr *nlh;
    struct genlmsghdr *gnlh;
    struct nlattr *attr;
    char buffer[BUFFER_SIZE];
    struct iovec iov;
    struct msghdr msg;
    ssize_t len;

    // Prepare destination address
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;

    // Allocate message
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(BUFFER_SIZE));
    memset(nlh, 0, NLMSG_SPACE(BUFFER_SIZE));
    nlh->nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN);
    nlh->nlmsg_type = family_id;
    nlh->nlmsg_flags = NLM_F_REQUEST;
    nlh->nlmsg_seq = 1;
    nlh->nlmsg_pid = getpid();

    // Generic netlink header
    gnlh = (struct genlmsghdr *)NLMSG_DATA(nlh);
    gnlh->cmd = TASKSTATS_CMD_GET;
    gnlh->version = 0;

    // Add attribute for the PID
    attr = (struct nlattr *)((char *)nlh + NLMSG_SPACE(GENL_HDRLEN));
    attr->nla_type = TASKSTATS_CMD_ATTR_PID;
    attr->nla_len = NLA_HDRLEN + sizeof(pid_t);
    *((pid_t *)NLA_DATA(attr)) = target_pid;

    nlh->nlmsg_len += NLMSG_ALIGN(attr->nla_len);

    // Prepare message
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // Send the request
    if (sendmsg(sock_fd, &msg, 0) < 0) {
        perror("sendmsg");
        free(nlh);
        return;
    }

    // Receive the response
    memset(buffer, 0, BUFFER_SIZE);
    len = recv(sock_fd, buffer, BUFFER_SIZE, 0);
    if (len < 0) {
        perror("recv");
        free(nlh);
        return;
    }

    // Process the response
    struct nlmsghdr *resp_nlh = (struct nlmsghdr *)buffer;
    for (; NLMSG_OK(resp_nlh, len); resp_nlh = NLMSG_NEXT(resp_nlh, len)) {
        if (resp_nlh->nlmsg_type == NLMSG_DONE) {
            break;
        }

        if (resp_nlh->nlmsg_type == NLMSG_ERROR) {
            fprintf(stderr, "Error in netlink response\n");

			struct nlmsgerr *err = (struct nlmsgerr *)NLMSG_DATA(resp_nlh);
        if (resp_nlh->nlmsg_len >= NLMSG_LENGTH(sizeof(struct nlmsgerr))) {
            fprintf(stderr, "Netlink error: %s\n", strerror(-err->error));
        } else {
            fprintf(stderr, "Netlink error: Unknown error\n");
        }
            break;
        }

        // Parse the taskstats response
        struct genlmsghdr *resp_gnlh = NLMSG_DATA(resp_nlh);
        if (resp_gnlh->cmd == TASKSTATS_CMD_NEW) {
            //struct nlattr *attr = (struct nlattr *)NLA_DATA(attr);
            int remaining_len = NLMSG_PAYLOAD(resp_nlh, 0) - GENL_HDRLEN;

			struct nlattr *attr = (struct nlattr *)((char *)resp_gnlh + GENL_HDRLEN);

            for (; NLA_OK(attr, remaining_len); attr = NLA_NEXT(attr, &remaining_len)) {
                if (attr->nla_type == TASKSTATS_TYPE_AGGR_PID) {
					// TODO: Figure out the 0xC offset
                    struct taskstats *stats = (struct taskstats *)((char * )NLA_DATA(attr) + 0xC);
					printf("Command: %s\n", stats->ac_comm);
                }
            }
        }
    }

    // Cleanup
    free(nlh);
}

int main(int argc, char* argv[]) {
    int sock_fd, family_id;
    struct sockaddr_nl src_addr;

	if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = atoi(argv[1]);

    // Create a netlink socket
    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    // Initialize source address
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    if (bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
        perror("bind");
        close(sock_fd);
        return -1;
    }

	for (volatile int i=0; i< 999999999; i++);
    // Get family ID for TASKSTATS
    family_id = get_family_id(sock_fd);
    if (family_id < 0) {
        fprintf(stderr, "Failed to get family ID for TASKSTATS\n");
        close(sock_fd);
        return -1;
    }

    printf("Family ID for TASKSTATS: %d\n", family_id);

	// Request statistics for the current process (or replace with any PID)
    request_task_stats(sock_fd, family_id, pid);



    close(sock_fd);
    return 0;
}

