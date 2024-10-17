#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>

// Calculate the checksum for IP and TCP headers
unsigned short calculateChecksum(unsigned short* buffer, int length) {
    unsigned long sum = 0;
    for (int i = 0; i < length / 2; i++) {
        sum += buffer[i];
    }
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (unsigned short)(~sum);
}

// Packet crafting and obfuscation
void craftAndSendPacket(const char* target_ip, const char* spoofed_ip) {
    // Create raw socket
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("Raw socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Buffer to hold the packet (IP header + TCP header)
    char packet[4096];
    memset(packet, 0, sizeof(packet));

    // IP header
    struct iphdr* iph = (struct iphdr*)packet;
    struct tcphdr* tcph = (struct tcphdr*)(packet + sizeof(struct iphdr));

    // Set up the IP header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    iph->id = htons(rand() % 65535);  // Randomize the ID field to obfuscate
    iph->frag_off = 0;
    iph->ttl = 255;  // High TTL to prevent premature discard
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;  // Will be calculated later
    iph->saddr = inet_addr(spoofed_ip);  // Spoofed IP address
    iph->daddr = inet_addr(target_ip);   // Target IP address

    // TCP header
    tcph->source = htons(rand() % 65535);  // Random source port to obfuscate
    tcph->dest = htons(80);  // Destination port (HTTP for example)
    tcph->seq = htonl(rand());  // Random sequence number
    tcph->ack_seq = 0;
    tcph->doff = 5;  // TCP header size
    tcph->syn = 1;   // SYN flag set for initiating connection
    tcph->window = htons(65535);  // Max window size
    tcph->check = 0;  // Will be calculated later
    tcph->urg_ptr = 0;

    // IP checksum calculation
    iph->check = calculateChecksum((unsigned short*)packet, iph->tot_len);

    // Set the destination address structure
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = tcph->dest;  // TCP destination port
    dest_addr.sin_addr.s_addr = inet_addr(target_ip);

    // Send the packet
    if (sendto(sockfd, packet, iph->tot_len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("Packet send failed");
    } else {
        std::cout << "Obfuscated packet sent to " << target_ip << " with spoofed IP: " << spoofed_ip << std::endl;
    }

    // Close the raw socket
    close(sockfd);
}

int main() {
    // Randomize seed for random number generation
    srand(time(NULL));

    // Target IP (destination) and Spoofed IP (source)
    const char* target_ip = "192.168.1.1";
    const char* spoofed_ip = "10.0.0.1";  // Example spoofed IP address

    craftAndSendPacket(target_ip, spoofed_ip);

    return 0;
}
