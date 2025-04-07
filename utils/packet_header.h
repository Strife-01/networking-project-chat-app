typedef union {
    unsigned int header;
    struct{
        unsigned char src_addr : 3;
        unsigned char next_hop : 3;
        unsigned char dst_addr : 3;
        bool MF : 1;
        unsigned char msg_id : 3;
        unsigned short fragment_id : 11;
        unsigned char type : 3;
        unsigned char payload_len : 5;
        
    }fields;
} packet_header;

