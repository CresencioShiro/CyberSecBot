void fragmentPackets(char* packet, size_t size) {
    size_t fragmentSize = size / 2;  // Simple fragmentation logic
    char fragment1[fragmentSize];
    char fragment2[size - fragmentSize];

    memcpy(fragment1, packet, fragmentSize);
    memcpy(fragment2, packet + fragmentSize, size - fragmentSize);

    // Send fragments here, obfuscating the traffic
    std::cout << "Packet fragmented and sent." << std::endl;
}
