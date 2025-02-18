#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/connector.h>
#include <linux/cn_proc.h>

#define MAX_PAYLOAD 1024

int main() {
    int sock_fd;
    struct sockaddr_nl sa;
    struct nlmsghdr *nlh;
    struct cn_msg *cn_msg;
    struct proc_event *ev;
    char buffer[MAX_PAYLOAD];

    // Create Netlink socket
    sock_fd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Initialize the sockaddr_nl structure
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = CN_IDX_PROC;
    sa.nl_pid = getpid();

    // Bind the socket
    if (bind(sock_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        perror("Bind failed");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    // Prepare the subscription message
    memset(buffer, 0, sizeof(buffer));

	// Packet has the Netlink header in the begining
    nlh = (struct nlmsghdr *)buffer;

	// Connector Message 
    cn_msg = (struct cn_msg *)NLMSG_DATA(nlh);

	// Populate Netlink Header
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(*cn_msg) + sizeof(int));
    nlh->nlmsg_type = NLMSG_DONE;
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_seq = 0;
    nlh->nlmsg_pid = getpid();

	//Populate Connector Message
    cn_msg->id.idx = CN_IDX_PROC; //CN_IDX_PROC indicates that the message is related to process events.
    cn_msg->id.val = CN_VAL_PROC; // Type of  message
    cn_msg->seq = 0;
    cn_msg->ack = 0;
    cn_msg->len = sizeof(int); // Only int sized message

    // Enable process events
    *((int *)cn_msg->data) = PROC_CN_MCAST_LISTEN; // We're asking to listen to multicast events

    if (send(sock_fd, nlh, nlh->nlmsg_len, 0) < 0) {
        perror("Send failed");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    printf("Listening for process events...\n");

    // Main loop to receive messages
    while (1) {
        ssize_t len = recv(sock_fd, buffer, sizeof(buffer), 0);
        if (len < 0) {
            perror("Receive failed");
            close(sock_fd);
            return EXIT_FAILURE;
        }

        nlh = (struct nlmsghdr *)buffer;
        cn_msg = (struct cn_msg *)NLMSG_DATA(nlh);
        ev = (struct proc_event *)cn_msg->data;

        switch (ev->what) {
            case PROC_EVENT_FORK:
                printf("Fork: Parent PID=%d, Child PID=%d\n",
                       ev->event_data.fork.parent_pid,
                       ev->event_data.fork.child_pid);
                break;

            case PROC_EVENT_EXEC:
                printf("Exec: PID=%d\n", ev->event_data.exec.process_pid);
                break;

            case PROC_EVENT_EXIT:
                printf("Exit: PID=%d, Exit Code=%d\n",
                       ev->event_data.exit.process_pid,
                       ev->event_data.exit.exit_code);
                break;

            default:
                printf("Other process event: %d\n", ev->what);
                break;
        }
    }

    // Disable process events before exiting
    *((int *)cn_msg->data) = PROC_CN_MCAST_IGNORE;
    send(sock_fd, nlh, nlh->nlmsg_len, 0);

    close(sock_fd);
    return EXIT_SUCCESS;
}

