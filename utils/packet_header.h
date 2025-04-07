typedef union {
    unsigned int header;
    struct{
        unsigned char src_addr : 3;
        unsigned char next_hop : 3;
        unsigned char dst_addr : 3;
        bool MF : 1;
        unsigned char msg_id : 3;
        unsigned short fragment_id : 11;
        unsigned char data : 8; //we don't have any type that only takes 3 bytes
        
    }fields;
} packet_header;

